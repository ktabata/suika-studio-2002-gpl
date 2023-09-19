#include "VNDLoad.h"
#include "VNSaveFile.h"
#include "resource.h"

// セーブファイルの番号を返す。-1ならキャンセル。
int VNDLoad::DoModal( HWND hWnd )
{
	select = -1;
	VNDialog::DoModal( "ロード", "IDD_LOAD", hWnd );
	return select;
}

void VNDLoad::OnCommand( WPARAM wParam, LPARAM lParam ) {
	WORD	wNotifyCode = HIWORD(wParam);	// 通知コード 
	WORD	wID = LOWORD(wParam);			// 項目、コントロール、またはアクセラレータの ID 
	HWND	hWndCtl = (HWND) lParam;		// コントロールのハンドル 

	switch( wID )	{
	case IDC_BTNLOAD:
		GetSelectIndex();
		if( select != -1 )
			if( available[select] )
				EndDialog (hWnd, FALSE);
		return;
	case IDCANCEL:
		select = -1;
		EndDialog (hWnd, FALSE);
		return;
	}
}

bool VNDLoad::OnInit() {
	for( int i = 0; i < 10; i++ ) {
		VNSaveFile	file;
		VNString str;
		if( ! file.Load( i ) ) {
			str = (VNString)"セーブ"+(VNString)i+" --------- NO DATA ---------";
			available[i] = false;
		} else {
			str = (VNString)"セーブ"+(VNString)i+" "+file.GetDate()+" "+file.GetTitle();
			available[i] = true;
		}
		AddItemToList( str.c_str() );
	}
	return true;
}

void VNDLoad::AddItemToList( const char *str ) {
	HWND	hWndLB = ::GetDlgItem (hWnd, IDC_LIST);
	::SendMessage (hWndLB, LB_ADDSTRING, (WPARAM) 0, (LPARAM)(LPCTSTR) str);
}

void VNDLoad::GetSelectIndex() {
	// イベントリストの選択項目のインデックスを取得
	HWND	hWndLB = ::GetDlgItem (hWnd, IDC_LIST);
	select = ::SendMessage (hWndLB, LB_GETCURSEL, 0, 0);
	if( select == LB_ERR )
		select = -1;
}
