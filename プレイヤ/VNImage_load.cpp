#include <stdio.h>
#include "VNImage.h"


bool VNImage::LoadRGBBmp( const char *file_name ) {
	int res;

	// �t�@�C�����I�[�v������
	FILE *fp = ::fopen( file_name, "rb" );
	if( fp == NULL )
		return false;

	// �C���[�W�̕����擾
	::fseek( fp, 18, SEEK_CUR );
	res  = ::fread( &width, 4, 1, fp );
	if( res < 1 ) {
		::fclose( fp );
		return false;
	}

	// �C���[�W�̍������擾
	res  = ::fread( &height, 4, 1, fp );
	if( res < 1 ) {
		::fclose( fp );
		return false;
	}

	// �r�b�g�}�b�v�t�@�C���̐F�r�b�g�����擾
	::fseek( fp, 2, SEEK_CUR );
	int bpp = ::fgetc( fp );
	if( bpp != 24 ) {
		::fclose( fp );
		return false;
	}

	// �擾�����T�C�Y�ŁA�Q�S�r�b�g�C���[�W���쐬����
	if( CreateRGB( width, height ) == false ) {
		::fclose( fp );
		return false;
	}

	// �r�b�g�}�b�v�t�@�C������J���[�f�[�^��ǂݍ���
	::fseek( fp, 25, SEEK_CUR );
	if( fread( image_rgb, bpl_rgb * height, 1, fp ) < 1 ) {
		::fclose( fp );
		return false;
	}

	// �t�@�C�����N���[�Y����
	::fclose( fp );

	// ����
	return true;
}

bool VNImage::LoadMaskBmp( const char *file_name ) {
	if( ! is_mask_enable )
		return false;

	if( ! CreateMaskBuffer() )
		return false;

	// �t�@�C�����I�[�v������
	FILE *fp = fopen( file_name, "rb" );
	if( fp == NULL )
		return false;


	// �}�X�N�C���[�W�̕����擾
	fseek( fp, 18, SEEK_CUR );
	int mask_width;
	int res = fread( &mask_width, 4, 1, fp );

	// �ǂݍ��݂Ɏ��s���邩�A�}�X�N�C���[�W�̃T�C�Y���쐬�ς݂̃C���[�W�̃T�C�Y�ƈႤ�Ȃ玸�s
	if( res < 1  ||  mask_width != width ) {
		fclose( fp );
		return false;
	}


	// �}�X�N�C���[�W�̍������擾
	int mask_height;
	res = fread( &mask_height, 4, 1, fp );

	// �ǂݍ��݂Ɏ��s���邩�A�}�X�N�C���[�W�̃T�C�Y���쐬�ς݂̃C���[�W�̃T�C�Y���Ⴄ�Ȃ玸�s
	if( res < 1  ||  mask_height != height ) {
		fclose( fp );
		return false;
	}


	// �r�b�g�}�b�v�t�@�C���̐F�r�b�g�����擾
	fseek( fp, 2, SEEK_CUR );
	int bpp = fgetc( fp );

	// �P�r�b�g�r�b�g�}�b�v�t�@�C���ł���Ύ��s
	if( bpp != 1 ) {
		fclose( fp );
		return false;
	}


	// �r�b�g�}�b�v�t�@�C������}�X�N�f�[�^��ǂݍ���
	fseek( fp, 25 + 8, SEEK_CUR );
	if( fread( image_mask, bpl_mask * height, 1, fp ) < 1 ) {
		::fclose( fp );
		return false;
	}

	// ����
	fclose( fp );

	return true;
}
