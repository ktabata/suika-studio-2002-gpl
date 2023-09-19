#include "VNPlayer.h"
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>

#define GLOBAL_FLAG_FILE	"global.flg"
#define LOCAL_FLAG_FILE		"local.flg"
#define ENTRY_EVENT_FILE	"start.evt"

#define RELEASE(obj)	if(obj!=NULL)delete obj


VNPlayer::VNPlayer() {
	ZeroMembers();
	prof.WindowImage = new VNString();
	prof.MsgFontName = new VNString();
}

VNPlayer::~VNPlayer() {
	ReleaseAll();
	delete prof.WindowImage;
	delete prof.MsgFontName;
}

bool VNPlayer::Play() {
	LoadProfile();
	InitWindow();
	InitSound();
	LoadStartEvent();
	LoadFlag();

	// 実行ループ
	try {
		for(;;) {
			if( ! Execute() ) {
				// ロード操作が行われた
				Load();
			} else
				break;	// 正常に実行終了
		}
	} catch ( VNWindowCloseException *e ) {
		// プレイ中にウィンドウが閉じられた
		delete e;
		MessageBox (NULL, "予期せぬ終了を正常に処理しました。", title.c_str(), MB_OK | MB_ICONINFORMATION);
	}

	if( ! global_flag->Save( "global.flg" ) )
		Warning( "global.flgのセーブに失敗しました。" );

	ReleaseAll();
	return true;
}

void VNPlayer::InitWindow() {
	// 初期タイトル
	title = "VNStudio";

	// ウィンドウを作る
	window = new VNPWindow();
	if( ! window->Open( title.c_str(), prof.WindowWidth, prof.WindowHeight ) )
		DebugBreak( "メインウィンドウをオープンできない" );
	window->Sync();

	// ウィンドウの背景イメージをロードする
	if( strcmp( prof.WindowImage->c_str(), "none" ) != 0 ) {
		window_image = new VNImage();
		if( ! window_image->LoadBmp( (VNString(CG_DIR)+*prof.WindowImage).c_str() ) ) {
			Warning( "ウィンドウ背景イメージをロードできませんでした。" );
			delete window;
			window = NULL;
		}
	}

	// バックバッファを作る
	back_buffer = window->GetBackBuffer();
	text_drawer = new VNTextDrawer( back_buffer );
	text_drawer->SelectFont( prof.MsgFontWidth, prof.MsgFontHeight, prof.MsgFontName->c_str());

	DrawBgAndChara();
	window->Flip();
}

void VNPlayer::InitSound() {
	// MCI の cdaudio 操作クラスを初期化する
	cdplayer = new VNCDPlayer();

	// MCI の sequencer 操作クラスを初期化する
	midiplayer = new VNMidiPlayer();
}

void VNPlayer::LoadStartEvent() {
	// イベントファイルをロードする
	event = new VNEventFile();
	if( ! event->Load( (VNString)EVENT_DIR+ENTRY_EVENT_FILE ) )
		DebugBreak( "start.evtのロードに失敗しました。" );

	offset = 0;
	event_name = ENTRY_EVENT_FILE;
}

void VNPlayer::LoadFlag() {
	// フラグをロードする
	global_flag = new VNFlagFile();
	if( ! global_flag->Load( GLOBAL_FLAG_FILE ) )
		DebugBreak( "global.flgのロードに失敗しました。" );

	local_flag = new VNFlagFile();
	if( ! local_flag->Load( LOCAL_FLAG_FILE ) )
		DebugBreak( "local.flgのロードに失敗しました。" );
}

// セーブデータのロードにより中断された場合は
bool VNPlayer::Execute() {
	int count = event->GetCount();

	for( ; offset < count; offset++ ) {
		VNEvent *e = event->GetEvent( offset );
		int	code = e->GetCode();

		if( code <= 0 || code > EVENTCODE_MAX )
			DebugBreak( "不正なイベントコード" );

		switch( code ) {
		case EVENTCODE_NARRATION:
			if( ! Narration( e->GetParamString(0).c_str() ) )
				return false;	// ロード操作が行われた
			break;
		case EVENTCODE_SERIF:
			if( ! Serif( e->GetParamString(0).c_str(), e->GetParamString(1).c_str() ) )
				return false;	// ロード操作が行われた
			break;
		case EVENTCODE_CHARACTER:
			CharaFade( e->GetParamInt(0), e->GetParamInt(1), e->GetParamString(2).c_str(), e->GetParamString(3).c_str() );
			break;
		case EVENTCODE_BACKGROUND:
			BackgroundFade( e->GetParamString(0).c_str() );
			break;
		case EVENTCODE_WAITCLICK:
			WaitClick();
			break;
		case EVENTCODE_WAIT:
			Wait( e->GetParamInt(0) );
			break;
		case EVENTCODE_PCM:
			PlayPcm( e->GetParamString(0).c_str() );
			break;
		case EVENTCODE_SETFLAG:
			SetFlag( e->GetParamString(0).c_str(), e->GetParamInt(1) );
			break;
		case EVENTCODE_JUMP:
			Jump( e->GetParamString(0).c_str(),
				  e->GetParamString(1).c_str(),
				  e->GetParamInt(2),
				  e->GetParamString(3).c_str());
			break;
		case EVENTCODE_TITLE:
			Title( e->GetParamString(0).c_str() );
			break;
		case EVENTCODE_LABEL:
		case EVENTCODE_COMMENT:
			break;
		case EVENTCODE_SELECT:
			if( ! Select( e->GetParamString(0).c_str(),
						  e->GetParamString(2).c_str(),
						  e->GetParamString(4).c_str(),
						  e->GetParamString(1).c_str(),
						  e->GetParamString(3).c_str(),
						  e->GetParamString(5).c_str() ) ) {
				return false;	// ロード操作が行われた
			}
			break;
		case EVENTCODE_CDDA:
			PlayCdda( e->GetParamInt(0), e->GetParamInt(1) );
			break;
		case EVENTCODE_MIDI:
			PlayMidi( e->GetParamString(0).c_str(), e->GetParamInt(1) );
			break;
		case EVENTCODE_MSGBOX:
			break;

		default:
			DebugBreak( "不正なイベントコード：キャッチできなかったイベントコード" );
			break;
		}

		// イベントファイルが変更されたかもしれないので、毎回countを設定する
		count = event->GetCount();
	}
	return true;	// 実行完了
}

