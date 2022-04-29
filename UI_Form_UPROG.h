//---------------------------------------------------------------------------

#ifndef UI_Form_UPROGH
#define UI_Form_UPROGH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
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
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *Z;
	TTabControl *TabControl1;
	TButton *Button1;
	TButton *Button2;
	TGroupBox *GroupBox2;
	TCheckBox *CheckBox1;
	TButton *Button3;
	TButton *Button4;
	TProgressBar *ProgressBar1;
	TGroupBox *GroupBox3;
	TButton *Button5;
	TButton *Button6;
	TButton *Button7;
	TGroupBox *GroupBox4;
	TButton *Button8;
	TButton *Button9;
	TButton *Button10;
	TGroupBox *GroupBox5;
	TEdit *Edit1;
	TButton *Button11;
	TGroupBox *GroupBox1;
	TMemo *Memo1;
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
