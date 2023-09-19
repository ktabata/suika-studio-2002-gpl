// メッセージボックスのクリック催促プロンプトを描画するクラス
// 「▼」が回転するアニメーション

#ifndef _VNPROMPTDRAWER_H_
#define _VNPROMPTDRAWER_H_

#include "VNImage.h"

#define PD_MILLISEC			(500)	// 半周にかかる時間（ミリ秒）


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
