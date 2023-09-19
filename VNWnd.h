#ifndef _VNWND_H_
#define _VNWND_H_

#include <windows.h>

#define VNWND_WINDOW_MAX		(256)


class VNWindowCloseException {
};


class VNWnd {
	friend long WINAPI VNWnd_WndProc (HWND, UINT, WPARAM, LPARAM);

public:
	VNWnd();
	~VNWnd();

	bool	Open( const char *,
				  const char *,
				  const char *,
				  int, int, int,
				  int, int, int, int, VNWnd * );
	void	Close();
	void	Sync();
	bool	IsOpen();

	HWND	GetHWND();
	static void AppMessageLoop();

protected:
	HWND	hWnd;
	HMENU	hMenu;

	bool	is_open;

	virtual void OnClose();
	virtual void OnDestroy();
	virtual void OnCommand( WPARAM, LPARAM );
	virtual void OnPaint();
	virtual void OnMouseMove( WPARAM, LPARAM );
	virtual void OnLButtonDown( WPARAM, LPARAM );
	virtual void OnLButtonUp( WPARAM, LPARAM );
	virtual void OnRButtonDown( WPARAM, LPARAM );
	virtual void OnRButtonUp( WPARAM, LPARAM );
	virtual void OnKeyDown( WPARAM, LPARAM );
	virtual void OnKeyUp( WPARAM, LPARAM );

private:
	static VNWnd *FromHandle( HWND );

	void	AddToList();
	void	DeleteFromList();
	static int SearchItem( VNWnd * );

	static VNWnd	*list[VNWND_WINDOW_MAX];
	static bool	is_initialized;
	static int	count;

	static void DebugBreak();
};


#endif
