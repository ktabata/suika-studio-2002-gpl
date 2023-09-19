#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "resource.h"
#include "VNDWait.h"


bool VNDWait::DoModal( int& millisec, HWND hWndParent ) {
	edit_string = new char [64];
	sprintf( edit_string, "%d", millisec );

	int b = VNDialog::DoModal( "ミリ秒ウェイト", "IDD_WAIT", hWndParent );
	bool res;

	if( b == TRUE ) {
		millisec = atoi( edit_string );
		res = true;
	} else
		res = false;

	delete edit_string;
	return res;
}

bool VNDWait::OnInit() {
	SetWindowText (GetDlgItem (hWnd, IDC_EDIT), edit_string);
	return true;
}

void VNDWait::OnCommand( WPARAM wParam, LPARAM lParam ) {
	WORD	wNotifyCode = HIWORD(wParam);	// 通知コード 
	WORD	wID = LOWORD(wParam);			// 項目、コントロール、またはアクセラレータの ID 
	HWND	hWndCtl = (HWND) lParam;		// コントロールのハンドル 

	switch( wID ) {
	case IDOK:
		GetWindowText (GetDlgItem (hWnd, IDC_EDIT), edit_string, 63);
		EndDialog (hWnd, TRUE);
		return;

	case IDCANCEL:
		EndDialog (hWnd, FALSE);
		return;
	}
}
