//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
int n;
ILE154 *pModule;
SHORT AdcBuffer[0x4];// ��� ��� (��������� �� ������ � ������ ���, ������� ���������� �������������)
double volts_array[16]; // ��� ���(��������� �� ������, � ������� ����� ������� ��������� ��������������)
bool flagStatusModule = true;
SHORT AdcBuffer1[0x4];  // ��� �������� � ����������
double volts_array1[16]; // ��� �������� � ����������
// -----------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	Top = 20;
	Left = 20;
	Form1->Label1->Caption = "";
	Form1->Label2->Caption = "";
	Form1->Label3->Caption = "";
	FlagPlot =  false; // ���� ��� ��������������� �������
	FlagCondition = true;  // ���� ������������. � ����� ������ true ( �� ���� ���������� � ������ ������� � ������� � �������)
	Form1->Timer1->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormDestroy(TObject *Sender)
{
	  StopModule();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	Form1->Memo1->Lines->Add("��������� ���, �������� � ���������� ��������");

	if (!FlagPlot) {

	DacMax = 120;
	DacStep = 10;
	Chart1->Axes->Bottom->Minimum = -3;
	Chart1->Axes->Bottom->Maximum = 3;
	Chart1->Axes->Left->Minimum = -10;
	Chart1->Axes->Left->Maximum = 18;
	Chart1->Axes->Bottom->Increment = 0.2;
	Chart1->Axes->Left->Increment = 1;
	}

	Application->ProcessMessages();
	Sleep(30);

	StopModule(); // c������ ��������� ������� �� ������, ���� �������,�� ���������
	StartModule();  // ��������� ������

	Form1->Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
	Form1->Memo1->Lines->Add("������ ����������");

	Application->ProcessMessages();

	StopModule();
}
//---------------------------------------------------------------------------
void TForm1::StartModule(void) {

	char ModuleName[7];
	WORD i;
	WORD Canale;

	ChannelsQuantity = 0x4;
	ChannelPoint = 1024; // ���-�� ����� �� �����


	Form1->Timer1->Enabled = false;

	Buffer = new short[ChannelPoint * ChannelsQuantity];
	if (Buffer) {
		memset(Buffer, 0x0, sizeof(SHORT) * ChannelPoint * ChannelsQuantity);
	} else {
		Form1->Memo1->Lines->Add(L"�� ���� �������� ������ ��� ����� ������!");
	}
	Application->ProcessMessages();

	// �������� ������ ������������ DLL ����������
	if (GetDllVersion() != CURRENT_VERSION_LUSBAPI) {
		String ErrorMessage = "������������ ������ ���������� Lusbapi.dll!\n";
		ErrorMessage += "�������: " + IntToStr
		((__int64)GetDllVersion() >> 0x10) + "." + IntToStr
		((__int64)GetDllVersion() & 0xFFFF) + "      ";
		ErrorMessage += "���������: " + IntToStr
		(CURRENT_VERSION_LUSBAPI >> 0x10) + "." + IntToStr
		(CURRENT_VERSION_LUSBAPI & 0xFFFF);

		Form1->Memo1->Lines->Add(ErrorMessage.c_str());
		flagStatusModule = false;
		return;
	}

	// ���������� �������� ��������� �� ��������� ��� ������ E-154
	pModule = static_cast<ILE154*>(CreateLInstance("e154"));
	if (!pModule) {
		Form1->Memo1->Lines->Add(L"�� ���� �������� ��������� �� ������ E-154!");
		flagStatusModule = false;
		return;
	}

	// ��������� ���������� ������ E-154 � ������ MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI ����������� ������
	for (i = 0x0; i < MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI; i++)
		if (pModule->OpenLDevice(i))
			break;
	if (i == MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI) {
		Form1->Memo1->Lines->Add(L"�� ���� ���������� ������ 'E-154' � ������ 127 ����������� ������!");
		flagStatusModule = false;
		return;
	}
	// ��������� ��������� ���������� ����������
	ModuleHandle = pModule->GetModuleHandle();
	if (ModuleHandle == INVALID_HANDLE_VALUE) {
		Form1->Memo1->Lines->Add(L"�� ���� �������� ���������� ����������!");
		flagStatusModule = false;
		return;
	}
	// ��������� �������� ������ � ������� ����������� �����
	else if (!pModule->GetModuleName(ModuleName)) {
		Form1->Memo1->Lines->Add(
			L"�� ���� ��������� �������� ������ � ������� ����������� �����!");
			flagStatusModule = false;
			return;
	}
	// ��������, ��� ��� 'E-154'
	else if (StrComp(ModuleName, "E154")) {
		Form1->Memo1->Lines->Add(
			L"� ������� ����������� ����� �� ������ 'E-154'! ������!!!");
			flagStatusModule = false;
			return;
	}
	// ��������� ���� ������
	else if (!pModule->GET_MODULE_DESCRIPTION(&ModuleDescription)) {
		Form1->Memo1->Lines->Add(L"�� ���� ��������� ���� � ������ E-154!");
		flagStatusModule = false;
		return;
	}
	// � ���� ��������� �������� ������?
	else if (strcmp(ModuleDescription.Module.DeviceName, "E-154")) {
		Form1->Memo1->Lines->Add(L"� ���� �� ��������� �������� ������!");
		flagStatusModule = false;
		return;
	}
	pModule->STOP_ADC();
	// ��������� ������� ��������� ������ ���
	if (!pModule->GET_ADC_PARS(&ap)) {
		Form1->Memo1->Lines->Add(L"�� ���� ��������� ������� GET_ADC_PARS()!");
		flagStatusModule = false;
		return;
	}

	ap.ClkSource = INT_ADC_CLOCK_E154; // ���������� ������ ���
	ap.EnableClkOutput = ADC_CLOCK_TRANS_DISABLED_E154;
	// ��� ���������� �������� �������� ���
	ap.InputMode = NO_SYNC_E154; // ����� ����� ����� � ���
	ap.ChannelsQuantity = ChannelsQuantity; // ������ �������� ������
	// ��������� ����������� ������� ���������� �������
	for (Canale = 0x0; Canale < ap.ChannelsQuantity; Canale++) {
		ap.ControlTable[Canale] = (WORD)
			(Canale |
			(ADC_INPUT_RANGE_5000mV_E154 << 0x6));
	}
	ap.AdcRate = 100.0; // ������� ������ ��� � ���
	pModule->SET_ADC_PARS(&ap); // �������� � ������ ���������� ������ ���
}
void TForm1::StopModule(void) {

	Form1->Timer1->Enabled = false;

	if (pModule) {
		pModule->ReleaseLInstance();
		pModule = NULL;
	}
	// ��������� ������
	if (Buffer) {
		delete[]Buffer;
		Buffer = NULL;
	}

	return;
}

