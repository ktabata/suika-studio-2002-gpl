#include "VNBinFileReader.h"
#include <windows.h>	// MessageBox


VNBinFileReader::VNBinFileReader() {
	fp = NULL;
}

VNBinFileReader::~VNBinFileReader() {
	if( fp != NULL )
		Close();
}

bool VNBinFileReader::Open( const char *fname ) {
	fp = fopen( fname, "rb" );
	return (fp == NULL) ? false : true;
}

bool VNBinFileReader::GetInt( signed long& i ) {
	int c = fgetc( fp );

	if( c =! BINFILE_VALTYPE_INT )
		return false;

	if( c == EOF )
		return false;

	int j;
	if( fread( &j, sizeof( signed int ), 1, fp ) < 1 )
		return false;

	i = j;
	return true;
}

bool VNBinFileReader::GetString( VNString& str ) {
	int c = fgetc( fp );

	if( c != BINFILE_VALTYPE_STRING )
		return false;

	if( c == EOF )
		return false;

	int len;
	if( fread( &len, sizeof(signed long), 1, fp ) < 1 )
		return false;

	if( len < 0 )
		DebugBreak("•¶Žš—ñ‚Ì’·‚³‚ª•‰‚Ì’l");

	char *buf = new char[len+1];
	if( len != 0 ) {
		if( fread( buf, len, 1, fp ) < 1 )
			return false;
	}
	buf[len] = '\0';

	str = buf;

	delete[] buf;
	return true;
}

bool VNBinFileReader::Close() {
	if( fp == NULL )
		return false;
	return (fclose( fp ) == 0) ? true : false;
}

void VNBinFileReader::DebugBreak( const char *s ) {
	MessageBox (NULL, s, "VNBinFileReader “à•”ƒGƒ‰[", MB_ICONEXCLAMATION|MB_OK);
//	abort();
	_asm{int 3}
}


VNBinFileReader::VNBinFileReader( const char *fname ) {
	fp = NULL;
	OpenEx( fname );
}

void VNBinFileReader::OpenEx( const char *fname ) {
	if( ! Open( fname ) )
		throw new VNIOException(NULL);
}

signed long VNBinFileReader::GetIntEx() {
	signed long i;
	if( ! GetInt( i ) )
		throw new VNIOException(NULL);
	return i;
}

VNString VNBinFileReader::GetStringEx() {
	VNString s;
	if( ! GetString( s ) )
		throw new VNIOException(NULL);
	return s;
}

void VNBinFileReader::CloseEx() {
	if( ! Close() )
		throw new VNIOException(NULL);
}
