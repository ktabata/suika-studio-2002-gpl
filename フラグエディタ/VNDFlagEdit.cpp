#include "VNDFlagEdit.h"
#include "VNFileDialog.h"
#include "resource.h"


bool VNDFlagEdit::DoModal( const char *title, const char *file_name, HWND hWndParent ) {
	flag_file = new VNFlagFile();
	this->file_name = file_name;
	is_modified = false;

	if( strcmp( file_name, "" ) != 0 ) {
		if( ! flag_file->Load( file_name ) ) {
			// 読み込みに失敗したらオブジェクトを構築し直す
			delete flag_file;
			flag_file = new VNFlagFile();
		}
	} else
		flag_file = new VNFlagFile();

	int res = VNDialog::DoModal( title, "IDD_FLAGEDIT", hWndParent );
	if( res )
		flag_file->Save( file_name );

	return true;
}

int VNDFlagEdit::ClosePrompt() {
	return MessageBox (hWnd,
					   "変更を保存しますか？",
					   "フラグエディタ",
					   MB_YESNOCANCEL | MB_ICONQUESTION);
}

bool VNDFlagEdit::OnInit() {
	if( ! InitListBox() )
		EndDialog( hWnd, FALSE );
	return true;
}

void VNDFlagEdit::OnClose() {
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

void VNDFlagEdit::OnCommand (WPARAM wParam, LPARAM lParam)
{
	WORD	wNotifyCode = HIWORD(wParam);	// 通知コード 
	WORD	wID = LOWORD(wParam);			// 項目、コントロール、またはアクセラレータの ID 
	HWND	hWndCtl = (HWND) lParam;		// コントロールのハンドル 

	switch (wID)	{
	case IDC_BTNOPEN:
		OnBtnOpen();
		return;
	case IDC_BTNDEL:
		OnBtnDelete();
		return;
	case IDC_BTNADD:
		OnBtnAdd();
		return;
	case IDC_BTNONOFF:
		OnBtnOnOff();
		return;
	case IDC_BTNSAVE:
		OnBtnSave();
		return;
	case IDC_BTNCLOSE:
		OnClose();
	}
}

void VNDFlagEdit::OnBtnOpen() {
	if( is_modified ) {
		switch( ClosePrompt() ) {
		case IDCANCEL:
			return;
		case IDYES:
			OnBtnSave();
			break;
		case IDNO:
			break;
		}
	}

	bool b = GetOpenFileNameIndirect( "ファイルを開く", "ﾌﾗｸﾞﾌｧｲﾙ(*.flg)\0*.flg\0",
									  "", file_name );
	if( b == false )
		return;

	VNFlagFile *new_flg = new VNFlagFile();
	if( ! new_flg->Load( file_name.c_str() ) ) {
		// 読み込みに失敗したらオブジェクトを構築し直す
		delete new_flg;
		::MessageBox (hWnd, "ロードに失敗しました。", "フラグエディタ", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	delete flag_file;
	flag_file = new_flg;

	InitListBox();
}


void VNDFlagEdit::OnBtnSave() {
	if( strcmp( file_name.c_str(), "" ) == 0 ) {
		bool b = GetSaveFileNameIndirect( "名前を付けて保存", "ﾌﾗｸﾞﾌｧｲﾙ(*.flg)\0*.flg\0",
										  "新規フラグ.flg", file_name );
		if( b == false )
			return;
	}

	if( ! flag_file->Save( file_name.c_str() ) )
		MessageBox (hWnd, "セーブに失敗しました。", "エラー", MB_ICONEXCLAMATION|MB_OK);
	is_modified = false;
}

void VNDFlagEdit::OnBtnDelete() {
	HWND	hWndLB = GetDlgItem (hWnd, IDC_FLAGLIST);

	// 選択項目のインデックスを取得
	int sel = SendMessage (hWndLB, LB_GETCURSEL, 0, 0);
	if( sel == LB_ERR )
		return;	// 項目が選択されていない

	// 選択項目のテキストを取得
	char *buf = new char[1024];
	if( SendMessage (hWndLB, LB_GETTEXT, sel, (LPARAM)(LPCTSTR) buf) == LB_ERR ) {
		// 異常なので何もせずにリターン
		delete buf;
		return;
	}

	// 選択項目を削除
	if( SendMessage (hWndLB, LB_DELETESTRING, sel, 0) == LB_ERR ) {
		// 異常なので何もせずにリターン
		delete buf;
		return;
	}

	// flag_fileから削除
	buf[ strlen(buf) - 3 ] = '\0';	// 末尾の" ○"あるいは" ×"を無視する
	if( ! flag_file->DeleteFlag( buf ) )
		DebugBreak();

	// リストボックスにフォーカスを設定
	SetFocus (GetDlgItem (hWnd, IDC_FLAGLIST) );
	int count = SendMessage (hWndLB, LB_GETCOUNT, 0, 0);
	if( count != LB_ERR ) {
		// 削除したのが最後の項目なら
		if( sel == count )
			SendMessage (hWndLB, LB_SETCURSEL, sel - 1, 0);
		else	// 最後以外の項目なら
			SendMessage (hWndLB, LB_SETCURSEL, sel, 0);
	}

	is_modified = true;
	return;
}

void VNDFlagEdit::OnBtnAdd() {
	VNString flag_name;

	VNDFlagEdit_Add add_dlg;
	if( ! add_dlg.DoModal( flag_name, hWnd) )
		return;

	if( ! flag_file->AddFlag( flag_name.c_str(), false ) ) {
		MessageBox (hWnd,
					"同じ名前のフラグがすでに存在しています。",
					"フラグエディタ",
					MB_OK | MB_ICONINFORMATION);
		return;
	}

	// リストボックスに項目を追加
	AddItemToListBox( flag_name.c_str(), false );

	is_modified = true;
	return;
}

void VNDFlagEdit::OnBtnOnOff() {
	HWND	hWndLB = GetDlgItem (hWnd, IDC_FLAGLIST);

	// 選択項目のインデックスを取得
	int sel = SendMessage (hWndLB, LB_GETCURSEL, 0, 0);
	if( sel == LB_ERR )
		return;

	// 選択項目のテキストを取得
	char *buf = new char[1024];
	if( SendMessage (hWndLB, LB_GETTEXT, sel, (LPARAM)(LPCTSTR) buf) == LB_ERR ) {
		// 異常なので何もせずにリターン
		delete buf;
		return;
	}

	// 選択項目を削除
	if( SendMessage (hWndLB, LB_DELETESTRING, sel, (LPARAM)(LPCTSTR) buf) == LB_ERR ) {
		// 異常なので何もせずにリターン
		delete buf;
		return;
	}

	// フラグのブーリアン値を反転
	buf[ strlen(buf) - 3 ] = '\0';	// 末尾の" ○"あるいは" ×"を無視する
	int n = flag_file->SearchFlag( buf );
	if( n < 0 )
		DebugBreak();

	if( ! flag_file->InvertFlagValue( n ) )
		DebugBreak();

	bool val;
	if( ! flag_file->GetFlagValue( n, val ) )
		DebugBreak();

	// リストボックスに項目を追加してフォーカスを設定
	AddItemToListBox( buf, val );
	SetFocus (GetDlgItem (hWnd, IDC_FLAGLIST) );
	if( SendMessage (hWndLB, LB_SETCURSEL, sel, 0) == LB_ERR ) {
		// 異常なので何もせずにリターン
		delete buf;
		return;
	}

	is_modified = true;
	return;
}

void VNDFlagEdit::AddItemToListBox( const char *name, bool val ) {
	if( name == NULL )
		DebugBreak();

	HWND	hWndLB = GetDlgItem (hWnd, IDC_FLAGLIST);

	char *buf = new char[ strlen(name) + 4 ]; // len + strlen("× ") + 1('\0')
	strcpy( buf, name );
	if( val )	strcat( buf, " ○" );
	else		strcat( buf, " ×" );

	if( SendMessage (hWndLB, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR) buf) == LB_ERR ) {
		// 異常なので何もせずにリターン
		delete buf;
		return;
	}
	delete[] buf;
}

bool VNDFlagEdit::InitListBox() {
	HWND	hWndLB = GetDlgItem (hWnd, IDC_FLAGLIST);
	::SendMessage (hWndLB, LB_RESETCONTENT, (WPARAM) 0, (LPARAM) 0);

	int cnt = flag_file->GetCount();
	for( int i = 0; i < cnt; i++ ) {
		VNString name;
		bool val;

		if( ! flag_file->GetFlagName( i, name ) ) 
			DebugBreak();
		
		if( ! flag_file->GetFlagValue( i, val ) )
			DebugBreak();

		AddItemToListBox( name.c_str(), val );
	}

	return true;
}

void VNDFlagEdit::DebugBreak() {
	MessageBox (hWnd, "内部エラー：要デバッグ！", "VNDFlagEdit", MB_ICONEXCLAMATION|MB_OK);
	abort();
}
