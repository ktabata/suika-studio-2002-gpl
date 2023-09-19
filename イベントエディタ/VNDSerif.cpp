#include <string.h>
#include <windows.h>
#include "resource.h"
#include "VNDSerif.h"


bool VNDSerif::DoModal( VNString& name, VNString& serif, HWND hWndParent ) {
	edit_name = new char [1024];
	strcpy( edit_name, name.c_str() );
	edit_serif = new char [65536];
	strcpy( edit_serif, serif.c_str() );

	int b = VNDialog::DoModal( "セリフ", "IDD_SERIF", hWndParent );
	bool res;

	if( b == TRUE ) {
		name = edit_name;
		serif = edit_serif;
		res = true;
	} else
		res = false;

	delete edit_name;
	delete edit_serif;
	return res;
}

bool VNDSerif::OnInit() {
	HWND hWndName = GetDlgItem (hWnd, IDC_EDITNAME);

	SetWindowText (hWndName, edit_name);
	SetWindowText (GetDlgItem (hWnd, IDC_EDITSERIF), edit_serif);
	SetFocus (hWndName);
	return FALSE;
}

void VNDSerif::OnCommand( WPARAM wParam, LPARAM lParam ) {
	WORD	wNotifyCode = HIWORD(wParam);	// 通知コード 
	WORD	wID = LOWORD(wParam);			// 項目、コントロール、またはアクセラレータの ID 
	HWND	hWndCtl = (HWND) lParam;		// コントロールのハンドル 

	switch( wID ) {
	case IDOK:
		GetWindowText (GetDlgItem (hWnd, IDC_EDITNAME), edit_name, 1023);
		GetWindowText (GetDlgItem (hWnd, IDC_EDITSERIF), edit_serif, 65535);
		EndDialog (hWnd, TRUE);
		return;

	case IDCANCEL:
		EndDialog (hWnd, FALSE);
		return;
	}
}
