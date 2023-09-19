#include <string.h>
#include <windows.h>
#include "resource.h"
#include "VNDFlagEdit_Add.h"


bool VNDFlagEdit_Add::DoModal( VNString& flag_name, HWND hWndParent ) {
	int res = VNDialog::DoModal( "�t���O�̒ǉ�", "IDD_FLAGEDITADD", hWndParent );

	if( res == TRUE ) {
		flag_name = edit_string;
		return true;
	} else {
		return false;
	}
}

bool VNDFlagEdit_Add::OnInit() {
	SetFocus (GetDlgItem (hWnd, IDC_EDITFLAGNAME) );
	return FALSE;
}

void VNDFlagEdit_Add::OnCommand( WPARAM wParam, LPARAM lParam ) {
	WORD	wNotifyCode = HIWORD(wParam);	// �ʒm�R�[�h 
	WORD	wID = LOWORD(wParam);			// ���ځA�R���g���[���A�܂��̓A�N�Z�����[�^�� ID 
	HWND	hWndCtl = (HWND) lParam;		// �R���g���[���̃n���h�� 

	switch( wID ) {
	case IDOK:
		GetWindowText (
			GetDlgItem (hWnd, IDC_EDITFLAGNAME),
			edit_string,
			1023);

			if( strcmp(edit_string, "") == 0 )
				EndDialog (hWnd, FALSE);
			else
				EndDialog (hWnd, TRUE);
			return;

	case IDCANCEL:
		EndDialog (hWnd, FALSE);
		return;
	}
}