void VNPlayer::ZeroMembers() {
	window = NULL;
	window_image = NULL;
	cdplayer = NULL;
	midiplayer = NULL;
	event = NULL;
	event_name = "未ロード";
	offset = 0;
	global_flag = NULL;
	local_flag = NULL;
	back_buffer = NULL;
	text_drawer = NULL;
	for( int i = 0; i < 4; i++ )
		image[i] = NULL;
}

void VNPlayer::ReleaseAll() {
	// PCM演奏を停止する
	sndPlaySound (NULL, SND_FILENAME);

	// ウィンドウを閉じる
	if( window != NULL ) {
		window->Close();
		delete window;
	}

	RELEASE( window_image );
	RELEASE( cdplayer );
	RELEASE( midiplayer );
	RELEASE( event );
	RELEASE( global_flag );
	RELEASE( local_flag );
	RELEASE( text_drawer );
	for( int i = 0; i < 4; i++ )
		RELEASE( image[i] );

	ZeroMembers();
}

int VNPlayer::GetIntProfile( const char *key, int def ) {
	char *dir = new char [1024];
	::GetCurrentDirectory( 1023, dir );
	int ret = ::GetPrivateProfileInt ("suika", key, def, (VNString(dir)+"\\suika.ini").c_str());
	delete[] dir;
	return ret;
}

VNString VNPlayer::GetStrProfile( const char *key, const char *def ) {
	char *dir = new char [1024];
	::GetCurrentDirectory( 1023, dir );
	char *str = new char [1024];
	::GetPrivateProfileString ("suika", key, def, str, 1023, (VNString(dir)+"\\suika.ini").c_str());
	VNString ret = str;
	delete[] str;
	delete[] dir;
	return ret;
}

void VNPlayer::LoadProfile() {
	prof.WindowWidth = GetIntProfile( "WindowWidth", 640 );
	prof.WindowHeight = GetIntProfile( "WindowHeight", 480 );
	*prof.WindowImage = GetStrProfile( "WindowImage", "none" );
	prof.BackgroundTop = GetIntProfile( "BackgroundTop", 0 );
	prof.BackgroundLeft = GetIntProfile( "BackgroundLeft", 0 );
	prof.BackgroundWidth = GetIntProfile( "BackgoundWidth", 640 );
	prof.BackgroundHeight = GetIntProfile( "BackgoundHeight", 480 );
	prof.MsgBoxColorStartR = GetIntProfile( "MsgBoxColorStartR", 95 );
	prof.MsgBoxColorStartG = GetIntProfile( "MsgBoxColorStartG", 88 );
	prof.MsgBoxColorStartB = GetIntProfile( "MsgBoxColorStartB", 191 );
	prof.MsgBoxColorStartA = GetIntProfile( "MsgBoxColorStartA", 200 );
	prof.MsgBoxColorEndR = GetIntProfile( "MsgBoxColorEndR", 207 );
	prof.MsgBoxColorEndG = GetIntProfile( "MsgBoxColorEndG", 207 );
	prof.MsgBoxColorEndB = GetIntProfile( "MsgBoxColorEndB", 255 );
	prof.MsgBoxColorEndA = GetIntProfile( "MsgBoxColorEndA", 200 );
	prof.MsgBoxTop = GetIntProfile( "MsgBoxTop", 335 );
	prof.MsgBoxLeft = GetIntProfile( "MsgBoxLeft", 19 );
	prof.MsgBoxWidth = GetIntProfile( "MsgBoxWidth", 601 );
	prof.MsgBoxHeight = GetIntProfile( "MsgBoxHeight", 131 );
	prof.MsgLineHeight = GetIntProfile( "MsgLineHeight", 26 );
	prof.MsgBoxFrameWidth = GetIntProfile( "MsgBoxFrameWidth", 5 );
	prof.MsgBoxFrameHeight = GetIntProfile( "MsgBoxFrameWidth", 5 );
	*prof.MsgFontName = GetStrProfile( "MsgFontName", "ＭＳ Ｐゴシック" );
	prof.MsgFontWidth = GetIntProfile( "MsgFontWidth", 12 );
	prof.MsgFontHeight = GetIntProfile( "MsgFontHeight", 24 );
}

void VNPlayer::Warning( const char *msg ) {
	VNString str = event_name + "(" + (VNString)offset + ")\r\n" + msg;
	MessageBox (
		window->GetHWND(),
		str.c_str(),
		"警告",
		MB_ICONEXCLAMATION | MB_OK);
}

void VNPlayer::DebugBreak( const char *msg ) {
	VNString str = event_name + "(" + (VNString)offset + ")\r\n" + msg;
	MessageBox (NULL, str.c_str(), "VNPlayer 内部エラー", MB_ICONEXCLAMATION | MB_OK);
#ifdef _DEBUG
	abort();
#else
	exit(1);
#endif
}
