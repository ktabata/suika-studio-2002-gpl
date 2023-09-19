#include <string.h>
#include <windows.h>
#include "resource.h"
#include "VNDPcm.h"


bool VNDPcm::DoModal( VNString& text, HWND hWndParent ) {
	edit_string = new char [4096];
	strcpy( edit_string, text.c_str() );

	int b = VNDialog::DoModal( "PCM 演奏", "IDD_PCM", hWndParent );
	bool res;

	if( b == TRUE ) {
		text = edit_string;
		res = true;
	} else
		res = false;

	delete edit_string;
	return res;
}

bool VNDPcm::OnInit() {
	HWND	hWndEdit = GetDlgItem (hWnd, IDC_EDIT);
	SetWindowText (hWndEdit, edit_string);
	SetFocus (GetDlgItem (hWnd, IDC_EDIT) );
	return FALSE;
}

void VNDPcm::OnCommand( WPARAM wParam, LPARAM lParam ) {
	WORD	wNotifyCode = HIWORD(wParam);	// 通知コード 
	WORD	wID = LOWORD(wParam);			// 項目、コントロール、またはアクセラレータの ID 
	HWND	hWndCtl = (HWND) lParam;		// コントロールのハンドル 

	switch( wID ) {
	case IDOK:
		GetWindowText (GetDlgItem (hWnd, IDC_EDIT), edit_string, 4098);
		EndDialog (hWnd, TRUE);
		return;

	case IDCANCEL:
		EndDialog (hWnd, FALSE);
		return;
	}
}
