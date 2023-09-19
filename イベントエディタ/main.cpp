#include <windows.h>
#include <io.h>
#include <string.h>
#include <assert.h>
#include "VNDEventEdit.h"


int WINAPI WinMain (HINSTANCE, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
	VNString file_name;

	if( strcmp( lpszCmdLine, "" ) != 0 ) {
		// 編集するファイルが指定された場合
		int len = strlen( lpszCmdLine );

		// ファイル名が""で囲まれている場合
		if( lpszCmdLine[0] == '\"' && lpszCmdLine[ len - 1 ] == '\"' )
			file_name = VNString( &lpszCmdLine[1], len - 2 );
		else
			file_name = VNString( lpszCmdLine );

		if( _access( file_name.c_str(), 00 ) == -1 ) {
			MessageBox (NULL,
						"指定されたファイルは存在しません。",
						"イベントエディタ",
						MB_OK | MB_ICONEXCLAMATION);
			return 0;
		}
	} else
		file_name = "";


	VNDEventEdit edit;
	bool res = edit.DoModal( "イベントエディタ", file_name.c_str(), NULL );

	if( ! res ) {
		MessageBox (NULL,
					"不明なエラーが発生しました。",
					"イベントエディタ",
					MB_OK | MB_ICONEXCLAMATION);
	}

	return 0;
}
