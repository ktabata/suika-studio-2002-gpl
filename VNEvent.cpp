#include "VNEvent.h"
#include <stddef.h>
#include <windows.h>


VNEvent::VNEvent( int type ) : event_type( event_type_desc[type] ) {
	if( event_type.code <= 0 || event_type.code > EVENTCODE_MAX )
		DebugBreak( "�����ȃC�x���g�^�C�v" );

	AllocArgList();
	for( int i = 0; i < params; i++ ) {
		if( event_type.arg_type_desc[i] == 'S' )
			str_arg[i] = new VNString( "" );
		else
			int_arg[i] = 0;
	}
}

VNEvent::~VNEvent() {
	delete int_arg;
	for( int i = 0; i < params; i++ ) {
		if( str_arg[i] != NULL )
			delete str_arg[i];
	}
	delete str_arg;
}

VNEvent::VNEvent( VNEvent& src ) : event_type( src.event_type ) {
	if( event_type.code <= 0 || event_type.code > EVENTCODE_MAX )
		DebugBreak( "�����ȃC�x���g�^�C�v" );

	AllocArgList();
	for( int i = 0; i < params; i++ ) {
		if( event_type.arg_type_desc[i] == 'S' )
			str_arg[i] = new VNString( *src.str_arg[i] );
		else
			int_arg[i] = src.int_arg[i];
	}
}

void VNEvent::AllocArgList() {
	params = 0;

	char *c = event_type.arg_type_desc;
	while( *c != '\0' ) {
		if( *c != 'I' && *c != 'S' )
			DebugBreak( "�����ȃp�����^�L�q�q" );
		params++, c++;
	}

	int_arg = new int [ params ];
	str_arg = new VNString * [ params ];
	for( int i = 0; i < params; i++ )
		str_arg[i] = NULL;
}

int VNEvent::GetCode() {
	return event_type.code;
}

const EventTypeDescriptor& VNEvent::GetDescriptor() {
	return event_type;
}

int VNEvent::GetParamInt( int n ) {
	if( n >= params  )
		DebugBreak( "�����ȃp�����^�ԍ�" );

	if( event_type.arg_type_desc[n] != 'I' )
		DebugBreak( "�p�����^�^���Ⴄ" );

	return int_arg[n];
}

const VNString& VNEvent::GetParamString( int n ) {
	if( n >= params  )
		DebugBreak( "�����ȃp�����^�ԍ�" );

	if( event_type.arg_type_desc[n] != 'S' )
		DebugBreak( "�p�����^�^���Ⴄ" );

	return *str_arg[n];
}

void VNEvent::SetParamInt( int n, int val ) {
	if( n >= params  )
		DebugBreak( "�����ȃp�����^�ԍ�" );

	if( event_type.arg_type_desc[n] != 'I' )
		DebugBreak( "�p�����^�^���Ⴄ" );

	int_arg[n] = val;
}

void VNEvent::SetParamString( int n, const char *val ) {
	if( n >= params  )
		DebugBreak( "�����ȃp�����^�ԍ�" );

	if( event_type.arg_type_desc[n] != 'S' )
		DebugBreak( "�p�����^�^���Ⴄ" );

	if( str_arg[n] != NULL )
		delete str_arg[n];

	str_arg[n] = new VNString( val );
}

