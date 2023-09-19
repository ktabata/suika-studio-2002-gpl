#include <windows.h>
#include "VNPlayer.h"

// SJIS２バイト文字の１バイト目か？
#define is_kanji1(c)	((unsigned char)c>=0x81&&(unsigned char)c<=0x9F||(unsigned char)c>=0xE0&(unsigned char)c<=0xFC)

#define WAIT_CHARS	(5)
char *wait_char[WAIT_CHARS] = { "…", "、", "。", "！", "？", };


bool VNPlayer::Narration( const char *msg ) {
	last_message = msg;
	backlog.AddString( msg );
	if( ! PutMessage( msg, 0xff, 0xff, 0xff, 30 ) ) {
		PutMessage( msg, 0xff, 0xff, 0xff, 0 );	// ディレイ表示中にクリックされた
		return MessageWait( true );	// quickモード
	} else
		return MessageWait( false );	// 通常モード
}

bool VNPlayer::Serif( const char *name, const char *msg ) {
	// 名前とメッセージを連結して描画
	last_message = VNString("【") + name + "】「" + msg + "」";
	backlog.AddString( last_message );
	if( ! PutMessage( last_message.c_str(), 0xff, 0xff, 0xff, 30 ) ) {
		PutMessage( last_message.c_str(), 0xff, 0xff, 0xff, 0 );	// ディレイ表示中にクリックされた
		return MessageWait( true );	// quickモード
	} else
		return MessageWait( false );	// 通常モード
}

bool VNPlayer::CharaFade( int pos, int align, const char *rgb_image_name, const char *mask_image_name ) {
	if( pos >= 3 )
		return false;

	// 画像のロード
	VNImage *new_img = NULL;
	if( strcmp( rgb_image_name, "" ) != 0 ) {
		new_img = new VNImage();

		bool b;
		if( strcmp( mask_image_name, "" ) != 0 ) {
			// マスク付き
			b = new_img->LoadBmp(
					(VNString(CG_DIR) + rgb_image_name).c_str(),
					(VNString(CG_DIR) + mask_image_name ).c_str() );
		} else {
			// マスクなし
			b = new_img->LoadBmp(
					(VNString(CG_DIR) + rgb_image_name).c_str(), NULL );
		}

		if( ! b ) {
			// ロードに失敗
			delete new_img;
			return false;
		}

		this->rgb_image_name[pos+1] = rgb_image_name;
		this->mask_image_name[pos] = mask_image_name;
	} else {
		this->rgb_image_name[pos+1] = "";
		this->mask_image_name[pos] = "";
	}

	// 古い画像を保存（フェードアウトさせるのに使う）
	VNImage *old_img = image[pos+1];
	int old_x = chara_x[pos];
	int old_y = chara_y[pos];
	int old_w = chara_width[pos];
	int old_h = chara_height[pos];

	// 新しい画像を設定
	SetChara( pos, align, new_img );

	// キャラを非表示にしておく
	image[pos+1] = NULL;

	// アニメーションループ
	int	last_mask = -1;
	unsigned long start = ::GetTickCount();
	bool is_l_down = window->IsMouseLDown();
	unsigned long fade_time = CHARA_FADE_MILLI;

	while(1) {
		// クリックやＣｔｒｌによるキャンセルを検出
		window->Sync();
		if( window->IsMouseLDown() ) {
			if( ! is_l_down )
				break;
		} else
			is_l_down = false;
		if( window->IsCtrlDown() )
			fade_time = CHARA_FADE_MILLI / 4;

		unsigned long lap = ::GetTickCount() - start;
		if( lap >= fade_time )
			break;

		float alpha = (float)lap / (float)fade_time * MASK_LEVELS; 
		if( (int)alpha == last_mask ) {
			window->Sync();
			continue;
		}

		int mask = last_mask = (int)alpha;

		DrawBgAndChara();

		// 古い画像を表示
		if( old_img != NULL )
			back_buffer->MaskBlt(
				MASK_LEVELS - 1 - mask,
				old_x, old_y,	// dst x,y
				old_img,
				0, 0,			// src x,y
				old_w, old_h );	// width, height

		// 新しい画像を表示
		if( new_img != NULL )
			back_buffer->MaskBlt(
				mask,
				chara_x[pos], chara_y[pos],	// dst x,y
				new_img,
				0, 0,	// src x,y
				chara_width[pos], chara_height[pos] );	// width, height

		window->Flip();
		window->Sync();
	}

	delete old_img;

	// キャラの表示を有効にする
	image[pos+1] = new_img;

	// 表示し直す
	DrawBgAndChara();
	window->Flip();
	window->Sync();

	return true;
}

