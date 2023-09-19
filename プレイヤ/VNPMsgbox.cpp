#include "VNPMsgbox.h"
#include "assert.h"

#define DEF_WIDTH	(500)
#define DEF_HEIGHT	(100)
#define DEF_POS_X	(50)
#define DEF_POS_Y	(350)


VNPMsgbox::VNPMsgbox() { }
VNPMsgbox::~VNPMsgbox() { }

void VNPMsgbox::Create( VNWnd *parent ) {
	// バックバッファを作る
	back_buffer = new VNImage();
	assert( back_buffer->CreateRGB( DEF_WIDTH, DEF_HEIGHT ) );
	back_buffer->Clear();

	assert( VNWnd::Open( "msgbox",
						 NULL,
						 NULL,
						 BLACK_BRUSH,
						 DEF_POS_X, DEF_POS_Y,
						 DEF_WIDTH, DEF_HEIGHT,
						 WS_CHILD | WS_THICKFRAME,
						 WS_EX_TRANSPARENT | WS_EX_MDICHILD,
						 parent ) );
}

void VNPMsgbox::Flip() {
	return;
	if( back_buffer == NULL )
		return;

	HDC hDC = GetDC (hWnd);

	// バックバッファの内容をウインドウにBitBlt()で描画
	BitBlt(		
		hDC,
		0,0,
		DEF_WIDTH, DEF_HEIGHT,
		back_buffer->GetDC(),
		0, 0,
		SRCCOPY );

	ReleaseDC (hWnd, hDC);
}

void VNPMsgbox::OnDestroy() {
	delete back_buffer;
	back_buffer = NULL;
}

void VNPMsgbox::OnPaint() {
	PAINTSTRUCT ps;
	BeginPaint (hWnd, &ps);
	Flip();
	EndPaint (hWnd, &ps);
}

VNImage *VNPMsgbox::GetBackBuffer() {
	return back_buffer;
}
