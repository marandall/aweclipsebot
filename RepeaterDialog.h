#pragma once
#include "afxwin.h"


// RepeaterDialog dialog

class RepeaterDialog : public CDialog
{
	DECLARE_DYNAMIC(RepeaterDialog)

public:
	RepeaterDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~RepeaterDialog();

// Dialog Data
	enum { IDD = IDD_COMMS_MULTILINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CString		v_InputBox;
	CListBox	c_Queue;
	CComboBox	c_Mps;
	CString		v_Output;
	CEdit		c_OutputBox;
	CButton		c_Paused;

public:
	afx_msg void OnBnClickedBegin();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSay();
	afx_msg void OnBnClickedSay2();
	afx_msg void OnBnClickedSay3();
	afx_msg void OnNcDestroy();	
public:
	afx_msg void OnBnClickedClear();
};
