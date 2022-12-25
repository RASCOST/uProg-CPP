//---------------------------------------------------------------------------

#ifndef UI_Form_UPROGH
#define UI_Form_UPROGH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Threading.hpp>

#include <FMX.Controls.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Dialogs.hpp>
#include <FMX.Edit.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Memo.Types.hpp>
#include <FMX.Menus.hpp>
#include <FMX.ScrollBox.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.TabControl.hpp>
#include <FMX.Types.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Dialogs.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Memo.Types.hpp>
#include <FMX.ScrollBox.hpp>
#include <FMX.Types.hpp>
#include <FMX.Edit.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.TabControl.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Menus.hpp>

#include <vector>
#include <algorithm>

#include "IProgrammer.h"
#include "AVRProgrammer.h"


//---------------------------------------------------------------------------
class TForm1 : public TForm, IUIProgrammer
{
__published:	// IDE-managed Components
	TGroupBox *groupBxLB;
	TButton *BtnReadLB;
	TButton *BtnWriteFB;
	TGroupBox *GroupBox2;
	TProgressBar *ProgressBar;
	TGroupBox *GroupBox3;
	TButton *BtnReadFlash;
	TButton *BtnWriteFlash;
	TButton *BtnVerifyFlash;
	TGroupBox *GroupBox4;
	TButton *Button8;
	TButton *Button9;
	TButton *Button10;
	TGroupBox *GroupBox5;
	TEdit *EditFileLocation;
	TButton *BtnOpenFile;
	TGroupBox *GroupBox1;
	TMemo *MemoConsole;
	TOpenDialog *OpenDialogFile;
	TCheckBox *CkBoxFBLB1;
	TCheckBox *CkBoxFBLB2;
	TTabControl *TabControl1;
	TTabItem *Low;
	TTabItem *High;
	TTabItem *Extended;
	TPanel *PanelFLB;
	TPanel *PanelFHB;
	TPanel *PanelFEB;
	TButton *BtnReadFBH;
	TButton *BtnWriteFBH;
	TCheckBox *ChBxFHB7;
	TCheckBox *ChBxFHB6;
	TCheckBox *ChBxFHB5;
	TCheckBox *ChBxFHB4;
	TCheckBox *ChBxFHB3;
	TCheckBox *ChBxFHB2;
	TCheckBox *ChBxFHB1;
	TCheckBox *ChBxFHB0;
	TButton *BtnReadFBL;
	TButton *BtnWritwFBL;
	TButton *BtnReadFSE;
	TButton *BtnWriteFSE;
	TCheckBox *ChBxFLB7;
	TCheckBox *ChBxFLB6;
	TCheckBox *ChBxFLB5;
	TCheckBox *ChBxFLB4;
	TCheckBox *ChBxFLB3;
	TCheckBox *ChBxFLB2;
	TCheckBox *ChBxFLB1;
	TCheckBox *ChBxFLB0;
	TCheckBox *ChBxFEB0;
	TMainMenu *MainMenu1;
	TMenuItem *MenuItemFile;
	TComboBox *CBxDevice;
	TLabel *LabelChooseDevice;
	TListBoxItem *ListBoxItemATtiny45;
	TMenuItem *MenuItemDevice;
	TGroupBox *GroupBox6;
	TListBoxItem *ListBoxItemAtmega88;
	TTimer *Timer;

	void __fastcall BtnOpenFileClick(TObject *Sender);
	void __fastcall BtnReadLBClick(TObject *Sender);
	void __fastcall BtnReadFBHClick(TObject *Sender);
	void __fastcall BtnReadFBLClick(TObject *Sender);
	void __fastcall BtnReadFSEClick(TObject *Sender);
	void __fastcall CBxDeviceChange(TObject *Sender);
	void __fastcall BtnWriteFlashClick(TObject *Sender);
	void __fastcall BtnVerifyFlashClick(TObject *Sender);

private:	// User declarations
	std::unique_ptr<IProgrammer> avrprog;
	template<typename T>
	void checkBits(T *t, uint8_t bits);

public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	void updateConsole(const std::wstring& message);
    void updateProgressBar(const float progress);
	void updateLockBits(const std::uint8_t lb);
	void updateFuseBytes(FUSE_BYTES byte, uint8_t bits);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
