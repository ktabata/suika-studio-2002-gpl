#ifndef _VNDSAVEFILE_H_
#define _VNDSAVEFILE_H_

#include "VNBinFileReader.h"
#include "VNBinFileWriter.h"


class VNSaveFile {
public:
	VNSaveFile( const VNString&, int, const VNString&, const VNString [4], const VNString [3], int [3] );
	VNSaveFile();

	bool	Save( int );
	bool	Load( int );

	const VNString&	GetDate();
	const VNString&	GetEeventName();
	int				GetOffset();
	const VNString&	GetTitle();
	const VNString&	GetRGBImageName( int );
	const VNString&	GetMaskImageName( int );
	int				GetVerticalAlign( int );

private:
	void LoadDate();

	VNString	date;
	VNString	event_name;			// イベントファイル名
	int			offset;				// イベントオフセット
	VNString	title;				// ウィンドウ、セーブのタイトル
	VNString	rgb_image_name[4];	// 背景、キャラ画像のカラーイメージ名
	VNString	mask_image_name[3];	// キャラ画像のマスクイメージ名
	int			v_align[3];			// キャラ画像の垂直整列
};


#endif
