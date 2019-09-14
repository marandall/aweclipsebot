#pragma once
#include "textbox.h"
#include "afxwin.h"


// QueryModify_ReplaceStringDialog dialog

class QueryModify_ReplaceStringDialog : public CDialog
{
	DECLARE_DYNAMIC(QueryModify_ReplaceStringDialog)

public:
	QueryModify_ReplaceStringDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~QueryModify_ReplaceStringDialog();

// Dialog Data
	enum { IDD = IDD_QUERY_MODIFY_REPLACE_STRING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	TextBox c_Find;
	TextBox c_Replace;
	CButton c_Models;
	CButton c_Descriptions;
	CButton c_Actions;
public:
	afx_msg void OnBnClickedReplaceSelected();
	CButton c_MatchCase;
};
