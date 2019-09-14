#pragma once
#include "textbox.h"
#include "afxwin.h"


// QueryFilter_StringsDialog dialog

class QueryFilter_StringsDialog : public CDialog
{
	DECLARE_DYNAMIC(QueryFilter_StringsDialog)

public:
	QueryFilter_StringsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~QueryFilter_StringsDialog();

// Dialog Data
	enum { IDD = IDD_QUERY_FILTER_STRINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	TextBox c_Search;
	CButton c_Models;
	CButton c_Descriptions;

public:
	afx_msg void OnBnClickedConfirm2();
	afx_msg void OnBnClickedUnselect();
public:
	CButton c_Actions;
};
