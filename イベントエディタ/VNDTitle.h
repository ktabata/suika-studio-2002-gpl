#ifndef _VNDTITLE_H_
#define _VNDTITLE_H_

#include "VNDialog.h"
#include "VNString.h"


class VNDTitle : public VNDialog {
public:
	bool DoModal( VNString&, HWND );

private:
	char	*edit_string;

	// virtual
	bool OnInit();
	void OnCommand( WPARAM, LPARAM );
};


#endif