VNString VNEvent::GetExplanation() {
	switch( event_type.code ) {
	case EVENTCODE_NARRATION:
		return VNString( "���@" ) + *str_arg[0];

	case EVENTCODE_SERIF:
		return VNString( "���@" ) + *str_arg[0] + "�u" + *str_arg[1];

	case EVENTCODE_CHARACTER:
	{
		VNString a;
		switch( int_arg[0] ) {
		case CHARA_H_CENTER:a = "�� �L�����@���@";	break;
		case CHARA_H_LEFT:	a = "�� �L�����@���@";	break;
		case CHARA_H_RIGHT:	a = "�� �L�����@�E�@";	break;
		default:
			MessageBox (NULL, "�s���ȃL�����|�W�V����", "VNEvent.cpp", MB_OK | MB_ICONEXCLAMATION);
			abort();	// never come here
			break;
		}

		if( strcmp( str_arg[2]->c_str(), "" ) == 0 )
			return a + "�ޏ�";
		else if( int_arg[1] == CHARA_V_BOTTOM )
			return a + *str_arg[2] + "�@������ ";
		else if( int_arg[1] == CHARA_V_CENTER )
			return a + *str_arg[2] + "�@�������� ";
		else if( int_arg[1] == CHARA_V_TOP )
			return a + *str_arg[2] + "�@�㑵�� ";
	}

	case EVENTCODE_BACKGROUND:
		if( strcmp( str_arg[0]->c_str(), "" ) == 0 )
			return VNString( "�� �w�@�i�@�t�F�[�h�A�E�g" ) + *str_arg[0];
		else
			return VNString( "�� �w�@�i�@" ) + *str_arg[0];

	case EVENTCODE_SELECT:
		return VNString( "�� �I�@���@" ) + *str_arg[0] + "�^" + *str_arg[2] + "�^" + *str_arg[4];

	case EVENTCODE_LABEL:
		return VNString( "�� ���x���@" ) + *str_arg[0];

	case EVENTCODE_CDDA:
		if( int_arg[0] == 0 )
			return VNString( "�� CD-DA �@��~�@�t�F�[�h" ) + VNString( int_arg[1] ) + "�~���b";
		else
			return VNString( "�� CD-DA �@�g���b�N" ) + VNString( int_arg[0] ) + "�@�t�F�[�h" + VNString( int_arg[1] ) + "�~���b";

	case EVENTCODE_MIDI:
		if( strcmp( str_arg[0]->c_str(), "" ) == 0 )
			return VNString( "�� MIDI�@�@��~�@�t�F�[�h" ) + VNString( int_arg[1] ) + "�~���b";
		else
			return VNString( "�� MIDI�@�@" ) + *str_arg[0] + "�@�t�F�[�h" + VNString( int_arg[1] ) + "�~���b";

	case EVENTCODE_PCM:
		if( strcmp( str_arg[0]->c_str(), "" ) == 0 )
			return VNString( "�� PCM �@�@�I�t" );
		else
			return VNString( "�� PCM �@�@" ) + *str_arg[0];

	case EVENTCODE_SETFLAG:
		switch( int_arg[1] ) {
		case SETFLAG_ON:
			return VNString( "�� �t���O�@�n�m�@" ) + *str_arg[0];
		case SETFLAG_OFF:
			return VNString( "�� �t���O�@�n�e�e�@" ) + *str_arg[0];
		case SETFLAG_INVERT:
			return VNString( "�� �t���O���]�@" ) + *str_arg[0];
		default:
			abort();
		}
		break;

	case EVENTCODE_JUMP:
	{
		VNString evt;
		if( strcmp( str_arg[0]->c_str(), "" ) == 0 )
			evt = "�t�@�C�����@";
		else
			evt = *str_arg[0] + "�@";

		VNString label;
		if( strcmp( str_arg[1]->c_str(), "" ) == 0 )
			label = "�擪�@";
		else
			label = VNString( "���x��\"" ) + *str_arg[1] + "\"�@";

		switch( int_arg[2] ) {
		case JUMP_FORCED:
			return VNString( "�� �W�����v�@�������@" ) + evt + label;
		case JUMP_IFFLAGSET:
			return VNString( "�� �W�����v�@" ) + evt + label + "�����F�t���O\"" + *str_arg[3] + "\"���n�m�ł���";
		case JUMP_IFFLAGNOTSET:
			return VNString( "�� �W�����v�@" ) + evt + label + "�����F�t���O\"" + *str_arg[3] + "\"���n�e�e�ł���";
		default:
			abort();
		}
		break;
	}

	case EVENTCODE_TITLE:
		return VNString( "�� �^�C�g���@" ) + *str_arg[0];

	case EVENTCODE_WAITCLICK:
		return VNString( "�� �N���b�N�E�F�C�g" );

	case EVENTCODE_WAIT:
		return VNString( "�� �E�F�C�g�@" ) + VNString( int_arg[0] ) + "�~���b";

	case EVENTCODE_MSGBOX:
		return VNString( "���b�Z�[�W�{�b�N�X�ݒ�" );

	case EVENTCODE_COMMENT:
		return VNString( "; " ) + *str_arg[0];
	}

	MessageBox (NULL, "�����ȃC�x���g�^�C�v�ł�", "VNEvent.cpp", MB_OK | MB_ICONEXCLAMATION);
	abort();
	return VNString( "�G���[" );	// �ꉞ
}

void VNEvent::DebugBreak( const char *msg ) {
	MessageBox (NULL, msg, "�����G���[(VNEvent�N���X)", MB_ICONEXCLAMATION | MB_OK);
	abort();
}
