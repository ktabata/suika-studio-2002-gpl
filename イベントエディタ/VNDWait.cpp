#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "resource.h"
#include "VNDWait.h"


bool VNDWait::DoModal( int& millisec, HWND hWndParent ) {
	edit_string = new char [64];
	sprintf( edit_string, "%d", millisec );

	int b = VNDialog::DoModal( "�~���b�E�F�C�g", "IDD_WAIT", hWndParent );
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
	WORD	wNotifyCode = HIWORD(wParam);	// �ʒm�R�[�h 
	WORD	wID = LOWORD(wParam);			// ���ځA�R���g���[���A�܂��̓A�N�Z�����[�^�� ID 
	HWND	hWndCtl = (HWND) lParam;		// �R���g���[���̃n���h�� 

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
