#pragma once
#include "afxwin.h"

// InputBoxDialog dialog

class InputBoxDialog : public CDialog
{
	DECLARE_DYNAMIC(InputBoxDialog)

public:
	InputBoxDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~InputBoxDialog();

// Dialog Data
	enum { IDD = IDD_INPUT_MULTILINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	virtual void PreInitDialog();
public:
	CEdit	c_Box;
	CString	v_String;
	CString v_Title;

public:
	virtual BOOL OnInitDialog();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnOK();
};
