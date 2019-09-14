// oreDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "CoreDialog.h"
#include "DialogExchangeTarget.h"
#include "CoreGreetingsDialog.h"
#include "CoreIpDialog.h"

// CoreDialog dialog

IMPLEMENT_DYNAMIC(CoreDialog, CDialog)

CoreDialog::CoreDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CoreDialog::IDD, pParent)
{

}

CoreDialog::~CoreDialog()
{
}

void CoreDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLACEMENT, c_Placement);
	DDX_Control(pDX, IDC_TAB1, c_Tabs);
}


BEGIN_MESSAGE_MAP(CoreDialog, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CoreDialog::OnTab_ChangedItem)
END_MESSAGE_MAP()


// CoreDialog message handlers

template<class C>
CWnd* CreateWindowPtr(CWnd* pWindow)
{
	C* pC = new C(pWindow);
	pC->Create(C::IDD, pWindow);
	return pC;
}

BOOL CoreDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	/* create windows */
	int index = 0;
	CWnd* pPane = NULL;

	/* create each dialog */
	pPane = CreateWindowPtr<CoreGreetingsDialog>(this);
	m_Panes.push_back(pPane);
	c_Tabs.InsertItem(index++, "Greetings"); 

	pPane = CreateWindowPtr<CoreIPDialog>(this);
	m_Panes.push_back(pPane);
	c_Tabs.InsertItem(index++, "IP Notify"); 

	/* resize everything */
	RECT placer;
	c_Placement.GetWindowRect(&placer);
	ScreenToClient(&placer);
	for (size_t i = 0; i < m_Panes.size(); i++)
		m_Panes[i]->MoveWindow(&placer);
	return TRUE;
}

void CoreDialog::OnTab_ChangedItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	int sel = c_Tabs.GetCurSel();
	for (size_t i = 0; i < m_Panes.size(); i++)
		m_Panes[i]->ShowWindow(sel == i ? SW_SHOW : SW_HIDE);
}

