#include <windows.h>
#include "VNPlayer.h"
#include "VNPromptDrawer.h"


void VNPlayer::WaitClick() {
	bool b[3] = { true, false, true };

	window->Sync();
	for( int i = 0; i < 3; i++ ) {
		while( window->IsMouseLDown() == b[i] ) {
			if( window->IsCtrlDown() )
				return;
			window->Sync();
		}
	}
}

void VNPlayer::Wait( int millisec ) {
	bool	is_l_down = window->IsMouseLDown();

	unsigned long t = GetTickCount();
	while( t + millisec > GetTickCount() ) {
		window->Sync();

		if( window->IsCtrlDown() )
			return;

		bool b = window->IsMouseLDown();
		if( ! is_l_down && b )
			break;
		else if( is_l_down && ! b )
			is_l_down = false;
	}
}

// ���b�Z�[�W���莞�̃Z�[�u�E���[�h�\�ȃ^�C�~���O�ł̃N���b�N�E�F�C�g�B
// ���b�Z�[�W�{�b�N�X�ɃA�j���[�V�����\���B
// �E�F�C�g���Ƀ��[�h���삪�s�����false��Ԃ��B
// use_quick�ɂ́A���b�Z�[�W�����{�^�������ō����\������Ă���Ƃ���true��n���B
// ���̏ꍇ�A���{�^�����������邾���ŁA���S�ȃN���b�N��҂����Ƀ��^�[������B
bool VNPlayer::MessageWait( bool use_quick ) {
	bool b[3] = { true, false, true };

	window->Sync();

	// �v�����v�g�A�j���[�V�����`��̃I�u�W�F�N�g
	VNPromptDrawer	pd( *back_buffer, prompt_x, prompt_y, prof.MsgFontWidth*2, prof.MsgFontHeight );
	pd.Start();

	// �v�����v�g�\���̈���Z�[�u(+1�͐��x�̌덷�ł͂ݏo�邱�Ƃ�z�肵�āj
	VNImage	save;
	save.CreateRGB( prof.MsgFontWidth*2+1, prof.MsgFontHeight+1 );
	save.BitBlt( 0, 0, back_buffer, prompt_x, prompt_y, prof.MsgFontWidth*2+1, prof.MsgFontHeight+1 );

	int cnt = use_quick ? 1 : 3;
	for( int i = 0; i < cnt; i++ ) {
		do {
			// �Z�[�u�����[�h�`�F�b�N
			if( window->IsSavePushed() )	// ���j���[�̃Z�[�u�������ꂽ��
				SaveDialog();
			if( window->IsLoadPushed() )	// ���j���[�̃��[�h�������ꂽ��
				if( LoadDialog() )
					return false;	// ���[�h���삪�s��ꂽ
			if( window->IsBackLogPushed() )
				BackLogDialog();

			if( window->IsCtrlDown() ) {
				// �v�����v�g���A�Z�[�u�����C���[�W�ŏ㏑�����ď���
				back_buffer->BitBlt( prompt_x, prompt_y, &save, 0, 0, prof.MsgFontWidth*2+1, prof.MsgFontHeight+1 );
				window->Update( prompt_x, prompt_y, prof.MsgFontWidth*2+1, prof.MsgFontHeight+1 );
				return true;
			}

			// �E�{�^���������̓��b�Z�[�W������
			if( window->IsMouseRDown() ) {
				DrawBgAndChara();
				window->Flip();
				while( window->IsMouseRDown() )
					window->Sync();
				PutMessage( last_message.c_str(), 0xff, 0xff, 0xff, 0 );
				window->Flip();
			}

			// ���̃A�j���v�����v�g��`��
			if( pd.NeedUpdate() ) {
				back_buffer->BitBlt( prompt_x, prompt_y, &save, 0, 0, prof.MsgFontWidth*2+1, prof.MsgFontHeight+1 );
				pd.Draw();
				window->Update( prompt_x, prompt_y, prof.MsgFontWidth*2+1, prof.MsgFontHeight+1 );
			}
			window->Sync();
		} while( window->IsMouseLDown() == b[i] );
	}
	// �v�����v�g���A�Z�[�u�����C���[�W�ŏ㏑�����ď���
	back_buffer->BitBlt( prompt_x, prompt_y, &save, 0, 0, prof.MsgFontWidth*2+1, prof.MsgFontHeight+1 );
	window->Update( prompt_x, prompt_y, prof.MsgFontWidth*2+1, prof.MsgFontHeight+1 );
	return true;
}

bool VNPlayer::GetFlagIndex( const char *flag, int& index, bool& is_local ) {
	// ���[�J���t���O���H
	is_local = true;
	index = local_flag->SearchFlag( flag );
	if( index == -1 ) {	// ������Ȃ�����
		// �O���[�o���t���O���H
		is_local = false;
		index = global_flag->SearchFlag( flag );
		if( index == -1 ) {
			// �ǂ���ɂ�������Ȃ�����
			return false;
		}
	}
	return true;
}