// 背景のフェードイン・アウト
bool VNPlayer::BackgroundFade( const char *image_name ) {
	// 画像のロード
	VNImage *new_img = NULL;
	if( strcmp( image_name, "" ) != 0 ) {
		new_img = new VNImage();
		bool b = new_img->LoadBmp( (VNString(CG_DIR) + image_name).c_str(), NULL );
		if( ! b ) {		// ロードに失敗
			delete new_img;
			new_img = NULL;
			rgb_image_name[0] = "";
		} else
			rgb_image_name[0] = image_name;
	} else
		rgb_image_name[0] = "";

	// 古い画像を保存（フェードアウトさせるのに使う）
	VNImage *old_img = image[0];

	if( old_img == NULL && new_img == NULL )
		return false;

	// 背景を非表示にしておく
	image[0] = NULL;

	// キャラの表示を無効にして画像を破棄する
	for( int i = 1; i <= 3; i++ ) {
		if( image[i] != NULL ) {
			delete image[i];
			image[i] = NULL;
		}
	}

	// フェードイン・フェードアウト
	int	last_mask = -1;
	unsigned long start = ::GetTickCount();
	bool is_l_down = window->IsMouseLDown();
	unsigned long fade_time = BG_FADE_MILLI;

	while(1) {
		// クリックやＣｔｒｌによるキャンセルを検出
		window->Sync();
		if( window->IsMouseLDown() ) {
			if( ! is_l_down )
				break;
		} else
			is_l_down = false;
		if( window->IsCtrlDown() )
			fade_time = BG_FADE_MILLI / 4;

		unsigned long lap = ::GetTickCount() - start;
		if( lap >= fade_time )
			break;

		float alpha = (float)lap / (float)fade_time * MASK_LEVELS; 
		if( (int)alpha == last_mask ) {
			window->Sync();
			continue;
		}

		int mask = last_mask = (int)alpha;

		// 古い画像を表示
		if( old_img != NULL )
			back_buffer->MaskBlt(
				MASK_LEVELS - mask - 1,
				prof.BackgroundLeft,
				prof.BackgroundTop,
				old_img );

		// 新しい画像を表示
		if( new_img != NULL )
			back_buffer->MaskBlt(
				mask,
				prof.BackgroundLeft,
				prof.BackgroundTop,
				new_img );
		else {
			// 背景をフェードアウトする場合
			if( window_image != NULL )
				back_buffer->MaskBlt(
					mask,
					prof.BackgroundLeft,
					prof.BackgroundTop,
					window_image,
					prof.BackgroundLeft,
					prof.BackgroundTop,
					old_img->GetWidth(),
					old_img->GetHeight() );
		}
		window->Flip();
		window->Sync();
	}

	delete old_img;

	// 背景を設定する
	image[0] = new_img;

	// 表示し直す
	back_buffer->Clear();
	DrawBgAndChara();
	window->Flip();
	window->Sync();

	return true;
}


///////////////////////////
// イベント処理関数の補助

void VNPlayer::DrawBgAndChara() {
	// ウィンドウ背景を描画
	if( window_image != NULL )
		back_buffer->BitBlt( 0, 0, window_image );

	// 背景を描画
	if( image[0] != NULL )
		back_buffer->BitBlt( prof.BackgroundLeft, prof.BackgroundTop, image[0] );

	// キャラを描画
	for( int i = 0; i < 3; i++ ) {
		if( image[1+i] != NULL ) {
			back_buffer->BitBlt(
				chara_x[i], chara_y[i],	// dst x,y
				image[1+i],
				0, 0,	// src x,y
				chara_width[i], chara_height[i] );	// width, height
		}
	}
}

