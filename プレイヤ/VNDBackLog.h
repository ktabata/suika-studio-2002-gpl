#ifndef _VNDBACKLOG_H_
#define _VNDBACKLOG_H_

#include "VNDialog.h"
#include "VNString.h"
#include "VNBAckLog.h"

class VNDBackLog : public VNDialog {
public:
	VNDBackLog(  const VNBackLog& );
	void DoModal( HWND );

private:
	const VNBackLog& backlog;

	// virtual
	bool OnInit();
	void OnCommand( WPARAM, LPARAM );
};


#endif
