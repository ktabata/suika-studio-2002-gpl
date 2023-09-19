#ifndef _VNDIALOG_H_
#define _VNDIALOG_H_

#include <windows.h>

#define VND_LIST_ITEMS	(256)

class VNDialog {
	friend int CALLBACK VNDialog_DlgProc (HWND, UINT, WPARAM, LPARAM);

public:
	VNDialog();
	~VNDialog();
	int DoModal( const char *, const char *, HWND );

protected:
	HWND	hWnd;

	virtual bool OnInit();
	virtual void OnClose();
	virtual void OnCommand( WPARAM, LPARAM );
	virtual void OnSize( int, int );

private:
	char	*title;

	static VNDialog *FromHandle( HWND );
	void AddToList();
	void DeleteFromList();
	static int SearchItem( VNDialog * );
	static VNDialog *list[VND_LIST_ITEMS];
	static bool	is_initialized;
	static int	count;

	void DebugBreak( const char * );
};


#endif
