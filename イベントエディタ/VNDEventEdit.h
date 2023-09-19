#ifndef _VNDEVENTEDIT_H_
#define _VNDEVENTEDIT_H_

#include "VNFileDialog.h"
#include "VNEventFile.h"

#include "VNDEventEdit.h"
#include "VNDNarration.h"
#include "VNDSerif.h"
#include "VNDCharacter.h"
#include "VNDBackground.h"
#include "VNDSelect.h"
#include "VNDLabel.h"
#include "VNDFlag.h"
#include "VNDCdda.h"
#include "VNDMidi.h"
#include "VNDPcm.h"
#include "VNDJump.h"
#include "VNDWait.h"
#include "VNDComment.h"
#include "VNDTitle.h"


class VNDEventEdit : public VNDialog {
public:
	bool DoModal( const char *, const char *, HWND );

private:
	void DebugBreak();

	VNEventFile *event_file;
	VNString	file_name;
	bool		is_modified;
	HFONT		hFont;

	VNEvent		*copy_item;	// copy,cut,pasteに使う

	// virtual
	bool OnInit();
	void OnClose();
	void OnCommand( WPARAM, LPARAM );

	// button
	void OnBtnDelete();
	void OnBtnAdd();
	void OnBtnEdit();
	void OnBtnCopy();
	void OnBtnCut();
	void OnBtnPaste();
	void OnBtnInsertEvent();
	void OnBtnOpen();
	void OnBtnFileInsert();
		// IDC_BTNSAVE はSave()で対応

	// list box
	void InsertItemToListBox( const char *, int = -1 );
	void DeleteItemFromListBox( int );
	void InitListBoxItem();
	void InitListBoxFont();

	// combo box
	void InitComboBox();

	// etc.
	int		ClosePrompt();
	void	Save();
	bool	EditEvent( VNEvent * );

	// 各イベント編集
	bool EditNarration( VNEvent * );
	bool EditSerif( VNEvent * );
	bool EditCharacter( VNEvent * );
	bool EditBackground( VNEvent * );
	bool EditSelect( VNEvent * );
	bool EditLabel( VNEvent * );
	bool EditFlag( VNEvent * );
	bool EditCdda( VNEvent * );
	bool EditMidi( VNEvent * );
	bool EditPcm( VNEvent * );
	bool EditJump( VNEvent * );
	bool EditWait( VNEvent * );
	bool EditComment( VNEvent * );
	bool EditTitle( VNEvent * );
};


#endif
