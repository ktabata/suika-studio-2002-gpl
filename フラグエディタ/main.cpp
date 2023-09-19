#include <windows.h>
#include <io.h>
#include <string.h>
#include "VNDFlagEdit.h"

VNString *GetOpenFileNameIndirect( const char *, const char *, const char *);

int WINAPI WinMain (HINSTANCE, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
	VNString file_name;

	if( strcmp( lpszCmdLine, "" ) != 0 ) {
		int len = strlen( lpszCmdLine );

		// ファイル名が""で囲まれている場合
		if( lpszCmdLine[0] == '\"' && lpszCmdLine[ len - 1 ] == '\"' )
			file_name = VNString( &lpszCmdLine[1], len - 2 );
		else
			file_name = lpszCmdLine;

		// ファイルが存在するか調べる
		if( _access( file_name.c_str(), 00 ) == -1 ) {
			MessageBox (NULL,
						"指定されたファイルは存在しません。",
						"フラグエディタ",
						MB_OK | MB_ICONEXCLAMATION);
			return 0;
		}
	} else
		file_name = "";


	VNDFlagEdit edit;
	bool res = edit.DoModal( "フラグエディタ", file_name.c_str(), NULL );

	if( ! res ) {
		MessageBox (NULL,
					"エラーが発生しました。",
					"フラグエディタ",
					MB_OK | MB_ICONEXCLAMATION);
	}

	return 0;
}
