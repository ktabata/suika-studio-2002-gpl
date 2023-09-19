#include <string.h>
#include <windows.h>
#include "resource.h"
#include "VNDCharacter.h"


bool VNDCharacter::DoModal( int& h_align, int& v_align, VNString& color, VNString& mask, HWND hWndParent ) {
	cb_h = h_align;
	cb_v = v_align;
	edit_color = new char [1024];
	strcpy( edit_color, color.c_str() );
	edit_mask = new char [1024];
	strcpy( edit_mask, mask.c_str() );

	int b = VNDialog::DoModal( "キャラクタ", "IDD_CHARACTER", hWndParent );
	bool res;

	if( b == TRUE  &&  cb_h != CB_ERR  &&  cb_v != CB_ERR ) {
		h_align = cb_h;
		v_align = cb_v;
		color = edit_color;
		mask = edit_mask;
		res = true;
	} else
		res = false;

	delete edit_color;
	delete edit_mask;
	return res;
}

bool VNDCharacter::OnInit() {
//	SetFocus (GetDlgItem (hWnd, IDC_EDITCOLOR));
	// エディットコントロールにファイル名をセット
	SetWindowText (GetDlgItem (hWnd, IDC_EDITCOLOR), edit_color);
	SetWindowText (GetDlgItem (hWnd, IDC_EDITMASK), edit_mask);

	// コンボボックスに項目を追加して選択
	HWND	hWndCBH = GetDlgItem (hWnd, IDC_CBHALIGN);
	HWND	hWndCBV = GetDlgItem (hWnd, IDC_CBVALIGN);
	static char *cb_item[] = { "中央", "左", "右", "下", "中央", "上" };

	for( int i = 0; i < 6; i++ )
		SendMessage ((i < 3) ? hWndCBH : hWndCBV, CB_ADDSTRING, 0, (LPARAM) (LPCTSTR) cb_item[i]);

	SendMessage (hWndCBH, CB_SETCURSEL, cb_h, 0);
	SendMessage (hWndCBV, CB_SETCURSEL, cb_v, 0);
	return FALSE;
}

void VNDCharacter::OnCommand( WPARAM wParam, LPARAM lParam ) {
	WORD	wNotifyCode = HIWORD(wParam);	// 通知コード 
	WORD	wID = LOWORD(wParam);			// 項目、コントロール、またはアクセラレータの ID 
	HWND	hWndCtl = (HWND) lParam;		// コントロールのハンドル 

	switch( wID ) {
	case IDOK:
		GetWindowText (GetDlgItem (hWnd, IDC_EDITCOLOR), edit_color, 1023);
		GetWindowText (GetDlgItem (hWnd, IDC_EDITMASK), edit_mask, 1023);
		cb_h = SendMessage (GetDlgItem (hWnd, IDC_CBHALIGN), CB_GETCURSEL, 0, 0);
		cb_v = SendMessage (GetDlgItem (hWnd, IDC_CBVALIGN), CB_GETCURSEL, 0, 0);
		EndDialog (hWnd, TRUE);
		return;

	case IDCANCEL:
		EndDialog (hWnd, FALSE);
		return;
	}
}
