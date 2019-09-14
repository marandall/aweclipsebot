#include "StdAfx.h"
#include "editextended.h"

#ifdef DEBUG
	#define new DEBUG_NEW
#endif


EditExtended::EditExtended(void)
{
	m_font_colour	= 0x00;
	m_br_back		= NULL;

}

EditExtended::~EditExtended(void)
{
	if (m_br_back)
		delete m_br_back;
}

BEGIN_MESSAGE_MAP(EditExtended, CEdit)
	ON_WM_GETDLGCODE()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

UINT EditExtended::OnGetDlgCode()
{
	return CEdit::OnGetDlgCode() | DLGC_WANTALLKEYS;
}

#include "InputHistory.h"
ConsoleInputHistory history;
int history_index = 0;


void 
EditExtended::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RETURN)
	{		
		// add to the buffer
		CString s;
		GetWindowText(s);
		history.add(s);
		history_index = (int)history.size();
	
		// send message back up
		GetParent()->SendMessage(UWM_CTRLKEY_RETURN, 0, 0);
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void 
EditExtended::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);

	if (nChar == VK_UP)
	{
		history_index--;
		if (history_index >= 0 && history_index < (int)history.size())
		{
			CStringA newText = history[history_index];
			SetWindowText(newText);
			SetSel(newText.GetLength(), newText.GetLength());
		}
		else
		{
			history_index = 0;
		}
	}
	else if (nChar == VK_DOWN)
	{
		history_index++;
		if (history_index >= 0 && history_index < (int)history.size())
		{
			CStringA newText = history[history_index];
			SetWindowText(newText);
			SetSel(newText.GetLength(), newText.GetLength());
		}
		else
		{
			history_index = (int)history.size();
		}
	}
}

void
EditExtended::setCourier()
{
	courier.CreatePointFont(100, "Courier New");

	SetFont(&courier, TRUE);
}

void EditExtended::setEditColours(COLORREF Font, COLORREF Back)
{
	m_font_colour	= Font;
	m_bg_colour		= Back;
	if (m_br_back)
		delete m_br_back;
	m_br_back = new CBrush(Back);
	Invalidate();
}

HBRUSH EditExtended::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	pDC->SetBkColor(m_bg_colour);

	if (m_br_back)	return (HBRUSH)m_br_back->m_hObject;
	else			return NULL;
}

