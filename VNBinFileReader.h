#ifndef _VNBINFILEREADER_H_
#define _VNBINFILEREADER_H_

#include <stdio.h>
#include "VNBinFileWriter.h"


class VNBinFileReader{
public:
	VNBinFileReader();
	~VNBinFileReader();

	bool	Open( const char * );
	bool	GetInt( signed long& );
	bool	GetString( VNString& );
	bool	Close();

	// throws VNIOException
	VNBinFileReader( const char * );
	void	OpenEx( const char * );
	signed long GetIntEx();
	VNString GetStringEx();
	void	CloseEx();

private:
	FILE	*fp;

	void DebugBreak( const char * );
};


#endif