// ---------------------------------------------------------------------------
// ������� ������ �������� � ����������
void TForm1::StartADC(void) {

	SHORT DacSample1;

	pModule->TTL_OUT(WORD(0x00)); // ������������ 1 ����� �� ������������� :�������� 00 �� ( D01 � D02)
	Sleep(250);

	DacSample1 = (SHORT)(0);
	pModule->DAC_SAMPLE(&DacSample1, WORD(0x0)); //   ����� � ����

	if (pModule->SET_ADC_PARS(&ap)) {  // ���� ��������� �����������
		if (!pModule->DAC_SAMPLE(&DacSample1, WORD(0x0))) {   //  ���� ������� �����������
			Form1->Memo1->Lines->Add("0 �� ����������");
		}
		else {
			ADCVOL = 0;
			ADCCUR = 0;
			if (!pModule->ADC_KADR(AdcBuffer1)) { // ���� ����� � ��� ������
				Form1->Memo1->Lines->Add("ADC_KADR �� ���������");
			}
			else {
				pModule->ProcessArray(AdcBuffer1, volts_array1, ChannelsQuantity,
					TRUE, TRUE);// �������������� � ������
				ADCCUR = volts_array1[2];  // ��� ���
				ADCVOL = volts_array1[3];  // ���� ����������
				ADCcurrent = -1 * 90.24 * ADCCUR;
				ADCVOL1 = 90.24 * ADCVOL;

				Sleep(10);
				Label1->Caption = FloatToStrF(ADCcurrent, ffFixed, 4, 2);
				Label2->Caption = FloatToStrF(ADCVOL1, ffFixed, 4, 2);

			}

		}
	}
	else {
		Form1->Memo1->Lines->Add("SET_ADC_PARS(&ap) �� �������");
	}
}

// ---------------------------------------------------------------------------

