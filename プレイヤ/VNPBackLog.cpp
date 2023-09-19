#include "VNPlayer.h"
#include "VNDBackLog.h"


void VNPlayer::BackLogDialog() {
	VNDBackLog	dlg( backlog );
	dlg.DoModal( window->GetHWND() );
}