void VNPlayer::SetChara( int pos, int align, VNImage *img ) {
	if( pos >= 3 )
		return;

	image[pos+1] = img;
	if( img == NULL )
		return;

	v_align[pos] = align;
	chara_width[pos] = img->GetWidth();
	chara_height[pos] = img->GetHeight();

	// 表示位置のＸ座標を計算する
	switch( pos ) {
	case CHARA_H_LEFT:	chara_x[pos]= 0; break;
	case CHARA_H_CENTER:chara_x[pos] = (640 - chara_width[pos]) / 2; break;
	case CHARA_H_RIGHT:	chara_x[pos] = 640 - chara_width[pos];
	}

	// 表示位置のＹ座標を計算する
	switch( align ) {
	case CHARA_V_TOP:	chara_y[pos] = 0; break;
	case CHARA_V_CENTER:chara_y[pos] = (480 - chara_height[pos]) / 2; break;
	case CHARA_V_BOTTOM:chara_y[pos] = 480 - chara_height[pos];
	}
}

void VNPlayer::DrawMessageBox() {
	back_buffer->DrawGradationBox(
		prof.MsgBoxLeft - prof.MsgBoxFrameWidth, prof.MsgBoxTop - prof.MsgBoxFrameHeight,
		prof.MsgBoxWidth + prof.MsgBoxFrameWidth * 2, prof.MsgBoxHeight + prof.MsgBoxFrameHeight * 2,
		prof.MsgBoxColorStartR, prof.MsgBoxColorStartG, prof.MsgBoxColorStartB, prof.MsgBoxColorStartA,
		prof.MsgBoxColorEndR, prof.MsgBoxColorEndG, prof.MsgBoxColorEndB, prof.MsgBoxColorEndA);
}

bool VNPlayer::PutMessage( const char *msg, int r, int g, int b, int delay ) {
	DrawBgAndChara();
	DrawMessageBox();
	window->Update( prof.MsgBoxLeft - prof.MsgBoxFrameWidth,
					prof.MsgBoxTop - prof.MsgBoxFrameHeight,
					prof.MsgBoxWidth + prof.MsgBoxFrameWidth * 2,
					prof.MsgBoxHeight + prof.MsgBoxFrameHeight * 2 );

	int	line = 0;
	int len = strlen( msg );
	while( len > 0 && line < 5 ) {
		int width;

		// １行分の幅になる最大の文字数を計算する
		int line_len = len;
		for(;;) {
			if( line_len <= 0 )
				return false;	// エラー

			width = text_drawer->GetWidth( msg, line_len );
			if( width == -1 )
				return false;	// エラー

			if( width <= prof.MsgBoxWidth )
				break;	// 計算完了

			// 残りの文字が２バイト以上で、最後の２バイトが２バイト文字か？
			if( line_len >= 2 ) {
				if( is_kanji1( *(msg+line_len-2) ) )
					line_len -= 2;
				else
					line_len--;
			} else
				line_len--;
		}

		// 描画する
		if( ! PutMessageLine( prof.MsgBoxLeft, prof.MsgBoxTop + line * prof.MsgLineHeight, msg, line_len, r, g, b, delay ) )
			return false;	// ディレイ表示中にクリックされた

		// クリック待ちプロンプトの位置
		prompt_x = prof.MsgBoxLeft + width;
		prompt_y = prof.MsgBoxTop + line * prof.MsgLineHeight;

		msg += line_len;
		len -= line_len;
		line++;
	}
	return true;
}

// ディレイ表示の途中でクリックされると、falseを返す
bool VNPlayer::PutMessageLine( int x, int y, const char *msg, int len, int r, int g, int b, int delay ) {
	int n = 0;
	bool is_l_down = window->IsMouseLDown();
	while( n < len ) {
		bool	need_wait = false;

		if( ! is_kanji1( *(msg+n) ) )
			n++;	// ASCII文字
		else {
			// ウェイト文字か？
			if( delay != 0 ) {
				for( int i = 0; i < WAIT_CHARS; i++ ) {
					if( strncmp( msg+n, wait_char[i], 2 ) == 0) {
						need_wait = true;
						break;
					}
				}
			}
			n += 2;	// ２バイト文字
		}

		text_drawer->SetColor( 0, 0 ,0 );
		text_drawer->Draw( x+2, y+2, msg, n );
		text_drawer->SetColor( r, g ,b );
		text_drawer->Draw( x, y, msg, n );

		if( delay != 0 ) {
			// クリックやＣｔｒｌによるキャンセルを検出
			window->Sync();
			if( window->IsMouseLDown() ) {
				if( ! is_l_down )
					return false;
			} else
				is_l_down = false;
			if( window->IsCtrlDown() )
				return false;

			// 描画＆ウェイト
			unsigned long t = GetTickCount();
//			window->Update( x, y, prof.MsgBoxLeft + prof.MsgBoxWidth - x, prof.MsgLineHeight );
			window->Update( x, y, prof.MsgBoxLeft + prof.MsgBoxFrameWidth*2 + prof.MsgBoxWidth - x, prof.MsgLineHeight );
			while( t + delay >= GetTickCount() ) {
				if( window->IsMouseLDown() ) {
					if( ! is_l_down )
						return false;
				} else
					is_l_down = false;
				window->Sync();
			}
	
			if( need_wait ) {
				// ウェイト文字によるウェイト
				unsigned long l = GetTickCount();
				while( l + 300 >= GetTickCount() ) {
					window->Sync();

					// クリックやＣｔｒｌによるキャンセルを検出
					if( window->IsMouseLDown() ) {
						if( ! is_l_down )
							return false;
					} else
						is_l_down = false;
					if( window->IsCtrlDown() )
						return false;
				} // while 
			} // if
		} // if( delay != 0 )
	} // while

	text_drawer->SetColor( 0, 0 , 0 );

	if( delay == 0 )	// まだ一度も描画されていないので描画
		window->Update( prof.MsgBoxLeft, prof.MsgBoxTop, prof.MsgBoxWidth, prof.MsgBoxHeight );
	return true;
}

