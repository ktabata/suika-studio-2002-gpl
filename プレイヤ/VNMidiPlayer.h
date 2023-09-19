#ifndef _VNMIDIPLAYER_H_
#define _VNMIDIPLAYER_H_

#include <windows.h>
#include <mmsystem.h>
#include "VNString.h"


class VNMidiPlayer {
	friend void OnMidiPlayerMciNotify( WPARAM, LPARAM );

public:
	VNMidiPlayer();
	~VNMidiPlayer();

	void Play( const char * );
	void Stop();

private:
	static HWND		hWnd;
	static bool		is_instanciated;
	static VNString	last_filename;

	void Sync();
};


#endif
