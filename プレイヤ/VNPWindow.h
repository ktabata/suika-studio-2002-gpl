#ifndef _VNPLAYERWINDOW_H_
#define _VNPLAYERWINDOW_H_

#include "VNWnd.h"
#include "VNImage.h"


class VNPWindow : public VNWnd {
public:
	VNPWindow();
	~VNPWindow();

	bool	Open( const char *, int, int );

	VNImage *GetBackBuffer();
	void	Flip();
	void	Update( int, int, int, int );
	bool	IsMouseLDown();
	bool	IsMouseRDown();
	bool	IsCtrlDown();
	int		GetMousePosX();
	int		GetMousePosY();
	bool	IsSavePushed();
	bool	IsLoadPushed();
	bool	IsBackLogPushed();
	void	SetTitle( const char * );

private:
	int		width;
	int		height;
	VNImage		*back_buffer;
	bool	is_menu_visible;
	bool	is_full_screen;
	bool	is_mouse_r_down;
	bool	is_mouse_l_down;
	bool	is_ctrl_down;
	int		mouse_pos_x;
	int		mouse_pos_y;
	bool	is_save_pushed;
	bool	is_load_pushed;
	bool	is_backlog_pushed;

	void	SetMenuVisible( bool b );
	void	SetFullScreen( bool b );
	void	SetMousePos( int, int );
	void	InitMembers();

	void	OnClose();
	void	OnMouseMove( WPARAM, LPARAM );
	void	OnCommand( WPARAM, LPARAM );
	void	OnPaint();
	void	OnLButtonDown( WPARAM , LPARAM );
	void	OnLButtonUp( WPARAM , LPARAM );
	void	OnRButtonDown( WPARAM , LPARAM );
	void	OnRButtonUp( WPARAM , LPARAM );
	void	OnKeyDown( WPARAM , LPARAM );
	void	OnKeyUp( WPARAM , LPARAM );
};


#endif
