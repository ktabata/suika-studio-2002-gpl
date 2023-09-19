#ifndef _VNDNARRATION_H_
#define _VNDNARRATION_H_

#include "VNDialog.h"
#include "VNString.h"


class VNDNarration : public VNDialog {
public:
	bool DoModal( VNString&, HWND );

private:
	char	*edit_string;

	// virtual
	bool OnInit();
	void OnCommand( WPARAM, LPARAM );
};


#endif
