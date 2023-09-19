#include "VNPlayer.h"
#include "VNDSave.h"
#include "VNDLoad.h"
#include "VNSaveFile.h"


void VNPlayer::SaveDialog() {
	VNDSave dlg;
	int index = dlg.DoModal( window->GetHWND() );
	if( index == -1 )
		return;	// キャンセルされた

	Save( index );
	return;
}

bool VNPlayer::LoadDialog() {
	VNDLoad dlg;
	int index = dlg.DoModal( window->GetHWND() );
	if( index == -1 )
		return false;	// キャンセルされた

	// ロードすべきセーブデータの番号を保存
	load_anchor = index;

	return true;	// 呼び出し側でロードを行う
}

bool VNPlayer::Save( int index ) {
	VNString rgb[4];
	VNString mask[3];
	int	v[3];

	for( int i = 0 ; i < 4; i++ )	rgb[i] = rgb_image_name[i];
	for( i = 0 ; i < 3; i++ )		mask[i] = mask_image_name[i];
	for( i = 0 ; i < 3; i++ )		v[i] = v_align[i];

	VNSaveFile	sav( event_name, offset, title, rgb, mask, v);
	if( ! sav.Save( index ) ) {
		Warning( "セーブに失敗しました。");
		return false;
	}

	if( ! global_flag->Save( "global.flg" ) ) {
		Warning( "グローバルフラグのセーブに失敗しました。");
		return false;
	}

	if( ! local_flag->Save( (VNString)"save"+(VNString)index+".flg" ) ) {
		Warning( "ローカルフラグのセーブに失敗しました。");
		return false;
	}

	return true;
}

bool VNPlayer::Load() {
	// セーブファイルをロード
	VNSaveFile	sav;
	if( ! sav.Load( load_anchor ) ) {
		Warning("ロードに失敗しました。");
		return false;
	}

	// ローカルフラグをロード
	VNFlagFile *new_local = new VNFlagFile();
	if( ! new_local->Load( (VNString)"save"+(VNString)load_anchor+".flg" ) ) {
		Warning("ローカルフラグのロードに失敗しました。");
		return false;
	}

	// イベントファイルを設定
	VNString evt_name = sav.GetEeventName();
	VNEventFile *new_evt = new VNEventFile();
	if( ! new_evt->Load( (VNString)EVENT_DIR+evt_name ) ) {
		Warning((VNString)"イベントファイル\""+evt_name+"\"が存在しません。");
		delete new_evt;
		return false;
	}
	event_name = evt_name;
	event = new_evt;

	// 新しいローカルフラグをセット
	delete local_flag;
	local_flag = new_local;

	// イベントオフセットを設定
	offset = sav.GetOffset();

	// 背景を設定
	VNString image_name = sav.GetRGBImageName( 0 );
	BackgroundFade( image_name );

	// キャラを設定
	for( int i = 0; i < 3; i++ ) {
		image_name = sav.GetRGBImageName(i+1);
		if( strcmp( image_name.c_str(), "" ) != 0 )
			CharaFade( i,
					   sav.GetVerticalAlign(i),
					   image_name,
					   sav.GetMaskImageName(i).c_str() );
	}


	// タイトルを設定
	title = sav.GetTitle();
	window->SetTitle( title );

	return true;
}
