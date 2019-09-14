#pragma once
#include "listctrlgpx.h"


// PropertySelectProject dialog

class PropertySelectProject : public CDialog
{
	DECLARE_DYNAMIC(PropertySelectProject)

public:
	PropertySelectProject(CWnd* pParent = NULL);   // standard constructor
	virtual ~PropertySelectProject();
	
public: // selection
	CStringA	selected_id;
	
// Dialog Data
	enum { IDD = IDD_QUERY_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	ListCtrlGpx c_list;
	afx_msg void OnList_DoubleClick(NMHDR *pNMHDR, LRESULT *pResult);
};
