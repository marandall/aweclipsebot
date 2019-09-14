#pragma once


// QueryProgressDialog dialog

class QueryProgressDialog : public CDialog
{
	DECLARE_DYNAMIC(QueryProgressDialog)

public:
	QueryProgressDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~QueryProgressDialog();

// Dialog Data
	enum { IDD = IDD_QUERY_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
