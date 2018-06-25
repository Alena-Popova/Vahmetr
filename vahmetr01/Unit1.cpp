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
SHORT AdcBuffer[0x4];// Для ВАХ (указатель на массив с кодами АЦП, который необходимо преобразовать)
double volts_array[16]; // Для ВАХ(указатель на массив, в который будет помещен результат преобразования)
bool flagStatusModule = true;
SHORT AdcBuffer1[0x4];  // для фототока и напряжения
double volts_array1[16]; // для фототока и напряжения
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
	FlagPlot =  false; // флаг для масштабирования графика
	FlagCondition = true;  // флаг переключения. в самом начале true ( то есть начинается с первой функции в условии в таймере)
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
	Form1->Memo1->Lines->Add("Измерение ВАХ, фототока и напряжения запущено");

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

	StopModule(); // cначала проверяем включен ли модуль, если включен,то выключаем
	StartModule();  // запускаем модуль

	Form1->Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
	Form1->Memo1->Lines->Add("Модуль остановлен");

	Application->ProcessMessages();

	StopModule();
}
//---------------------------------------------------------------------------
void TForm1::StartModule(void) {

	char ModuleName[7];
	WORD i;
	WORD Canale;

	ChannelsQuantity = 0x4;
	ChannelPoint = 1024; // кол-во точек на канал


	Form1->Timer1->Enabled = false;

	Buffer = new short[ChannelPoint * ChannelsQuantity];
	if (Buffer) {
		memset(Buffer, 0x0, sizeof(SHORT) * ChannelPoint * ChannelsQuantity);
	} else {
		Form1->Memo1->Lines->Add(L"Не могу выделить память под буфер данных!");
	}
	Application->ProcessMessages();

	// проверим версию используемой DLL библиотеки
	if (GetDllVersion() != CURRENT_VERSION_LUSBAPI) {
		String ErrorMessage = "Неправильная версия библиотеки Lusbapi.dll!\n";
		ErrorMessage += "Текущая: " + IntToStr
		((__int64)GetDllVersion() >> 0x10) + "." + IntToStr
		((__int64)GetDllVersion() & 0xFFFF) + "      ";
		ErrorMessage += "Требуется: " + IntToStr
		(CURRENT_VERSION_LUSBAPI >> 0x10) + "." + IntToStr
		(CURRENT_VERSION_LUSBAPI & 0xFFFF);

		Form1->Memo1->Lines->Add(ErrorMessage.c_str());
		flagStatusModule = false;
		return;
	}

	// попытаемся получить указатель на интерфейс для модуля E-154
	pModule = static_cast<ILE154*>(CreateLInstance("e154"));
	if (!pModule) {
		Form1->Memo1->Lines->Add(L"Не могу получить интерфейс на модуль E-154!");
		flagStatusModule = false;
		return;
	}

	// попробуем обнаружить модуль E-154 в первых MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI виртуальных слотах
	for (i = 0x0; i < MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI; i++)
		if (pModule->OpenLDevice(i))
			break;
	if (i == MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI) {
		Form1->Memo1->Lines->Add(L"Не могу обнаружить модуль 'E-154' в первых 127 виртуальных слотах!");
		flagStatusModule = false;
		return;
	}
	// попробуем прочитать дескриптор устройства
	ModuleHandle = pModule->GetModuleHandle();
	if (ModuleHandle == INVALID_HANDLE_VALUE) {
		Form1->Memo1->Lines->Add(L"Не могу получить дескриптор устройства!");
		flagStatusModule = false;
		return;
	}
	// прочитаем название модуля в нулевом виртуальном слоте
	else if (!pModule->GetModuleName(ModuleName)) {
		Form1->Memo1->Lines->Add(
			L"Не могу прочитать название модуля в нулевом виртуальном слоте!");
			flagStatusModule = false;
			return;
	}
	// проверим, что это 'E-154'
	else if (StrComp(ModuleName, "E154")) {
		Form1->Memo1->Lines->Add(
			L"В нулевом виртуальном слоте не модуль 'E-154'! ОШИБКА!!!");
			flagStatusModule = false;
			return;
	}
	// прочитаем ППЗУ модуля
	else if (!pModule->GET_MODULE_DESCRIPTION(&ModuleDescription)) {
		Form1->Memo1->Lines->Add(L"Не могу прочитать ППЗУ в модуле E-154!");
		flagStatusModule = false;
		return;
	}
	// в ППЗУ прописано название модуля?
	else if (strcmp(ModuleDescription.Module.DeviceName, "E-154")) {
		Form1->Memo1->Lines->Add(L"В ППЗУ не прописано название модуля!");
		flagStatusModule = false;
		return;
	}
	pModule->STOP_ADC();
	// прочитаем текущие параметры работы АЦП
	if (!pModule->GET_ADC_PARS(&ap)) {
		Form1->Memo1->Lines->Add(L"Не могу выполнить функцию GET_ADC_PARS()!");
		flagStatusModule = false;
		return;
	}

	ap.ClkSource = INT_ADC_CLOCK_E154; // внутренний запуск АЦП
	ap.EnableClkOutput = ADC_CLOCK_TRANS_DISABLED_E154;
	// без трансляции тактовых импульсо АЦП
	ap.InputMode = NO_SYNC_E154; // режим ввода даных с АЦП
	ap.ChannelsQuantity = ChannelsQuantity; // четыре активных канала
	// формируем управляющую таблицу логических каналов
	for (Canale = 0x0; Canale < ap.ChannelsQuantity; Canale++) {
		ap.ControlTable[Canale] = (WORD)
			(Canale |
			(ADC_INPUT_RANGE_5000mV_E154 << 0x6));
	}
	ap.AdcRate = 100.0; // частота работы АЦП в кГц
	pModule->SET_ADC_PARS(&ap); // передача в модуль параметров работы АЦП
}
void TForm1::StopModule(void) {

	Form1->Timer1->Enabled = false;

	if (pModule) {
		pModule->ReleaseLInstance();
		pModule = NULL;
	}
	// освободим память
	if (Buffer) {
		delete[]Buffer;
		Buffer = NULL;
	}

	return;
}

