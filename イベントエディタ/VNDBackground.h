#ifndef _VNDBACKGROUND_H_
#define _VNDBACKGROUND_H_

#include "VNDialog.h"
#include "VNString.h"


class VNDBackground : public VNDialog {
public:
	bool DoModal( VNString&, HWND );

private:
	char	*edit_string;

	// virtual
	bool OnInit();
	void OnCommand( WPARAM, LPARAM );
};


#endif
