#ifndef _VNDFLAGEDIT_ADD_H_
#define _VNDFLAGEDIT_ADD_H_

#include "VNDialog.h"
#include "VNString.h"

class VNDFlagEdit_Add : public VNDialog {
public:
	bool DoModal( VNString&, HWND );

private:
	char	edit_string[1024];

	// virtual
	bool OnInit();
	void OnCommand( WPARAM, LPARAM );
};


#endif
