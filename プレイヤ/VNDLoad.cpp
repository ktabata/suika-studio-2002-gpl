#include "VNDLoad.h"
#include "VNSaveFile.h"
#include "resource.h"

// �Z�[�u�t�@�C���̔ԍ���Ԃ��B-1�Ȃ�L�����Z���B
int VNDLoad::DoModal( HWND hWnd )
{
	select = -1;
	VNDialog::DoModal( "���[�h", "IDD_LOAD", hWnd );
	return select;
}

void VNDLoad::OnCommand( WPARAM wParam, LPARAM lParam ) {
	WORD	wNotifyCode = HIWORD(wParam);	// �ʒm�R�[�h 
	WORD	wID = LOWORD(wParam);			// ���ځA�R���g���[���A�܂��̓A�N�Z�����[�^�� ID 
	HWND	hWndCtl = (HWND) lParam;		// �R���g���[���̃n���h�� 

	switch( wID )	{
	case IDC_BTNLOAD:
		GetSelectIndex();
		if( select != -1 )
			if( available[select] )
				EndDialog (hWnd, FALSE);
		return;
	case IDCANCEL:
		select = -1;
		EndDialog (hWnd, FALSE);
		return;
	}
}

bool VNDLoad::OnInit() {
	for( int i = 0; i < 10; i++ ) {
		VNSaveFile	file;
		VNString str;
		if( ! file.Load( i ) ) {
			str = (VNString)"�Z�[�u"+(VNString)i+" --------- NO DATA ---------";
			available[i] = false;
		} else {
			str = (VNString)"�Z�[�u"+(VNString)i+" "+file.GetDate()+" "+file.GetTitle();
			available[i] = true;
		}
		AddItemToList( str.c_str() );
	}
	return true;
}

void VNDLoad::AddItemToList( const char *str ) {
	HWND	hWndLB = ::GetDlgItem (hWnd, IDC_LIST);
	::SendMessage (hWndLB, LB_ADDSTRING, (WPARAM) 0, (LPARAM)(LPCTSTR) str);
}

void VNDLoad::GetSelectIndex() {
	// �C�x���g���X�g�̑I�����ڂ̃C���f�b�N�X���擾
	HWND	hWndLB = ::GetDlgItem (hWnd, IDC_LIST);
	select = ::SendMessage (hWndLB, LB_GETCURSEL, 0, 0);
	if( select == LB_ERR )
		select = -1;
}
