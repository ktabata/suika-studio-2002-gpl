#include <windows.h>
#include "VNPlayer.h"
#include "VNPromptDrawer.h"


void VNPlayer::WaitClick() {
	bool b[3] = { true, false, true };

	window->Sync();
	for( int i = 0; i < 3; i++ ) {
		while( window->IsMouseLDown() == b[i] ) {
			if( window->IsCtrlDown() )
				return;
			window->Sync();
		}
	}
}

void VNPlayer::Wait( int millisec ) {
	bool	is_l_down = window->IsMouseLDown();

	unsigned long t = GetTickCount();
	while( t + millisec > GetTickCount() ) {
		window->Sync();

		if( window->IsCtrlDown() )
			return;

		bool b = window->IsMouseLDown();
		if( ! is_l_down && b )
			break;
		else if( is_l_down && ! b )
			is_l_down = false;
	}
}

// メッセージ送り時のセーブ・ロード可能なタイミングでのクリックウェイト。
// メッセージボックスにアニメーション表示。
// ウェイト中にロード操作が行われるとfalseを返す。
// use_quickには、メッセージが左ボタン押下で高速表示されているときにtrueを渡す。
// その場合、左ボタンが解放されるだけで、完全なクリックを待たずにリターンする。
bool VNPlayer::MessageWait( bool use_quick ) {
	bool b[3] = { true, false, true };

	window->Sync();

	// プロンプトアニメーション描画のオブジェクト
	VNPromptDrawer	pd( *back_buffer, prompt_x, prompt_y, prof.MsgFontWidth*2, prof.MsgFontHeight );
	pd.Start();

	// プロンプト表示領域をセーブ(+1は精度の誤差ではみ出ることを想定して）
	VNImage	save;
	save.CreateRGB( prof.MsgFontWidth*2+1, prof.MsgFontHeight+1 );
	save.BitBlt( 0, 0, back_buffer, prompt_x, prompt_y, prof.MsgFontWidth*2+1, prof.MsgFontHeight+1 );

	int cnt = use_quick ? 1 : 3;
	for( int i = 0; i < cnt; i++ ) {
		do {
			// セーブ＆ロードチェック
			if( window->IsSavePushed() )	// メニューのセーブが押されたか
				SaveDialog();
			if( window->IsLoadPushed() )	// メニューのロードが押されたか
				if( LoadDialog() )
					return false;	// ロード操作が行われた
			if( window->IsBackLogPushed() )
				BackLogDialog();

			if( window->IsCtrlDown() ) {
				// プロンプトを、セーブしたイメージで上書きして消す
				back_buffer->BitBlt( prompt_x, prompt_y, &save, 0, 0, prof.MsgFontWidth*2+1, prof.MsgFontHeight+1 );
				window->Update( prompt_x, prompt_y, prof.MsgFontWidth*2+1, prof.MsgFontHeight+1 );
				return true;
			}

			// 右ボタン押下中はメッセージを消す
			if( window->IsMouseRDown() ) {
				DrawBgAndChara();
				window->Flip();
				while( window->IsMouseRDown() )
					window->Sync();
				PutMessage( last_message.c_str(), 0xff, 0xff, 0xff, 0 );
				window->Flip();
			}

			// ▼のアニメプロンプトを描く
			if( pd.NeedUpdate() ) {
				back_buffer->BitBlt( prompt_x, prompt_y, &save, 0, 0, prof.MsgFontWidth*2+1, prof.MsgFontHeight+1 );
				pd.Draw();
				window->Update( prompt_x, prompt_y, prof.MsgFontWidth*2+1, prof.MsgFontHeight+1 );
			}
			window->Sync();
		} while( window->IsMouseLDown() == b[i] );
	}
	// プロンプトを、セーブしたイメージで上書きして消す
	back_buffer->BitBlt( prompt_x, prompt_y, &save, 0, 0, prof.MsgFontWidth*2+1, prof.MsgFontHeight+1 );
	window->Update( prompt_x, prompt_y, prof.MsgFontWidth*2+1, prof.MsgFontHeight+1 );
	return true;
}

bool VNPlayer::GetFlagIndex( const char *flag, int& index, bool& is_local ) {
	// ローカルフラグか？
	is_local = true;
	index = local_flag->SearchFlag( flag );
	if( index == -1 ) {	// 見つからなかった
		// グローバルフラグか？
		is_local = false;
		index = global_flag->SearchFlag( flag );
		if( index == -1 ) {
			// どちらにも見つからなかった
			return false;
		}
	}
	return true;
}

