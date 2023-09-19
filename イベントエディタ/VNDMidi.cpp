#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "resource.h"
#include "VNDMidi.h"


bool VNDMidi::DoModal( VNString& file, int& fade, HWND hWndParent ) {
	edit_file = new char [4096];
	strcpy( edit_file, file.c_str() );
	edit_millisec = new char [64];
	sprintf( edit_millisec, "%d", fade );

	int b = VNDialog::DoModal( "MIDI 演奏", "IDD_MIDI", hWndParent );
	bool res;

	if( b == TRUE ) {
		file = edit_file;
		fade = atoi( edit_millisec );
		res = true;
	} else
		res = false;

	delete edit_file;
	delete edit_millisec;
	return res;
}

bool VNDMidi::OnInit() {
	SetWindowText (GetDlgItem (hWnd, IDC_EDITFILE), edit_file);
	SetWindowText (GetDlgItem (hWnd, IDC_EDITMILLISEC), edit_millisec);
	return true;
}

void VNDMidi::OnCommand( WPARAM wParam, LPARAM lParam ) {
	WORD	wNotifyCode = HIWORD(wParam);	// 通知コード 
	WORD	wID = LOWORD(wParam);			// 項目、コントロール、またはアクセラレータの ID 
	HWND	hWndCtl = (HWND) lParam;		// コントロールのハンドル 

	switch( wID ) {
	case IDOK:
		GetWindowText (GetDlgItem (hWnd, IDC_EDITFILE), edit_file, 4098);
		GetWindowText (GetDlgItem (hWnd, IDC_EDITMILLISEC), edit_millisec, 63);
		EndDialog (hWnd, TRUE);
		return;

	case IDCANCEL:
		EndDialog (hWnd, FALSE);
		return;
	}
}
