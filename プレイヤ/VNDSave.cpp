#include "VNDSave.h"
#include "VNSaveFile.h"
#include "resource.h"

// �Z�[�u�t�@�C���̔ԍ���Ԃ��B-1�Ȃ�L�����Z���B
int VNDSave::DoModal( HWND hWnd )
{
	select = -1;
	VNDialog::DoModal( "�Z�[�u", "IDD_SAVE", hWnd );
	return select;
}

void VNDSave::OnCommand( WPARAM wParam, LPARAM lParam ) {
	WORD	wNotifyCode = HIWORD(wParam);	// �ʒm�R�[�h 
	WORD	wID = LOWORD(wParam);			// ���ځA�R���g���[���A�܂��̓A�N�Z�����[�^�� ID 
	HWND	hWndCtl = (HWND) lParam;		// �R���g���[���̃n���h�� 

	switch( wID )	{
	case IDC_BTNSAVE:
		GetSelectIndex();
		if( select != -1 )
			EndDialog (hWnd, FALSE);
		return;
	case IDCANCEL:
		select = -1;
		EndDialog (hWnd, FALSE);
		return;
	}
}

bool VNDSave::OnInit() {
	for( int i = 0; i < 10; i++ ) {
		VNSaveFile	file;
		VNString str;
		file.Load( i );
		if( ! file.Load( i ) )
			str = (VNString)"�Z�[�u"+(VNString)i+" --------- NO DATA ---------";
		else
			str = (VNString)"�Z�[�u"+(VNString)i+" "+file.GetDate()+" "+file.GetTitle();
		AddItemToList( str.c_str() );
	}
	return true;
}

void VNDSave::AddItemToList( const char *str ) {
	HWND	hWndLB = ::GetDlgItem (hWnd, IDC_LIST);
	::SendMessage (hWndLB, LB_ADDSTRING, (WPARAM) 0, (LPARAM)(LPCTSTR) str);
}

void VNDSave::GetSelectIndex() {
	// �C�x���g���X�g�̑I�����ڂ̃C���f�b�N�X���擾
	HWND	hWndLB = ::GetDlgItem (hWnd, IDC_LIST);
	select = ::SendMessage (hWndLB, LB_GETCURSEL, 0, 0);
	if( select == LB_ERR )
		select = -1;
}
