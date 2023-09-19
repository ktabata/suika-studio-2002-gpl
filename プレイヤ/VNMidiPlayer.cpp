#include "VNMidiPlayer.h"
#include <assert.h>

bool		VNMidiPlayer::is_instanciated = false;
VNString	VNMidiPlayer::last_filename;
HWND		VNMidiPlayer::hWnd = NULL;

static long WINAPI VNMidiPlayer_WndProc(HWND, UINT, WPARAM, LPARAM);
static void OnMidiPlayerMciNotify( WPARAM, LPARAM );


VNMidiPlayer::VNMidiPlayer() {
	if( is_instanciated )
		abort();

	// MCIメッセージ用の表示しないウィンドウを作る
	HINSTANCE hInstance = GetModuleHandle(NULL);
	char		classname[] = "VNMidiPlayer";
	WNDCLASS	wc;
	wc.style			= 0;
	wc.lpfnWndProc		= VNMidiPlayer_WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= NULL;
	wc.hCursor			= NULL;
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= classname;
	RegisterClass (&wc);

	hWnd = CreateWindow (classname, classname, WS_POPUP,
				0, 0, 100, 100, NULL, NULL, hInstance, NULL);
	if( hWnd == NULL )
		return;

	Sync();
}

VNMidiPlayer::~VNMidiPlayer() {
	Stop();
	DestroyWindow (hWnd);
	Sync();
}

void VNMidiPlayer::Sync() {
	MSG		msg;
	while( ::PeekMessage (&msg, hWnd, 0, 0, PM_REMOVE) ) {
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}
}

long WINAPI VNMidiPlayer_WndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
	{
	case MM_MCINOTIFY:
		{
			OnMidiPlayerMciNotify( wParam, lParam );
			break;
		}
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

void OnMidiPlayerMciNotify( WPARAM wParam, LPARAM lParam ) {
	if( wParam != MCI_NOTIFY_SUCCESSFUL )
		return;

	char ret[256];
	VNString open_str = "open ";
	open_str = open_str + VNMidiPlayer::last_filename + " type sequencer alias vnmidi";

	::mciSendString ("close vnmidi",ret, 255, VNMidiPlayer::hWnd);
	::mciSendString (open_str.c_str(), ret, 255, VNMidiPlayer::hWnd);
	::mciSendString ("play vnmidi notify", ret, 255, VNMidiPlayer::hWnd);
}


void VNMidiPlayer::Play( const char *filename ) {
	Stop();

	char ret[256];
	last_filename = filename;
	VNString open_str = "open ";
	open_str = open_str + filename + " type sequencer alias vnmidi";

	::mciSendString (open_str.c_str(), ret, 255, hWnd);
	::mciSendString ("play vnmidi notify", ret, 255, hWnd);
	Sync();
}

void VNMidiPlayer::Stop() {
	char ret[256];

	::mciSendString ("stop vnmidi",ret, 255, hWnd);
	::mciSendString ("close vnmidi",ret, 255, hWnd);
	Sync();
}
