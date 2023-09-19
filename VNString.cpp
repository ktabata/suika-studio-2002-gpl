#include "VNString.h"
#include <stdio.h>	// sprintf
#include <string.h>	// strlen, strcat, strcmp
#include <windows.h>	// MessageBox


VNString::VNString() {
	len = 0;
	str = new char [1];
	*str = '\0';
}

VNString::~VNString() {
	delete[] str;
}

VNString::VNString( const VNString& s ) {
	len = s.len;
	str = new char[len+1];
	strcpy( str, s.str );
	CheckLength();
}

VNString::VNString( const char *str ) {
	len = strlen(str);
	this->str = new char[len+1];
	strcpy( this->str, str );
	CheckLength();
}

VNString::VNString( const char *str, int n ) {
	len = n;
	this->str = new char [n+1];
	memcpy( this->str, str, n );
	this->str[n] = '\0';
	CheckLength();
}

VNString::VNString( int i ) {
	str = new char [20];
	len = 20;
	sprintf( str, "%d", i );
}

VNString& VNString::operator=( const char *str ) {
	if( this->str != NULL )
		delete[] this->str;

	len = strlen(str);
	this->str = new char[len+1];
	strcpy( this->str, str );

	CheckLength();
	return *this;
}

VNString& VNString::operator=( const VNString& s ) {
	// s=s のような代入を防ぐ
	if( str != s.str ) {
		if( str != NULL )
			delete[] str;

		len = strlen(s.str);
		str = new char[len+1];
		strcpy( str, s.str );
	}
	CheckLength();
	return *this;
}

const char *VNString::c_str() const {
	return str;
}

VNString VNString::operator+( const char *s ) {
	char *tmp = new char [ len + strlen(s) + 1 ];
	strcpy( tmp, str );
	strcat( tmp, s );
	VNString res( tmp );
	delete[] tmp;
	return res;
}

VNString VNString::operator+( const VNString& s ) {
	char *tmp = new char [ len + s.len + 1 ];
	strcpy( tmp, str );
	strcat( tmp, s.str );
	VNString res( tmp );
	delete[] tmp;
	return res;
}

VNString& VNString::operator+=( const char *str ) {
	len += strlen(str);
	char *ptr = new char[len+1];
	strcpy( ptr, this->str );
	strcat( ptr, str );
	delete[] this->str;
	this->str = ptr;
	CheckLength();
	return *this;
}

VNString& VNString::operator+=( const VNString& str ) {
	len += str.len;
	char *ptr = new char[len+1];
	strcpy( ptr, this->str );
	strcat( ptr, str.str );
	delete[] this->str;
	this->str = ptr;
	CheckLength();
	return *this;
}

void VNString::CopyTo( char *d ) const {
	CheckLength();

	char *s = str;
	for( int i = 0; i < len; i++ ) {
		*d++ = *s++;
	}
	*d = '\0';
}

void VNString::CheckLength() const {
	if( len < 0 ) {
		MessageBox (NULL,
					"VNString : 文字列の長さに０未満の値が指定されました。",
					"エラー",
					MB_ICONEXCLAMATION | MB_OK);
		abort();
	}
}

VNString::operator const char *() {
	return c_str();
}

