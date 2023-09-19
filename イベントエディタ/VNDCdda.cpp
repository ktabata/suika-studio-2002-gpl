#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "resource.h"
#include "VNDCdda.h"


bool VNDCdda::DoModal( int& track, int& fade, HWND hWndParent ) {
	edit_track = new char [64];
	sprintf( edit_track, "%d", track );
	edit_millisec = new char [64];
	sprintf( edit_millisec, "%d", fade );

	int b = VNDialog::DoModal( "CD-DA 演奏", "IDD_CDDA", hWndParent );
	bool res;

	if( b == TRUE ) {
		track = atoi( edit_track );
		fade = atoi( edit_millisec );
		res = true;
	} else
		res = false;

	delete edit_track;
	delete edit_millisec;
	return res;
}

bool VNDCdda::OnInit() {
	SetWindowText (GetDlgItem (hWnd, IDC_EDITTRACK), edit_track);
	SetWindowText (GetDlgItem (hWnd, IDC_EDITMILLISEC), edit_millisec);
	return true;
}

void VNDCdda::OnCommand( WPARAM wParam, LPARAM lParam ) {
	WORD	wNotifyCode = HIWORD(wParam);	// 通知コード 
	WORD	wID = LOWORD(wParam);			// 項目、コントロール、またはアクセラレータの ID 
	HWND	hWndCtl = (HWND) lParam;		// コントロールのハンドル 

	switch( wID ) {
	case IDOK:
		GetWindowText (GetDlgItem (hWnd, IDC_EDITTRACK), edit_track, 63);
		GetWindowText (GetDlgItem (hWnd, IDC_EDITMILLISEC), edit_millisec, 63);
		EndDialog (hWnd, TRUE);
		return;

	case IDCANCEL:
		EndDialog (hWnd, FALSE);
		return;
	}
}
