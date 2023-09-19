#ifndef _VNDFLAG_H_
#define _VNDFLAG_H_

#include "VNDialog.h"
#include "VNString.h"


class VNDFlag : public VNDialog {
public:
	bool DoModal( VNString&, int&, HWND );

private:
	char	*edit;
	int		cb;

	// virtual
	bool OnInit();
	void OnCommand( WPARAM, LPARAM );
};


#endif
