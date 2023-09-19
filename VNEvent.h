#ifndef _VNEVENT_H_
#define _VNEVENT_H_

#include "VNString.h"


#define EVENTCODE_MAX		(16)

// イベント
#define EVENTCODE_NARRATION			(1)
#define EVENTCODE_SERIF				(2)
#define EVENTCODE_CHARACTER			(3)
#define EVENTCODE_BACKGROUND		(4)
#define EVENTCODE_SELECT			(5)
#define EVENTCODE_LABEL				(6)
#define EVENTCODE_CDDA				(7)
#define EVENTCODE_MIDI				(8)
#define EVENTCODE_PCM				(9)
#define EVENTCODE_SETFLAG			(10)
#define EVENTCODE_JUMP				(11)
#define EVENTCODE_TITLE				(12)
#define EVENTCODE_WAITCLICK			(13)
#define EVENTCODE_WAIT				(14)
#define EVENTCODE_MSGBOX			(15)
#define EVENTCODE_COMMENT			(16)

// フラグ設定イベントの種類
#define SETFLAG_ON		(0)
#define SETFLAG_OFF		(1)
#define SETFLAG_INVERT	(2)

// ジャンプイベントの種類
#define JUMP_FORCED			(0)
#define JUMP_IFFLAGSET		(1)
#define JUMP_IFFLAGNOTSET	(2)

// キャライベントのキャラの位置
#define CHARA_H_CENTER	(0)
#define CHARA_H_LEFT	(1)
#define CHARA_H_RIGHT	(2)
#define	CHARA_V_BOTTOM	(0)
#define	CHARA_V_CENTER	(1)
#define	CHARA_V_TOP		(2)


struct EventTypeDescriptor {
	int		code;
	int		argc;
	char	*arg_type_desc;
};

extern EventTypeDescriptor	event_type_desc[EVENTCODE_MAX + 1];

class VNEvent {
public:
	VNEvent( int );
	~VNEvent();

	VNEvent( VNEvent& );
	VNEvent operator=( VNEvent& );

	int GetGenre();
	int GetCode();
	const EventTypeDescriptor& GetDescriptor();

	int	GetParamInt( int );
	const VNString& GetParamString( int );

	void SetParamInt( int, int );
	void SetParamString( int, const char* );

	VNString GetExplanation();	// イベントエディタに表示する、「イベントの説明」を返す

private:
	void AllocArgList();

	const EventTypeDescriptor&	event_type;
	int		params;
	int		*int_arg;
	VNString **str_arg;

	void DebugBreak( const char * );
};


#endif
