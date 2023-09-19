#include "VNEvent.h"
#include <stddef.h>	// NULL


EventTypeDescriptor	event_type_desc[EVENTCODE_MAX + 1] = {
{ 0, 0, NULL },		/// index-0 for invalidation
{ EVENTCODE_NARRATION, 1, "S" },
{ EVENTCODE_SERIF, 2, "SS" },
{ EVENTCODE_CHARACTER, 4, "IISS" }, // 左中右、上中下、RGB、マスク
{ EVENTCODE_BACKGROUND, 1, "S" },
{ EVENTCODE_SELECT, 6, "SSSSSS" },	// 最大で三者択一、nearジャンプ
{ EVENTCODE_LABEL, 1, "S" },
{ EVENTCODE_CDDA, 2, "II" },
{ EVENTCODE_MIDI, 2, "SI" },
{ EVENTCODE_PCM, 1, "S" },
{ EVENTCODE_SETFLAG, 2, "SI" },
{ EVENTCODE_JUMP, 4, "SSIS" },
{ EVENTCODE_TITLE, 1, "S" },	// セーブタイトル、ウィンドウタイトル
{ EVENTCODE_WAITCLICK, 0, "" },
{ EVENTCODE_WAIT, 1, "I" },
{ EVENTCODE_MSGBOX, 1, "I" },
{ EVENTCODE_COMMENT, 1, "S" }
};
