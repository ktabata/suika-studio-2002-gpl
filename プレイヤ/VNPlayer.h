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

	bool	Play();	// start.evtから実行開始

private:
// デバッグ
	void Warning( const char * );
	void DebugBreak( const char * );

// イベント処理ルーチン
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

// サブルーチン
	void	ZeroMembers();
	int			GetIntProfile( const char *, int );
	VNString	GetStrProfile( const char *, const char * );
	void	LoadProfile();
	void	ReleaseAll();
	void	InitWindow();
	void	InitSound();
	void	LoadStartEvent();
	void	LoadFlag();
	bool	Execute();	// 実行ループ：ユーザがロード操作を行うとfalseを返す
	bool	GetFlagIndex( const char *flag, int& index, bool& is_local );
	void	SaveDialog();
	bool	LoadDialog();
	void	BackLogDialog();
	bool	Save( int );
	bool	Load();
	bool	MessageWait( bool );
	void	DrawBgAndChara();		// 背景、キャラを描画する
	void	DrawMessageBox();		// メッセージボックスの背景を描画する
	void	SetChara( int, int, VNImage * );
	bool	PutMessage( const char *, int, int, int, int );	// メッセージボックスに文字列を出力する
	bool	PutMessageLine( int, int, const char *, int, int, int, int, int );
	int		SelectLoop( const char *[3], int );	// メッセージボックスで選択肢を選択させる

#include "VNPlayerProfile.h"	// Suika.iniからロードするプロファイルを格納する構造体を宣言している

// 実行時コンテキスト
	VNPWindow			*window;		// ウインドウオブジェクト
	VNImage				*window_image;	// ウィンドウの背景
	VNCDPlayer			*cdplayer;		// MCI cdaudio 操作クラス
	VNMidiPlayer		*midiplayer;	// MCI sequencer 操作クラス
	VNEventFile			*event;			// イベントファイルのデータ
	int					offset;			// セーブ項目：イベントオフセット
	VNString			event_name;		// セーブ項目：イベントファイル名
	VNFlagFile			*global_flag;	// セーブ項目：グローバルフラグ
	VNFlagFile			*local_flag;	// セーブ項目：ローカルフラグ
	VNString			title;			// セーブ項目：ウィンドウ＆セーブタイトル
	VNImage				*back_buffer;	// ウィンドウのバックバッファ
	VNTextDrawer		*text_drawer;
	VNImage				*image[4];
	VNString			rgb_image_name[4];	// セーブ項目
	VNString			mask_image_name[4];	// セーブ項目
	int					v_align[3];			// セーブ項目
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
