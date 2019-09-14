#pragma once
#include "afxwin.h"


// QueryFilter_TypeDialog dialog

class QueryFilter_TypeDialog : public CDialog
{
	DECLARE_DYNAMIC(QueryFilter_TypeDialog)

public:
	QueryFilter_TypeDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~QueryFilter_TypeDialog();

// Dialog Data
	enum { IDD = IDD_QUERY_FILTER_TYPES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton c_Version3;
	CButton c_Zones;
	CButton c_Particles;
	CButton c_Movers;
	CButton c_Cameras;
public:
	afx_msg void OnBnClickedConfirm2();
public:
	afx_msg void OnBnClickedUnselect3();
};
