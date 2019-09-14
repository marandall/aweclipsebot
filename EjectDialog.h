#pragma once
#include "afxwin.h"


// EjectDialog dialog

class EjectDialog : public CDialog
{
	DECLARE_DYNAMIC(EjectDialog)

public:
	EjectDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~EjectDialog();

// Dialog Data
	enum { IDD = IDD_EJECT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString v_Minutes;
	CString v_Hours;
	CString v_Days;
	CString v_Log;
	CString v_UserMessage;
	String	m_TitleMsg;
	String	m_TargetId;
public:
	afx_msg void OnNcDestroy();
public:
	virtual BOOL OnInitDialog();
public:
	CEdit c_Minutes;
	CString v_name;
	CString v_dns;
};
