#ifndef _KTEXTDRAWER_H_
#define _KTEXTDRAWER_H_

#include "VNImage.h"


class VNTextDrawer {
public:
	VNTextDrawer( VNImage * );
	~VNTextDrawer();
	void	SelectFont( int, int, const char * );
	void	SetColor( unsigned char, unsigned char, unsigned char );
	void	Draw( int, int, const char *, int = -1 );
	int		GetWidth( const char *, int );

private:
	VNImage	*image;
	HDC		hDC;
	HFONT	hFont;
};


#endif
