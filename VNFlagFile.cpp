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

		// シグネチャを確認
		VNString signature = fin.GetStringEx();
		if( strcmp( signature.c_str(), SIGNATURE ) != 0 )
			return false;

		// フラグ数をロード
		int flags = fin.GetIntEx();
		if( flags < 0 || flags >= 4096 )	// 4096以上を異常とする
			return false;

		// フラグをロード
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

		// シグネチャを書き込む
		fout.PutStringEx( SIGNATURE );

		// フラグ数を書き込む
		fout.PutIntEx( item_count );

		// フラグを書き込む
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
	// 同名のフラグがすでにあるなら失敗
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

// 必要ならバッファを再確保する
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
					"VNFlagFile::IsValidIndex() : 不正なインデックスです。",
					"エラー",
					MB_ICONEXCLAMATION | MB_OK);
		abort();
	}
}
