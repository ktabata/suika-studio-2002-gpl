#ifndef _VNDMIDI_H_
#define _VNDMIDI_H_

#include "VNDialog.h"
#include "VNString.h"


class VNDMidi : public VNDialog {
public:
	bool DoModal( VNString&, int&, HWND );

private:
	char	*edit_file;
	char	*edit_millisec;

	// virtual
	bool OnInit();
	void OnCommand( WPARAM, LPARAM );
};


#endif
