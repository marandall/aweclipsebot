#pragma once

class ColourButton;
class ComboBox;
class OutputFormatCtrl;

void DEX_Exchange(bool FromRight, CString& LHS,		CString& RHS);
void DEX_Exchange(bool FromRight, int& LHS,			CString& RHS);
void DEX_Exchange(bool FromRight, float& LHS,		CString& RHS);
void DEX_Exchange(bool FromRight, bool& LHS,		CString& RHS);

// data exchange, 

void DEX_Exchange(bool FromRight, int& LHS,			int& RHS);
void DEX_Exchange(bool FromRight, float& LHS,		float& RHS);
void DEX_Exchange(bool FromRight, int& LHS,			CComboBox& RHS);
void DEX_Exchange(bool FromRight, COLORREF& LHS,	ColourButton& RHS);
void DEX_Exchange(bool FromRight, bool& LHS,		BOOL& RHS);