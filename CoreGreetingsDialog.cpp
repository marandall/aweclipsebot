// oreGreetingsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "CoreGreetingsDialog.h"
#include "core_greetings.h"

// CoreGreetingsDialog dialog

IMPLEMENT_DYNAMIC(CoreGreetingsDialog, CDialog)

CoreGreetingsDialog::CoreGreetingsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CoreGreetingsDialog::IDD, pParent)
	, v_EnterEnabled(FALSE)
	, v_ExitEnabled(FALSE)
{

}

CoreGreetingsDialog::~CoreGreetingsDialog()
{
}

void CoreGreetingsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_WM_ENABLED, v_EnterEnabled);
	DDX_Check(pDX, IDC_WM_ENABLED2, v_ExitEnabled);
	DDX_Control(pDX, IDC_ENTERMSG, c_Enter);
	DDX_Control(pDX, IDC_EXITMSG, c_Exit);
}


BEGIN_MESSAGE_MAP(CoreGreetingsDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CoreGreetingsDialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CoreGreetingsDialog::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CoreGreetingsDialog::OnBnClickedButton1)
	ON_COMMAND(ID_SUPPORT_WELCOME, &CoreGreetingsDialog::OnSupport_WelcomeDeparture)
END_MESSAGE_MAP()


// CoreGreetingsDialog message handlers

void CoreGreetingsDialog::OnBnClickedButton2()
{
	MessageInfo_EditBox(EvoCore::g_EnterMessages.m_enter, this);
}

void CoreGreetingsDialog::OnBnClickedButton3()
{
	MessageInfo_EditBox(EvoCore::g_EnterMessages.m_exit, this);
}

bool CoreGreetingsDialog::DET_ToDialog()
{ 
	return false;
}

bool CoreGreetingsDialog::DET_FromDialog()	
{ 
	return false; 
}
void CoreGreetingsDialog::OnBnClickedButton1()
{
	MessageInfo_EditBox(config, GetParent());
}

void CoreGreetingsDialog::OnSupport_WelcomeDeparture()
{
}
