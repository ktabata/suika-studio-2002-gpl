#include <stdio.h>
#include "VNImage.h"


bool VNImage::LoadRGBBmp( const char *file_name ) {
	int res;

	// ファイルをオープンする
	FILE *fp = ::fopen( file_name, "rb" );
	if( fp == NULL )
		return false;

	// イメージの幅を取得
	::fseek( fp, 18, SEEK_CUR );
	res  = ::fread( &width, 4, 1, fp );
	if( res < 1 ) {
		::fclose( fp );
		return false;
	}

	// イメージの高さを取得
	res  = ::fread( &height, 4, 1, fp );
	if( res < 1 ) {
		::fclose( fp );
		return false;
	}

	// ビットマップファイルの色ビット数を取得
	::fseek( fp, 2, SEEK_CUR );
	int bpp = ::fgetc( fp );
	if( bpp != 24 ) {
		::fclose( fp );
		return false;
	}

	// 取得したサイズで、２４ビットイメージを作成する
	if( CreateRGB( width, height ) == false ) {
		::fclose( fp );
		return false;
	}

	// ビットマップファイルからカラーデータを読み込む
	::fseek( fp, 25, SEEK_CUR );
	if( fread( image_rgb, bpl_rgb * height, 1, fp ) < 1 ) {
		::fclose( fp );
		return false;
	}

	// ファイルをクローズする
	::fclose( fp );

	// 成功
	return true;
}

bool VNImage::LoadMaskBmp( const char *file_name ) {
	if( ! is_mask_enable )
		return false;

	if( ! CreateMaskBuffer() )
		return false;

	// ファイルをオープンする
	FILE *fp = fopen( file_name, "rb" );
	if( fp == NULL )
		return false;


	// マスクイメージの幅を取得
	fseek( fp, 18, SEEK_CUR );
	int mask_width;
	int res = fread( &mask_width, 4, 1, fp );

	// 読み込みに失敗するか、マスクイメージのサイズが作成済みのイメージのサイズと違うなら失敗
	if( res < 1  ||  mask_width != width ) {
		fclose( fp );
		return false;
	}


	// マスクイメージの高さを取得
	int mask_height;
	res = fread( &mask_height, 4, 1, fp );

	// 読み込みに失敗するか、マスクイメージのサイズが作成済みのイメージのサイズが違うなら失敗
	if( res < 1  ||  mask_height != height ) {
		fclose( fp );
		return false;
	}


	// ビットマップファイルの色ビット数を取得
	fseek( fp, 2, SEEK_CUR );
	int bpp = fgetc( fp );

	// １ビットビットマップファイルでければ失敗
	if( bpp != 1 ) {
		fclose( fp );
		return false;
	}


	// ビットマップファイルからマスクデータを読み込む
	fseek( fp, 25 + 8, SEEK_CUR );
	if( fread( image_mask, bpl_mask * height, 1, fp ) < 1 ) {
		::fclose( fp );
		return false;
	}

	// 成功
	fclose( fp );

	return true;
}
