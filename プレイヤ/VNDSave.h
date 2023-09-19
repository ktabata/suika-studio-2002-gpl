#ifndef _VNDSAVE_H_
#define _VNDSAVE_H_

#include "VNDialog.h"
#include "VNEventFile.h"
#include "VNFlagFile.h"


class VNDSave : public VNDialog {
public:
	int DoModal( HWND );

private:
	int		select;

	// virtual
	bool OnInit();
	void OnCommand( WPARAM, LPARAM );

	void AddItemToList( const char * );
	void GetSelectIndex();
};


#endif
