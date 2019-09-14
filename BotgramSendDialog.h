#pragma once
#include "textbox.h"


// BotgramSendDialog dialog

class BotgramSendDialog : public CDialog
{
	DECLARE_DYNAMIC(BotgramSendDialog)

public:
	BotgramSendDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~BotgramSendDialog();

// Dialog Data
	enum { IDD = IDD_BOTGRAM_SEND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	TextBox c_Message;
	TextBox c_Subject;
	TextBox c_Target;
	TextBox c_BotName;
	afx_msg void OnBnClickedSend();
	CString v_to;
	CString v_botname;
};
