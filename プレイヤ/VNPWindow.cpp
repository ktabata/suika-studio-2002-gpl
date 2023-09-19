#include "VNPWindow.h"
#include "resource.h"


VNPWindow::VNPWindow() {
	InitMembers();
}

VNPWindow::~VNPWindow() {
	SetFullScreen( false );
	Close();
	if( back_buffer != NULL ) {
		delete back_buffer;
		back_buffer = NULL;
	}
}

bool VNPWindow::Open( const char *title, int width, int height ) {
	// メンバを初期化
	InitMembers();

	this->width = width;
	this->height = height;

	// バックバッファを作る
	back_buffer = new VNImage();
	if( ! back_buffer->CreateRGB( width, height ) )
		return false;

	back_buffer->Clear();

	int h = height +
			GetSystemMetrics( SM_CYCAPTION ) +
			GetSystemMetrics( SM_CYFIXEDFRAME ) * 2 +
			GetSystemMetrics( SM_CYMENU );

	if( ! VNWnd::Open( title,
					   "IDR_MENU",
					   "IDI_ICON",
					   WHITE_BRUSH,
					   CW_USEDEFAULT, CW_USEDEFAULT,
					   width + GetSystemMetrics( SM_CXFIXEDFRAME ) * 2,
					   h,
					   WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_OVERLAPPED,
					   0,
					   NULL ) ) {
		delete back_buffer;
		return false;
	}

	is_menu_visible = true;
	return true;
}

VNImage *VNPWindow::GetBackBuffer() {
	return back_buffer;
}

int VNPWindow::GetMousePosX() {
	return mouse_pos_x;
}

int VNPWindow::GetMousePosY() {
	return mouse_pos_y;
}

void VNPWindow::Flip() {
	if( back_buffer == NULL )
		return;

	HDC hDC = GetDC (hWnd);

	// バックバッファの内容をウインドウにBitBlt()で描画
	BitBlt(		
		hDC,
		0,0,
		width, height,
		back_buffer->GetDC(),
		0, 0,
		SRCCOPY );

	ReleaseDC (hWnd,hDC);
}

void VNPWindow::SetTitle( const char *title ) {
	SetWindowText (hWnd, title);
}

void VNPWindow::Update( int x, int y, int width, int height ) {
	if( back_buffer == NULL )
		return;

	HDC hDC = GetDC (hWnd);

	// バックバッファの内容をウインドウにBitBlt()で描画
	BitBlt(		
		hDC,
		x,y,
		width, height,
		back_buffer->GetDC(),
		x, y,
		SRCCOPY );

	ReleaseDC (hWnd,hDC);
}

void VNPWindow::SetMenuVisible( bool b ) {
	is_menu_visible = b;
	::SetMenu( hWnd, b ? hMenu : NULL );
}

void VNPWindow::SetFullScreen( bool b ) {
	if( is_full_screen == b )
		return;

	if( b == true ) {
		// 解像度を640x480にする
		is_full_screen = true;

		HDC		hDCDisp = ::GetDC( NULL );
		int		bpp = ::GetDeviceCaps( hDCDisp, BITSPIXEL );
		ReleaseDC( NULL, hDCDisp );

		DEVMODE dm;
		DWORD	n = 0;
		while( ::EnumDisplaySettings( NULL, n, &dm ) ) {
			if( dm.dmPelsWidth == (DWORD)width &&
				dm.dmPelsHeight == (DWORD)height &&
				dm.dmBitsPerPel == (DWORD)bpp )	{
				::ChangeDisplaySettings( &dm, 4 );
				return;
			}
			n++;
		}
	} else {
		// 解像度を元に戻す
		is_full_screen = false;

		DEVMODE dm;
		dm.dmSize = sizeof(dm);
		dm.dmFields = 0;
		::ChangeDisplaySettings( &dm, 0x00000004 /* CDS_FULLSCREEN */ );
	}
}

void VNPWindow::SetMousePos( int x, int y ) {
	mouse_pos_x = x;
	mouse_pos_y = y;
}

bool VNPWindow::IsMouseLDown() {
	return is_mouse_l_down;
}

bool VNPWindow::IsMouseRDown() {
	return is_mouse_r_down;
}

void VNPWindow::OnLButtonDown( WPARAM wParam, LPARAM lParam ) {
	SetMousePos( LOWORD(lParam), HIWORD(lParam) );
	is_mouse_l_down = true;
}

void VNPWindow::OnLButtonUp( WPARAM wParam, LPARAM lParam ) {
	SetMousePos( LOWORD(lParam), HIWORD(lParam) );
	is_mouse_l_down = false;
}

