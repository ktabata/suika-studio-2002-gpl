#ifndef _VNDSERIF_H_
#define _VNDSERIF_H_

#include "VNDialog.h"
#include "VNString.h"


class VNDSerif : public VNDialog {
public:
	bool DoModal( VNString&, VNString&, HWND );

private:
	char	*edit_name;
	char	*edit_serif;

	// virtual
	bool OnInit();
	void OnCommand( WPARAM, LPARAM );
};


#endif
