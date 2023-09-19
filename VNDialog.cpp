#include "VNDialog.h"

int CALLBACK VNDialog_DlgProc (HWND, UINT, WPARAM, LPARAM);

VNDialog *VNDialog::list[256];
bool	VNDialog::is_initialized = false;
int		VNDialog::count = 0;


VNDialog::VNDialog() {
	hWnd = NULL;

	// staticメンバの初期化（初回のみ）
	if( ! is_initialized ) {
		for( int i = 0; i < VND_LIST_ITEMS; i++ )
			list[i] = NULL;
		is_initialized = true;
	}
}

VNDialog::~VNDialog() {
	// 実行中に削除された
	if( hWnd != NULL )
		DebugBreak("実行中のデストラクタ呼び出し");
}

int VNDialog::SearchItem( VNDialog *obj ) {
	// オブジェクトリストから一致項目を探す
	for( int i = 0; i < VND_LIST_ITEMS; i++ ) {
		if( list[i] == obj )
			return i;
	}
	return -1;
}

void VNDialog::AddToList() {
	if( ++count >= VND_LIST_ITEMS )
		DebugBreak("最大オブジェクト個数を越えてダイアログを作ろうとした");

	// 空き項目を探す
	int n = SearchItem( NULL );
	if( n < 0 )
		DebugBreak("空き項目が見つからない（明らかなバグ：リストからの削除が正常に行われていない）");

	list[n] = this;
}

void VNDialog::DeleteFromList() {
	if( count <= 0 )
		DebugBreak("リストに項目がないのに削除しようとした");

	int n = SearchItem( this );
	if( n < 0 )
		DebugBreak("削除する項目がリストから見つからない");

	list[n] = NULL;
}

VNDialog *VNDialog::FromHandle( HWND hWnd ) {
	for( int cnt = 0, i = 0; cnt < count && i < VND_LIST_ITEMS; i++ ) {
		if( list[i] == NULL )
			continue;
		if( list[i]->hWnd == hWnd )
			return list[i];	// found
		cnt++;
	}
	// not found
	return NULL;
}

int VNDialog::DoModal( const char *title, const char *template_name, HWND hWndParent ) {
	this->title = (char *) title;
	int res = DialogBoxParamA (
				GetModuleHandle(NULL),
				template_name,
				hWndParent,
				(DLGPROC) VNDialog_DlgProc,
				(LPARAM) this);
	DeleteFromList();
	hWnd = NULL;
	return res;
}

int CALLBACK VNDialog_DlgProc (HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if( uMsg == WM_INITDIALOG ) {
		VNDialog *obj = (VNDialog *) lParam;
		obj->hWnd = hDlg;
		obj->AddToList();
		SetWindowText (hDlg, obj->title);
		return obj->OnInit() == true ? TRUE : FALSE;
	}

	VNDialog *obj = VNDialog::FromHandle( hDlg );
	if( obj != NULL ) {
		switch (uMsg) {
		case WM_COMMAND:
			obj->OnCommand( wParam, lParam );
			return TRUE;
		case WM_CLOSE:
			obj->OnClose();
			return TRUE;
		case WM_SIZE:
			obj->OnSize( LOWORD(lParam), HIWORD(lParam) );
			return TRUE;
		}
	}

	return FALSE;
}

bool VNDialog::OnInit() {
	return true;
}

void VNDialog::OnCommand( WPARAM wParam, LPARAM lParam ) {
/*	wNotifyCode = HIWORD(wParam); // 通知コード 
	wID = LOWORD(wParam); // 項目、コントロール、またはアクセラレータの ID
	hwndCtl = (HWND) lParam; // コントロールのハンドル 
*/
	return;
}

void VNDialog::OnSize( int, int ) { }

void VNDialog::OnClose() {
	EndDialog (hWnd, 0);
}

void VNDialog::DebugBreak( const char *s ) {
	int i = 3;
	MessageBox (hWnd, s, "VNDialog 内部エラー", MB_ICONEXCLAMATION|MB_OK);
//	abort();
	_asm{int 3}
}
