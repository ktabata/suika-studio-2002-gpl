#include "VNFileDialog.h"

#include "VNDEventEdit.h"
#include "resource.h"

// リストボックスのフォント
#define LB_FONT_NAME	"ＭＳ ゴシック"
#define LB_FONT_WIDTH	(6)
#define LB_FONT_HEIGHT	(12)
// リストボックスの項目の高さ（ピクセル数）
#define LB_CY_ITEM		(LB_FONT_HEIGHT+5)


bool VNDEventEdit::DoModal( const char *title, const char *file_name, HWND hWndParent ) {
	event_file = new VNEventFile();
	this->file_name = file_name;
	is_modified = false;
	hFont = NULL;
	copy_item = NULL;

	if( strcmp( file_name, "" ) != 0 ) {
		if( ! event_file->Load( file_name ) ) {
			// 読み込みに失敗したらオブジェクトを構築し直す
			delete event_file;
			event_file = new VNEventFile();
		}
	} else
		event_file = new VNEventFile();

	int res = VNDialog::DoModal( title, "IDD_EVENTEDIT", hWndParent );
	if( res )
		Save();

	if( hFont != NULL )
		DeleteObject (hFont);

	return true;
}

void VNDEventEdit::OnCommand (WPARAM wParam, LPARAM lParam)
{
	WORD	wNotifyCode = HIWORD(wParam);	// 通知コード 
	WORD	wID = LOWORD(wParam);			// 項目、コントロール、またはアクセラレータの ID 
	HWND	hWndCtl = (HWND) lParam;		// コントロールのハンドル 

	switch (wID)	{
	case IDC_BTNOPEN:
		OnBtnOpen();
		return;
	case IDC_BTNFILEINSERT:
		OnBtnFileInsert();
		return;
	case IDC_BTNEDIT:
		OnBtnEdit();
		return;
	case IDC_BTNSAVE:
		Save();
		return;
	case IDC_BTNCOPY:
		OnBtnCopy();
		return;
	case IDC_BTNPASTE:
		OnBtnPaste();
		return;
	case IDC_BTNCUT:
		OnBtnCopy();
		OnBtnDelete();
		return;
	case IDC_BTNDELETE:
		OnBtnDelete();
		return;
	case IDC_BTNINSERTEVENT:
		OnBtnInsertEvent();
		return;
	case IDC_BTNCLOSE:
		OnClose();
		return;
	}
}

