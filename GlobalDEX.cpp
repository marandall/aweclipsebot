#include "stdafx.h"
#include "ComboBox.h"
#include "TextBox.h"
#include "ColourButton.h"
#include "OutputFormatCtrl.h"
#include "GlobalDEX.h"
#include "awsdk_colours.h"

// integer to types
void DataExchanger::exchange(int& Type, CString& Ctrl)
{
	if (exchange_mode == FROM_CONTROL)
		Type = atoi(Ctrl);
	else
		Ctrl.Format(_T("%d"), Type);
}

void DataExchanger::exchange(int& Type, bool& Ctrl)
{
	if (exchange_mode == FROM_CONTROL)
		Type = Ctrl ? TRUE : FALSE;
	else
		Ctrl = (Type ? true : false);
}

void DataExchanger::exchange(int& Type, TextBox& Ctrl)
{
	if (exchange_mode == FROM_CONTROL)
		Ctrl.OverloadedGet(Type);
	else
		Ctrl.OverloadedSet(Type);
}

void DataExchanger::exchange(int& Type, ComboBox& Ctrl)
{
	if (exchange_mode == FROM_CONTROL)
		Type = static_cast<int>(Ctrl.GetItemData(Ctrl.GetCurSel()));
	else
		Ctrl.SelectByItemData(Type);
}

void DataExchanger::exchange(int& Type, ColourButton& Ctrl)
{
	if (exchange_mode == FROM_CONTROL)
		Type = Ctrl.getColour();
	else
		Ctrl = static_cast<COLORREF>(Type);
}

void DataExchanger::exchange(int& Type, CButton& Ctrl)
{
	if (exchange_mode == FROM_CONTROL)
		Type = Ctrl.GetCheck();
	else
		Ctrl.SetCheck(Type);
}


// floats to types
void DataExchanger::exchange(float& Type, CString& Ctrl)
{
	if (exchange_mode == FROM_CONTROL)
		Type = static_cast<float>(atof(Ctrl));
	else
		Ctrl.Format(_T("%.4f"), Type);
}

void DataExchanger::exchange(float& Type, TextBox& Ctrl)
{
	if (exchange_mode == FROM_CONTROL)
		Ctrl.OverloadedGet(Type);
	else
		Ctrl.OverloadedSet(Type);
}


// boolean to types
void DataExchanger::exchange(bool& Type, CString& Ctrl)
{
	if (exchange_mode == FROM_CONTROL)
		Type = atoi(Ctrl) ? true : false;
	else
		Ctrl.Format(_T("%d"), Type ? 1 : 0);
}

void DataExchanger::exchange(bool& Type, bool& Ctrl)
{
	if (exchange_mode == FROM_CONTROL)
		Type = Ctrl;
	else
		Ctrl = Type;
}

void DataExchanger::exchange(bool& Type, CButton& Ctrl)
{
	if (exchange_mode == FROM_CONTROL)
		Type = Ctrl.GetCheck() == TRUE;
	else
		Ctrl.SetCheck(Type ? TRUE : FALSE);
}

void DataExchanger::exchange(CString& Type, TextBox& Ctrl)
{
	if (exchange_mode == FROM_CONTROL)
		Ctrl.OverloadedGet(Type);
	else
		Ctrl.OverloadedSet(Type);
}

// colour to types
void DataExchanger::exchange(AW::Colour& Type, ColourButton& Ctrl)
{
	if (exchange_mode == FROM_CONTROL)
		Type.value = Ctrl.getColour();
	else
		Ctrl = static_cast<COLORREF>(Type.value);
}

// messageinfo to types
void DataExchanger::exchange(MessageInfo& Type, OutputFormatCtrl& Ctrl)
{
	if (exchange_mode == FROM_CONTROL)
		Type = Ctrl.GetInfo();
	else
		Ctrl.SetInfo(Type);
}
