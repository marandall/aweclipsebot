#pragma once


// ChatFilterDialog dialog

class ChatFilterDialog : public CDialog
{
	DECLARE_DYNAMIC(ChatFilterDialog)

public:
	ChatFilterDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ChatFilterDialog();

// Dialog Data
	enum { IDD = IDD_CHAT_FILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString v_List;
	BOOL v_Enabled;
	int v_Duration;
	CString v_Message;
};
