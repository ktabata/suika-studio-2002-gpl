#ifndef _VNDCDDA_H_
#define _VNDCDDA_H_

#include "VNDialog.h"
#include "VNString.h"


class VNDCdda : public VNDialog {
public:
	bool DoModal( int&, int&, HWND );

private:
	char	*edit_track;
	char	*edit_millisec;

	// virtual
	bool OnInit();
	void OnCommand( WPARAM, LPARAM );
};


#endif
