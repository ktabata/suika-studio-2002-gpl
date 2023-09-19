#include "VNEvent.h"
#include <stddef.h>
#include <windows.h>


VNEvent::VNEvent( int type ) : event_type( event_type_desc[type] ) {
	if( event_type.code <= 0 || event_type.code > EVENTCODE_MAX )
		DebugBreak( "無効なイベントタイプ" );

	AllocArgList();
	for( int i = 0; i < params; i++ ) {
		if( event_type.arg_type_desc[i] == 'S' )
			str_arg[i] = new VNString( "" );
		else
			int_arg[i] = 0;
	}
}

VNEvent::~VNEvent() {
	delete int_arg;
	for( int i = 0; i < params; i++ ) {
		if( str_arg[i] != NULL )
			delete str_arg[i];
	}
	delete str_arg;
}

VNEvent::VNEvent( VNEvent& src ) : event_type( src.event_type ) {
	if( event_type.code <= 0 || event_type.code > EVENTCODE_MAX )
		DebugBreak( "無効なイベントタイプ" );

	AllocArgList();
	for( int i = 0; i < params; i++ ) {
		if( event_type.arg_type_desc[i] == 'S' )
			str_arg[i] = new VNString( *src.str_arg[i] );
		else
			int_arg[i] = src.int_arg[i];
	}
}

void VNEvent::AllocArgList() {
	params = 0;

	char *c = event_type.arg_type_desc;
	while( *c != '\0' ) {
		if( *c != 'I' && *c != 'S' )
			DebugBreak( "無効なパラメタ記述子" );
		params++, c++;
	}

	int_arg = new int [ params ];
	str_arg = new VNString * [ params ];
	for( int i = 0; i < params; i++ )
		str_arg[i] = NULL;
}

int VNEvent::GetCode() {
	return event_type.code;
}

const EventTypeDescriptor& VNEvent::GetDescriptor() {
	return event_type;
}

int VNEvent::GetParamInt( int n ) {
	if( n >= params  )
		DebugBreak( "無効なパラメタ番号" );

	if( event_type.arg_type_desc[n] != 'I' )
		DebugBreak( "パラメタ型が違う" );

	return int_arg[n];
}

const VNString& VNEvent::GetParamString( int n ) {
	if( n >= params  )
		DebugBreak( "無効なパラメタ番号" );

	if( event_type.arg_type_desc[n] != 'S' )
		DebugBreak( "パラメタ型が違う" );

	return *str_arg[n];
}

void VNEvent::SetParamInt( int n, int val ) {
	if( n >= params  )
		DebugBreak( "無効なパラメタ番号" );

	if( event_type.arg_type_desc[n] != 'I' )
		DebugBreak( "パラメタ型が違う" );

	int_arg[n] = val;
}

void VNEvent::SetParamString( int n, const char *val ) {
	if( n >= params  )
		DebugBreak( "無効なパラメタ番号" );

	if( event_type.arg_type_desc[n] != 'S' )
		DebugBreak( "パラメタ型が違う" );

	if( str_arg[n] != NULL )
		delete str_arg[n];

	str_arg[n] = new VNString( val );
}

