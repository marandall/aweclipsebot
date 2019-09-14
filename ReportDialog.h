#pragma once
#include "afxwin.h"


// ReportDialog dialog

class ReportDialog : public CDialog
{
	DECLARE_DYNAMIC(ReportDialog)

public:
	ReportDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ReportDialog();

private: // fonts
	CFont _bodyFont;

// Dialog Data
	enum { IDD = IDD_REPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CEdit c_write;
	CString v_output;
};
