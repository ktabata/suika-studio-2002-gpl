#include "VNDialog.h"

int CALLBACK VNDialog_DlgProc (HWND, UINT, WPARAM, LPARAM);

VNDialog *VNDialog::list[256];
bool	VNDialog::is_initialized = false;
int		VNDialog::count = 0;


VNDialog::VNDialog() {
	hWnd = NULL;

	// static�����o�̏������i����̂݁j
	if( ! is_initialized ) {
		for( int i = 0; i < VND_LIST_ITEMS; i++ )
			list[i] = NULL;
		is_initialized = true;
	}
}

VNDialog::~VNDialog() {
	// ���s���ɍ폜���ꂽ
	if( hWnd != NULL )
		DebugBreak("���s���̃f�X�g���N�^�Ăяo��");
}

int VNDialog::SearchItem( VNDialog *obj ) {
	// �I�u�W�F�N�g���X�g�����v���ڂ�T��
	for( int i = 0; i < VND_LIST_ITEMS; i++ ) {
		if( list[i] == obj )
			return i;
	}
	return -1;
}

void VNDialog::AddToList() {
	if( ++count >= VND_LIST_ITEMS )
		DebugBreak("�ő�I�u�W�F�N�g�����z���ă_�C�A���O����낤�Ƃ���");

	// �󂫍��ڂ�T��
	int n = SearchItem( NULL );
	if( n < 0 )
		DebugBreak("�󂫍��ڂ�������Ȃ��i���炩�ȃo�O�F���X�g����̍폜������ɍs���Ă��Ȃ��j");

	list[n] = this;
}

void VNDialog::DeleteFromList() {
	if( count <= 0 )
		DebugBreak("���X�g�ɍ��ڂ��Ȃ��̂ɍ폜���悤�Ƃ���");

	int n = SearchItem( this );
	if( n < 0 )
		DebugBreak("�폜���鍀�ڂ����X�g���猩����Ȃ�");

	list[n] = NULL;
}

VNDialog *VNDialog::FromHandle( HWND hWnd ) {
	for( int cnt = 0, i = 0; cnt < count && i < VND_LIST_ITEMS; i++ ) {
		if( list[i] == NULL )
			continue;
		if( list[i]->hWnd == hWnd )
			return list[i];	// found
		cnt++;
	}
	// not found
	return NULL;
}

int VNDialog::DoModal( const char *title, const char *template_name, HWND hWndParent ) {
	this->title = (char *) title;
	int res = DialogBoxParamA (
				GetModuleHandle(NULL),
				template_name,
				hWndParent,
				(DLGPROC) VNDialog_DlgProc,
				(LPARAM) this);
	DeleteFromList();
	hWnd = NULL;
	return res;
}

int CALLBACK VNDialog_DlgProc (HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if( uMsg == WM_INITDIALOG ) {
		VNDialog *obj = (VNDialog *) lParam;
		obj->hWnd = hDlg;
		obj->AddToList();
		SetWindowText (hDlg, obj->title);
		return obj->OnInit() == true ? TRUE : FALSE;
	}

	VNDialog *obj = VNDialog::FromHandle( hDlg );
	if( obj != NULL ) {
		switch (uMsg) {
		case WM_COMMAND:
			obj->OnCommand( wParam, lParam );
			return TRUE;
		case WM_CLOSE:
			obj->OnClose();
			return TRUE;
		case WM_SIZE:
			obj->OnSize( LOWORD(lParam), HIWORD(lParam) );
			return TRUE;
		}
	}

	return FALSE;
}

bool VNDialog::OnInit() {
	return true;
}

void VNDialog::OnCommand( WPARAM wParam, LPARAM lParam ) {
/*	wNotifyCode = HIWORD(wParam); // �ʒm�R�[�h 
	wID = LOWORD(wParam); // ���ځA�R���g���[���A�܂��̓A�N�Z�����[�^�� ID
	hwndCtl = (HWND) lParam; // �R���g���[���̃n���h�� 
*/
	return;
}

void VNDialog::OnSize( int, int ) { }

void VNDialog::OnClose() {
	EndDialog (hWnd, 0);
}

void VNDialog::DebugBreak( const char *s ) {
	int i = 3;
	MessageBox (hWnd, s, "VNDialog �����G���[", MB_ICONEXCLAMATION|MB_OK);
//	abort();
	_asm{int 3}
}
