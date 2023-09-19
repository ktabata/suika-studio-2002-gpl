#include <windows.h>
#include <io.h>
#include <string.h>
#include <assert.h>
#include "VNDEventEdit.h"


int WINAPI WinMain (HINSTANCE, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
	VNString file_name;

	if( strcmp( lpszCmdLine, "" ) != 0 ) {
		// �ҏW����t�@�C�����w�肳�ꂽ�ꍇ
		int len = strlen( lpszCmdLine );

		// �t�@�C������""�ň͂܂�Ă���ꍇ
		if( lpszCmdLine[0] == '\"' && lpszCmdLine[ len - 1 ] == '\"' )
			file_name = VNString( &lpszCmdLine[1], len - 2 );
		else
			file_name = VNString( lpszCmdLine );

		if( _access( file_name.c_str(), 00 ) == -1 ) {
			MessageBox (NULL,
						"�w�肳�ꂽ�t�@�C���͑��݂��܂���B",
						"�C�x���g�G�f�B�^",
						MB_OK | MB_ICONEXCLAMATION);
			return 0;
		}
	} else
		file_name = "";


	VNDEventEdit edit;
	bool res = edit.DoModal( "�C�x���g�G�f�B�^", file_name.c_str(), NULL );

	if( ! res ) {
		MessageBox (NULL,
					"�s���ȃG���[���������܂����B",
					"�C�x���g�G�f�B�^",
					MB_OK | MB_ICONEXCLAMATION);
	}

	return 0;
}
