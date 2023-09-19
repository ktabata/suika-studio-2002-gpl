#include "VNFileDialog.h"
#include <string.h>
#include <windows.h>


static bool Helper( bool is_save, const char *title, const char *filter,
				    const char *deffilename, VNString& res ) {
	char	*filename = new char [1024];
	if( filename == NULL )
		return false;

	if( deffilename != NULL )
		strcpy( filename, deffilename );
	else
		filename[0] = '\0';

	OPENFILENAME ofn;
	ofn.lStructSize = sizeof( ofn );
	ofn.hwndOwner = NULL; 
	ofn.hInstance = NULL; 
	ofn.lpstrFilter = filter;
	ofn.lpstrCustomFilter = NULL; 
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0; 
	ofn.lpstrFile = filename;
	ofn.nMaxFile = 1024;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir =NULL;
	ofn.lpstrTitle = title;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_HIDEREADONLY;	// OFN_CREATEPROMPT | OFN_NOCHANGEDIR OFN_NONETWORKBUTTON OFN_OVERWRITEPROMPT
	ofn.lpstrDefExt = NULL;

	bool b = false;
	if( is_save ) {
		if (GetSaveFileName (&ofn)) {
			res = filename;
			b = true;
		}
	} else {
		if (GetOpenFileName (&ofn)) {
			res = filename;
			b = true;
		}
	}

	free( filename );
	return b;
}


bool GetOpenFileNameIndirect( const char *title, const char *filter, const char *deffilename, VNString& res ) {
	return Helper( false, title, filter, deffilename, res );
}

bool GetSaveFileNameIndirect( const char *title, const char *filter, const char *deffilename, VNString& res ) {
	return Helper( true, title, filter, deffilename, res );
}

