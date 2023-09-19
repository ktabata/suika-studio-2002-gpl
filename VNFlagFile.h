#ifndef _VNFLAGFILE_H_
#define _VNFLAGFILE_H_

#include "VNString.h"


class VNFlagFile {
public:
	VNFlagFile();
	~VNFlagFile();

	bool	Load( const char * );
	bool	Save( const char * );

	bool	AddFlag( const char *, bool );
	bool	DeleteFlag( const char * );
	bool	DeleteFlag( int );

	int		GetCount();
	bool	GetFlagName( int, VNString& );
	bool	GetFlagValue( const char *, bool& );
	bool	GetFlagValue( int, bool& );

	bool	SetFlagValue( const char *, bool );
	bool	SetFlagValue( int, bool );
	bool	InvertFlagValue( const char * );
	bool	InvertFlagValue( int );

	int		SearchFlag( const char * );

private:
	VNString	**flag_name;
	bool		*value;

	int		item_count;
	int		allocated_count;
	void	Realloc();

	void	IsValidIndex( int n );
};


#endif
