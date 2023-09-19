#include "VNDFlagEdit.h"
#include "VNFileDialog.h"
#include "resource.h"


bool VNDFlagEdit::DoModal( const char *title, const char *file_name, HWND hWndParent ) {
	flag_file = new VNFlagFile();
	this->file_name = file_name;
	is_modified = false;

	if( strcmp( file_name, "" ) != 0 ) {
		if( ! flag_file->Load( file_name ) ) {
			// �ǂݍ��݂Ɏ��s������I�u�W�F�N�g���\�z������
			delete flag_file;
			flag_file = new VNFlagFile();
		}
	} else
		flag_file = new VNFlagFile();

	int res = VNDialog::DoModal( title, "IDD_FLAGEDIT", hWndParent );
	if( res )
		flag_file->Save( file_name );

	return true;
}

int VNDFlagEdit::ClosePrompt() {
	return MessageBox (hWnd,
					   "�ύX��ۑ����܂����H",
					   "�t���O�G�f�B�^",
					   MB_YESNOCANCEL | MB_ICONQUESTION);
}

bool VNDFlagEdit::OnInit() {
	if( ! InitListBox() )
		EndDialog( hWnd, FALSE );
	return true;
}

void VNDFlagEdit::OnClose() {
	if( ! is_modified ) {
		EndDialog (hWnd, FALSE);
		return;
	}

	switch( ClosePrompt() ) {
	case IDCANCEL:
		return;
	case IDYES:
		EndDialog (hWnd, TRUE);
		return;
	case IDNO:
		EndDialog (hWnd, FALSE);
		return;
	}
	// never come here
	abort();
}

void VNDFlagEdit::OnCommand (WPARAM wParam, LPARAM lParam)
{
	WORD	wNotifyCode = HIWORD(wParam);	// �ʒm�R�[�h 
	WORD	wID = LOWORD(wParam);			// ���ځA�R���g���[���A�܂��̓A�N�Z�����[�^�� ID 
	HWND	hWndCtl = (HWND) lParam;		// �R���g���[���̃n���h�� 

	switch (wID)	{
	case IDC_BTNOPEN:
		OnBtnOpen();
		return;
	case IDC_BTNDEL:
		OnBtnDelete();
		return;
	case IDC_BTNADD:
		OnBtnAdd();
		return;
	case IDC_BTNONOFF:
		OnBtnOnOff();
		return;
	case IDC_BTNSAVE:
		OnBtnSave();
		return;
	case IDC_BTNCLOSE:
		OnClose();
	}
}

