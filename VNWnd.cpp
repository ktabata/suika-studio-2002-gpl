#include "VNWnd.h"


long WINAPI VNWnd_WndProc(HWND, UINT, WPARAM, LPARAM);

VNWnd *VNWnd::list[VNWND_WINDOW_MAX];
bool	VNWnd::is_initialized = false;
int		VNWnd::count = 0;


VNWnd::VNWnd() {
	// static�����o�̏������i����̂݁j
	if( ! is_initialized ) {
		for( int i = 0; i <= VNWND_WINDOW_MAX; i++ )
			list[i] = NULL;
		is_initialized = true;
	}

	hWnd = NULL;
	hMenu = NULL;
	is_open = false;
}

VNWnd::~VNWnd() {
	// ���s���ɍ폜���ꂽ
	if( hWnd != NULL )
		DebugBreak();
}

HWND VNWnd::GetHWND() {
	return hWnd;
}

bool VNWnd::Open( const char *title,
				  const char *menu,
				  const char *icon,
				  int bgcolor,
				  int x, int y,
				  int w, int h,
				  int style,
				  int style_ex,
				  VNWnd *parent ) {
	if( is_open )
		DebugBreak();

	// �A�v���P�[�V�����̃C���X�^���X�n���h�����擾
	HINSTANCE hInstance = GetModuleHandle(NULL);

	// ���j���[�����[�h
	if( menu != NULL ) {
		hMenu = ::LoadMenu (hInstance, menu);
		if( hMenu == NULL )
			DebugBreak();
	} else
		hMenu = NULL;

	// �E�B���h�E�N���X�̓o�^
	char		classname[] = "VNWnd";
	WNDCLASS	wc;
	wc.style			= 0;
	wc.lpfnWndProc		= VNWnd_WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon (hInstance, icon);
	wc.hCursor			= LoadCursor (NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject (bgcolor);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= classname;
	RegisterClass (&wc);

	// �E�B���h�E���쐬
	HWND hWndParent = NULL;
	if( parent != NULL )
		hWndParent = parent->hWnd;

	hWnd = CreateWindowEx (
				style_ex,
				classname,
				title,
				style,
				x, y, w, h,
				hWndParent,
				hMenu,
				hInstance,
				NULL);
	if( hWnd == NULL )
		DebugBreak();

	// �E�C���h�E��\��
	ShowWindow (hWnd, SW_SHOWNA);
	UpdateWindow (hWnd);
    SetFocus (hWnd);

	// ����
	AddToList();
	is_open = true;
	return true;
}

void VNWnd::Sync() {
	MSG		msg;
	while( ::PeekMessage (&msg, NULL, 0, 0, PM_REMOVE) ) {
		TranslateMessage (&msg);
		DispatchMessage (&msg);

		if (hWnd == NULL)
			throw new VNWindowCloseException();
	}
}

bool VNWnd::IsOpen() {
	return (hWnd == NULL) ? false : true;
}

void VNWnd::Close() {
	if( hWnd == NULL )
		return;
	::DestroyWindow (hWnd);
}

void VNWnd::AppMessageLoop()
{
	if( count <= 0 )
		DebugBreak();

	MSG		msg;
	int		res;
	while( (res = ::GetMessage( &msg, NULL, 0, 0 ) ) != 0 ) {
		if( res == -1 )
			break;

		::TranslateMessage( &msg );
		::DispatchMessage( &msg );
	}
}

long WINAPI VNWnd_WndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	VNWnd *obj = VNWnd::FromHandle( hWnd );

	if ( obj != NULL ) {
	    switch (uMsg)
		{
		case WM_PAINT:
			obj->OnPaint();
			return 0;
		case WM_COMMAND:
			obj->OnCommand( wParam, lParam );
			return 0;
		case WM_MOUSEMOVE:
			obj->OnMouseMove( wParam, lParam );
			return 0;
		case WM_LBUTTONDOWN:
			obj->OnLButtonDown( wParam, lParam );
			return 0;
		case WM_LBUTTONUP:
			obj->OnLButtonUp( wParam, lParam );
			return 0;
		case WM_RBUTTONDOWN:
			obj->OnRButtonDown( wParam, lParam );
			return 0;
		case WM_RBUTTONUP:
			obj->OnRButtonUp( wParam, lParam );
			return 0;
		case WM_KEYDOWN:
			obj->OnKeyDown( wParam, lParam );
			return 0;
		case WM_KEYUP:
			obj->OnKeyUp( wParam, lParam );
			return 0;
		case WM_CLOSE:
			obj->OnClose();
			return 0;
		case WM_DESTROY:
			obj->OnDestroy();
			obj->DeleteFromList();
			return 0;
		}
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

void VNWnd::OnCommand( WPARAM wParam, LPARAM lParam ) {
	WORD	wNotifyCode = HIWORD(wParam);	// �ʒm�R�[�h
	WORD	wID = LOWORD(wParam);			// ���ځA�R���g���[���A�܂��̓A�N�Z�����[�^�� ID
	HWND	hWndCtl = (HWND) lParam;		// �R���g���[���̃n���h��
	return;
}

void VNWnd::OnClose() {
	DestroyWindow (hWnd);
}

void VNWnd::OnDestroy() { }
void VNWnd::OnPaint() { }
void VNWnd::OnMouseMove( WPARAM, LPARAM ) { }
void VNWnd::OnLButtonDown( WPARAM, LPARAM ) { }
void VNWnd::OnLButtonUp( WPARAM, LPARAM ) { }
void VNWnd::OnRButtonDown( WPARAM, LPARAM ) { }
void VNWnd::OnRButtonUp( WPARAM, LPARAM ) { }
void VNWnd::OnKeyDown( WPARAM, LPARAM ) { }
void VNWnd::OnKeyUp( WPARAM, LPARAM ) { }

VNWnd *VNWnd::FromHandle( HWND hWnd ) {
	for( int cnt = 0, i = 0; cnt < count && i < VNWND_WINDOW_MAX; i++ ) {
		if( list[i] == NULL )
			continue;
		if( list[i]->hWnd == hWnd )
			return list[i];	// found

		cnt++;
	}
	// not found
	return NULL;
}

int VNWnd::SearchItem( VNWnd *obj ) {
	// �I�u�W�F�N�g���X�g�����v���ڂ�T��
	for( int i = 0; i <= VNWND_WINDOW_MAX; i++ ) {
		if( list[i] == obj )
			return i;
	}
	return -1;
}

void VNWnd::AddToList() {
	if( ++count > VNWND_WINDOW_MAX )
		DebugBreak();

	// �󂫍��ڂ�T��
	int n = SearchItem( NULL );
	if( n < 0 )
		DebugBreak();

	list[n] = this;
}

void VNWnd::DeleteFromList() {
	if( count <= 0 )
		DebugBreak();

	int n = SearchItem( this );
	if( n < 0 )
		DebugBreak();

	MSG		msg;
	while( ::PeekMessage( &msg, list[n]->hWnd, 0, 0, PM_REMOVE ) ) {
		::TranslateMessage( &msg );
		::DispatchMessage( &msg );
	}

	if( list[n]->hMenu != NULL )
		::DestroyMenu( list[n]->hMenu );

	list[n]->is_open = false;
	list[n]->hWnd = NULL;

	list[n] = NULL;
	count--;
}

void VNWnd::DebugBreak() {
	MessageBox (NULL, "VNWnd�N���X���œ����G���[���������܂����B", "�G���[", MB_ICONEXCLAMATION | MB_OK);
	abort();
}
