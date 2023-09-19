#include "VNImage.h"


unsigned short mask_brush_bitmap[MASK_LEVELS][8] = {
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x80 },
	{ 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x88 },
	{ 0x00, 0x02, 0x00, 0x88, 0x00, 0x22, 0x00, 0x88 },
	{ 0x00, 0x22, 0x00, 0x88, 0x00, 0x22, 0x00, 0xA8 },
	{ 0x00, 0x22, 0x00, 0x8A, 0x00, 0x22, 0x00, 0xAA },
	{ 0x00, 0x2A, 0x00, 0xAA, 0x00, 0xA2, 0x00, 0xAA },
	{ 0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 0x88 },
	{ 0x00, 0xAA, 0x04, 0xAA, 0x00, 0xAA, 0x40, 0xAA },
	{ 0x00, 0xAA, 0x44, 0xAA, 0x00, 0xAA, 0x44, 0xAA },
	{ 0x01, 0xAA, 0x44, 0xAA, 0x11, 0xAA, 0x44, 0xAA },
	{ 0x11, 0xAA, 0x44, 0xAA, 0x11, 0xAA, 0x54, 0xAA },
	{ 0x11, 0xAA, 0x45, 0xAA, 0x11, 0xAA, 0x55, 0xAA },
	{ 0x15, 0xAA, 0x55, 0xAA, 0x51, 0xAA, 0x55, 0xAA },
	{ 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA },
	{ 0x55, 0xAA, 0x55, 0xAE, 0x55, 0xAA, 0x55, 0xEA },
	{ 0x55, 0xAA, 0x55, 0xEE, 0x55, 0xAA, 0x55, 0xEE },
	{ 0x55, 0xAB, 0x55, 0xEE, 0x55, 0xBB, 0x55, 0xEE },
	{ 0x55, 0xBB, 0x55, 0xEE, 0x55, 0xBB, 0x55, 0xEE },
	{ 0x55, 0xBB, 0x55, 0xEE, 0x55, 0xBB, 0x55, 0xFE },
	{ 0x55, 0xBB, 0x55, 0xEF, 0x55, 0xBB, 0x55, 0xFF },
	{ 0x55, 0xBB, 0x55, 0xFF, 0x55, 0xFB, 0x55, 0xFF },
	{ 0x77, 0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x77, 0xDD },
	{ 0x55, 0xFF, 0x5D, 0xFF, 0x55, 0xFF, 0xD5, 0xFF },
	{ 0x55, 0xFF, 0xDD, 0xFF, 0x75, 0xFF, 0xDD, 0xFF },
	{ 0x77, 0xFF, 0x77, 0xFF, 0x77, 0xFF, 0xFF, 0xFF },
	{ 0x7F, 0xFF, 0xFF, 0xFF, 0xF7, 0xFF, 0xFF, 0xFF },
	{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }
};


void VNImage::Clear( bool white /* = false */ ) {
	unsigned long val = white ? 0xffffffff : 0;
	unsigned long addr = (unsigned long)image_rgb;
	unsigned long dwords = bpl_rgb / 4 * height;

	__asm {
		mov		edi, addr
		mov		ecx, dwords
		mov		eax, val
		cld
		rep		stosd
	}
}

void VNImage::BitBlt(
		int dst_pos_x,
		int dst_pos_y,
		VNImage *src,
		int src_pos_x /* = -1 */,
		int src_pos_y /* = -1 */,
		int width /* = -1 */,
		int height /* = -1 */ )
{
	if( src_pos_x == -1 ) {
		src_pos_x = 0;
		src_pos_y = 0;
		width = src->width;
		height = src->height;
	}

	if( src->is_mask_enable == true ) {
		::BitBlt (hDCRGB,
				  dst_pos_x, dst_pos_y,
				  width, height,
				  src->hDCMask,
				  src_pos_x, src_pos_y,
				  SRCAND);
		::BitBlt (hDCRGB,
				  dst_pos_x, dst_pos_y,
				  width, height,
				  src->hDCRGB,
				  0, 0,
				  SRCPAINT);
	} else
		::BitBlt (hDCRGB,
				  dst_pos_x, dst_pos_y,
				  width, height,
				  src->hDCRGB,
				  src_pos_x, src_pos_y,
				  SRCCOPY);
}

