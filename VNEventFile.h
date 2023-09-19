#ifndef _VNEVENTFILE_H_
#define _VNEVENTFILE_H_

#include "VNEvent.h"


class VNEventFile {
public:
	VNEventFile();
	~VNEventFile();

	bool Load( const char * );
	bool Save( const char * );

	bool AddEvent( VNEvent * );
	bool InsertEvent( VNEvent *, int = -1 );
	bool DeleteEvent( int );
	VNEvent *GetEvent( int );
	int GetCount();
	int GetLabelOffset( const char * );

private:
	VNEvent	**list;
	int item_count;
	int allocated_count;
	void Realloc();

	void DebugBreak( const char * );
};


#endif
