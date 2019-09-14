#pragma once
#include "PropertyScriptCtrl.h"
#include "afxwin.h"

// QueryFilterDialog dialog

class QueryFilterDialog : public CDialog
{
	DECLARE_DYNAMIC(QueryFilterDialog)

public:
	QueryFilterDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~QueryFilterDialog();

// Dialog Data
	enum { IDD = IDD_QUERY_FILTER };

public: // handling processors
	PropertyBuilderHelper	c_PsHelper;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic c_TrackingRect;
	afx_msg void OnBnClickedOk();
};
