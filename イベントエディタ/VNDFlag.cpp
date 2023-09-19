#include <string.h>
#include <windows.h>
#include "resource.h"
#include "VNDFlag.h"


bool VNDFlag::DoModal( VNString& flagname, int& val, HWND hWndParent ) {
	cb = val;
	edit = new char [512];
	strcpy( edit, flagname.c_str() );

	int b = VNDialog::DoModal( "�t���O", "IDD_FLAG", hWndParent );
	bool res;

	if( b == TRUE  &&  cb != CB_ERR ) {
		val = cb;
		flagname = edit;
		res = true;
	} else
		res = false;

	delete edit;
	return res;
}

bool VNDFlag::OnInit() {
//	SetFocus (GetDlgItem (hWnd, IDC_EDITCOLOR));
	// �G�f�B�b�g�R���g���[���Ƀt�@�C�������Z�b�g
	SetWindowText (GetDlgItem (hWnd, IDC_EDIT), edit);

	// �R���{�{�b�N�X�ɍ��ڂ�ǉ����đI��
	HWND	hWndCB = GetDlgItem (hWnd, IDC_COMBO);
	static char *cb_item[] = { "�n�m", "�n�e�e", "���]" };
	for( int i = 0; i < 3; i++ )
		SendMessage (hWndCB, CB_ADDSTRING, 0, (LPARAM) (LPCTSTR) cb_item[i]);

	SendMessage (hWndCB, CB_SETCURSEL, cb, 0);
	return FALSE;
}

void VNDFlag::OnCommand( WPARAM wParam, LPARAM lParam ) {
	WORD	wNotifyCode = HIWORD(wParam);	// �ʒm�R�[�h 
	WORD	wID = LOWORD(wParam);			// ���ځA�R���g���[���A�܂��̓A�N�Z�����[�^�� ID 
	HWND	hWndCtl = (HWND) lParam;		// �R���g���[���̃n���h�� 

	switch( wID ) {
	case IDOK:
		GetWindowText (GetDlgItem (hWnd, IDC_EDIT), edit, 511);
		cb = SendMessage (GetDlgItem (hWnd, IDC_COMBO), CB_GETCURSEL, 0, 0);
		EndDialog (hWnd, TRUE);
		return;

	case IDCANCEL:
		EndDialog (hWnd, FALSE);
		return;
	}
}