void VNPlayer::SetFlag( const char *flag, int cond ) {
	bool	is_local;
	int		n;

	for( int i = 0; i < 10; i++ ) {
	if( ! GetFlagIndex( flag, n, is_local ) ) {
		Warning( (VNString("フラグ\"")+flag+"\"が存在しません。").c_str() );
		return;
	}
	}

	VNFlagFile *flagfile = is_local ? local_flag : global_flag;
	switch( cond ) {
	case SETFLAG_ON:
		flagfile->SetFlagValue( n, true );
		break;
	case SETFLAG_OFF:
		flagfile->SetFlagValue( n, false );
		break;
	case SETFLAG_INVERT:
		flagfile->InvertFlagValue( n );
		break;
	default:
		DebugBreak( "要デバッグ" );
	}
}

void VNPlayer::Jump( const char *file, const char *label, int cond, const char *flag ) {
	bool ifflagset = true;
	switch( cond ) {
	case JUMP_FORCED:
		break;
	case JUMP_IFFLAGNOTSET:
		ifflagset = false;
	case JUMP_IFFLAGSET:
	{
		bool	is_local;
		int		n;
		if( ! GetFlagIndex( flag, n, is_local ) ) {
			Warning( (VNString("フラグ\"")+flag+"\"が存在しません。").c_str() );
			return;
		}
		VNFlagFile *flagfile = is_local ? local_flag : global_flag;
		bool val;
		if( ! flagfile->GetFlagValue( n, val ) )
			DebugBreak( "要デバッグ" );
		if( val != ifflagset )
			return;	// 条件に当てはまらない
		break;
	}
	default:
		DebugBreak( "要デバッグ" );
	}

	if( strcmp( file, "" ) == 0 ) {
		// 同一イベントファイル内でのジャンプ
		int new_offset;

		if( strcmp( label, "" ) != 0 ) {
			// ラベルにジャンプ
			new_offset = event->GetLabelOffset( label );
			if( new_offset == -1 ) {
				Warning( (VNString("ラベル\"") + label + "\"が存在しません。").c_str() );
				return;
			}
		} else {
			// ファイル先頭にジャンプ
			new_offset = -1;	// ループでインクリメントされるので１減らしておく
		}
		offset = new_offset;	// ループでインクリメントされるのでラベルの分は足さなくていい
		return;
	}

	// 他ファイルへのジャンプ

	// 新しくイベントファイルをロードする
	VNEventFile *new_event = new VNEventFile();
	if( ! new_event->Load( VNString(EVENT_DIR)+file ) ) {
		Warning( (VNString("イベントファイル\"")+file+"\"が存在しません。").c_str() );
		delete new_event;
		return;
	}

	int new_offset;

	if( strcmp( label, "" ) != 0 ) {
		// ラベルにジャンプ
		new_offset = new_event->GetLabelOffset( label );
		if( new_offset == -1 ) {
			Warning(
				(VNString("イベントファイル\"") +
				file +
				"\"にラベル\"" +
				label +
				"\"が存在しません。").c_str() );
			delete new_event;
			return;
		}
	} else {
		// ファイル先頭にジャンプ
		new_offset = -1;	// ループでインクリメントされるので１減らしておく
	}

	event_name = file;
	delete event;
	event = new_event;
	offset = new_offset;	// ループでインクリメントされるのでラベルの分は足さなくていい
}

void VNPlayer::Title( const char *str ) {
	title = str;
	window->SetTitle( str );
}

// ロードが行われるとfalseを返す。
bool VNPlayer::Select( const char *msg1, const char *msg2, const char *msg3,
					   const char *label1, const char *label2, const char *label3 )
{
	backlog.AddString("");

	int items = 3;
	if( strcmp( msg3, "" ) == 0 )
		items = 2;

	char *messages[3];
	messages[0] = (char *)msg1;
	messages[1] = (char *)msg2;
	messages[2] = (char *)msg3;

	char *labels[3];
	labels[0] = (char *)label1;
	labels[1] = (char *)label2;
	labels[2] = (char *)label3;

	int select = SelectLoop( (const char **)messages, items );

	if( select == -1 )
		return false;	// ロード操作が行われた

	if( select > items )
		DebugBreak( "要デバッグ" );

	int ofs = event->GetLabelOffset( labels[select] );
	if( ofs == -1 ) {
		Warning( (VNString("ラベル\"")+labels[select]+"\2が存在しません。\r\n大文字と小文字や半角と全角の違いをチェックしてください。\r\nそれでも直らない場合は、K24Jに報告をお願いします。").c_str() );
		return true;	// ロードは行われていないのでtrue
	}
	offset = ofs;	// ジャンプ先を指定

	return true;
}
