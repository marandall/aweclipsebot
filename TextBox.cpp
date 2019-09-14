// TextBox.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "TextBox.h"


// TextBox

IMPLEMENT_DYNAMIC(TextBox, CEdit)

TextBox::TextBox()
{

}

TextBox::~TextBox()
{
}


BEGIN_MESSAGE_MAP(TextBox, CEdit)
	ON_CONTROL_REFLECT(EN_SETFOCUS, &TextBox::OnEnSetfocus)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, &TextBox::OnEnKillfocus)
END_MESSAGE_MAP()



// TextBox message handlers
CStringA TextBox::GetText()
{
	CString str;
	GetWindowText(str);
	return str;
}

TextBox& TextBox::operator=(const char* Text)
{
	HWND hw = m_hWnd;
	SetWindowTextA(Text);
	return *this;
}

void TextBox::OverloadedSet(CStringA Value)
{
	HWND hw = m_hWnd;
	SetWindowTextA(Value);
	format_type = TBF_STRING;
}

void TextBox::OverloadedSet(float Value)
{
	CString s;
	s.Format(_T("%.3f"), Value);
	(*this) = s;
	format_type = TBF_FLOAT;
}

void TextBox::OverloadedSet(int Value)
{
	CString s;
	s.Format(_T("%d"), Value);
	(*this) = s;
	format_type = TBF_INT;
}



void TextBox::OverloadedGet(CStringA& Value)
{
	Value = GetText();
}

void TextBox::OverloadedGet(float& Value)
{
	Value = (float)atof(GetText());
}

void TextBox::OverloadedGet(int& Value)
{
	Value = (int)atoi(GetText());
}

void TextBox::OnEnSetfocus()
{
	// TODO: Add your control notification handler code here
}

void TextBox::OnEnKillfocus()
{
	CStringA text = GetText();
	if (format_type == TBF_FLOAT)
	{
		OverloadedSet((float)atof(text));
	}
	else if (format_type == TBF_INT)
	{
		OverloadedSet((int)atoi(text));
	}

}
