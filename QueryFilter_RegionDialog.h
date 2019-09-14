#pragma once
#include "textbox.h"


// QueryFilter_RegionDialog dialog

class QueryFilter_RegionDialog : public CDialog
{
	DECLARE_DYNAMIC(QueryFilter_RegionDialog)

public:
	QueryFilter_RegionDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~QueryFilter_RegionDialog();

// Dialog Data
	enum { IDD = IDD_QUERY_FILTER_BOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	TextBox c_Top;
public:
	TextBox c_Bottom;
};
