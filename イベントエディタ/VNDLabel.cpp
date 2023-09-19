#include <string.h>
#include <windows.h>
#include "resource.h"
#include "VNDLabel.h"


bool VNDLabel::DoModal( VNString& text, HWND hWndParent ) {
	edit_string = new char [512];
	strcpy( edit_string, text.c_str() );

	int b = VNDialog::DoModal( "���x��", "IDD_LABEL", hWndParent );
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
	WORD	wNotifyCode = HIWORD(wParam);	// �ʒm�R�[�h 
	WORD	wID = LOWORD(wParam);			// ���ځA�R���g���[���A�܂��̓A�N�Z�����[�^�� ID 
	HWND	hWndCtl = (HWND) lParam;		// �R���g���[���̃n���h�� 

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
