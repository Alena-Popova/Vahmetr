//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include "Lusbapi.h"
#include <Vcl.ExtCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TMenuItem *N1;
	TMenuItem *N2;
	TMenuItem *N3;
	TMenuItem *N4;
	TMenuItem *N5;
	TMenuItem *N6;
	TGroupBox *GroupBox1;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TCheckBox *CheckBox1;
	TGroupBox *GroupBox2;
	TButton *Button4;
	TButton *Button5;
	TButton *Button6;
	TButton *Button7;
	TGroupBox *GroupBox3;
	TMemo *Memo1;
	TButton *Button8;
	TGroupBox *GroupBox4;
	TGroupBox *GroupBox5;
	TLabel *Label1;
	TLabel *Label2;
	TChart *Chart1;
	TFastLineSeries *Series1;
	TTimer *Timer1;
	TLabel *Label3;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall Button7Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);

		SHORT* Buffer;
	double current, ADCcurrent;     // значения тока
	double ADCVOL, ADCCUR,ADCVOL1;


	DWORD ChannelsQuantity;
	DWORD ChannelPoint;

	bool FlagCondition;
	bool FlagPlot;

	WORD TTL_Canale;

	int n;

	int DacMax, DacStep,DacN; // макс знач напр, итерация, переменная с итерациями
	bool SynchroAdType, SynchroAdMode;

	// дескриптор устройства
	HANDLE ModuleHandle;
	// структура параметров работы АЦП модуля
	ADC_PARS_E154 ap;
	// структура с полной информацией о модуле
	MODULE_DESCRIPTION_E154 ModuleDescription;

	void StopModule(void);
	void StartModule(void);
	void StartADC(void);
	void StartVAH(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
