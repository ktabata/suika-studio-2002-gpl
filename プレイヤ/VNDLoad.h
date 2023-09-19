#ifndef _VNDLAOD_H_
#define _VNDLOAD_H_

#include "VNDialog.h"
#include "VNEventFile.h"
#include "VNFlagFile.h"


class VNDLoad : public VNDialog {
public:
	int DoModal( HWND );

private:
	int		select;
	bool	available[10];

	// virtual
	bool OnInit();
	void OnCommand( WPARAM, LPARAM );

	void AddItemToList( const char * );
	void GetSelectIndex();
};


#endif
