#include "VNPlayer.h"
#include "VNDSave.h"
#include "VNDLoad.h"
#include "VNSaveFile.h"


void VNPlayer::SaveDialog() {
	VNDSave dlg;
	int index = dlg.DoModal( window->GetHWND() );
	if( index == -1 )
		return;	// �L�����Z�����ꂽ

	Save( index );
	return;
}

bool VNPlayer::LoadDialog() {
	VNDLoad dlg;
	int index = dlg.DoModal( window->GetHWND() );
	if( index == -1 )
		return false;	// �L�����Z�����ꂽ

	// ���[�h���ׂ��Z�[�u�f�[�^�̔ԍ���ۑ�
	load_anchor = index;

	return true;	// �Ăяo�����Ń��[�h���s��
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
		Warning( "�Z�[�u�Ɏ��s���܂����B");
		return false;
	}

	if( ! global_flag->Save( "global.flg" ) ) {
		Warning( "�O���[�o���t���O�̃Z�[�u�Ɏ��s���܂����B");
		return false;
	}

	if( ! local_flag->Save( (VNString)"save"+(VNString)index+".flg" ) ) {
		Warning( "���[�J���t���O�̃Z�[�u�Ɏ��s���܂����B");
		return false;
	}

	return true;
}

bool VNPlayer::Load() {
	// �Z�[�u�t�@�C�������[�h
	VNSaveFile	sav;
	if( ! sav.Load( load_anchor ) ) {
		Warning("���[�h�Ɏ��s���܂����B");
		return false;
	}

	// ���[�J���t���O�����[�h
	VNFlagFile *new_local = new VNFlagFile();
	if( ! new_local->Load( (VNString)"save"+(VNString)load_anchor+".flg" ) ) {
		Warning("���[�J���t���O�̃��[�h�Ɏ��s���܂����B");
		return false;
	}

	// �C�x���g�t�@�C����ݒ�
	VNString evt_name = sav.GetEeventName();
	VNEventFile *new_evt = new VNEventFile();
	if( ! new_evt->Load( (VNString)EVENT_DIR+evt_name ) ) {
		Warning((VNString)"�C�x���g�t�@�C��\""+evt_name+"\"�����݂��܂���B");
		delete new_evt;
		return false;
	}
	event_name = evt_name;
	event = new_evt;

	// �V�������[�J���t���O���Z�b�g
	delete local_flag;
	local_flag = new_local;

	// �C�x���g�I�t�Z�b�g��ݒ�
	offset = sav.GetOffset();

	// �w�i��ݒ�
	VNString image_name = sav.GetRGBImageName( 0 );
	BackgroundFade( image_name );

	// �L������ݒ�
	for( int i = 0; i < 3; i++ ) {
		image_name = sav.GetRGBImageName(i+1);
		if( strcmp( image_name.c_str(), "" ) != 0 )
			CharaFade( i,
					   sav.GetVerticalAlign(i),
					   image_name,
					   sav.GetMaskImageName(i).c_str() );
	}


	// �^�C�g����ݒ�
	title = sav.GetTitle();
	window->SetTitle( title );

	return true;
}
