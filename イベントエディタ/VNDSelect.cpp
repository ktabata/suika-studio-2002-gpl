#include <assert.h>
#include <string.h>
#include <windows.h>
#include "resource.h"
#include "VNDSelect.h"


bool VNDSelect::DoModal( VNString *msg[3], VNString *label[3], HWND hWndParent ) {
	if( msg == NULL || label == NULL )
		return false;

	for( int i = 0; i < 3; i ++ ) {
		assert( msg[i] != NULL );
		assert( label[i] != NULL );

		edit_msg[i] = new char [1024];
		edit_label[i] = new char [512];
		msg[i]->CopyTo( edit_msg[i] );
		label[i]->CopyTo( edit_label[i] );
	}

	int b = VNDialog::DoModal( "選択肢", "IDD_SELECT", hWndParent );
	bool res;

	if( b == TRUE ) {
		for( i = 0; i < 3; i ++ ) {
			*msg[i] = edit_msg[i];
			*label[i] = edit_label[i];
		}
		res = true;
	} else
		res = false;

	for( i = 0; i < 3; i ++ ) {
		delete edit_msg[i];
		delete edit_label[i];
	}
	return res;
}

bool VNDSelect::OnInit() {
	SetWindowText (GetDlgItem (hWnd, IDC_EDITMSG1), edit_msg[0]);
	SetWindowText (GetDlgItem (hWnd, IDC_EDITMSG2), edit_msg[1]);
	SetWindowText (GetDlgItem (hWnd, IDC_EDITMSG3), edit_msg[2]);
	SetWindowText (GetDlgItem (hWnd, IDC_EDITLABEL1), edit_label[0]);
	SetWindowText (GetDlgItem (hWnd, IDC_EDITLABEL2), edit_label[1]);
	SetWindowText (GetDlgItem (hWnd, IDC_EDITLABEL3), edit_label[2]);
	return TRUE;
}

void VNDSelect::OnCommand( WPARAM wParam, LPARAM lParam ) {
	WORD	wNotifyCode = HIWORD(wParam);	// 通知コード 
	WORD	wID = LOWORD(wParam);			// 項目、コントロール、またはアクセラレータの ID 
	HWND	hWndCtl = (HWND) lParam;		// コントロールのハンドル 

	switch( wID ) {
	case IDOK:
		GetWindowText (GetDlgItem (hWnd, IDC_EDITMSG1), edit_msg[0], 1023);
		GetWindowText (GetDlgItem (hWnd, IDC_EDITMSG2), edit_msg[1], 1023);
		GetWindowText (GetDlgItem (hWnd, IDC_EDITMSG3), edit_msg[2], 1023);
		GetWindowText (GetDlgItem (hWnd, IDC_EDITLABEL1), edit_label[0], 511);
		GetWindowText (GetDlgItem (hWnd, IDC_EDITLABEL2), edit_label[1], 511);
		GetWindowText (GetDlgItem (hWnd, IDC_EDITLABEL3), edit_label[2], 511);
		EndDialog (hWnd, TRUE);
		return;

	case IDCANCEL:
		EndDialog (hWnd, FALSE);
		return;
	}
}
