#include <string.h>
#include "VNTextDrawer.h"


VNTextDrawer::VNTextDrawer( VNImage *image ) {
	this->image = image;
	hDC = NULL;
	hFont = NULL;
}

VNTextDrawer::~VNTextDrawer() {
	if( hDC != NULL )
		DeleteDC (hDC);
	if( hFont != NULL )
		DeleteObject (hFont);
}

void VNTextDrawer::SelectFont( int width, int height, const char *font_name ) {
	// デフォルトのフォント名
	if( stricmp( font_name, "default" ) == 0 )
		font_name = "ＭＳ ゴシック";
	
	hFont = CreateFont(
		height, width,
		0, 0,
		400, FALSE,
		FALSE, FALSE,
		SHIFTJIS_CHARSET,
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
//		FF_MODERN | FIXED_PITCH,
		FF_DONTCARE | VARIABLE_PITCH,
		font_name );

	hDC = image->GetDC();
	::SelectObject(hDC, hFont);
	::SetBkMode (hDC, TRANSPARENT);
}

void VNTextDrawer::SetColor( unsigned char r, unsigned char g, unsigned char b ) {
	::SetTextColor (hDC, b | (g<<8) | (r<<16));
}


void VNTextDrawer::Draw( int x, int y, const char *msg, int len /* = -1 */ ) {
	if( len == -1 )
		len = strlen(msg);

	::TextOut (hDC, x, y, msg, len);
}

int VNTextDrawer::GetWidth( const char *str, int n ) {
	SIZE size;
	if( str == NULL )
		return -1;
	if (GetTextExtentPoint32 (hDC, str, n, &size) == FALSE)
		return -1;
	return size.cx;
}
