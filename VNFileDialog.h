#ifndef _VNFILEDIALOG_H_
#define _VNFILEDIALOG_H_

#include "VNString.h"

bool GetOpenFileNameIndirect( const char *, const char *, const char *, VNString& );
bool GetSaveFileNameIndirect( const char *, const char *, const char *, VNString& );

#endif