void VNDFlagEdit::OnBtnOpen() {
	if( is_modified ) {
		switch( ClosePrompt() ) {
		case IDCANCEL:
			return;
		case IDYES:
			OnBtnSave();
			break;
		case IDNO:
			break;
		}
	}

	bool b = GetOpenFileNameIndirect( "�t�@�C�����J��", "�׸�̧��(*.flg)\0*.flg\0",
									  "", file_name );
	if( b == false )
		return;

	VNFlagFile *new_flg = new VNFlagFile();
	if( ! new_flg->Load( file_name.c_str() ) ) {
		// �ǂݍ��݂Ɏ��s������I�u�W�F�N�g���\�z������
		delete new_flg;
		::MessageBox (hWnd, "���[�h�Ɏ��s���܂����B", "�t���O�G�f�B�^", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	delete flag_file;
	flag_file = new_flg;

	InitListBox();
}


void VNDFlagEdit::OnBtnSave() {
	if( strcmp( file_name.c_str(), "" ) == 0 ) {
		bool b = GetSaveFileNameIndirect( "���O��t���ĕۑ�", "�׸�̧��(*.flg)\0*.flg\0",
										  "�V�K�t���O.flg", file_name );
		if( b == false )
			return;
	}

	if( ! flag_file->Save( file_name.c_str() ) )
		MessageBox (hWnd, "�Z�[�u�Ɏ��s���܂����B", "�G���[", MB_ICONEXCLAMATION|MB_OK);
	is_modified = false;
}

void VNDFlagEdit::OnBtnDelete() {
	HWND	hWndLB = GetDlgItem (hWnd, IDC_FLAGLIST);

	// �I�����ڂ̃C���f�b�N�X���擾
	int sel = SendMessage (hWndLB, LB_GETCURSEL, 0, 0);
	if( sel == LB_ERR )
		return;	// ���ڂ��I������Ă��Ȃ�

	// �I�����ڂ̃e�L�X�g���擾
	char *buf = new char[1024];
	if( SendMessage (hWndLB, LB_GETTEXT, sel, (LPARAM)(LPCTSTR) buf) == LB_ERR ) {
		// �ُ�Ȃ̂ŉ��������Ƀ��^�[��
		delete buf;
		return;
	}

	// �I�����ڂ��폜
	if( SendMessage (hWndLB, LB_DELETESTRING, sel, 0) == LB_ERR ) {
		// �ُ�Ȃ̂ŉ��������Ƀ��^�[��
		delete buf;
		return;
	}

	// flag_file����폜
	buf[ strlen(buf) - 3 ] = '\0';	// ������" ��"���邢��" �~"�𖳎�����
	if( ! flag_file->DeleteFlag( buf ) )
		DebugBreak();

	// ���X�g�{�b�N�X�Ƀt�H�[�J�X��ݒ�
	SetFocus (GetDlgItem (hWnd, IDC_FLAGLIST) );
	int count = SendMessage (hWndLB, LB_GETCOUNT, 0, 0);
	if( count != LB_ERR ) {
		// �폜�����̂��Ō�̍��ڂȂ�
		if( sel == count )
			SendMessage (hWndLB, LB_SETCURSEL, sel - 1, 0);
		else	// �Ō�ȊO�̍��ڂȂ�
			SendMessage (hWndLB, LB_SETCURSEL, sel, 0);
	}

	is_modified = true;
	return;
}

void VNDFlagEdit::OnBtnAdd() {
	VNString flag_name;

	VNDFlagEdit_Add add_dlg;
	if( ! add_dlg.DoModal( flag_name, hWnd) )
		return;

	if( ! flag_file->AddFlag( flag_name.c_str(), false ) ) {
		MessageBox (hWnd,
					"�������O�̃t���O�����łɑ��݂��Ă��܂��B",
					"�t���O�G�f�B�^",
					MB_OK | MB_ICONINFORMATION);
		return;
	}

	// ���X�g�{�b�N�X�ɍ��ڂ�ǉ�
	AddItemToListBox( flag_name.c_str(), false );

	is_modified = true;
	return;
}

void VNDFlagEdit::OnBtnOnOff() {
	HWND	hWndLB = GetDlgItem (hWnd, IDC_FLAGLIST);

	// �I�����ڂ̃C���f�b�N�X���擾
	int sel = SendMessage (hWndLB, LB_GETCURSEL, 0, 0);
	if( sel == LB_ERR )
		return;

	// �I�����ڂ̃e�L�X�g���擾
	char *buf = new char[1024];
	if( SendMessage (hWndLB, LB_GETTEXT, sel, (LPARAM)(LPCTSTR) buf) == LB_ERR ) {
		// �ُ�Ȃ̂ŉ��������Ƀ��^�[��
		delete buf;
		return;
	}

	// �I�����ڂ��폜
	if( SendMessage (hWndLB, LB_DELETESTRING, sel, (LPARAM)(LPCTSTR) buf) == LB_ERR ) {
		// �ُ�Ȃ̂ŉ��������Ƀ��^�[��
		delete buf;
		return;
	}

	// �t���O�̃u�[���A���l�𔽓]
	buf[ strlen(buf) - 3 ] = '\0';	// ������" ��"���邢��" �~"�𖳎�����
	int n = flag_file->SearchFlag( buf );
	if( n < 0 )
		DebugBreak();

	if( ! flag_file->InvertFlagValue( n ) )
		DebugBreak();

	bool val;
	if( ! flag_file->GetFlagValue( n, val ) )
		DebugBreak();

	// ���X�g�{�b�N�X�ɍ��ڂ�ǉ����ăt�H�[�J�X��ݒ�
	AddItemToListBox( buf, val );
	SetFocus (GetDlgItem (hWnd, IDC_FLAGLIST) );
	if( SendMessage (hWndLB, LB_SETCURSEL, sel, 0) == LB_ERR ) {
		// �ُ�Ȃ̂ŉ��������Ƀ��^�[��
		delete buf;
		return;
	}

	is_modified = true;
	return;
}

void VNDFlagEdit::AddItemToListBox( const char *name, bool val ) {
	if( name == NULL )
		DebugBreak();

	HWND	hWndLB = GetDlgItem (hWnd, IDC_FLAGLIST);

	char *buf = new char[ strlen(name) + 4 ]; // len + strlen("�~ ") + 1('\0')
	strcpy( buf, name );
	if( val )	strcat( buf, " ��" );
	else		strcat( buf, " �~" );

	if( SendMessage (hWndLB, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR) buf) == LB_ERR ) {
		// �ُ�Ȃ̂ŉ��������Ƀ��^�[��
		delete buf;
		return;
	}
	delete[] buf;
}

bool VNDFlagEdit::InitListBox() {
	HWND	hWndLB = GetDlgItem (hWnd, IDC_FLAGLIST);
	::SendMessage (hWndLB, LB_RESETCONTENT, (WPARAM) 0, (LPARAM) 0);

	int cnt = flag_file->GetCount();
	for( int i = 0; i < cnt; i++ ) {
		VNString name;
		bool val;

		if( ! flag_file->GetFlagName( i, name ) ) 
			DebugBreak();
		
		if( ! flag_file->GetFlagValue( i, val ) )
			DebugBreak();

		AddItemToListBox( name.c_str(), val );
	}

	return true;
}

void VNDFlagEdit::DebugBreak() {
	MessageBox (hWnd, "�����G���[�F�v�f�o�b�O�I", "VNDFlagEdit", MB_ICONEXCLAMATION|MB_OK);
	abort();
}
