#ifndef _VNDWAIT_H_
#define _VNDWAIT_H_

#include "VNDialog.h"
#include "VNString.h"


class VNDWait : public VNDialog {
public:
	bool DoModal( int&, HWND );

private:
	char	*edit_string;

	// virtual
	bool OnInit();
	void OnCommand( WPARAM, LPARAM );
};


#endif
