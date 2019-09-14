#pragma once
#include "afxwin.h"


// SingleLineDialog dialog

class SingleLineDialog : public CDialog
{
	DECLARE_DYNAMIC(SingleLineDialog)

public:
	SingleLineDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~SingleLineDialog();

// Dialog Data
	enum { IDD = IDD_SINGLE_LINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

public: // input parameters
	String	v_title;
	String	v_about;

public: // handling parameter
	static const char* GetInputLine(CWnd* Wnd, const char* Question, const char* Title, const char* Text = 0);
	virtual BOOL OnInitDialog();
	CStatic c_about;
	CStatic c_about2;
	CEdit c_input;
	CString v_text;
};
