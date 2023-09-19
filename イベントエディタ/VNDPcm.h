#ifndef _VNDPCM_H_
#define _VNDPCM_H_

#include "VNDialog.h"
#include "VNString.h"


class VNDPcm : public VNDialog {
public:
	bool DoModal( VNString&, HWND );

private:
	char	*edit_string;

	// virtual
	bool OnInit();
	void OnCommand( WPARAM, LPARAM );
};


#endif
