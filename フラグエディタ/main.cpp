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

		// �t�@�C������""�ň͂܂�Ă���ꍇ
		if( lpszCmdLine[0] == '\"' && lpszCmdLine[ len - 1 ] == '\"' )
			file_name = VNString( &lpszCmdLine[1], len - 2 );
		else
			file_name = lpszCmdLine;

		// �t�@�C�������݂��邩���ׂ�
		if( _access( file_name.c_str(), 00 ) == -1 ) {
			MessageBox (NULL,
						"�w�肳�ꂽ�t�@�C���͑��݂��܂���B",
						"�t���O�G�f�B�^",
						MB_OK | MB_ICONEXCLAMATION);
			return 0;
		}
	} else
		file_name = "";


	VNDFlagEdit edit;
	bool res = edit.DoModal( "�t���O�G�f�B�^", file_name.c_str(), NULL );

	if( ! res ) {
		MessageBox (NULL,
					"�G���[���������܂����B",
					"�t���O�G�f�B�^",
					MB_OK | MB_ICONEXCLAMATION);
	}

	return 0;
}
