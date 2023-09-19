#include "VNEventFile.h"
#include "VNBinFileWriter.h"
#include "VNBinFileReader.h"
#include <stdio.h>	// abort
#include <string.h>	// strcmp
#include <malloc.h>	// malloc
#include <windows.h>	// MessageBox

#define ALLOC_ONCE		(1)
#define SIGNATURE		"eventdata"


VNEventFile::VNEventFile() {
	item_count = 0;
	allocated_count = 0;
	list = NULL;
}

VNEventFile::~VNEventFile() {
	if( list == NULL )
		return;

	for( int i = 0; i < item_count; i++ ) {
		if( list[i] == NULL )
			DebugBreak( "���X�g����NULL�̍��ڂ�����" );
		delete list[i];
	}

	free( list );
}

bool VNEventFile::Load( const char *file_name ) {
	try {
		VNBinFileReader file( file_name );

		// �V�O�l�`�����m�M
		VNString	signature = file.GetStringEx();
		if( strcmp( signature.c_str(), SIGNATURE ) != 0 )
			return false;

		// �X�̃C�x���g�����[�h����
		int count = file.GetIntEx();
		for( int i = 0; i < count; i++ ) {
			int desc_index = file.GetIntEx();
			if( desc_index <= 0 || desc_index > EVENTCODE_MAX )
				DebugBreak( "���[�h���̃t�@�C���ɖ����ȃC�x���g�^�C�v" );

			const EventTypeDescriptor& desc = event_type_desc[desc_index];
			VNEvent event( desc_index );

			for( int j = 0; j < desc.argc; j++ ) {
				char c = desc.arg_type_desc[j];
				if( c == 'S' ) {
					event.SetParamString( j, file.GetStringEx().c_str() );
				} else {
					if( c != 'I' )
						DebugBreak( "���[�h���̃t�@�C���ɖ����ȃp�����^�L�q�q" );
					event.SetParamInt( j, file.GetIntEx() );
				}
			}
			AddEvent( new VNEvent(event) );
		}
	} catch( VNIOException *e ) {
		delete e;
		return false;
	}

	return true;
}

bool VNEventFile::Save( const char *file_name ) {
	try {
		VNBinFileWriter file( file_name );

		file.PutStringEx( SIGNATURE );
		file.PutIntEx( item_count );

		for( int i = 0; i < item_count; i++ ) {
			const EventTypeDescriptor& desc = list[i]->GetDescriptor();
			file.PutIntEx( desc.code );

			for( int j = 0; j < desc.argc; j++ ) {
				char c = desc.arg_type_desc[j];
				if( c == 'S' )
					file.PutStringEx( list[i]->GetParamString( j ).c_str() );
				else {
					if( c != 'I' )
						DebugBreak( "�Z�[�u���̃f�[�^�ɖ����ȃp�����^�L�q�q" );
					file.PutIntEx( list[i]->GetParamInt( j ) );
				}
			}
		}
	} catch( VNIOException *e ) {
		delete e;
		return false;
	}
	return true;
}

bool VNEventFile::AddEvent( VNEvent *event ) {
	if( event == NULL )
		DebugBreak( "NULL�|�C���^���" );
	Realloc();
	list[item_count++] = event;
	return true;
}

bool VNEventFile::InsertEvent( VNEvent *event, int n /* -1 */ ) {
	if( n == -1 )
		return AddEvent( event );

	if( n > item_count || n < 0 )
		DebugBreak( "�I�u�W�F�N�g�}���ʒu�ɖ����Ȑ��l���n���ꂽ" );
	Realloc();
	memmove( &list[n+1], &list[n], sizeof( VNEvent * ) * (item_count - n) );
	list[n] = event;
	item_count++;
	return true;
}

bool VNEventFile::DeleteEvent( int n ) {
	if( n > item_count || n < 0 )
		DebugBreak( "�폜����I�u�W�F�N�g�ԍ��ɖ����Ȑ��l���n���ꂽ" );
	delete list[n];
	memmove( &list[n], &list[n+1], sizeof( VNEvent * ) * (item_count - n - 1) );
	list[--item_count] = NULL;
	return true;
}

VNEvent *VNEventFile::GetEvent( int n ) {
	return list[n];
}

int VNEventFile::GetCount() {
	return item_count;
}

int VNEventFile::GetLabelOffset( const char *label ) {
	for( int i = 0; i < item_count; i++ ) {
		if( list[i]->GetDescriptor().code != EVENTCODE_LABEL )
			continue;
		if( strcmp( list[i]->GetParamString(0).c_str(), label ) == 0 )
			return i;
	}
	return -1;	// ������Ȃ�����
}

void VNEventFile::Realloc() {
	if( item_count < allocated_count )
		return;

	allocated_count += ALLOC_ONCE;
	list = (VNEvent **) realloc( list, sizeof(VNEvent *) * allocated_count );

	for( int i = item_count; i < allocated_count; i++ )
		list[i] = NULL;
}

void VNEventFile::DebugBreak( const char *msg ) {
	MessageBox (NULL, msg, "�����G���[(VNEentFile�N���X)", MB_ICONEXCLAMATION | MB_OK);
	//abort();
	_asm{int 3}
}