void TForm1::StartVAH(void) {

	double VA, IA;
	SHORT  DacSample0;
	double V2P, V2M;
	SHORT DacSample;

	AnsiString WayFile1;
	AnsiString NameFile1;
	AnsiString DattaFile11;
	AnsiString DattaFile21;

	Application->ProcessMessages();
	// ����������
	Sleep(30);

	pModule->ENABLE_TTL_OUT(true);
	pModule->TTL_OUT(WORD(0x03)); // ��� ����������� ������� � �������
	Sleep(100);
	//�������� 11 �� ( D01 � D02)
	// ��� ������� �������� �� ��,��� ���� ������������� �� ������ ������ � ������ ����.

	if (pModule->SET_ADC_PARS(&ap)) { //���� ��������� �����������
	DacSample = (SHORT)(0);
	pModule->DAC_SAMPLE(&DacSample, WORD(0x0));

	if (!pModule->DAC_SAMPLE(&DacSample, WORD(0x0))) {//���� ����� �  �� ����
		Timer1->Enabled = false;
		Form1->Memo1->Lines->Add("������ 1 �� �������");
	}
	//���� ����� �   ����
	Chart1->Series[0]->Clear();

  if (Form1->CheckBox1->Checked == true) {
  DacN = 0;
  } else{
  DacN = -DacMax;
  }
  // ��� ��� ��� �������� ���������� ���

  while (DacN <= DacMax) {
	VA = 0;
	IA = 0;
	DacSample0 = (SHORT)(DacN);
	pModule->DAC_SAMPLE(&DacSample0, WORD(0x0));//������ ��� ����� � ���� ��� �����������
	if (!pModule->ADC_KADR(AdcBuffer)) {
		Form1->Memo1->Lines->Add("ADC_KADR �� ���������");
	}
	else {

		pModule->ProcessArray(AdcBuffer, volts_array, ChannelsQuantity,TRUE, TRUE);
		VA = volts_array[0];
		IA = volts_array[1];
		current = -1 * 1000 * VA / 190;
		DacN = DacN + DacStep;
		Sleep(10);
		Chart1->Series[0]->AddXY(IA, current, "", clRed);
	}
	}
	Chart1->Repaint();
	}
	else {
		Form1->Memo1->Lines->Add("������ 1 �� �������");
	}
}



void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	if (flagStatusModule == true) {


	StartVAH(); // ��������� ����������� ������ ���

	StartADC();// ��������� ����������� ������ ���� � ����������

	Label3->Caption = TimeToStr(Now());
	Application->ProcessMessages();
	Sleep(30);
	} else {
		Timer1->Enabled = false;
		Form1->Memo1->Lines->Add("������ 1 �� �������: error flagStatusModule");
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
		FlagPlot = true;
		DacMax = 8;
		DacStep = 2;
		Chart1->Axes->Bottom->Minimum = -0.25;
		Chart1->Axes->Bottom->Maximum = 0.25;
		Chart1->Axes->Left->Minimum = -3;
		Chart1->Axes->Left->Maximum = 3;
		Chart1->Axes->Bottom->Increment = 0.05;
		Chart1->Axes->Left->Increment = 0.5;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
		FlagPlot = true;
		DacMax = 24;
		DacStep = 4;
		Chart1->Axes->Bottom->Minimum = -1;
		Chart1->Axes->Bottom->Maximum = 1;
		Chart1->Axes->Left->Minimum = -1;
		Chart1->Axes->Left->Maximum = 4;
		Chart1->Axes->Bottom->Increment = 0.25;
		Chart1->Axes->Left->Increment = 1;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
		FlagPlot = true;
		DacMax = 45;
		DacStep = 5;
		Chart1->Axes->Bottom->Minimum = -1.25;
		Chart1->Axes->Bottom->Maximum = 1.25;
		Chart1->Axes->Left->Minimum = -12;
		Chart1->Axes->Left->Maximum = 12;
		Chart1->Axes->Bottom->Increment = 0.05;
		Chart1->Axes->Left->Increment = 2;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender)
{
		FlagPlot = true;
		DacMax = 120;
		DacStep = 10;
		Chart1->Axes->Bottom->Minimum = -3;
		Chart1->Axes->Bottom->Maximum = 3;
		Chart1->Axes->Left->Minimum = -10;
		Chart1->Axes->Left->Maximum = 18;
		Chart1->Axes->Bottom->Increment = 0.2;
		Chart1->Axes->Left->Increment = 1;
}
//---------------------------------------------------------------------------


