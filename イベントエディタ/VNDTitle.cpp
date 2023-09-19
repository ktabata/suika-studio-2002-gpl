#include <string.h>
#include <windows.h>
#include "resource.h"
#include "VNDTitle.h"


bool VNDTitle::DoModal( VNString& text, HWND hWndParent ) {
	edit_string = new char [1024];
	strcpy( edit_string, text.c_str() );

	int b = VNDialog::DoModal( "タイトル", "IDD_Title", hWndParent );
	bool res;

	if( b == TRUE ) {
		text = edit_string;
		res = true;
	} else
		res = false;

	delete edit_string;
	return res;
}

bool VNDTitle::OnInit() {
	HWND	hWndEdit = GetDlgItem (hWnd, IDC_EDIT);
	SetWindowText (hWndEdit, edit_string);
	SetFocus (GetDlgItem (hWnd, IDC_EDIT) );
	return FALSE;
}

void VNDTitle::OnCommand( WPARAM wParam, LPARAM lParam ) {
	WORD	wNotifyCode = HIWORD(wParam);	// 通知コード 
	WORD	wID = LOWORD(wParam);			// 項目、コントロール、またはアクセラレータの ID 
	HWND	hWndCtl = (HWND) lParam;		// コントロールのハンドル 

	switch( wID ) {
	case IDOK:
		GetWindowText (GetDlgItem (hWnd, IDC_EDIT), edit_string, 1023);
		EndDialog (hWnd, TRUE);
		return;

	case IDCANCEL:
		EndDialog (hWnd, FALSE);
		return;
	}
}
