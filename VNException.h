#ifndef _VNEXCEPTION_H_
#define _VNEXCEPTION_H_

#include "VNString.h"


class VNException {
public:
	VNException( const char * );
	operator VNString() const;

protected:
	VNString str;
};


class VNIOException : public VNException{
public:
	VNIOException( const char * );
};


#endif
