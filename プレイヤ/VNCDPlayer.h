#ifndef _VNCDPLAYER_H_
#define _VNCDPLAYER_H_

#include <windows.h>
#include <mmsystem.h>
#include "VNString.h"


class VNCDPlayer {
	friend void OnCDPlayerMciNotify( WPARAM, LPARAM );

public:
	VNCDPlayer();
	~VNCDPlayer();

	void Play( int from, int to, bool loop );
	void Stop();

private:
	static HWND		hWnd;
	static bool		is_instanciated;
	static bool		loop;
	static VNString	last_string;

	void Sync();
};


#endif
