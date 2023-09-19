#include <string.h>
#include <windows.h>
#include "resource.h"
#include "VNDJump.h"


bool VNDJump::DoModal( VNString& event, VNString& label, int& condition, VNString& flag, HWND hWndParent ) {
	cb = condition;
	edit_event = new char [1024];
	strcpy( edit_event, event.c_str() );
	edit_label = new char [1024];
	strcpy( edit_label, label.c_str() );
	edit_flag = new char [1024];
	strcpy( edit_flag, flag.c_str() );

	int b = VNDialog::DoModal( "ジャンプ", "IDD_JUMP", hWndParent );
	bool res;

	if( b == TRUE  &&  cb != CB_ERR ) {
		condition = cb;
		event = edit_event;
		label = edit_label;
		flag = edit_flag;
		res = true;
	} else
		res = false;

	delete edit_event;
	delete edit_label;
	delete edit_flag;
	return res;
}

bool VNDJump::OnInit() {
	// エディットコントロールにファイル名をセット
	SetWindowText (GetDlgItem (hWnd, IDC_EDITEVENT), edit_event);
	SetWindowText (GetDlgItem (hWnd, IDC_EDITLABEL), edit_label);
	SetWindowText (GetDlgItem (hWnd, IDC_EDITFLAG), edit_flag);

	// コンボボックスに項目を追加して選択
	HWND	hWndCB = GetDlgItem (hWnd, IDC_COMBO);
	static char *cb_item[] = { "条件なし（強制）", "フラグがＯＮである", "フラグがＯＦＦである" };

	for( int i = 0; i < 3; i++ )
		SendMessage (hWndCB, CB_ADDSTRING, 0, (LPARAM) (LPCTSTR) cb_item[i]);

	SendMessage (hWndCB, CB_SETCURSEL, cb, 0);
	return FALSE;
}

void VNDJump::OnCommand( WPARAM wParam, LPARAM lParam ) {
	WORD	wNotifyCode = HIWORD(wParam);	// 通知コード 
	WORD	wID = LOWORD(wParam);			// 項目、コントロール、またはアクセラレータの ID 
	HWND	hWndCtl = (HWND) lParam;		// コントロールのハンドル 

	switch( wID ) {
	case IDOK:
		GetWindowText (GetDlgItem (hWnd, IDC_EDITEVENT), edit_event, 1023);
		GetWindowText (GetDlgItem (hWnd, IDC_EDITLABEL), edit_label, 1023);
		GetWindowText (GetDlgItem (hWnd, IDC_EDITFLAG), edit_flag, 1023);
		cb = SendMessage (GetDlgItem (hWnd, IDC_COMBO), CB_GETCURSEL, 0, 0);
		EndDialog (hWnd, TRUE);
		return;

	case IDCANCEL:
		EndDialog (hWnd, FALSE);
		return;
	}
}
