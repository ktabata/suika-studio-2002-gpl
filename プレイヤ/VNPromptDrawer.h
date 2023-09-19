// ���b�Z�[�W�{�b�N�X�̃N���b�N�Ñ��v�����v�g��`�悷��N���X
// �u���v����]����A�j���[�V����

#ifndef _VNPROMPTDRAWER_H_
#define _VNPROMPTDRAWER_H_

#include "VNImage.h"

#define PD_MILLISEC			(500)	// �����ɂ����鎞�ԁi�~���b�j


class VNPromptDrawer {
public:
	VNPromptDrawer( VNImage&, int, int, int, int );

	void	Start();
	bool	NeedUpdate();
	void	Draw();

private:
	VNImage&		image;
	int				x;
	int				y;
	int				width;
	int				height;
	unsigned long	start_time;
	int				last_s;
};


#endif