VNString VNEvent::GetExplanation() {
	switch( event_type.code ) {
	case EVENTCODE_NARRATION:
		return VNString( "＊　" ) + *str_arg[0];

	case EVENTCODE_SERIF:
		return VNString( "＞　" ) + *str_arg[0] + "「" + *str_arg[1];

	case EVENTCODE_CHARACTER:
	{
		VNString a;
		switch( int_arg[0] ) {
		case CHARA_H_CENTER:a = "◆ キャラ　中　";	break;
		case CHARA_H_LEFT:	a = "◆ キャラ　左　";	break;
		case CHARA_H_RIGHT:	a = "◆ キャラ　右　";	break;
		default:
			MessageBox (NULL, "不正なキャラポジション", "VNEvent.cpp", MB_OK | MB_ICONEXCLAMATION);
			abort();	// never come here
			break;
		}

		if( strcmp( str_arg[2]->c_str(), "" ) == 0 )
			return a + "退場";
		else if( int_arg[1] == CHARA_V_BOTTOM )
			return a + *str_arg[2] + "　下揃い ";
		else if( int_arg[1] == CHARA_V_CENTER )
			return a + *str_arg[2] + "　中央揃い ";
		else if( int_arg[1] == CHARA_V_TOP )
			return a + *str_arg[2] + "　上揃い ";
	}

	case EVENTCODE_BACKGROUND:
		if( strcmp( str_arg[0]->c_str(), "" ) == 0 )
			return VNString( "★ 背　景　フェードアウト" ) + *str_arg[0];
		else
			return VNString( "★ 背　景　" ) + *str_arg[0];

	case EVENTCODE_SELECT:
		return VNString( "▼ 選　択　" ) + *str_arg[0] + "／" + *str_arg[2] + "／" + *str_arg[4];

	case EVENTCODE_LABEL:
		return VNString( "→ ラベル　" ) + *str_arg[0];

	case EVENTCODE_CDDA:
		if( int_arg[0] == 0 )
			return VNString( "♪ CD-DA 　停止　フェード" ) + VNString( int_arg[1] ) + "ミリ秒";
		else
			return VNString( "♪ CD-DA 　トラック" ) + VNString( int_arg[0] ) + "　フェード" + VNString( int_arg[1] ) + "ミリ秒";

	case EVENTCODE_MIDI:
		if( strcmp( str_arg[0]->c_str(), "" ) == 0 )
			return VNString( "♪ MIDI　　停止　フェード" ) + VNString( int_arg[1] ) + "ミリ秒";
		else
			return VNString( "♪ MIDI　　" ) + *str_arg[0] + "　フェード" + VNString( int_arg[1] ) + "ミリ秒";

	case EVENTCODE_PCM:
		if( strcmp( str_arg[0]->c_str(), "" ) == 0 )
			return VNString( "♪ PCM 　　オフ" );
		else
			return VNString( "♪ PCM 　　" ) + *str_arg[0];

	case EVENTCODE_SETFLAG:
		switch( int_arg[1] ) {
		case SETFLAG_ON:
			return VNString( "▽ フラグ　ＯＮ　" ) + *str_arg[0];
		case SETFLAG_OFF:
			return VNString( "▽ フラグ　ＯＦＦ　" ) + *str_arg[0];
		case SETFLAG_INVERT:
			return VNString( "▽ フラグ反転　" ) + *str_arg[0];
		default:
			abort();
		}
		break;

	case EVENTCODE_JUMP:
	{
		VNString evt;
		if( strcmp( str_arg[0]->c_str(), "" ) == 0 )
			evt = "ファイル内　";
		else
			evt = *str_arg[0] + "　";

		VNString label;
		if( strcmp( str_arg[1]->c_str(), "" ) == 0 )
			label = "先頭　";
		else
			label = VNString( "ラベル\"" ) + *str_arg[1] + "\"　";

		switch( int_arg[2] ) {
		case JUMP_FORCED:
			return VNString( "← ジャンプ　無条件　" ) + evt + label;
		case JUMP_IFFLAGSET:
			return VNString( "← ジャンプ　" ) + evt + label + "条件：フラグ\"" + *str_arg[3] + "\"がＯＮである";
		case JUMP_IFFLAGNOTSET:
			return VNString( "← ジャンプ　" ) + evt + label + "条件：フラグ\"" + *str_arg[3] + "\"がＯＦＦである";
		default:
			abort();
		}
		break;
	}

	case EVENTCODE_TITLE:
		return VNString( "§ タイトル　" ) + *str_arg[0];

	case EVENTCODE_WAITCLICK:
		return VNString( "◎ クリックウェイト" );

	case EVENTCODE_WAIT:
		return VNString( "◎ ウェイト　" ) + VNString( int_arg[0] ) + "ミリ秒";

	case EVENTCODE_MSGBOX:
		return VNString( "メッセージボックス設定" );

	case EVENTCODE_COMMENT:
		return VNString( "; " ) + *str_arg[0];
	}

	MessageBox (NULL, "無効なイベントタイプです", "VNEvent.cpp", MB_OK | MB_ICONEXCLAMATION);
	abort();
	return VNString( "エラー" );	// 一応
}

void VNEvent::DebugBreak( const char *msg ) {
	MessageBox (NULL, msg, "内部エラー(VNEventクラス)", MB_ICONEXCLAMATION | MB_OK);
	abort();
}
