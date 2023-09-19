#ifndef _VNDCOMMENT_H_
#define _VNDCOMMENT_H_

#include "VNDialog.h"
#include "VNString.h"


class VNDComment : public VNDialog {
public:
	bool DoModal( VNString&, HWND );

private:
	char	*edit_string;

	// virtual
	bool OnInit();
	void OnCommand( WPARAM, LPARAM );
};


#endif
