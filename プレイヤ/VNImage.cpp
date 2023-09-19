#include "VNImage.h"


VNImage::VNImage() {
	width	= 0;
	height	= 0;

	image_rgb	= NULL;
	image_mask	= NULL;

	hDCRGB		= NULL;
	hBitmapRGB	= NULL;

	hDCMask		= NULL;
	hBitmapMask	= NULL;

	is_mask_enable = false;
}

void VNImage::ReleaseAll() {
	if( hDCRGB != NULL ) {
		::DeleteDC( hDCRGB );
		if( hBitmapRGB != NULL ) {
			::DeleteObject( hBitmapRGB );
			hBitmapRGB = NULL;
		}
		hDCRGB = NULL;
	}
	if( hDCMask != NULL ) {
		::DeleteDC( hDCMask );
		if( hBitmapMask != NULL ) {
			::DeleteObject( hBitmapMask );
			hBitmapMask = NULL;
		}
		hDCMask = NULL;
	}
}

VNImage::~VNImage() {
	ReleaseAll();
}

// �o�b�N�o�b�t�@�p�̃C���[�W���쐬����
bool VNImage::CreateRGB( int w, int h ) {
	// �摜�T�C�Y�ƐF�r�b�g����ۑ�
	width	= w;
	height	= h;

	return CreateRGBBuffer();
}

// Bitmap�t�@�C�������[�h���ăC���[�W���쐬����
bool VNImage::LoadBmp( const char *bmp24_fname, const char *bmp1_fname ) {
	if( ! LoadRGBBmp( bmp24_fname ) )
		return false;

	if( bmp1_fname == NULL ) {
		is_mask_enable = false;
		return true;
	}

	is_mask_enable = true;

	return LoadMaskBmp( bmp1_fname );
}

int	VNImage::GetWidth() {
	return width;
}

int VNImage::GetHeight() {
	return height;
}

HDC VNImage::GetDC() {
	return hDCRGB;
}
