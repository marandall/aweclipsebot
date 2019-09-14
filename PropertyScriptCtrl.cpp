// PropertyScriptCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "PropertyScriptCtrl.h"


// PropertyScriptCtrl dialog

IMPLEMENT_DYNAMIC(PropertyScriptCtrl, CDialog)

PropertyScriptCtrl::PropertyScriptCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(PropertyScriptCtrl::IDD, pParent)
{
	top					= 30;
	ELEMENT_SEPERATOR	= 3;
	ELEMENT_INSET		= 0;
	CONTROL_INSET		= 160;
}

PropertyScriptCtrl::~PropertyScriptCtrl()
{
}

void PropertyScriptCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABS, c_Tabs);
	DDX_Control(pDX, IDC_TITLE, c_TitleText);
}


BEGIN_MESSAGE_MAP(PropertyScriptCtrl, CDialog)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABS, &PropertyScriptCtrl::OnTcnSelchangeTabs)
END_MESSAGE_MAP()


// PropertyScriptCtrl message handlers

BOOL PropertyScriptCtrl::OnInitDialog()
{
	CDialog::OnInitDialog();

	// setup font
	m_font.CreatePointFont(80, _T("Arial"), NULL); 
	m_title_font.CreateFont(
			24,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_BOLD,									 // nWeight
			TRUE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			_T("Arial"));                 // lpszFacename

	m_section_font.CreateFont(
			16,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_BOLD,									 // nWeight
			TRUE,                     // bItalic
			TRUE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			_T("Arial"));                 // lpszFacename


	// create
	c_TitleText.SetFont(&m_title_font);

	// save the original size
	GetWindowRect(m_rcOriginalRect);

	// initial scroll position
	m_nScrollPos = 0; 

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

int last_ctrl_id = 10;

void PropertyScriptCtrl::CreateAtPosition(CWnd& Parent, CWnd& Wnd)
{
	Create(PropertyScriptCtrl::IDD, &Parent);
	ShowWindow(SW_SHOW);

	// get the size and resize to fit it
	CRect rt;
	Wnd.GetWindowRect(&rt);
	Parent.ScreenToClient(&rt);

	// move
	MoveWindow(&rt);
}

void PropertyScriptCtrl::create_group(int Group, CStringA Name)
{
	int id = c_Tabs.InsertItem(c_Tabs.GetItemCount(), Name);
	m_groups[Group].title = Name;
}

CStatic* PropertyScriptCtrl::create_label(int Group, CStringA Key, CStringA Text)
{
	// create the static control
	CStatic* pStatic = new CStatic();
	pStatic->Create(CString(Text), WS_CHILD, CRect(0, 0, 500, 30), this);
	pStatic->SetFont(&m_section_font);

	// setup group element
	GroupElement grE;
	grE.ctrl_id	= 0;
	grE.pLabel	= pStatic; 
	grE.text	= Text;
	grE.pWnd	= NULL;
	grE.height	= 20;
	grE.width	= -1;

	// shove it on the back
	m_groups[Group].push_back(grE);
	return pStatic;
}

TextBox* PropertyScriptCtrl::create_edit(int Group, CStringA Key, CStringA Text)
{
	// create the edit control
	TextBox* pEdit = new TextBox();
	pEdit->CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), NULL,
				WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_LEFT, 
				CRect(100, 0, 100, 16), this, last_ctrl_id);
	pEdit->SetFont(&m_font);

	// create the static control
	CStatic* pStatic = new CStatic();
	pStatic->Create(CString(Text + ":"), WS_CHILD, CRect(0, 0, 100, 16), this);
	pStatic->SetFont(&m_font);

	// setup group element
	GroupElement grE;
	grE.ctrl_id	= last_ctrl_id;
	grE.pLabel	= pStatic; 
	grE.text	= Text;
	grE.pWnd	= pEdit;
	grE.height	= 22;
	grE.width	= -1;
	
	// additional control id required
	last_ctrl_id++;

	// add it to the controls
	m_groups[Group].push_back(grE);
	return pEdit;
}

ComboBox* PropertyScriptCtrl::create_combo(int Group, CStringA Key, CStringA Text)
{
	// create the edit control
	ComboBox* pCombo = new ComboBox();
	pCombo->Create(CBS_DROPDOWNLIST | CBS_NOINTEGRALHEIGHT | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_LEFT, 
				CRect(100, 0, 100, 16), this, last_ctrl_id);
	pCombo->SetFont(&m_font);

	// create the static control
	CStatic* pStatic = new CStatic();
	pStatic->Create(CString(Text + ":"), WS_CHILD , CRect(0, 0, 100, 16), this);
	pStatic->SetFont(&m_font);

	// setup group element
	GroupElement grE;
	grE.ctrl_id	= last_ctrl_id;
	grE.pLabel	= pStatic; 
	grE.text	= Text;
	grE.pWnd	= pCombo;
	grE.height	= 25;
	grE.width	= 150;
	
	// additional control id required
	last_ctrl_id++;

	// add it to the controls
	m_groups[Group].push_back(grE);
	return pCombo;
}

