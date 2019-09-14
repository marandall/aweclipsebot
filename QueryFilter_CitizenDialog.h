#pragma once
#include "textbox.h"


// QueryFilter_CitizenDialog dialog

class QueryFilter_CitizenDialog : public CDialog
{
	DECLARE_DYNAMIC(QueryFilter_CitizenDialog)

public:
	QueryFilter_CitizenDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~QueryFilter_CitizenDialog();

// Dialog Data
	enum { IDD = IDD_QUERY_FILTER_CITIZEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	TextBox c_CitizenNumbers;
public:
	afx_msg void OnBnClickedConfirm2();
public:
	afx_msg void OnBnClickedUnselect();
};
