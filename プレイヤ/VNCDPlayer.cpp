#include "VNCDPlayer.h"
#include <assert.h>

bool		VNCDPlayer::is_instanciated = false;
bool		VNCDPlayer::loop = true;
VNString	VNCDPlayer::last_string;
HWND		VNCDPlayer::hWnd = NULL;

static long WINAPI VNCDPlayer_WndProc(HWND, UINT, WPARAM, LPARAM);
static void OnCDPlayerMciNotify( WPARAM, LPARAM );


VNCDPlayer::VNCDPlayer() {
	if( is_instanciated )
		abort();

	// MCIメッセージ用の表示しないウィンドウを作る
	HINSTANCE hInstance = GetModuleHandle(NULL);
	char		classname[] = "VNCDPlayer";
	WNDCLASS	wc;
	wc.style			= 0;
	wc.lpfnWndProc		= VNCDPlayer_WndProc;
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

	char ret[256];
	::mciSendString ("open cdaudio", ret, 255, hWnd);
	::mciSendString ("set cdaudio time format tmsf", ret, 255, hWnd);
	Sync();
}

VNCDPlayer::~VNCDPlayer() {
	char ret[256];
	Stop();
	::mciSendString ("close cdaudio", ret, 255, hWnd);
	Sync();
	DestroyWindow (hWnd);
	Sync();
}

void VNCDPlayer::Sync() {
	MSG		msg;
	while( ::PeekMessage (&msg, hWnd, 0, 0, PM_REMOVE) ) {
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}
}

long WINAPI VNCDPlayer_WndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
	{
	case MM_MCINOTIFY:
		{
			OnCDPlayerMciNotify( wParam, lParam );
			break;
		}
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

void OnCDPlayerMciNotify( WPARAM wParam, LPARAM lParam ) {
	if( wParam != MCI_NOTIFY_SUCCESSFUL )
		return;

	if( VNCDPlayer::loop ) {
		char ret[256];
		::mciSendString (
			VNCDPlayer::last_string.c_str(),
			ret,
			255,
			VNCDPlayer::hWnd);
	}
}


void VNCDPlayer::Play( int from, int to, bool loop ) {
	char ret[256];

	VNCDPlayer::loop = loop;
	last_string = "play cdaudio from ";
	last_string = last_string + (VNString)from + " to " + (VNString)to + " notify";

	::mciSendString (last_string.c_str(), ret, 255, hWnd);
	Sync();
}

void VNCDPlayer::Stop() {
	char ret[256];
	::mciSendString ("stop cdaudio", ret, 255, hWnd);
	Sync();
}