CButton* PropertyScriptCtrl::create_check(int Group, CStringA Key, CStringA Text)
{
	// create the edit control
	CButton* pButton = new CButton();
	pButton->CreateEx(0, "BUTTON", NULL, BS_AUTOCHECKBOX | WS_CHILD, 
				CRect(100, 0, 100, 16), this, last_ctrl_id);
	pButton->SetFont(&m_font);

	// create the static control
	CStatic* pStatic = new CStatic();
	pStatic->Create(CString(Text + ":"), SS_LEFT | WS_CHILD, CRect(0, 0, 100, 16), this);
	pStatic->SetFont(&m_font);

	// setup group element
	GroupElement grE;
	grE.ctrl_id	= last_ctrl_id;
	grE.pLabel	= pStatic; 
	grE.text	= Text;
	grE.pWnd	= pButton;
	grE.height	= 22;
	grE.width	= 50;
	
	// additional control id required
	last_ctrl_id++;

	// add it to the controls
	m_groups[Group].push_back(grE);
	return pButton;
}

CButton* PropertyScriptCtrl::create_tristate(int Group, CStringA Key, CStringA Text)
{
	// create the edit control
	CButton* pButton = new CButton();
	pButton->CreateEx(0, "BUTTON", NULL, WS_CHILD | BS_AUTO3STATE, 
				CRect(100, 0, 100, 16), this, last_ctrl_id);
	pButton->SetFont(&m_font);

	// create the static control
	CStatic* pStatic = new CStatic();
	pStatic->Create(CString(Text + ":"), SS_LEFT | WS_CHILD, CRect(0, 0, 100, 16), this);
	pStatic->SetFont(&m_font);

	// setup group element
	GroupElement grE;
	grE.ctrl_id	= last_ctrl_id;
	grE.pLabel	= pStatic; 
	grE.text	= Text;
	grE.pWnd	= pButton;
	grE.height	= 22;
	grE.width	= 50;
	
	// additional control id required
	last_ctrl_id++;

	// add it to the controls
	m_groups[Group].push_back(grE);
	return pButton;
}

CWnd* PropertyScriptCtrl::create_special(int Group, CStringA Key, 
		CWnd* SpecialObject, int Height, int Width, CStringA Text)
{
	// create the static control
	CStatic* pStatic = new CStatic();
	pStatic->Create(CString(Text + ":"), SS_LEFT | WS_CHILD, CRect(0, 0, 100, 16), this);
	pStatic->SetFont(&m_font);

	// setup group element
	GroupElement grE;
	grE.ctrl_id	= last_ctrl_id;
	grE.pLabel	= pStatic; 
	grE.text	= Text;
	grE.pWnd	= SpecialObject;
	grE.height	= Height;
	grE.width	= Width;
	
	// additional control id required
	last_ctrl_id++;

	// add it to the controls
	m_groups[Group].push_back(grE);
	return SpecialObject;
}

ColourButton* PropertyScriptCtrl::create_colour(int Group, CStringA Key, CStringA Text)
{
	// create the edit control
	ColourButton* pButton = new ColourButton();
	pButton->CreateEx(0, "BUTTON", NULL, WS_CHILD, 
				CRect(100, 0, 100, 16), this, last_ctrl_id);
	pButton->SetFont(&m_font);

	// create the static control
	CStatic* pStatic = new CStatic();
	pStatic->Create(CString(Text + ":"), SS_LEFT | WS_CHILD, CRect(0, 0, 100, 16), this);
	pStatic->SetFont(&m_font);

	// setup group element
	GroupElement grE;
	grE.ctrl_id	= last_ctrl_id;
	grE.pLabel	= pStatic; 
	grE.text	= Text;
	grE.pWnd	= pButton;
	grE.height	= 22;
	grE.width	= 25;
	
	// additional control id required
	last_ctrl_id++;

	// add it to the controls
	m_groups[Group].push_back(grE);
	return pButton;
}