// 選択された番号(0-3)を返す。ロード操作が行われると-1を返す
int VNPlayer::SelectLoop( const char *msg[3], int items ){
	DrawBgAndChara();
	DrawMessageBox();
	window->Update( prof.MsgBoxLeft - prof.MsgBoxFrameWidth,
					prof.MsgBoxTop - prof.MsgBoxFrameHeight,
					prof.MsgBoxWidth + prof.MsgBoxFrameWidth * 2,
					prof.MsgBoxHeight + prof.MsgBoxFrameHeight * 2 );

	// 各メッセージ文字列の長さを取得
	int len[3];
	for( int i = 0; i < items; i++ )
		len[i] = strlen( msg[i] );

	// 各項目の始点Ｙ座標を計算
	int item_y[3];
	for( i = 0; i < items; i++ )
		item_y[i] = prof.MsgBoxTop + i * prof.MsgLineHeight;

	// 各選択肢を描画する
	for( i = 0; i < items; i++ )
		PutMessageLine( prof.MsgBoxLeft, item_y[i], msg[i], len[i], 0x88, 0x88, 0x88, 0);

	window->Sync();
	bool is_l_down = window->IsMouseLDown();	// 左ボタンが押下されたか

	int select = -1;
	int	mx, my;	// マウス座標

	// 左ボタンがされるまでループ
	for(;;) {
		window->Sync();
		// セーブ＆ロードチェック
		if( window->IsSavePushed() )	// メニューのセーブが押されたか
			SaveDialog();
		if( window->IsLoadPushed() )	// メニューのロードが押されたか
			if( LoadDialog() )
				return -1;	// ロード操作が行われた
		if( window->IsBackLogPushed() )
			BackLogDialog();

		// マウスの状態を取得
		bool b = window->IsMouseLDown();
		mx = window->GetMousePosX();
		my = window->GetMousePosY();

		// マウスポインタの位置によってポイントされている項目を変更する
		for( i = 0; i < items; i++ ) {
			if( my >= item_y[i] && my < item_y[i] + prof.MsgLineHeight )
				break;
		}
		if( i != items && i != select ) {
			// ポイントされている項目が変更された
			if( select != -1 )	// 前にポイントされていた項目があれば暗い色で描画する
				PutMessageLine( prof.MsgBoxLeft, item_y[select], msg[select], len[select], 0x88, 0x88, 0x88, 0);
			// 新しくポイントされた項目を明るい色で描画する
			select = i;
			PutMessageLine( prof.MsgBoxLeft, item_y[select], msg[select], len[select], 0xff, 0xff, 0xff, 0);
		}

		// クリックによる選択決定を処理する
		if( ! is_l_down && b ) {
			// 左ボタンが押下された
			is_l_down = true;
			continue;
		}
		if( is_l_down && ! b ) {
			// 押下された左ボタンが開放され、すでにポイントされた項目がある

			// マウスポインタが項目の上にあるか？
			for( i = 0; i < items; i++ ) {
				if( my >= item_y[i] && my < item_y[i] + prof.MsgLineHeight )
					break;
			}
			if( i != items ) {
				select == i;
				break;	// 選択完了
			} else
				is_l_down = false;
		}  // if
	} // for

	return select;
}
