#include <string.h>
#include <windows.h>
#include "VNDBackLog.h"
#include "resource.h"


VNDBackLog::VNDBackLog( const VNBackLog& backlog ) : backlog( backlog ) { }

void VNDBackLog::DoModal( HWND hWndParent )
{
	VNDialog::DoModal( "バックログ", "IDD_BACKLOG", hWndParent );
}

bool VNDBackLog::OnInit() {
	int cnt = backlog.GetStockCount();
	VNString msg;
	for( int i = 0; i < cnt; i++ )
		msg += backlog.GetString( i );
	SetWindowText (GetDlgItem (hWnd, IDC_EDIT), msg.c_str());
	return TRUE;
}

void VNDBackLog::OnCommand( WPARAM wParam, LPARAM lParam ) {
	WORD	wNotifyCode = HIWORD(wParam);	// 通知コード 
	WORD	wID = LOWORD(wParam);			// 項目、コントロール、またはアクセラレータの ID 
	HWND	hWndCtl = (HWND) lParam;		// コントロールのハンドル 

	switch( wID ) {
	case IDOK:
		EndDialog (hWnd, TRUE);
		return;
	}
}