void VNDEventEdit::Save() {
	if( strcmp( file_name.c_str(), "" ) == 0 ) {
		bool b = GetSaveFileNameIndirect( "名前を付けて保存", "ｲﾍﾞﾝﾄﾌｧｲﾙ(*.evt)\0*.evt\0",
										  "新規イベント.evt", file_name );
		if( b == false )
			return;
	}

	bool res = event_file->Save( file_name.c_str() );
	if( ! res ) {
		MessageBox (hWnd, "保存に失敗しました。", "イベントエディタ", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	is_modified = false;
}

int VNDEventEdit::ClosePrompt() {
	return MessageBox (hWnd,
					   "変更を保存しますか？",
					   "イベントエディタ",
					   MB_YESNOCANCEL | MB_ICONQUESTION);
}

bool VNDEventEdit::OnInit() {
	::SendMessage (
		hWnd,
		WM_SETICON,
		(WPARAM)::LoadIcon (::GetModuleHandle (NULL), "IDI_ICON"),
		(LPARAM) TRUE);
	InitListBoxItem();
	InitListBoxFont();
	InitComboBox();
	return true;
}

void VNDEventEdit::OnClose() {
	if( ! is_modified ) {
		EndDialog (hWnd, FALSE);
		return;
	}

	switch( ClosePrompt() ) {
	case IDCANCEL:
		return;
	case IDYES:
		EndDialog (hWnd, TRUE);
		return;
	case IDNO:
		EndDialog (hWnd, FALSE);
		return;
	}
	// never come here
	abort();
}

void VNDEventEdit::OnBtnOpen() {
	if( is_modified ) {
		switch( ClosePrompt() ) {
		case IDCANCEL:
			return;
		case IDYES:
			Save();
			break;
		case IDNO:
			break;
		}
	}

	bool b = GetOpenFileNameIndirect( "ファイルを開く", "ｲﾍﾞﾝﾄﾌｧｲﾙ(*.evt)\0*.evt\0",
									  "", file_name );
	if( b == false )
		return;

	delete event_file;
	event_file = new VNEventFile();
	if( ! event_file->Load( file_name.c_str() ) ) {
		// 読み込みに失敗したらオブジェクトを構築し直す
		delete event_file;
		event_file = new VNEventFile();
	}
	is_modified = false;
	InitListBoxItem();
}
		
void VNDEventEdit::OnBtnFileInsert() {
	HWND	hWndLB = GetDlgItem (hWnd, IDC_LBEVENT);

	// イベントリストの選択項目のインデックスを取得
	int index = SendMessage (hWndLB, LB_GETCURSEL, 0, 0);
	if( index == LB_ERR )
		return;

	// 挿入するファイルの名前を取得
	VNString fn;
	bool b = GetOpenFileNameIndirect( "挿入するファイルを開く", "ｲﾍﾞﾝﾄﾌｧｲﾙ\0*.evt\0",
									  "", fn );
	if( b == false )
		return;

	// 挿入するファイルをロードする
	VNEventFile *ins_evt = new VNEventFile();
	if( ! ins_evt->Load( fn.c_str() ) ) {
		// 読み込みに失敗
		delete event_file;
		MessageBox (hWnd, "ロードに失敗しました。ファイルが不正です。", "イベントエディタ", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	// 挿入する
	int cnt = ins_evt->GetCount();
	for( int i = 0; i < cnt; i++ ) {
		if( ! event_file->InsertEvent( new VNEvent( *ins_evt->GetEvent(i) ), index + i ) )
			DebugBreak();
	}
	delete ins_evt;

	// リストボックスを更新
	InitListBoxItem();

	// リストボックスにフォーカスを設定
	SetFocus (hWndLB);
	SendMessage (hWndLB, LB_SETCURSEL, index, 0);

	is_modified = true;
}

void VNDEventEdit::OnBtnInsertEvent() {
	HWND	hWndLB = GetDlgItem (hWnd, IDC_LBEVENT);

	// イベントリストの選択項目のインデックスを取得
	int index = SendMessage (hWndLB, LB_GETCURSEL, 0, 0);
	if( index == LB_ERR )
		index = -1;

	// 項目数を取得（最後尾の空の行が一つ含まれる）
	int count = SendMessage (hWndLB, LB_GETCOUNT, 0, 0);
	if( count == LB_ERR )
		return;	// 不明なエラー

	// コンボボックスで選択されているイベントタイプを取得
	HWND	hWndCB = GetDlgItem (hWnd, IDC_CBEVENTNAME);
	int code = SendMessage (hWndCB, CB_GETCURSEL, 0, 0);
	if( code == CB_ERR )
		return;

	// イベント作成＆編集
	VNEvent *e = new VNEvent( code + 1 );
	bool res = EditEvent( e );
	if( res == false ) {		// イベント追加がキャンセルされた
		delete e;
		return;
	}

	// イベントリストに追加
	if( ! event_file->InsertEvent( e, index ) ) {
		MessageBox (hWnd, "不明なエラー：イベントを追加できない", "VNDEventEdit.cpp", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	// 最後尾の空の行の手前に追加するため
	if( index == -1 )
		index = count - 1;

	// リストボックスに項目を追加
	InsertItemToListBox( e->GetExplanation().c_str(), index );

	// リストボックスにフォーカスを設定
	SetFocus (hWndLB);
	SendMessage (hWndLB, LB_SETCURSEL, index+1, 0);

	is_modified = true;
	return;
}

void VNDEventEdit::OnBtnEdit() {
	HWND	hWndLB = GetDlgItem (hWnd, IDC_LBEVENT);

	// イベントリストの選択項目のインデックスを取得
	int index = SendMessage (hWndLB, LB_GETCURSEL, 0, 0);
	if( index == LB_ERR )
		return;

	// 項目数を取得（最後尾の空の行が一つ含まれる）
	int count = SendMessage (hWndLB, LB_GETCOUNT, 0, 0);
	if( count == LB_ERR )
		return;	// 不明なエラー

	// 選択されたのが最後尾の空の行なら何もしない
	if( index == count - 1 )
		return;

	VNEvent *e = event_file->GetEvent( index );
	if( e == NULL )
		DebugBreak();
	bool res = EditEvent( e );
	if( res == false )	// イベント編集がキャンセルされた
		return;

	// リストボックスの項目を更新
	if( SendMessage (hWndLB, LB_DELETESTRING, index, 0) == LB_ERR )
		DebugBreak();
	InsertItemToListBox( e->GetExplanation().c_str(), index );

	// リストボックスにフォーカスを設定
	SetFocus (hWndLB);
	SendMessage (hWndLB, LB_SETCURSEL, index, 0);

	is_modified = true;
	return;
}

void VNDEventEdit::OnBtnCopy() {
	HWND	hWndLB = GetDlgItem (hWnd, IDC_LBEVENT);

	// 選択項目のインデックスを取得
	int index = SendMessage (hWndLB, LB_GETCURSEL, 0, 0);
	if( index == LB_ERR )
		return;	// 項目が選択されていない

	// 項目数を取得（最後尾の空の行が一つ含まれる）
	int count = SendMessage (hWndLB, LB_GETCOUNT, 0, 0);
	if( count == LB_ERR )
		return;	// 不明なエラー

	// 選択されたのが最後尾の空の行なら何もしない
	if( index == count - 1 )
		return;

	// 現在の保持しているコピーされたイベントを削除
	if( copy_item != NULL )
		delete copy_item;

	// イベントをコピーして保存
	VNEvent *evt = event_file->GetEvent( index );
	if( evt == NULL )
		DebugBreak();
	copy_item = new VNEvent( *evt );
}

void VNDEventEdit::OnBtnPaste() {
	// コピーされたイベントがなければリターン
	if( copy_item == NULL )
		return;

	HWND	hWndLB = GetDlgItem (hWnd, IDC_LBEVENT);

	// イベントリストの選択項目のインデックスを取得
	int index = SendMessage (hWndLB, LB_GETCURSEL, 0, 0);
	if( index == LB_ERR )
		index = -1;

	// 項目数を取得（最後尾の空の行が一つ含まれる）
	int count = SendMessage (hWndLB, LB_GETCOUNT, 0, 0);
	if( count == LB_ERR )
		return;	// 不明なエラー

	// イベントオブジェクトを複製
	VNEvent *evt = new VNEvent( *copy_item );

	// イベントリストに追加
	if( ! event_file->InsertEvent( evt, index ) ) {
		MessageBox (hWnd, "不明なエラー：イベントを追加できない", "VNDEventEdit.cpp", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	// 最後尾の空の行の手前に追加するため
	if( index == -1 )
		index = count - 1;

	// リストボックスに項目を追加
	InsertItemToListBox( evt->GetExplanation().c_str(), index );

	// リストボックスにフォーカスを設定
	SetFocus (hWndLB);
	SendMessage (hWndLB, LB_SETCURSEL, index, 0);

	is_modified = true;
	return;
}

void VNDEventEdit::OnBtnDelete() {
	HWND	hWndLB = GetDlgItem (hWnd, IDC_LBEVENT);

	// 選択項目のインデックスを取得
	int index = SendMessage (hWndLB, LB_GETCURSEL, 0, 0);
	if( index == LB_ERR )
		return;	// 項目が選択されていない

	// 項目数を取得（最後尾の空の行が一つ含まれる）
	int count = SendMessage (hWndLB, LB_GETCOUNT, 0, 0);
	if( count == LB_ERR )
		return;	// 不明なエラー

	// 選択されたのが最後尾の空の行なら何もしない
	if( index == count - 1 )
		return;

	// 選択項目を削除
	if( SendMessage (hWndLB, LB_DELETESTRING, index, 0) == LB_ERR )
		return;

	// event_fileから削除
	if( ! event_file->DeleteEvent( index ) )
		DebugBreak();

	// リストボックスにフォーカスを設定し項目を選択
	SetFocus (hWndLB);
	SendMessage (hWndLB, LB_SETCURSEL, index, 0);

	is_modified = true;
	return;
}

void VNDEventEdit::InsertItemToListBox( const char *str, int n /* -1 */ ) {
	HWND	hWndLB = GetDlgItem (hWnd, IDC_LBEVENT);
	if( SendMessage (hWndLB, LB_INSERTSTRING, n, (LPARAM)(LPCTSTR) str) == LB_ERR )
		DebugBreak();
}

void VNDEventEdit::InitListBoxItem() {
	HWND	hWndLB = GetDlgItem (hWnd, IDC_LBEVENT);
	::SendMessage (hWndLB, LB_RESETCONTENT, (WPARAM) 0, (LPARAM) 0);

	// ファイルからロードした項目を追加（新規作成の場合追加されない）
	int cnt = event_file->GetCount();
	for( int i = 0; i < cnt; i++ ) {
		VNEvent *e;
		e = event_file->GetEvent( i );
		if( e == NULL )
			DebugBreak();

		VNString a = e->GetExplanation();
		InsertItemToListBox( a.c_str() );
	}

	// 空の行を最後尾に追加
	InsertItemToListBox( "" );
	return;
}

void VNDEventEdit::InitListBoxFont() {
	HWND hWndLB = GetDlgItem (hWnd, IDC_LBEVENT);

	if( hFont == NULL ) {
		// 見やすいように大きいフォントを設定
		hFont = ::CreateFont(
			LB_FONT_HEIGHT, LB_FONT_WIDTH,
			0, 0,
			10, FALSE,
			FALSE, FALSE,
			SHIFTJIS_CHARSET,
			OUT_TT_ONLY_PRECIS,
			CLIP_DEFAULT_PRECIS,
			PROOF_QUALITY,
			FF_MODERN | FIXED_PITCH,
			LB_FONT_NAME );
		if( hFont != NULL )
			::SendMessage (hWndLB, WM_SETFONT, (WPARAM) hFont, MAKELPARAM(TRUE, 0));

		// 見やすいように項目の高さ（ピクセル数）を設定
		::SendMessage (hWndLB, LB_SETITEMHEIGHT , 0, MAKELPARAM(LB_CY_ITEM, 0));
	}
}

void VNDEventEdit::InitComboBox() {
	static char *item[EVENTCODE_MAX] = {
		"ナレーション",
		"セリフ",
		"キャラクタ",
		"背景",
		"選択肢",
		"ラベル",
		"ＣＤ－ＤＡ",
		"ＭＩＤＩ",
		"ＰＣＭ",
		"フラグ設定",
		"ジャンプ",
		"タイトル",
		"クリックウェイト",
		"ミリ秒ウェイト",
		"メッセージボックス設定",
		"コメント",
	};

	HWND	hWndCB = GetDlgItem (hWnd, IDC_CBEVENTNAME);
	for( int i = 0; i < EVENTCODE_MAX; i++ )
		if( SendMessage (hWndCB, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR) item[i]) == CB_ERR )
			DebugBreak();

	SendMessage (hWndCB, CB_SETCURSEL, 0, 0);
}

void VNDEventEdit::DebugBreak() {
	MessageBox (NULL,
				"内部エラーが発生しました。",
				"イベントエディタ",
				MB_ICONEXCLAMATION | MB_OK);
	abort();
}

bool VNDEventEdit::EditEvent( VNEvent *e ) {
	bool res;
	switch( e->GetDescriptor().code ) {
	case EVENTCODE_NARRATION:	res = EditNarration( e );	break;
	case EVENTCODE_SERIF:		res = EditSerif( e );		break;
	case EVENTCODE_CHARACTER:	res = EditCharacter( e );	break;
	case EVENTCODE_BACKGROUND:	res = EditBackground( e );	break;
	case EVENTCODE_SELECT:		res = EditSelect( e );		break;
	case EVENTCODE_LABEL:		res = EditLabel( e );		break;
	case EVENTCODE_SETFLAG:		res = EditFlag( e );		break;
	case EVENTCODE_CDDA:		res = EditCdda( e );		break;
	case EVENTCODE_MIDI:		res = EditMidi( e );		break;
	case EVENTCODE_PCM:			res = EditPcm( e );			break;
	case EVENTCODE_JUMP:		res = EditJump( e );		break;
	case EVENTCODE_WAITCLICK:	res = true;					break;
	case EVENTCODE_WAIT:		res = EditWait( e );		break;
	case EVENTCODE_TITLE:		res = EditTitle( e );		break;
	case EVENTCODE_COMMENT:		res = EditComment( e );		break;
	default:	return false;
	}
	return res;
}

bool VNDEventEdit::EditNarration( VNEvent *e ) {
	if( e->GetDescriptor().code != EVENTCODE_NARRATION )
		DebugBreak();

	VNString text = e->GetParamString( 0 );
	VNDNarration dlg;
	if( ! dlg.DoModal( text, hWnd) )
		return false;

	e->SetParamString( 0, text.c_str() );
	return true;
}

bool VNDEventEdit::EditSerif( VNEvent *e ) {
	if( e->GetDescriptor().code != EVENTCODE_SERIF )
		DebugBreak();

	VNString name = e->GetParamString( 0 );
	VNString serif = e->GetParamString( 1 );

	VNDSerif dlg;
	if( ! dlg.DoModal( name, serif, hWnd) )
		return false;

	e->SetParamString( 0, name.c_str() );
	e->SetParamString( 1, serif.c_str() );
	return true;
}

bool VNDEventEdit::EditCharacter( VNEvent *e ) {
	if( e->GetDescriptor().code != EVENTCODE_CHARACTER )
		DebugBreak();

	int	h_align = e->GetParamInt( 0 );
	int	v_align = e->GetParamInt( 1 );
	VNString color = e->GetParamString( 2 );
	VNString mask = e->GetParamString( 3 );

	VNDCharacter dlg;
	if( ! dlg.DoModal( h_align, v_align, color, mask, hWnd) )
		return false;

	e->SetParamInt( 0, h_align );
	e->SetParamInt( 1, v_align );
	e->SetParamString( 2, color.c_str());
	e->SetParamString( 3, mask.c_str() );
	return true;
}

bool VNDEventEdit::EditBackground( VNEvent *e ) {
	if( e->GetDescriptor().code != EVENTCODE_BACKGROUND )
		DebugBreak();

	VNString text = e->GetParamString( 0 );
	VNDBackground dlg;
	if( ! dlg.DoModal( text, hWnd) )
		return false;

	e->SetParamString( 0, text.c_str() );
	return true;
}

bool VNDEventEdit::EditSelect( VNEvent *e ) {
	if( e->GetDescriptor().code != EVENTCODE_SELECT )
		DebugBreak();

	VNString *msg[3];
	VNString *label[3];
	for( int i = 0; i < 3; i++ ) {
		msg[i] = new VNString( e->GetParamString( i*2 ) );
		label[i] = new VNString( e->GetParamString( i*2+1 ) );
	}

	VNDSelect dlg;
	if( ! dlg.DoModal( msg, label, hWnd) )
		return false;

	for( i = 0; i < 3; i++ ) {
		e->SetParamString( i*2, msg[i]->c_str() );
		e->SetParamString( i*2+1, label[i]->c_str() );
		delete msg[i];
		delete label[i];
	}
	return true;
}

bool VNDEventEdit::EditLabel( VNEvent *e ) {
	if( e->GetDescriptor().code != EVENTCODE_LABEL )
		DebugBreak();

	VNString text = e->GetParamString( 0 );
	VNDLabel dlg;
	if( ! dlg.DoModal( text, hWnd) )
		return false;

	e->SetParamString( 0, text.c_str() );
	return true;
}

bool VNDEventEdit::EditFlag( VNEvent *e ) {
	if( e->GetDescriptor().code != EVENTCODE_SETFLAG )
		DebugBreak();

	VNString name = e->GetParamString( 0 );
	int val = e->GetParamInt( 1 );

	VNDFlag dlg;
	if( ! dlg.DoModal( name, val, hWnd) )
		return false;

	e->SetParamString( 0, name.c_str());
	e->SetParamInt( 1, val);
	return true;
}

bool VNDEventEdit::EditCdda( VNEvent *e ) {
	if( e->GetDescriptor().code != EVENTCODE_CDDA )
		DebugBreak();

	int track = e->GetParamInt( 0 );
	int fade = e->GetParamInt( 1 );

	VNDCdda dlg;
	if( ! dlg.DoModal( track, fade, hWnd) )
		return false;

	e->SetParamInt( 0, track );
	e->SetParamInt( 1, fade );
	return true;
}

bool VNDEventEdit::EditMidi( VNEvent *e ) {
	if( e->GetDescriptor().code != EVENTCODE_MIDI )
		DebugBreak();

	VNString file = e->GetParamString( 0 );
	int fade = e->GetParamInt( 1 );

	VNDMidi dlg;
	if( ! dlg.DoModal( file, fade, hWnd) )
		return false;

	e->SetParamString( 0, file.c_str() );
	e->SetParamInt( 1, fade );
	return true;
}

bool VNDEventEdit::EditPcm( VNEvent *e ) {
	if( e->GetDescriptor().code != EVENTCODE_PCM )
		DebugBreak();

	VNString file = e->GetParamString( 0 );

	VNDPcm dlg;
	if( ! dlg.DoModal( file, hWnd) )
		return false;

	e->SetParamString( 0, file.c_str() );
	return true;
}

bool VNDEventEdit::EditJump( VNEvent *e ) {
	if( e->GetDescriptor().code != EVENTCODE_JUMP )
		DebugBreak();

	VNString event = e->GetParamString( 0 );
	VNString label = e->GetParamString( 1 );
	int	condition = e->GetParamInt( 2 );
	VNString flag = e->GetParamString( 3 );

	VNDJump dlg;
	if( ! dlg.DoModal( event, label, condition, flag, hWnd) )
		return false;

	e->SetParamString( 0, event.c_str() );
	e->SetParamString( 1, label.c_str() );
	e->SetParamInt( 2, condition );
	e->SetParamString( 3, flag.c_str() );
	return true;
}

bool VNDEventEdit::EditWait( VNEvent *e ) {
	if( e->GetDescriptor().code != EVENTCODE_WAIT )
		DebugBreak();

	int millisec = e->GetParamInt( 0 );

	VNDWait dlg;
	if( ! dlg.DoModal( millisec, hWnd) )
		return false;

	e->SetParamInt( 0, millisec );
	return true;
}

bool VNDEventEdit::EditComment( VNEvent *e ) {
	if( e->GetDescriptor().code != EVENTCODE_COMMENT )
		DebugBreak();

	VNString text = e->GetParamString( 0 );
	VNDComment dlg;
	if( ! dlg.DoModal( text, hWnd) )
		return false;

	e->SetParamString( 0, text.c_str() );
	return true;
}

bool VNDEventEdit::EditTitle( VNEvent *e ) {
	if( e->GetDescriptor().code != EVENTCODE_TITLE )
		DebugBreak();

	VNString text = e->GetParamString( 0 );
	VNDTitle dlg;
	if( ! dlg.DoModal( text, hWnd) )
		return false;

	e->SetParamString( 0, text.c_str() );
	return true;
}
