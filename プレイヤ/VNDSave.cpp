#include "VNDSave.h"
#include "VNSaveFile.h"
#include "resource.h"

// セーブファイルの番号を返す。-1ならキャンセル。
int VNDSave::DoModal( HWND hWnd )
{
	select = -1;
	VNDialog::DoModal( "セーブ", "IDD_SAVE", hWnd );
	return select;
}

void VNDSave::OnCommand( WPARAM wParam, LPARAM lParam ) {
	WORD	wNotifyCode = HIWORD(wParam);	// 通知コード 
	WORD	wID = LOWORD(wParam);			// 項目、コントロール、またはアクセラレータの ID 
	HWND	hWndCtl = (HWND) lParam;		// コントロールのハンドル 

	switch( wID )	{
	case IDC_BTNSAVE:
		GetSelectIndex();
		if( select != -1 )
			EndDialog (hWnd, FALSE);
		return;
	case IDCANCEL:
		select = -1;
		EndDialog (hWnd, FALSE);
		return;
	}
}

bool VNDSave::OnInit() {
	for( int i = 0; i < 10; i++ ) {
		VNSaveFile	file;
		VNString str;
		file.Load( i );
		if( ! file.Load( i ) )
			str = (VNString)"セーブ"+(VNString)i+" --------- NO DATA ---------";
		else
			str = (VNString)"セーブ"+(VNString)i+" "+file.GetDate()+" "+file.GetTitle();
		AddItemToList( str.c_str() );
	}
	return true;
}

void VNDSave::AddItemToList( const char *str ) {
	HWND	hWndLB = ::GetDlgItem (hWnd, IDC_LIST);
	::SendMessage (hWndLB, LB_ADDSTRING, (WPARAM) 0, (LPARAM)(LPCTSTR) str);
}

void VNDSave::GetSelectIndex() {
	// イベントリストの選択項目のインデックスを取得
	HWND	hWndLB = ::GetDlgItem (hWnd, IDC_LIST);
	select = ::SendMessage (hWndLB, LB_GETCURSEL, 0, 0);
	if( select == LB_ERR )
		select = -1;
}