void VNImage::MaskBlt(
		int mask_level,
		int dst_pos_x,
		int dst_pos_y,
		VNImage *src,
		int src_pos_x /* = -1 */,
		int src_pos_y /* = -1 */,
		int width /* = -1 */,
		int height /* = -1 */ )
{
	if( mask_level >= MASK_LEVELS )
		return;

	mask_level = MASK_LEVELS - mask_level - 1;

	if( src_pos_x == -1 ) {
		src_pos_x = 0;
		src_pos_y = 0;
		width = src->width;
		height = src->height;
	}

	HBITMAP hBm = ::CreateBitmap (8, 8, 1, 1, mask_brush_bitmap[mask_level]);
	if (hBm == NULL)
		return;

	HBRUSH hBrush = ::CreatePatternBrush (hBm);
	if (hBrush == NULL)
		return;

	// デバイスコンテキストの背景が白の場合(ウインドウの場合WNDCLASSで指定したブラシ）
	::SetBkColor (hDCRGB, 0xffffff);
	HBRUSH hBrushOld = ::SelectObject (hDCRGB, hBrush);

	if( src->is_mask_enable ) {
		::BitBlt (hDCRGB,
				  dst_pos_x, dst_pos_y,
				  width, height,
				  src->hDCMask,
				  src_pos_x, src_pos_y,
				  0x00A803A9);	// DPSoa  DST&=SRC|BRUSH
	} else {
		::BitBlt (hDCRGB,
				  dst_pos_x, dst_pos_y,
				  width, height,
				  src->hDCRGB,
				  src_pos_x, src_pos_y,
				  0x00A000C9);	// DPa  DST&=BRUSH
	}

	::BitBlt (hDCRGB,
			  dst_pos_x, dst_pos_y,
			  width, height,
			  src->hDCRGB,
			  src_pos_x, src_pos_y,
			  0x00AE0B06);	// DSPnao DST|=(!BRUSH)&SRC

	::DeleteObject (::SelectObject (hDCRGB, hBrushOld));
	::DeleteObject (hBm);
}

// 上から下にグラデーションする長方形をアルファブレンドで描画する（メッセージボックス用）
void VNImage::DrawGradationBox(
		int x, int y, int width, int height,
		unsigned char start_r, unsigned char start_g, unsigned char start_b, unsigned char start_a,
		unsigned char end_r, unsigned char end_g, unsigned char end_b, unsigned char end_a )
{
	if( width <= 0 || height <= 0 ) return;
	if( x < 0 ) x = 0;
	if( y < 0 ) y = 0;
	if( x + width >= this->width ) width = this->width - x;
	if( y + height >= this->height ) height = this->height - y;

	// ※下から上に向かって描画する

	float r = end_r,
		  g = end_g,
		  b = end_b,
		  a = end_a;

	float dr = (start_r - r) / height,
		  dg = (start_g - g) / height,
		  db = (start_b - b) / height,
		  da = (start_a - a) / height;

	const int bpp = 3;	// ここは後で変更になるかもしれない

	long delta_line = bpl_rgb;
	unsigned char *line = image_rgb + x * bpp + delta_line * (480-y-height);

	for( int py = 0; py < height; py++ ) {
		unsigned char *pix = line;
		int tmp_a = (int)a;
		unsigned char *brush_alpha_table = alpha_table[tmp_a];
		unsigned char *src_alpha_table = alpha_table[255 - tmp_a];

		for( int px = 0; px < width; px++ ) {
			// src_alpha_tableに集中してアクセス
			long rgb = *(unsigned long *) pix;
			int tmp_b = src_alpha_table[ (unsigned char) rgb ];
			int tmp_g = src_alpha_table[ (rgb & 0xff00) >> 8 ];
			int tmp_r = src_alpha_table[ (rgb & 0xff0000) >> 16 ];

			// brush_alpha_tableに集中してアクセス
			tmp_r += brush_alpha_table[ (int) r ];
			tmp_g += brush_alpha_table[ (int) g ];
			tmp_b += brush_alpha_table[ (int) b ];

			// pixに集中してアクセス
			*pix++ = (unsigned char) tmp_b;
			*pix++ = (unsigned char) tmp_g;
			*pix++ = (unsigned char) tmp_r;
		}

		r += dr;	g += dg;	b += db;	a += da;
		line += delta_line;
	}
}
