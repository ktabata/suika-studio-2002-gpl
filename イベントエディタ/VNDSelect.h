#ifndef _VNDSELECT_H_
#define _VNDSELECT_H_

#include "VNDialog.h"
#include "VNString.h"


class VNDSelect : public VNDialog {
public:
	bool DoModal( VNString *[3], VNString *[3], HWND );

private:
	char	*edit_msg[3];
	char	*edit_label[3];

	// virtual
	bool OnInit();
	void OnCommand( WPARAM, LPARAM );
};


#endif
