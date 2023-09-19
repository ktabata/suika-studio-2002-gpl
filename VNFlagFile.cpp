#include "VNFlagFile.h"
#include "VNBinFileReader.h"
#include "VNBinFileWriter.h"
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <windows.h>	// MessageBox

#define ALLOC_ONCE		(50)
#define SIGNATURE		"flagdata"


VNFlagFile::VNFlagFile() {
	item_count = 0;
	flag_name = NULL;
	value = 0;
	allocated_count = 0;
}

VNFlagFile::~VNFlagFile() {
	for( int i = 0; i < item_count; i++ ) {
		if( flag_name[i] != NULL )
			delete flag_name[i];
	}
	free( flag_name );
	free( value );
}

bool VNFlagFile::Load( const char *file_name ) {
	try {
		VNBinFileReader fin( file_name );

		// �V�O�l�`�����m�F
		VNString signature = fin.GetStringEx();
		if( strcmp( signature.c_str(), SIGNATURE ) != 0 )
			return false;

		// �t���O�������[�h
		int flags = fin.GetIntEx();
		if( flags < 0 || flags >= 4096 )	// 4096�ȏ���ُ�Ƃ���
			return false;

		// �t���O�����[�h
		for( int i = 0; i < flags; i++ ) {
			VNString	name = fin.GetStringEx();
			int			n = fin.GetIntEx();
			bool		val = n ? true : false;

			if( ! AddFlag( name.c_str(), val ) )
				return false;
		}
	} catch( VNIOException *e ) {
		delete e;
		return false;
	}
	return true;
}

bool VNFlagFile::Save( const char *file_name ) {
	try {
		VNBinFileWriter fout( file_name );

		// �V�O�l�`������������
		fout.PutStringEx( SIGNATURE );

		// �t���O������������
		fout.PutIntEx( item_count );

		// �t���O����������
		for( int i = 0; i < item_count; i++ ) {
			fout.PutString( flag_name[i]->c_str() );
			fout.PutInt( value[i] );
		}
	} catch( VNIOException *e ) {
		delete e;
		return false;
	}
	return true;
}

bool VNFlagFile::AddFlag( const char *name, bool val ) {
	// �����̃t���O�����łɂ���Ȃ玸�s
	if( SearchFlag( name ) != -1 )
		return false;

	Realloc();

	flag_name[item_count] = new VNString( name );
	value[item_count++] = val;
	return true;
}

bool VNFlagFile::DeleteFlag( int n ) {
	IsValidIndex( n );

	delete flag_name[n];
	memmove( &flag_name[n], &flag_name[n+1], sizeof( VNString * ) * (item_count - n - 1) );
	memmove( &value[n], &value[n+1], sizeof( bool ) * (item_count - n - 1) );
	flag_name[--item_count] = NULL;
	return true;
}

bool VNFlagFile::DeleteFlag( const char *name ) {
	int n = SearchFlag( name );
	if( n == -1 )
		return false;
	return DeleteFlag( n );
}

int VNFlagFile::GetCount() {
	return item_count;
}

bool VNFlagFile::GetFlagName( int n, VNString& name ) {
	IsValidIndex( n );

	name = *flag_name[n];
	return true;
}

bool VNFlagFile::GetFlagValue( int n, bool& val ) {
	IsValidIndex( n );

	val = value[n];
	return true;
}

bool VNFlagFile::GetFlagValue( const char *name, bool& val ) {
	int n = SearchFlag( name );
	if( n == -1 )
		return false;
	return GetFlagValue( n, val );
}

bool VNFlagFile::SetFlagValue( int n, bool val ) {
	IsValidIndex( n );

	value[n] = val;
	return true;
}

bool VNFlagFile::SetFlagValue( const char *name, bool val ) {
	int n = SearchFlag( name );
	if( n == -1 )
		return false;
	return SetFlagValue( n, val );
}

bool VNFlagFile::InvertFlagValue( int n ){
	IsValidIndex( n );

	value[n] = value[n] ? false :true;
	return true;
}

bool VNFlagFile::InvertFlagValue( const char *name ) {
	int n = SearchFlag( name );
	if( n == -1 )
		return false;
	return InvertFlagValue( n );
}

int VNFlagFile::SearchFlag( const char *name ) {
	for( int i = 0; i < item_count; i++ ) {
		if( flag_name[i] == NULL )
			continue;
		if( strcmp( name, flag_name[i]->c_str() ) == 0 )
			return i;
	}
	return -1;
}

// �K�v�Ȃ�o�b�t�@���Ċm�ۂ���
void VNFlagFile::Realloc() {
	if( item_count < allocated_count )
		return;

	allocated_count += ALLOC_ONCE;
	flag_name = (VNString **) realloc( flag_name, sizeof(VNString*) * allocated_count );
	value = (bool *) realloc( value, sizeof(bool) * allocated_count );

	for( int i = item_count; i < allocated_count; i++ )
		flag_name[i] = NULL;
}

void VNFlagFile::IsValidIndex( int n ) {
	if( n >= item_count ) {
		MessageBox (NULL,
					"VNFlagFile::IsValidIndex() : �s���ȃC���f�b�N�X�ł��B",
					"�G���[",
					MB_ICONEXCLAMATION | MB_OK);
		abort();
	}
}
