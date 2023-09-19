// Studio K24J's Library

#ifndef _VNIMAGE_H_
#define _VNIMAGE_H_

#include <windows.h>

#define MASK_LEVELS		(28)

extern "C" {
	extern unsigned char alpha_table[256][256];
}


class VNImage {
public:
	VNImage();
	~VNImage();

	bool	CreateRGB( int width, int height );
	bool	LoadBmp( const char *, const char * = NULL );

	void	BitBlt( int, int, VNImage *, int = -1, int = -1, int = -1, int = -1 );
	void	MaskBlt( int , int, int, VNImage *, int = -1, int = -1, int = -1, int = -1 );

	void	DrawGradationBox( int, int, int, int,
							  unsigned char, unsigned char, unsigned char, unsigned char,
							  unsigned char, unsigned char, unsigned char, unsigned char );

	void	Clear( bool = false );

	int		GetWidth();
	int		GetHeight();

	HDC		GetDC();

protected:
	void	ReleaseAll();

	bool	CreateRGBBuffer();
	bool	CreateMaskBuffer();

	bool	LoadRGBBmp( const char * );
	bool	LoadMaskBmp( const char * );

	void	BitBlt_RGB( int, int, VNImage *, int, int, int, int );
	void	BitBlt_RGBM( int, int, VNImage *, int, int, int, int );
	void	MaskBlt_RGB( int, int, int, VNImage *, int, int, int, int );
	void	MaskBlt_RGBM( int, int, int, VNImage *, int, int, int, int );

private:
	int			width;
	int			height;

	unsigned char *image_rgb;
	int			bpl_rgb;		// bytes per line

	unsigned char *image_mask;
	int			bpl_mask;		// bytes per line

	bool		is_mask_enable;

	HBITMAP		hBitmapRGB;
	HBITMAP		hBitmapMask;
	HDC			hDCRGB;
	HDC			hDCMask;
};

#endif
