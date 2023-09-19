#include <windows.h>
#include "VNPlayer.h"

// SJIS�Q�o�C�g�����̂P�o�C�g�ڂ��H
#define is_kanji1(c)	((unsigned char)c>=0x81&&(unsigned char)c<=0x9F||(unsigned char)c>=0xE0&(unsigned char)c<=0xFC)

#define WAIT_CHARS	(5)
char *wait_char[WAIT_CHARS] = { "�c", "�A", "�B", "�I", "�H", };


bool VNPlayer::Narration( const char *msg ) {
	last_message = msg;
	backlog.AddString( msg );
	if( ! PutMessage( msg, 0xff, 0xff, 0xff, 30 ) ) {
		PutMessage( msg, 0xff, 0xff, 0xff, 0 );	// �f�B���C�\�����ɃN���b�N���ꂽ
		return MessageWait( true );	// quick���[�h
	} else
		return MessageWait( false );	// �ʏ탂�[�h
}

bool VNPlayer::Serif( const char *name, const char *msg ) {
	// ���O�ƃ��b�Z�[�W��A�����ĕ`��
	last_message = VNString("�y") + name + "�z�u" + msg + "�v";
	backlog.AddString( last_message );
	if( ! PutMessage( last_message.c_str(), 0xff, 0xff, 0xff, 30 ) ) {
		PutMessage( last_message.c_str(), 0xff, 0xff, 0xff, 0 );	// �f�B���C�\�����ɃN���b�N���ꂽ
		return MessageWait( true );	// quick���[�h
	} else
		return MessageWait( false );	// �ʏ탂�[�h
}

