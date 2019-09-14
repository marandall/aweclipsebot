#pragma once
#include "colourbutton.h"
#include "resource.h"
#include "afxwin.h"

// MessageInfoDialog dialog

class MessageInfoDialog : public CDialog
{
	DECLARE_DYNAMIC(MessageInfoDialog)

public:
	MessageInfoDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~MessageInfoDialog();

// Dialog Data
	enum { IDD = IDD_MESSAGE_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString v_Message;
public:
	int v_Style;
public:
	ColourButton c_Colour;
public:
	BOOL v_Bold;
public:
	BOOL v_Italic;
public:
	virtual BOOL OnInitDialog();
public:
	CComboBox c_Style;
	afx_msg void OnBnClickedOk();
	BOOL v_Suppress;
};
