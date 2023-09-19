#ifndef _VNPLAYER_H_
#define _VNPLAYER_H_

#include "VNPWindow.h"
#include "VNEventFile.h"
#include "VNFlagFile.h"
#include "VNTextDrawer.h"
#include "VNCDPlayer.h"
#include "VNMidiPlayer.h"
#include "VNBackLog.h"

#define CHARA_FADE_MILLI	(500)
#define BG_FADE_MILLI		(500)

#define EVENT_DIR		"event\\"
#define CG_DIR			"cg\\"
#define PCM_DIR			"pcm\\"
#define MIDI_DIR		"\\midi\\"


class VNPWindow;

class VNPlayer {
public:
	VNPlayer();
	~VNPlayer();

	bool	Play();	// start.evt������s�J�n

private:
// �f�o�b�O
	void Warning( const char * );
	void DebugBreak( const char * );

// �C�x���g�������[�`��
	bool	Narration( const char * );
	bool	Serif( const char *, const char * );
	bool	CharaFade( int, int, const char *, const char * );
	bool	BackgroundFade( const char * );
	void	VisibleMessage( bool );
	void	PlayPcm( const char * );
	void	PlayCdda( int, int );
	void	PlayMidi( const char *, int );
	void	WaitClick();
	void	Wait( int );
	void	SetFlag( const char *, int );
	void	Jump( const char *, const char *, int, const char * );
	bool	Select( const char *, const char *, const char *, const char *, const char *, const char * );
	void	Title( const char * );
	//	void	Messagebox();

// �T�u���[�`��
	void	ZeroMembers();
	int			GetIntProfile( const char *, int );
	VNString	GetStrProfile( const char *, const char * );
	void	LoadProfile();
	void	ReleaseAll();
	void	InitWindow();
	void	InitSound();
	void	LoadStartEvent();
	void	LoadFlag();
	bool	Execute();	// ���s���[�v�F���[�U�����[�h������s����false��Ԃ�
	bool	GetFlagIndex( const char *flag, int& index, bool& is_local );
	void	SaveDialog();
	bool	LoadDialog();
	void	BackLogDialog();
	bool	Save( int );
	bool	Load();
	bool	MessageWait( bool );
	void	DrawBgAndChara();		// �w�i�A�L������`�悷��
	void	DrawMessageBox();		// ���b�Z�[�W�{�b�N�X�̔w�i��`�悷��
	void	SetChara( int, int, VNImage * );
	bool	PutMessage( const char *, int, int, int, int );	// ���b�Z�[�W�{�b�N�X�ɕ�������o�͂���
	bool	PutMessageLine( int, int, const char *, int, int, int, int, int );
	int		SelectLoop( const char *[3], int );	// ���b�Z�[�W�{�b�N�X�őI������I��������

#include "VNPlayerProfile.h"	// Suika.ini���烍�[�h����v���t�@�C�����i�[����\���̂�錾���Ă���

// ���s���R���e�L�X�g
	VNPWindow			*window;		// �E�C���h�E�I�u�W�F�N�g
	VNImage				*window_image;	// �E�B���h�E�̔w�i
	VNCDPlayer			*cdplayer;		// MCI cdaudio ����N���X
	VNMidiPlayer		*midiplayer;	// MCI sequencer ����N���X
	VNEventFile			*event;			// �C�x���g�t�@�C���̃f�[�^
	int					offset;			// �Z�[�u���ځF�C�x���g�I�t�Z�b�g
	VNString			event_name;		// �Z�[�u���ځF�C�x���g�t�@�C����
	VNFlagFile			*global_flag;	// �Z�[�u���ځF�O���[�o���t���O
	VNFlagFile			*local_flag;	// �Z�[�u���ځF���[�J���t���O
	VNString			title;			// �Z�[�u���ځF�E�B���h�E���Z�[�u�^�C�g��
	VNImage				*back_buffer;	// �E�B���h�E�̃o�b�N�o�b�t�@
	VNTextDrawer		*text_drawer;
	VNImage				*image[4];
	VNString			rgb_image_name[4];	// �Z�[�u����
	VNString			mask_image_name[4];	// �Z�[�u����
	int					v_align[3];			// �Z�[�u����
	int					chara_width[3];
	int					chara_height[3];
	int					chara_x[3];
	int					chara_y[3];
	VNBackLog			backlog;
	int					load_anchor;
	VNString			last_message;
	int					prompt_x;
	int					prompt_y;
};


#endif
