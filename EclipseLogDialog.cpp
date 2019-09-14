// EclipseLogDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "EclipseLogDialog.h"


// EclipseLogDialog dialog

IMPLEMENT_DYNAMIC(EclipseLogDialog, CDialog)

EclipseLogDialog::EclipseLogDialog(CWnd* pParent /*=NULL*/)
	: CDialog(EclipseLogDialog::IDD, pParent)
{

}

EclipseLogDialog::~EclipseLogDialog()
{
}

void EclipseLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TECHNICAL_LOG, c_TechLog);
	DDX_Control(pDX, IDC_SECURITY_LOG, c_SecurityLog);
	DDX_Control(pDX, IDC_USERLIST_LOG, c_UserLog);
	DDX_Control(pDX, IDC_DEBUG_LOG, c_DebugLog);
	DDX_Control(pDX, IDC_TECH, c_Tech);
	DDX_Control(pDX, IDC_SECURITY, c_Security);
	DDX_Control(pDX, IDC_UUL, c_UserList);
	DDX_Control(pDX, IDC_DEBUG, c_Debug);
}


BEGIN_MESSAGE_MAP(EclipseLogDialog, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_TECH, &EclipseLogDialog::OnBnClickedTech)
	ON_BN_CLICKED(IDC_SECURITY, &EclipseLogDialog::OnBnClickedSecurity)
	ON_BN_CLICKED(IDC_UUL, &EclipseLogDialog::OnBnClickedUul)
	ON_BN_CLICKED(IDC_DEBUG, &EclipseLogDialog::OnBnClickedDebug)
END_MESSAGE_MAP()


// EclipseLogDialog message handlers

BOOL EclipseLogDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	/* configure the list */
	WindowPair tech		= { &c_Tech,		&c_TechLog };
	WindowPair security = { &c_Security,	&c_SecurityLog };
	WindowPair uul		= { &c_UserList,	&c_UserLog };
	WindowPair debug	= { &c_Debug,		&c_DebugLog };
	windows.push_back(tech);	windows.push_back(security);
	windows.push_back(uul);		windows.push_back(debug);

	/* resize as whole */
	for (size_t i = 0; i < windows.size(); i++) {
		windows[i].window->setFirstTab();
		windows[i].window->ModifyStyle(0, WS_BORDER);
		windows[i].button->ModifyStyle(0, BS_RADIOBUTTON | BS_FLAT | BS_PUSHLIKE, 0);
	}

	SelectWindow(c_Tech);
	return TRUE;
}

void EclipseLogDialog::SelectWindow(CButton& Button)
{
	/* resize as whole */
	for (size_t i = 0; i < windows.size(); i++) {
		if (windows[i].button == &Button) {
			windows[i].button->SetCheck(TRUE);
			windows[i].window->ShowWindow(SW_SHOW);
		} else {
			windows[i].button->SetCheck(FALSE);
			windows[i].window->ShowWindow(SW_HIDE);
		}
	}
}

void EclipseLogDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	/* size buttons */
	const int BUTTON_WIDTH  = 100;
	const int BUTTON_HEIGHT = 16;
	const int BUTTON_SPACER = 1;

	/* resize as whole */
	for (size_t i = 0; i < windows.size(); i++) {
		windows[i].button->MoveWindow(0, (int)((BUTTON_HEIGHT + BUTTON_SPACER) * i), BUTTON_WIDTH, BUTTON_HEIGHT);
		windows[i].window->MoveWindow(BUTTON_WIDTH + BUTTON_SPACER, 0, cx - (BUTTON_WIDTH + BUTTON_SPACER), cy);
	}
}

void EclipseLogDialog::OnBnClickedTech() {
	SelectWindow(c_Tech);
}

void EclipseLogDialog::OnBnClickedSecurity() {
	SelectWindow(c_Security);
}

void EclipseLogDialog::OnBnClickedUul() {
	SelectWindow(c_UserList);
}

void EclipseLogDialog::OnBnClickedDebug() {
	SelectWindow(c_Debug);
}

void EclipseLogDialog::OnCancel()
{
}
