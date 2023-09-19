#ifndef _VNPMSGBOX_H_
#define _VNPMSGBOX_H_

#include "VNWnd.h"
#include "VNImage.h"


class VNPMsgbox: protected VNWnd {
public:
	VNPMsgbox();
	~VNPMsgbox();

	void	Create( VNWnd * );
	void	Visible( bool );
	void	SetPos( int, int );

	VNImage *GetBackBuffer();
	void	Flip();

private:
	VNImage	*back_buffer;

	void	OnDestroy();
	void	OnPaint();
};


#endif
