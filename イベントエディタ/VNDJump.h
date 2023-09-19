#ifndef _VNDJUMP_H_
#define _VNDJUMP_H_

#include "VNDialog.h"
#include "VNString.h"


class VNDJump : public VNDialog {
public:
	bool DoModal( VNString&, VNString&, int&, VNString&, HWND );

private:
	char	*edit_event;
	char	*edit_label;
	char	*edit_flag;
	int		cb;

	// virtual
	bool OnInit();
	void OnCommand( WPARAM, LPARAM );
};


#endif
