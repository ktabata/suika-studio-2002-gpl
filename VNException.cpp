#include "VNException.h"
#include <stddef.h>


VNException::VNException( const char *s ) {
	if( s != NULL )
		str = s;
	else
		str = "Exception throwed.";
}

VNException::operator VNString() const {
	return str;
}


VNIOException::VNIOException( const char *s )
 : VNException( s ) {
}