void VNPWindow::OnRButtonDown( WPARAM wParam, LPARAM lParam ) {
	SetMousePos( LOWORD(lParam), HIWORD(lParam) );
	is_mouse_r_down = true;
}

void VNPWindow::OnRButtonUp( WPARAM wParam, LPARAM lParam ) {
	SetMousePos( LOWORD(lParam), HIWORD(lParam) );
	is_mouse_r_down = false;
}

void VNPWindow::OnKeyDown( WPARAM wParam, LPARAM lParam ) {
	if( (int) wParam == VK_CONTROL )
		is_ctrl_down = true;
}

void VNPWindow::OnKeyUp( WPARAM wParam, LPARAM lParam ) {
	if( (int) wParam == VK_CONTROL )
		is_ctrl_down = false;
}

bool VNPWindow::IsCtrlDown() {
	bool b = (GetAsyncKeyState( VK_CONTROL ) & 0x8000) ? true : false;
	return (is_ctrl_down && b) ? true : false;
}

bool VNPWindow::IsSavePushed() {
	bool b = is_save_pushed;
	is_save_pushed = false;
	return b;
}

bool VNPWindow::IsLoadPushed() {
	bool b = is_load_pushed;
	is_load_pushed = false;
	return b;
}

bool VNPWindow::IsBackLogPushed() {
	bool b = is_backlog_pushed;
	is_backlog_pushed = false;
	return b;
}

void VNPWindow::OnPaint() {
	PAINTSTRUCT ps;
	BeginPaint (hWnd, &ps);
	Flip();
	EndPaint (hWnd, &ps);
}

void VNPWindow::OnCommand( WPARAM wParam, LPARAM lParam ) {
	int id = LOWORD(wParam);

	if (id == ID_MENUITEMSAVE) {
		is_save_pushed = true;
		return;
	} else if (id == ID_MENUITEMLOAD) {
		is_load_pushed = true;
		return;
	} else if (id == ID_MENUITEMBACKLOG) {
		is_backlog_pushed = true;
		return;
	} else if (id == ID_MENUITEMSCREEN) {
	// メニューバーの「画面モード」アイテムが選択された
		if( is_full_screen == false ) {
		// 現在ウィンドウモード
			SetFullScreen( true );
			SetMenuVisible( false );
			::SetWindowLong (hWnd, GWL_STYLE, WS_POPUP);
			::SetWindowLong (hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);
			::SetWindowPos (hWnd, HWND_TOP, 0, 0, width, height, SWP_SHOWWINDOW);
		} else {
		// 現在フルスクリーンモード
			SetFullScreen( false );
			SetMenuVisible( true );
			::SetWindowLong (hWnd, GWL_EXSTYLE, 0);
			::SetWindowLong (hWnd, GWL_STYLE, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_OVERLAPPED);
			int w = width + ::GetSystemMetrics (SM_CXFIXEDFRAME) * 2;
			int h = height + ::GetSystemMetrics (SM_CYCAPTION) + ::GetSystemMetrics (SM_CYFIXEDFRAME) * 2 + ::GetSystemMetrics (SM_CYMENU);
			::SetWindowPos (hWnd, HWND_TOP, 0, 0, w, h, SWP_SHOWWINDOW);
		}
		return;
	}
}

void VNPWindow::OnMouseMove( WPARAM wParam, LPARAM lParam ) {
	// マウスカーソルの位置を更新
	mouse_pos_x = LOWORD(lParam);
	mouse_pos_y = HIWORD(lParam);

	// フルスクリーンモードの時、マウスカーソルが画面上方にある間だけメニューを表示
	if( ! is_full_screen )
		return;

	if( is_menu_visible == false ) {
		if( mouse_pos_y < ::GetSystemMetrics(SM_CYMENU) )
			SetMenuVisible( true );
	} else if( mouse_pos_y >= 0 )
		// ↑なぜメニューの高さではなく０か？：メニューが表示されているときは、メニューの高さだけ原点が移動するから。
		VNPWindow::SetMenuVisible( false );
}

void VNPWindow::OnClose() {
	int res = MessageBox (
				hWnd,
				"終了してもよろしいですか？",
				"確認",
				MB_OKCANCEL | MB_ICONQUESTION);
	if (res == IDOK) {
		SetFullScreen( false );
		Close();
	}
}

void VNPWindow::InitMembers() {
	back_buffer = NULL;
	is_menu_visible = false;
	is_full_screen = false;
	is_mouse_l_down = false;
	is_mouse_r_down = false;
	is_ctrl_down = false;
	mouse_pos_x = 0;
	mouse_pos_y = 0;
	is_save_pushed = false;
	is_load_pushed = false;
	is_backlog_pushed = false;
}
