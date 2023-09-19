#include <string.h>
#include <windows.h>
#include "resource.h"
#include "VNDFlag.h"


bool VNDFlag::DoModal( VNString& flagname, int& val, HWND hWndParent ) {
	cb = val;
	edit = new char [512];
	strcpy( edit, flagname.c_str() );

	int b = VNDialog::DoModal( "フラグ", "IDD_FLAG", hWndParent );
	bool res;

	if( b == TRUE  &&  cb != CB_ERR ) {
		val = cb;
		flagname = edit;
		res = true;
	} else
		res = false;

	delete edit;
	return res;
}

bool VNDFlag::OnInit() {
//	SetFocus (GetDlgItem (hWnd, IDC_EDITCOLOR));
	// エディットコントロールにファイル名をセット
	SetWindowText (GetDlgItem (hWnd, IDC_EDIT), edit);

	// コンボボックスに項目を追加して選択
	HWND	hWndCB = GetDlgItem (hWnd, IDC_COMBO);
	static char *cb_item[] = { "ＯＮ", "ＯＦＦ", "反転" };
	for( int i = 0; i < 3; i++ )
		SendMessage (hWndCB, CB_ADDSTRING, 0, (LPARAM) (LPCTSTR) cb_item[i]);

	SendMessage (hWndCB, CB_SETCURSEL, cb, 0);
	return FALSE;
}

void VNDFlag::OnCommand( WPARAM wParam, LPARAM lParam ) {
	WORD	wNotifyCode = HIWORD(wParam);	// 通知コード 
	WORD	wID = LOWORD(wParam);			// 項目、コントロール、またはアクセラレータの ID 
	HWND	hWndCtl = (HWND) lParam;		// コントロールのハンドル 

	switch( wID ) {
	case IDOK:
		GetWindowText (GetDlgItem (hWnd, IDC_EDIT), edit, 511);
		cb = SendMessage (GetDlgItem (hWnd, IDC_COMBO), CB_GETCURSEL, 0, 0);
		EndDialog (hWnd, TRUE);
		return;

	case IDCANCEL:
		EndDialog (hWnd, FALSE);
		return;
	}
}