OutputFormatCtrl* PropertyScriptCtrl::create_formmater(int Group, CStringA Key, CStringA Text)
{
	// create the edit control
	OutputFormatCtrl* pButton = new OutputFormatCtrl(this);
	pButton->Create(OutputFormatCtrl::IDD, this);

	// create the static control
	CStatic* pStatic = new CStatic();
	pStatic->Create(CString(Text + ":"), SS_LEFT | WS_CHILD, CRect(0, 0, 100, 16), this);
	pStatic->SetFont(&m_font);

	// setup group element
	GroupElement grE;
	grE.ctrl_id	= last_ctrl_id;
	grE.pLabel	= pStatic; 
	grE.text	= Text;
	grE.pWnd	= pButton;
	grE.height	= 22;
	grE.width	= -1;
	
	// additional control id required
	last_ctrl_id++;

	// add it to the controls
	m_groups[Group].push_back(grE);
	return pButton;
}

int	PropertyScriptCtrl::getHeight()
{
	return 0;
}

int	PropertyScriptCtrl::getGroupHeight(int Group)
{
	int top					= 30;
	int ELEMENT_SEPERATOR	= this->ELEMENT_SEPERATOR;
	int ELEMENT_INSET		= this->ELEMENT_INSET;
	int CONTROL_INSET		= this->CONTROL_INSET;

	// look up the group elements box
	GroupElements& grEM = m_groups[Group];

	// each element
	for (GroupElements::iterator ePtr = grEM.begin(); ePtr != grEM.end(); ePtr++)
	{
		GroupElement& grE = *ePtr;

		if (grE.pWnd != NULL)
		{			
			// the actual width
			int height = grE.height;
		
			// check if its a combo
			CComboBox* pCombo = dynamic_cast<CComboBox*>(grE.pWnd);
			if (pCombo) height = 200;
		}
		else
		{
			top+= ELEMENT_SEPERATOR;
		}

		// top becomes the top, plus height, plus seperator
		top+= grE.height + ELEMENT_SEPERATOR;
	}

	return top;
}

BOOL PropertyScriptCtrl::show_group(int Group)
{
	// sanity check; does the group exist. If it does not return false.
	if (m_groups.find(Group) == m_groups.end())
		return FALSE;

	// reset scroll info
	SCROLLINFO si;
	si.cbSize	= sizeof(SCROLLINFO);
	si.nPos		= 0;
	si.nPage	= 0;
	SetScrollInfo(SB_VERT, &si, TRUE);

	current_group			= Group;
	int top					= 30;
	int ELEMENT_SEPERATOR	= this->ELEMENT_SEPERATOR;
	int ELEMENT_INSET		= this->ELEMENT_INSET;
	int CONTROL_INSET		= this->CONTROL_INSET;
	
	// do not redraw
	SetRedraw(FALSE);

	// set the origional rectangle (again)
	int gh = getGroupHeight(Group);
	m_rcOriginalRect.top	= 0;
	m_rcOriginalRect.bottom = gh;

	// set the vertical tabbing
	si.cbSize	= sizeof(SCROLLINFO);
	si.fMask	= SIF_ALL; 
	si.nMin		= 0;
	si.nMax		= gh;
	si.nPage	= 50;
	si.nPos		= 0;

	// window rectangle
	CRect rtx;
	GetWindowRect(rtx);	

	// is height at current attrition
	if (si.nMax < rtx.Height())
	{
		si.nMax = rtx.Height() + 5;
	}

	// assign scroll info
    SetScrollInfo(SB_VERT, &si, TRUE);

	c_TitleText.MoveWindow(0, 0, rtx.Width(), 30);

	// actual rect
	int oH = m_rcOriginalRect.bottom;
	int cH = rtx.Height();
	int os = GetSystemMetrics(SM_CXVSCROLL) + 5;

	// hide everything else
	for (PageGroups::iterator gPtr = m_groups.begin(); gPtr !=  m_groups.end(); gPtr++)
	{
		GroupElements& grEM = m_groups[gPtr->first];
		for (GroupElements::iterator ePtr = grEM.begin(); ePtr != grEM.end(); ePtr++)
		{
			ePtr->pLabel->ShowWindow(SW_HIDE);
			if (ePtr->pWnd)
				ePtr->pWnd->ShowWindow(SW_HIDE);
		}
	}

	// look up the group elements box
	GroupElements& grEM = m_groups[Group];

	// title text
	c_TitleText.SetWindowTextA(grEM.title);

	// get width
	RECT rt;
	GetWindowRect(&rt);
	int window_width = rt.right - rt.left - os;

	// for each control
	for (GroupElements::iterator ePtr = grEM.begin(); ePtr != grEM.end(); ePtr++)
	{
		GroupElement& grE = *ePtr;

		if (grE.pWnd != NULL)
		{
			// alignment for the label control
			const int LABEL_HEIGHT = 15;
			const int offset = static_cast<int>((grE.height - LABEL_HEIGHT) / 2);
			
			// the actual width
			int width = grE.width;
			int height = grE.height;
			if (grE.width == -1)
				width = window_width - CONTROL_INSET;
		
			// check if its a combo
			CComboBox* pCombo = dynamic_cast<CComboBox*>(grE.pWnd);
			if (pCombo) height = 200;

			// setup rectangle objects
			CRect rtObject(CONTROL_INSET, top, CONTROL_INSET + width, top + height);
			CRect rtLabel (ELEMENT_INSET, top + offset, CONTROL_INSET - ELEMENT_INSET - 2, top + grE.height);
			
			// set the status
			grE.pWnd	->MoveWindow(rtObject, FALSE);
			grE.pLabel	->MoveWindow(rtLabel,  FALSE);
			grE.pWnd	->ShowWindow(SW_SHOW);
			grE.pLabel	->ShowWindow(SW_SHOW);
		}
		else
		{
			grE.pLabel->ShowWindow(SW_SHOW);
			grE.pLabel->MoveWindow(ELEMENT_INSET, top + ELEMENT_SEPERATOR, window_width, grE.height);
			top+= ELEMENT_SEPERATOR;
		}

		// top becomes the top, plus height, plus seperator
		top+= grE.height + ELEMENT_SEPERATOR;
	}

	// set the vertical tabbing
	si.cbSize	= sizeof(SCROLLINFO);
	si.fMask	= SIF_ALL; 
	si.nMin		= 0;

	if (top > rtx.Height())
	{
		si.nMax		= top;
	}
	else
	{
		si.nMax = rtx.Height();
	}

	si.nPage	= rtx.Height();
	si.nPos		= 0;
	SetScrollInfo(SB_VERT, &si, TRUE);
	m_nScrollPos = 0;

	// now redraw
	SetRedraw(TRUE);
	Invalidate();
	return TRUE;
}
void PropertyScriptCtrl::OnDestroy()
{
	CDialog::OnDestroy();

	// destroy the child windows
	m_groups.clear();
}

