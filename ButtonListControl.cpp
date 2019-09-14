// ButtonListControl.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "ButtonListControl.h"


// ButtonListControl dialog

IMPLEMENT_DYNAMIC(ButtonListControl, CDialog)

ButtonListControl::ButtonListControl(CWnd* pParent /*=NULL*/)
	: CDialog(ButtonListControl::IDD, pParent)
{
	last_ctrl_id = 4000;

	header_font.CreateFontA(
		  16,                        // nHeight
		   0,                         // nWidth
		   0,                         // nEscapement
		   0,                         // nOrientation
		   FW_BOLD,                   // nWeight
		   TRUE,                     // bItalic
		   FALSE,                     // bUnderline
		   0,                         // cStrikeOut
		   ANSI_CHARSET,              // nCharSet
		   OUT_DEFAULT_PRECIS,        // nOutPrecision
		   CLIP_DEFAULT_PRECIS,       // nClipPrecision
		   DEFAULT_QUALITY,           // nQuality
		   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		   "Arial");                  // lpszFacename

	scroll_pos = 0;

}

ButtonListControl::~ButtonListControl()
{
	categories.clear();
}

ButtonListControl::Category::~Category()
{
	Category& C = *this;
	for (Category::ButtonList::iterator ptr = C.buttons.begin();
		ptr != C.buttons.end();
		ptr++)
	{
		ptr->button->DestroyWindow();
		delete ptr->button;
	}

	// delete
	buttons.clear();
}

void ButtonListControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ButtonListControl, CDialog)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// ButtonListControl message handlers

void ButtonListControl::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	resize();
}

void ButtonListControl::onButton(ButtonListButtonControl* B)
{
	for (CatList::iterator itr = categories.begin(); itr != categories.end(); itr++)
	{
		// button expansion
		if (itr->group_button == B)
		{
			if (itr->expanded)
			{
				collapseCategory(*itr);
			}
			else
			{
				expandCategory(*itr);
			}
		}

		// if it is visible
		for (Category::ButtonList::iterator ptr = itr->buttons.begin();
			ptr != itr->buttons.end();
			ptr++)
		{
			if (ptr->button == B)
				broadcastToListeners(LE_CLICKED, &ClickNotificationMessage(ptr->id));
		}
	}
}

void ButtonListControl::expandCategory(Category& C)
{
	// go through the buttons
	C.expanded = true;
	C.group_button->SetCheck(TRUE);
	for (Category::ButtonList::iterator ptr = C.buttons.begin();
		ptr != C.buttons.end();
		ptr++)
	{
		ptr->button->ShowWindow(SW_SHOW);
	}


	// size
	resize();
}

void ButtonListControl::collapseCategory(Category& C)
{
	// go through the buttons
	C.expanded = false;
	C.group_button->SetCheck(FALSE);
	for (Category::ButtonList::iterator ptr = C.buttons.begin();
		ptr != C.buttons.end();
		ptr++)
	{
		ptr->button->ShowWindow(SW_HIDE);
	}

	// size
	resize();
}

void ButtonListControl::resize()
{
	RECT r;
	GetWindowRect(&r);
	int cx = r.right - r.left;

	int offset			= -scroll_pos;
	int top				= -scroll_pos;
	int header_height	= 30;
	int y_seperator		= 3;
	int width			= cx - GetSystemMetrics(SM_CXVSCROLL) - y_seperator;
	int button_height	= 20;

	// resize everything
	for (CatList::iterator itr = categories.begin(); itr != categories.end(); itr++)
	{
		itr->group_button->MoveWindow(0, top, width, header_height, FALSE);
		top+= header_height + y_seperator;

		// if it is visible
		if (itr->expanded)
		{
			for (Category::ButtonList::iterator ptr = itr->buttons.begin();
				ptr != itr->buttons.end();
				ptr++)
			{
				ptr->button->MoveWindow(0, top, width, button_height, FALSE);
				top+= button_height + y_seperator;
			}
		}
	}

	Invalidate(TRUE);

	// pixel size
	max_height = top + scroll_pos;
	int cy = r.bottom - r.top;

	// control bar
	SCROLLINFO si;
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin  = 0;
	si.nMax  = max_height;
	if (max_height < cy)
		si.nPage = max_height;
	else
		si.nPage = r.bottom - r.top;

	// manager
	SetScrollInfo(SB_VERT, &si);
}

BOOL ButtonListControl::OnInitDialog()
{
	__super::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void ButtonListControl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (nSBCode != 8)
	{
		SCROLLINFO si;
		si.nPos  = nPos;
		si.fMask = SIF_POS;

		SetScrollInfo(SB_VERT, &si);
		scroll_pos = si.nPos;
		resize();
	}

	// scroll

	__super::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL ButtonListControl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	return __super::OnMouseWheel(nFlags, zDelta, pt);
}
