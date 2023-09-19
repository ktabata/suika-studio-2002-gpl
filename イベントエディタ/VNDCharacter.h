#ifndef _VNDCHARACTER_H_
#define _VNDCHARACTER_H_

#include "VNDialog.h"
#include "VNString.h"


class VNDCharacter : public VNDialog {
public:
	bool DoModal( int&, int&, VNString&, VNString&, HWND );

private:
	char	*edit_color;
	char	*edit_mask;
	int		cb_h;
	int		cb_v;

	// virtual
	bool OnInit();
	void OnCommand( WPARAM, LPARAM );
};


#endif