void PropertyScriptCtrl::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// set the vertical tabbing
	SCROLLINFO si;
	si.cbSize	= sizeof(SCROLLINFO);
	si.fMask	= SIF_ALL; 
	si.nMin		= 0;
	si.nMax		= m_rcOriginalRect.Height();
	si.nPage	= cy;
	si.nPos		= 0;
    SetScrollInfo(SB_VERT, &si, TRUE); 	

	// current height
	m_nCurHeight = cy;

	// is height at current attrition
	if (si.nMax < cx)
	{
		si.nMax = cy;
	}

	if (IsWindow(c_Tabs))
	{
		RECT rt;
		GetWindowRect(&rt);
		c_Tabs.MoveWindow(0, 0, cx, rt.bottom - rt.top);
	}
}

void PropertyScriptCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nDelta;
	int nMaxPos = m_rcOriginalRect.Height() - m_nCurHeight;

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (m_nScrollPos >= nMaxPos)
			return;

		nDelta = min(max(nMaxPos/20,5),nMaxPos-m_nScrollPos);
		break;

	case SB_LINEUP:
		if (m_nScrollPos <= 0)
			return;
		nDelta = -min(max(nMaxPos/20,5),m_nScrollPos);
		break;
	case SB_PAGEDOWN:
		if (m_nScrollPos >= nMaxPos)
			return;
		nDelta = min(max(nMaxPos/10,5),nMaxPos-m_nScrollPos);
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		nDelta = (int)nPos - m_nScrollPos;
		break;

	case SB_PAGEUP:
		if (m_nScrollPos <= 0)
			return;
		nDelta = -min(max(nMaxPos/10,5),m_nScrollPos);
		break;
	
         default:
		return;
	}

	m_nScrollPos += nDelta;
	SetScrollPos(SB_VERT,m_nScrollPos,TRUE);
	ScrollWindow(0,-nDelta);
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL PropertyScriptCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CRect windowSize;
	GetWindowRect(windowSize);

	int nMaxPos = getGroupHeight(current_group) - windowSize.Height();;

	if (zDelta<0)
	{
		if (m_nScrollPos < nMaxPos)
		{
			zDelta = min(max(nMaxPos/20,5),nMaxPos-m_nScrollPos);

			m_nScrollPos += zDelta;
			SetScrollPos(SB_VERT,m_nScrollPos,TRUE);
			ScrollWindow(0,-zDelta);
		}
	}
	else
	{
		if (m_nScrollPos > 0)
		{
			zDelta = -min(max(nMaxPos/20,5),m_nScrollPos);

			m_nScrollPos += zDelta;
			SetScrollPos(SB_VERT,m_nScrollPos,TRUE);
			ScrollWindow(0,-zDelta);
		}
	}

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void PropertyScriptCtrl::OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
