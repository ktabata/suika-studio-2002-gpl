#include <string.h>
#include <windows.h>
#include "resource.h"
#include "VNDLabel.h"


bool VNDLabel::DoModal( VNString& text, HWND hWndParent ) {
	edit_string = new char [512];
	strcpy( edit_string, text.c_str() );

	int b = VNDialog::DoModal( "ラベル", "IDD_LABEL", hWndParent );
	bool res;

	if( b == TRUE ) {
		text = edit_string;
		res = true;
	} else
		res = false;

	delete edit_string;
	return res;
}

bool VNDLabel::OnInit() {
	HWND	hWndEdit = GetDlgItem (hWnd, IDC_EDIT);
	SetWindowText (hWndEdit, edit_string);
	SetFocus (GetDlgItem (hWnd, IDC_EDIT) );
	return FALSE;
}

void VNDLabel::OnCommand( WPARAM wParam, LPARAM lParam ) {
	WORD	wNotifyCode = HIWORD(wParam);	// 通知コード 
	WORD	wID = LOWORD(wParam);			// 項目、コントロール、またはアクセラレータの ID 
	HWND	hWndCtl = (HWND) lParam;		// コントロールのハンドル 

	switch( wID ) {
	case IDOK:
		GetWindowText (GetDlgItem (hWnd, IDC_EDIT), edit_string, 511);
		EndDialog (hWnd, TRUE);
		return;

	case IDCANCEL:
		EndDialog (hWnd, FALSE);
		return;
	}
}
