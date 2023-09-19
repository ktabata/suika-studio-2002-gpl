#include "VNBinFileWriter.h"
#include <string.h>


VNBinFileWriter::VNBinFileWriter() {
	fp = NULL;
}

VNBinFileWriter::~VNBinFileWriter() {
	if( fp != NULL )
		Close();
}

bool VNBinFileWriter::Create( const char *fname ) {
	fp = fopen( fname, "wb" );
	return (fp == NULL) ? false : true;
}

bool VNBinFileWriter::PutInt( signed long i ) {
	if( fputc( BINFILE_VALTYPE_INT, fp ) == EOF )
		return false;

	if( fwrite( &i, sizeof( signed long ), 1, fp ) < 1 )
		return false;

	return true;
}

bool VNBinFileWriter::PutString( const char *str ) {
	if( fputc( BINFILE_VALTYPE_STRING, fp ) == EOF )
		return false;

	int len = strlen( str );

	if( fwrite( &len, sizeof( int ), 1, fp ) < 1 )
		return false;

	if( len != 0 ) {
		if( fwrite( str, len, 1, fp ) < 1 )
			return false;
	}

	return true;
}

bool VNBinFileWriter::Close() {
	if( fp == NULL )
		return false;
	return (fclose( fp ) == 0) ? true : false;
}


VNBinFileWriter::VNBinFileWriter( const char *fname ) {
	fp = NULL;
	CreateEx( fname );
}

void VNBinFileWriter::CreateEx( const char *fname ) {
	if(! Create( fname ) )
		throw new VNIOException(NULL);
}

void VNBinFileWriter::PutIntEx( signed long i ) {
	if(! PutInt( i ) )
		throw new VNIOException(NULL);
}

void VNBinFileWriter::PutStringEx( const char *str ) {
	if(! PutString( str ) )
		throw new VNIOException(NULL);
}

void VNBinFileWriter::CloseEx() {
	if(! Close() )
		throw new VNIOException(NULL);
}
