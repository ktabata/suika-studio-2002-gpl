#include "VNPromptDrawer.h"
#include <windows.h>


VNPromptDrawer::VNPromptDrawer( VNImage& image, int x, int y, int w, int h ) : image( image ) {
	this->x = x;
	this->y = y;
	width = w;
	height = h;
}

void VNPromptDrawer::Start() {
	start_time = ::GetTickCount();
	last_s = width + 100;	// あり得ない値
}

bool VNPromptDrawer::NeedUpdate() {
	unsigned long t = ::GetTickCount();
	unsigned long lap = (t - start_time) % PD_MILLISEC;

	// 「左上の頂点」の右移動分（ドット）
	int		s = (int)( (float)width / (float)PD_MILLISEC * (float) lap );
	if( s == last_s )
		return false;

	return true;
}

void VNPromptDrawer::Draw() {
	unsigned long t = ::GetTickCount();
	unsigned long lap = (t - start_time) % PD_MILLISEC;

	// 「左上の頂点」の右移動分（ドット）
	int		s = (int)( (float)width / (float)PD_MILLISEC * (float) lap );
	last_s = s;

	POINT p[4];
	p[0].x = x + s;
	p[0].y = y;
	p[1].x = x + width - s;
	p[1].y = y;
	p[2].x = x + width / 2;
	p[2].y = y + height;
	p[3].x = x + s;
	p[3].y = y;

	HDC hDC = image.GetDC();
	::Polygon( hDC, p, 4 );
}

/* Polygon()を呼び出すループを書くと、なぜかＰＣから音がする（笑）。
  それで、呼び出しの回数を減らすために、苦し紛れの作として
  NeedUpdate()を作った。 */
