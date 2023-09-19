#ifndef _VNDLABEL_H_
#define _VNDLABEL_H_

#include "VNDialog.h"
#include "VNString.h"


class VNDLabel : public VNDialog {
public:
	bool DoModal( VNString&, HWND );

private:
	char	*edit_string;

	// virtual
	bool OnInit();
	void OnCommand( WPARAM, LPARAM );
};


#endif