// ---------------------------------------------------------------------------
// Функция снятия фототока и напряжения
void TForm1::StartADC(void) {

	SHORT DacSample1;

	pModule->TTL_OUT(WORD(0x00)); // задействован 1 выход на переключателе :посылаем 00 на ( D01 и D02)
	Sleep(250);

	DacSample1 = (SHORT)(0);
	pModule->DAC_SAMPLE(&DacSample1, WORD(0x0)); //   земля в ноль

	if (pModule->SET_ADC_PARS(&ap)) {  // если параматры установлены
		if (!pModule->DAC_SAMPLE(&DacSample1, WORD(0x0))) {   //  если земляне установлена
			Form1->Memo1->Lines->Add("0 не установлен");
		}
		else {
			ADCVOL = 0;
			ADCCUR = 0;
			if (!pModule->ADC_KADR(AdcBuffer1)) { // ввод кадра с ацп модуля
				Form1->Memo1->Lines->Add("ADC_KADR не выполнено");
			}
			else {
				pModule->ProcessArray(AdcBuffer1, volts_array1, ChannelsQuantity,
					TRUE, TRUE);// преобразование в вольты
				ADCCUR = volts_array1[2];  // это ток
				ADCVOL = volts_array1[3];  // если напряжение
				ADCcurrent = -1 * 90.24 * ADCCUR;
				ADCVOL1 = 90.24 * ADCVOL;

				Sleep(10);
				Label1->Caption = FloatToStrF(ADCcurrent, ffFixed, 4, 2);
				Label2->Caption = FloatToStrF(ADCVOL1, ffFixed, 4, 2);

			}

		}
	}
	else {
		Form1->Memo1->Lines->Add("SET_ADC_PARS(&ap) не запущен");
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
	// задержечка
	Sleep(30);

	pModule->ENABLE_TTL_OUT(true);
	pModule->TTL_OUT(WORD(0x03)); // два управляющих сигнала в единицы
	Sleep(100);
	//посылаем 11 на ( D01 и D02)
	// эти сигналы отвечают за то,что ключ переключается на снятие данных с разных плат.

	if (pModule->SET_ADC_PARS(&ap)) { //если параметры установлены
	DacSample = (SHORT)(0);
	pModule->DAC_SAMPLE(&DacSample, WORD(0x0));

	if (!pModule->DAC_SAMPLE(&DacSample, WORD(0x0))) {//если земля в  не нуле
		Timer1->Enabled = false;
		Form1->Memo1->Lines->Add("Таймер 1 не запущен");
	}
	//если земля в   нуле
	Chart1->Series[0]->Clear();

  if (Form1->CheckBox1->Checked == true) {
  DacN = 0;
  } else{
  DacN = -DacMax;
  }
  // тут код для рассчета параметров вах

  while (DacN <= DacMax) {
	VA = 0;
	IA = 0;
	DacSample0 = (SHORT)(DacN);
	pModule->DAC_SAMPLE(&DacSample0, WORD(0x0));//каждый раз землю в ноль для уверенности
	if (!pModule->ADC_KADR(AdcBuffer)) {
		Form1->Memo1->Lines->Add("ADC_KADR не выполнено");
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
		Form1->Memo1->Lines->Add("Таймер 1 не запущен");
	}
}



void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	if (flagStatusModule == true) {


	StartVAH(); // запускаем однократное снятие вах

	StartADC();// запускаем однократное снятие тока и напряжения

	Label3->Caption = TimeToStr(Now());
	Application->ProcessMessages();
	Sleep(30);
	} else {
		Timer1->Enabled = false;
		Form1->Memo1->Lines->Add("Таймер 1 не запущен: error flagStatusModule");
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