bool VNPlayer::CharaFade( int pos, int align, const char *rgb_image_name, const char *mask_image_name ) {
	if( pos >= 3 )
		return false;

	// �摜�̃��[�h
	VNImage *new_img = NULL;
	if( strcmp( rgb_image_name, "" ) != 0 ) {
		new_img = new VNImage();

		bool b;
		if( strcmp( mask_image_name, "" ) != 0 ) {
			// �}�X�N�t��
			b = new_img->LoadBmp(
					(VNString(CG_DIR) + rgb_image_name).c_str(),
					(VNString(CG_DIR) + mask_image_name ).c_str() );
		} else {
			// �}�X�N�Ȃ�
			b = new_img->LoadBmp(
					(VNString(CG_DIR) + rgb_image_name).c_str(), NULL );
		}

		if( ! b ) {
			// ���[�h�Ɏ��s
			delete new_img;
			return false;
		}

		this->rgb_image_name[pos+1] = rgb_image_name;
		this->mask_image_name[pos] = mask_image_name;
	} else {
		this->rgb_image_name[pos+1] = "";
		this->mask_image_name[pos] = "";
	}

	// �Â��摜��ۑ��i�t�F�[�h�A�E�g������̂Ɏg���j
	VNImage *old_img = image[pos+1];
	int old_x = chara_x[pos];
	int old_y = chara_y[pos];
	int old_w = chara_width[pos];
	int old_h = chara_height[pos];

	// �V�����摜��ݒ�
	SetChara( pos, align, new_img );

	// �L�������\���ɂ��Ă���
	image[pos+1] = NULL;

	// �A�j���[�V�������[�v
	int	last_mask = -1;
	unsigned long start = ::GetTickCount();
	bool is_l_down = window->IsMouseLDown();
	unsigned long fade_time = CHARA_FADE_MILLI;

	while(1) {
		// �N���b�N��b�������ɂ��L�����Z�������o
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

		// �Â��摜��\��
		if( old_img != NULL )
			back_buffer->MaskBlt(
				MASK_LEVELS - 1 - mask,
				old_x, old_y,	// dst x,y
				old_img,
				0, 0,			// src x,y
				old_w, old_h );	// width, height

		// �V�����摜��\��
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

	// �L�����̕\����L���ɂ���
	image[pos+1] = new_img;

	// �\��������
	DrawBgAndChara();
	window->Flip();
	window->Sync();

	return true;
}

// �w�i�̃t�F�[�h�C���E�A�E�g
bool VNPlayer::BackgroundFade( const char *image_name ) {
	// �摜�̃��[�h
	VNImage *new_img = NULL;
	if( strcmp( image_name, "" ) != 0 ) {
		new_img = new VNImage();
		bool b = new_img->LoadBmp( (VNString(CG_DIR) + image_name).c_str(), NULL );
		if( ! b ) {		// ���[�h�Ɏ��s
			delete new_img;
			new_img = NULL;
			rgb_image_name[0] = "";
		} else
			rgb_image_name[0] = image_name;
	} else
		rgb_image_name[0] = "";

	// �Â��摜��ۑ��i�t�F�[�h�A�E�g������̂Ɏg���j
	VNImage *old_img = image[0];

	if( old_img == NULL && new_img == NULL )
		return false;

	// �w�i���\���ɂ��Ă���
	image[0] = NULL;

	// �L�����̕\���𖳌��ɂ��ĉ摜��j������
	for( int i = 1; i <= 3; i++ ) {
		if( image[i] != NULL ) {
			delete image[i];
			image[i] = NULL;
		}
	}

	// �t�F�[�h�C���E�t�F�[�h�A�E�g
	int	last_mask = -1;
	unsigned long start = ::GetTickCount();
	bool is_l_down = window->IsMouseLDown();
	unsigned long fade_time = BG_FADE_MILLI;

	while(1) {
		// �N���b�N��b�������ɂ��L�����Z�������o
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

		// �Â��摜��\��
		if( old_img != NULL )
			back_buffer->MaskBlt(
				MASK_LEVELS - mask - 1,
				prof.BackgroundLeft,
				prof.BackgroundTop,
				old_img );

		// �V�����摜��\��
		if( new_img != NULL )
			back_buffer->MaskBlt(
				mask,
				prof.BackgroundLeft,
				prof.BackgroundTop,
				new_img );
		else {
			// �w�i���t�F�[�h�A�E�g����ꍇ
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

	// �w�i��ݒ肷��
	image[0] = new_img;

	// �\��������
	back_buffer->Clear();
	DrawBgAndChara();
	window->Flip();
	window->Sync();

	return true;
}


///////////////////////////
// �C�x���g�����֐��̕⏕

void VNPlayer::DrawBgAndChara() {
	// �E�B���h�E�w�i��`��
	if( window_image != NULL )
		back_buffer->BitBlt( 0, 0, window_image );

	// �w�i��`��
	if( image[0] != NULL )
		back_buffer->BitBlt( prof.BackgroundLeft, prof.BackgroundTop, image[0] );

	// �L������`��
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

	// �\���ʒu�̂w���W���v�Z����
	switch( pos ) {
	case CHARA_H_LEFT:	chara_x[pos]= 0; break;
	case CHARA_H_CENTER:chara_x[pos] = (640 - chara_width[pos]) / 2; break;
	case CHARA_H_RIGHT:	chara_x[pos] = 640 - chara_width[pos];
	}

	// �\���ʒu�̂x���W���v�Z����
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

		// �P�s���̕��ɂȂ�ő�̕��������v�Z����
		int line_len = len;
		for(;;) {
			if( line_len <= 0 )
				return false;	// �G���[

			width = text_drawer->GetWidth( msg, line_len );
			if( width == -1 )
				return false;	// �G���[

			if( width <= prof.MsgBoxWidth )
				break;	// �v�Z����

			// �c��̕������Q�o�C�g�ȏ�ŁA�Ō�̂Q�o�C�g���Q�o�C�g�������H
			if( line_len >= 2 ) {
				if( is_kanji1( *(msg+line_len-2) ) )
					line_len -= 2;
				else
					line_len--;
			} else
				line_len--;
		}

		// �`�悷��
		if( ! PutMessageLine( prof.MsgBoxLeft, prof.MsgBoxTop + line * prof.MsgLineHeight, msg, line_len, r, g, b, delay ) )
			return false;	// �f�B���C�\�����ɃN���b�N���ꂽ

		// �N���b�N�҂��v�����v�g�̈ʒu
		prompt_x = prof.MsgBoxLeft + width;
		prompt_y = prof.MsgBoxTop + line * prof.MsgLineHeight;

		msg += line_len;
		len -= line_len;
		line++;
	}
	return true;
}

// �f�B���C�\���̓r���ŃN���b�N�����ƁAfalse��Ԃ�
bool VNPlayer::PutMessageLine( int x, int y, const char *msg, int len, int r, int g, int b, int delay ) {
	int n = 0;
	bool is_l_down = window->IsMouseLDown();
	while( n < len ) {
		bool	need_wait = false;

		if( ! is_kanji1( *(msg+n) ) )
			n++;	// ASCII����
		else {
			// �E�F�C�g�������H
			if( delay != 0 ) {
				for( int i = 0; i < WAIT_CHARS; i++ ) {
					if( strncmp( msg+n, wait_char[i], 2 ) == 0) {
						need_wait = true;
						break;
					}
				}
			}
			n += 2;	// �Q�o�C�g����
		}

		text_drawer->SetColor( 0, 0 ,0 );
		text_drawer->Draw( x+2, y+2, msg, n );
		text_drawer->SetColor( r, g ,b );
		text_drawer->Draw( x, y, msg, n );

		if( delay != 0 ) {
			// �N���b�N��b�������ɂ��L�����Z�������o
			window->Sync();
			if( window->IsMouseLDown() ) {
				if( ! is_l_down )
					return false;
			} else
				is_l_down = false;
			if( window->IsCtrlDown() )
				return false;

			// �`�恕�E�F�C�g
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
				// �E�F�C�g�����ɂ��E�F�C�g
				unsigned long l = GetTickCount();
				while( l + 300 >= GetTickCount() ) {
					window->Sync();

					// �N���b�N��b�������ɂ��L�����Z�������o
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

	if( delay == 0 )	// �܂���x���`�悳��Ă��Ȃ��̂ŕ`��
		window->Update( prof.MsgBoxLeft, prof.MsgBoxTop, prof.MsgBoxWidth, prof.MsgBoxHeight );
	return true;
}

// �I�����ꂽ�ԍ�(0-3)��Ԃ��B���[�h���삪�s�����-1��Ԃ�
int VNPlayer::SelectLoop( const char *msg[3], int items ){
	DrawBgAndChara();
	DrawMessageBox();
	window->Update( prof.MsgBoxLeft - prof.MsgBoxFrameWidth,
					prof.MsgBoxTop - prof.MsgBoxFrameHeight,
					prof.MsgBoxWidth + prof.MsgBoxFrameWidth * 2,
					prof.MsgBoxHeight + prof.MsgBoxFrameHeight * 2 );

	// �e���b�Z�[�W������̒������擾
	int len[3];
	for( int i = 0; i < items; i++ )
		len[i] = strlen( msg[i] );

	// �e���ڂ̎n�_�x���W���v�Z
	int item_y[3];
	for( i = 0; i < items; i++ )
		item_y[i] = prof.MsgBoxTop + i * prof.MsgLineHeight;

	// �e�I������`�悷��
	for( i = 0; i < items; i++ )
		PutMessageLine( prof.MsgBoxLeft, item_y[i], msg[i], len[i], 0x88, 0x88, 0x88, 0);

	window->Sync();
	bool is_l_down = window->IsMouseLDown();	// ���{�^�����������ꂽ��

	int select = -1;
	int	mx, my;	// �}�E�X���W

	// ���{�^���������܂Ń��[�v
	for(;;) {
		window->Sync();
		// �Z�[�u�����[�h�`�F�b�N
		if( window->IsSavePushed() )	// ���j���[�̃Z�[�u�������ꂽ��
			SaveDialog();
		if( window->IsLoadPushed() )	// ���j���[�̃��[�h�������ꂽ��
			if( LoadDialog() )
				return -1;	// ���[�h���삪�s��ꂽ
		if( window->IsBackLogPushed() )
			BackLogDialog();

		// �}�E�X�̏�Ԃ��擾
		bool b = window->IsMouseLDown();
		mx = window->GetMousePosX();
		my = window->GetMousePosY();

		// �}�E�X�|�C���^�̈ʒu�ɂ���ă|�C���g����Ă��鍀�ڂ�ύX����
		for( i = 0; i < items; i++ ) {
			if( my >= item_y[i] && my < item_y[i] + prof.MsgLineHeight )
				break;
		}
		if( i != items && i != select ) {
			// �|�C���g����Ă��鍀�ڂ��ύX���ꂽ
			if( select != -1 )	// �O�Ƀ|�C���g����Ă������ڂ�����ΈÂ��F�ŕ`�悷��
				PutMessageLine( prof.MsgBoxLeft, item_y[select], msg[select], len[select], 0x88, 0x88, 0x88, 0);
			// �V�����|�C���g���ꂽ���ڂ𖾂邢�F�ŕ`�悷��
			select = i;
			PutMessageLine( prof.MsgBoxLeft, item_y[select], msg[select], len[select], 0xff, 0xff, 0xff, 0);
		}

		// �N���b�N�ɂ��I���������������
		if( ! is_l_down && b ) {
			// ���{�^�����������ꂽ
			is_l_down = true;
			continue;
		}
		if( is_l_down && ! b ) {
			// �������ꂽ���{�^�����J������A���łɃ|�C���g���ꂽ���ڂ�����

			// �}�E�X�|�C���^�����ڂ̏�ɂ��邩�H
			for( i = 0; i < items; i++ ) {
				if( my >= item_y[i] && my < item_y[i] + prof.MsgLineHeight )
					break;
			}
			if( i != items ) {
				select == i;
				break;	// �I������
			} else
				is_l_down = false;
		}  // if
	} // for

	return select;
}