void VNPlayer::SetFlag( const char *flag, int cond ) {
	bool	is_local;
	int		n;

	for( int i = 0; i < 10; i++ ) {
	if( ! GetFlagIndex( flag, n, is_local ) ) {
		Warning( (VNString("�t���O\"")+flag+"\"�����݂��܂���B").c_str() );
		return;
	}
	}

	VNFlagFile *flagfile = is_local ? local_flag : global_flag;
	switch( cond ) {
	case SETFLAG_ON:
		flagfile->SetFlagValue( n, true );
		break;
	case SETFLAG_OFF:
		flagfile->SetFlagValue( n, false );
		break;
	case SETFLAG_INVERT:
		flagfile->InvertFlagValue( n );
		break;
	default:
		DebugBreak( "�v�f�o�b�O" );
	}
}

void VNPlayer::Jump( const char *file, const char *label, int cond, const char *flag ) {
	bool ifflagset = true;
	switch( cond ) {
	case JUMP_FORCED:
		break;
	case JUMP_IFFLAGNOTSET:
		ifflagset = false;
	case JUMP_IFFLAGSET:
	{
		bool	is_local;
		int		n;
		if( ! GetFlagIndex( flag, n, is_local ) ) {
			Warning( (VNString("�t���O\"")+flag+"\"�����݂��܂���B").c_str() );
			return;
		}
		VNFlagFile *flagfile = is_local ? local_flag : global_flag;
		bool val;
		if( ! flagfile->GetFlagValue( n, val ) )
			DebugBreak( "�v�f�o�b�O" );
		if( val != ifflagset )
			return;	// �����ɓ��Ă͂܂�Ȃ�
		break;
	}
	default:
		DebugBreak( "�v�f�o�b�O" );
	}

	if( strcmp( file, "" ) == 0 ) {
		// ����C�x���g�t�@�C�����ł̃W�����v
		int new_offset;

		if( strcmp( label, "" ) != 0 ) {
			// ���x���ɃW�����v
			new_offset = event->GetLabelOffset( label );
			if( new_offset == -1 ) {
				Warning( (VNString("���x��\"") + label + "\"�����݂��܂���B").c_str() );
				return;
			}
		} else {
			// �t�@�C���擪�ɃW�����v
			new_offset = -1;	// ���[�v�ŃC���N�������g�����̂łP���炵�Ă���
		}
		offset = new_offset;	// ���[�v�ŃC���N�������g�����̂Ń��x���̕��͑����Ȃ��Ă���
		return;
	}

	// ���t�@�C���ւ̃W�����v

	// �V�����C�x���g�t�@�C�������[�h����
	VNEventFile *new_event = new VNEventFile();
	if( ! new_event->Load( VNString(EVENT_DIR)+file ) ) {
		Warning( (VNString("�C�x���g�t�@�C��\"")+file+"\"�����݂��܂���B").c_str() );
		delete new_event;
		return;
	}

	int new_offset;

	if( strcmp( label, "" ) != 0 ) {
		// ���x���ɃW�����v
		new_offset = new_event->GetLabelOffset( label );
		if( new_offset == -1 ) {
			Warning(
				(VNString("�C�x���g�t�@�C��\"") +
				file +
				"\"�Ƀ��x��\"" +
				label +
				"\"�����݂��܂���B").c_str() );
			delete new_event;
			return;
		}
	} else {
		// �t�@�C���擪�ɃW�����v
		new_offset = -1;	// ���[�v�ŃC���N�������g�����̂łP���炵�Ă���
	}

	event_name = file;
	delete event;
	event = new_event;
	offset = new_offset;	// ���[�v�ŃC���N�������g�����̂Ń��x���̕��͑����Ȃ��Ă���
}

void VNPlayer::Title( const char *str ) {
	title = str;
	window->SetTitle( str );
}

// ���[�h���s�����false��Ԃ��B
bool VNPlayer::Select( const char *msg1, const char *msg2, const char *msg3,
					   const char *label1, const char *label2, const char *label3 )
{
	backlog.AddString("");

	int items = 3;
	if( strcmp( msg3, "" ) == 0 )
		items = 2;

	char *messages[3];
	messages[0] = (char *)msg1;
	messages[1] = (char *)msg2;
	messages[2] = (char *)msg3;

	char *labels[3];
	labels[0] = (char *)label1;
	labels[1] = (char *)label2;
	labels[2] = (char *)label3;

	int select = SelectLoop( (const char **)messages, items );

	if( select == -1 )
		return false;	// ���[�h���삪�s��ꂽ

	if( select > items )
		DebugBreak( "�v�f�o�b�O" );

	int ofs = event->GetLabelOffset( labels[select] );
	if( ofs == -1 ) {
		Warning( (VNString("���x��\"")+labels[select]+"\2�����݂��܂���B\r\n�啶���Ə������┼�p�ƑS�p�̈Ⴂ���`�F�b�N���Ă��������B\r\n����ł�����Ȃ��ꍇ�́AK24J�ɕ񍐂����肢���܂��B").c_str() );
		return true;	// ���[�h�͍s���Ă��Ȃ��̂�true
	}
	offset = ofs;	// �W�����v����w��

	return true;
}
