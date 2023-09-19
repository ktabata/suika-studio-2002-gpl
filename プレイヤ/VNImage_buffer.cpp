#include "VNImage.h"


bool VNImage::CreateRGBBuffer() {
	// width と height はセットされている

	// 画像１ラインのバイト数を計算
	bpl_rgb = width * 3;
	if( width % 4 )
		bpl_rgb += 4 - (bpl_rgb % 4);

	// HBITMAPハンドルを作成する
	BITMAPINFO	bi;

	::memset( &bi, 0, sizeof(BITMAPINFO) );
	bi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth		= width;
	bi.bmiHeader.biHeight		= height;
	bi.bmiHeader.biPlanes		= 1;
	bi.bmiHeader.biBitCount		= 24;
	bi.bmiHeader.biCompression	= BI_RGB;

	hDCRGB = ::CreateCompatibleDC( NULL );
	if( hDCRGB == NULL )
		return false;

	hBitmapRGB = ::CreateDIBSection( hDCRGB, &bi, DIB_RGB_COLORS, (VOID **)&image_rgb, NULL, 0);
	if( ( hBitmapRGB == NULL ) || ( image_rgb == NULL ) ) {
		::DeleteDC( hDCRGB );
		return false;
	}

	// ビットマップをデバイスコンテキストに選択する
	::SelectObject( hDCRGB, hBitmapRGB );

	return true;
}

bool VNImage::CreateMaskBuffer() {
	if( ! is_mask_enable )
		return false;

	// 画像１ラインのバイト数を計算
	if( width % 32 != 0 )
		bpl_mask = (width + 32 - width % 32) / 8;
	else
		bpl_mask = width / 8;


	// HBITMAPハンドルを作成する
	BITMAPINFO	*bi;
	int size = sizeof(BITMAPINFO) + sizeof(RGBQUAD);

	bi = (BITMAPINFO*) malloc( size );
	::memset( bi, 0, size );

	bi->bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	bi->bmiHeader.biWidth		= width;
	bi->bmiHeader.biHeight		= height;
	bi->bmiHeader.biPlanes		= 1;
	bi->bmiHeader.biBitCount	= 1;
	bi->bmiHeader.biCompression	= BI_RGB;

	bi->bmiColors[0].rgbBlue	= 0;
	bi->bmiColors[0].rgbGreen	= 0;
	bi->bmiColors[0].rgbRed		= 0;
	bi->bmiColors[0].rgbReserved= 0;
	bi->bmiColors[1].rgbBlue	= 255;
	bi->bmiColors[1].rgbGreen	= 255;
	bi->bmiColors[1].rgbRed		= 255;
	bi->bmiColors[1].rgbReserved= 0;

	hDCMask = ::CreateCompatibleDC( NULL );
	if( hDCMask == NULL ) {
		free(bi);
		return false;
	}

	hBitmapMask = ::CreateDIBSection( hDCMask, bi, DIB_RGB_COLORS, (VOID **)&image_mask, NULL, 0);
	if( ( hBitmapMask == NULL ) || ( image_mask == NULL ) ) {
		::DeleteDC( hDCMask );
		free(bi);
		return false;
	}

	// ビットマップをデバイスコンテキストに選択する
	::SelectObject( hDCMask, hBitmapMask );

	free(bi);

	return true;
}
