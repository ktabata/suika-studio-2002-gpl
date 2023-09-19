#ifndef _VNBINFILEWRITER_H_
#define _VNBINFILEWRITER_H_

#include <stdio.h>
#include "VNException.h"

#define BINFILE_VALTYPE_INT		(1)
#define BINFILE_VALTYPE_STRING	(2)


class VNBinFileWriter {
public:
	VNBinFileWriter();
	~VNBinFileWriter();

	bool	Create( const char * );
	bool	PutInt( signed long );
	bool	PutString( const char * );
	bool	Close();

	// throws VNIOException
	VNBinFileWriter( const char * );
	void	CreateEx( const char * );
	void	PutIntEx( signed long );
	void	PutStringEx( const char * );
	void	CloseEx();

private:
	FILE	*fp;
};


#endif
