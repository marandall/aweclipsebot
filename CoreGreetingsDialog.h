#pragma once
#include "colourbutton.h"
#include "DialogExchangeTarget.h"
#include "messageselectionctrl.h"

// CoreGreetingsDialog dialog

class CoreGreetingsDialog : 
	public CDialog,
	public DialogExchangeTarget
{
	DECLARE_DYNAMIC(CoreGreetingsDialog)

public:
	CoreGreetingsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CoreGreetingsDialog();

// Dialog Data
	enum { IDD = IDD_CORE_GREETINGS };
	bool	DET_ToDialog();
	bool	DET_FromDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	BOOL v_EnterEnabled;
	BOOL v_ExitEnabled;
	MessageInfo			 config;
	MessageSelectionCtrl c_Enter;
	MessageSelectionCtrl c_Exit;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnSupport_WelcomeDeparture();
};
