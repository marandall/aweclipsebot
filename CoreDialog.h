#pragma once
#include <vector>
#include "afxwin.h"
#include "afxcmn.h"

class CoreDialog : public CDialog
{
	DECLARE_DYNAMIC(CoreDialog)
public:
	CoreDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CoreDialog();

// Dialog Data
	enum { IDD = IDD_CORE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public: /* control for multiple surfaces */
	vector<CWnd*>	m_Panes;

public:
	virtual BOOL OnInitDialog();
public:
	CStatic		c_Placement;
	CTabCtrl	c_Tabs;
	afx_msg		void OnTab_ChangedItem(NMHDR *pNMHDR, LRESULT *pResult);
};
