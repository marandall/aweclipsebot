// EclipseInputDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "EclipseInputDialog.h"
#include "CIS_Dispatch.h"

// EclipseInputDialog dialog

IMPLEMENT_DYNAMIC(EclipseInputDialog, CDialog)

EclipseInputDialog::EclipseInputDialog(CWnd* pParent /*=NULL*/)
	: CDialog(EclipseInputDialog::IDD, pParent)
{

}

EclipseInputDialog::~EclipseInputDialog()
{
}

void EclipseInputDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WHISPER, c_Whisper);
	DDX_Control(pDX, IDC_COMMANDS, c_Commands);
	DDX_Control(pDX, IDC_CONSOLE, c_Console);
	DDX_Control(pDX, IDC_SPEAK, c_Speak);
	DDX_Control(pDX, IDC_INPUT, c_InputBox);
	DDX_Control(pDX, IDC_CONSOLE_MODE, c_ConsoleMode);
}


BEGIN_MESSAGE_MAP(EclipseInputDialog, CDialog)
	ON_BN_CLICKED(IDC_SPEAK, &EclipseInputDialog::OnBnClickedSpeak)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_WHISPER, &EclipseInputDialog::OnBnClickedWhisper)
	ON_BN_CLICKED(IDC_COMMANDS, &EclipseInputDialog::OnBnClickedCommands)
	ON_BN_CLICKED(IDC_CONSOLE, &EclipseInputDialog::OnBnClickedConsole)
	ON_BN_CLICKED(IDC_CONSOLE_MODE, &EclipseInputDialog::OnBnClickedConsoleMode)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// EclipseInputDialog message handlers
void EclipseInputDialog::SwitchToCheck(CButton& Button)
{
	CButton* pButtons[] = { &c_Speak, &c_Whisper, &c_Commands, &c_Console };
	for (size_t i = 0; i < 4; i++)
		pButtons[i]->SetCheck((&Button == pButtons[i]) ? TRUE : FALSE);
}

void EclipseInputDialog::OnBnClickedSpeak()
{
	SwitchToCheck(c_Speak);
}

BOOL EclipseInputDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	c_InputBox.ModifyStyle(0, WS_BORDER);

	CButton* pButtons[] = { &c_Speak, &c_Whisper, &c_Commands, &c_Console };
	for (size_t i = 0; i < 4; i++)
		pButtons[i]->ModifyStyle(0, BS_RADIOBUTTON | BS_FLAT | BS_PUSHLIKE, 0);

	// set console
	m_boxFont.CreatePointFont(90, "Courier New");
	c_InputBox.SetFont(&m_boxFont);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void EclipseInputDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	/* skipp if no window */
	if (!IsWindow(c_Speak))
		return;

	/* size buttons */
	const int BUTTON_WIDTH  = 100;
	const int BUTTON_HEIGHT = 16;
	const int BUTTON_SPACER = 1;
	const int BUTTON_EXT	= 20;

	CButton* pButtons[] = { &c_Speak, &c_Whisper, &c_Commands, &c_Console };
	for (size_t i = 0; i < 4; i++) {
		if (pButtons[i] == &c_Console) {
			pButtons[i]->MoveWindow(0, (int)((BUTTON_HEIGHT + BUTTON_SPACER) * i), 
				BUTTON_WIDTH - BUTTON_EXT - BUTTON_SPACER, BUTTON_HEIGHT);
			c_ConsoleMode.MoveWindow(BUTTON_WIDTH - BUTTON_EXT, (int)((BUTTON_HEIGHT + BUTTON_SPACER) * i),
				BUTTON_EXT, BUTTON_HEIGHT);
		} else {
			pButtons[i]->MoveWindow(0, (int)((BUTTON_HEIGHT + BUTTON_SPACER) * i), BUTTON_WIDTH, BUTTON_HEIGHT);
		}
	}
	c_InputBox.MoveWindow(BUTTON_WIDTH + BUTTON_SPACER, 0, cx - (BUTTON_WIDTH + BUTTON_SPACER), cy);
	Invalidate(TRUE);
}

void EclipseInputDialog::OnBnClickedWhisper()
{
	SwitchToCheck(c_Whisper);
}

void EclipseInputDialog::OnBnClickedCommands()
{
	SwitchToCheck(c_Commands);
}

void EclipseInputDialog::OnBnClickedConsole()
{
	SwitchToCheck(c_Console);
}

LRESULT EclipseInputDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UWM_CTRLKEY_RETURN)
	{
		String text;
		c_InputBox.GetWindowTextA(text);

		if (c_Speak.GetCheck() == TRUE)
		{
			bot.say(text);
		}
		else if (c_Whisper.GetCheck() == TRUE)
		{
			for (size_t i = 0; i < avatars.size(); i++)
				if (avatars[i].whisper_target)
					avatars[i].WhisperEx(text, false);
		}
		else if (c_Commands.GetCheck() == TRUE)
		{
			String log;
			log.Format("CMD>\t%s", text);
			WriteMsg(log, 0x0055AA, OT_MAIN);
			CIS::CommandInterpreter::beginConsole(text);
		}
		else if (c_Console.GetCheck() == TRUE)
		{
			String log;
			/* extract window text for loop */
			CString wndText;
			c_ConsoleMode.GetWindowTextA(wndText);

			/* Use the bots name */
			if (wndText == "B") 
			{
				log.Format("[%s]:\t%s", bot.getName(), text);
				bot.consoleBroadcast(log, 0x772277, CM_BOLD);
			} 

			/* Use the privilege passwords name */
			else if (wndText == "U") 
			{
				log.Format("%s:\t%s", bot._string(AW_LOGIN_PRIVILEGE_NAME), text);
				bot.consoleBroadcast(log, 0x772277, CM_BOLD);
			}

			/* Use no prefix */
			else 
			{
				bot.consoleBroadcast(text, 0x772277, CM_BOLD);
			}
		}

		c_InputBox.SetWindowTextA("");
		c_InputBox.GetFocus();
		return TRUE;
	}
	else
	{
		return CDialog::WindowProc(message, wParam, lParam);
	}
}

void EclipseInputDialog::OnBnClickedConsoleMode()
{
	/* extract window text for loop */
	CString wndText;
	c_ConsoleMode.GetWindowTextA(wndText);

	/* switch mdoes */
	if (wndText == "S")			// simple mode
		c_ConsoleMode.SetWindowTextA(_T("B"));
	else if (wndText == "B")	// bot mode
		c_ConsoleMode.SetWindowTextA(_T("U"));
	else if (wndText == "U")	// ownership mode
		c_ConsoleMode.SetWindowTextA(_T("S"));
	c_InputBox.SetFocus();
}

void EclipseInputDialog::OnCancel()
{
	int t = 0;
}

void EclipseInputDialog::OnClose()
{
	CDialog::OnClose();
}

void EclipseInputDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnOK();
}

BOOL EclipseInputDialog::PreTranslateMessage(MSG* pMsg)
{
	return CDialog::PreTranslateMessage(pMsg);
}

