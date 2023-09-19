#ifndef _VNDFLAGEDIT_H_
#define _VNDFLAGEDIT_H_

#include "VNFlagFile.h"
#include "VNDFlagEdit_Add.h"


class VNDFlagEdit : public VNDialog {
public:
	bool DoModal( const char *, const char *, HWND );

private:
	VNFlagFile	*flag_file;
	VNString	file_name;
	bool		is_modified;

	// virtual
	bool OnInit();
	void OnClose();
	void OnCommand( WPARAM, LPARAM );

	// button
	void OnBtnOpen();
	void OnBtnSave();
	void OnBtnDelete();
	void OnBtnAdd();
	void OnBtnOnOff();

	// list box
	void AddItemToListBox( const char *, bool );
	bool InitListBox();

	// etc.
	int ClosePrompt();
	void DebugBreak();
};


#endif
