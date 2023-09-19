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
	VNString	event_name;			// �C�x���g�t�@�C����
	int			offset;				// �C�x���g�I�t�Z�b�g
	VNString	title;				// �E�B���h�E�A�Z�[�u�̃^�C�g��
	VNString	rgb_image_name[4];	// �w�i�A�L�����摜�̃J���[�C���[�W��
	VNString	mask_image_name[3];	// �L�����摜�̃}�X�N�C���[�W��
	int			v_align[3];			// �L�����摜�̐�������
};


#endif
