#pragma once
#include "listctrlgpx.h"


// MoversDialog dialog

class MoversDialog : public CDialog
{
	DECLARE_DYNAMIC(MoversDialog)

public:
	MoversDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~MoversDialog();

// Dialog Data
	enum { IDD = IDD_MOVERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public: // reload the list
	void reload();
	Umbra::Variant vntSelected;

public:
	ListCtrlGpx c_list;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnList_DoubleClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCreateNewDriver();
	afx_msg void OnBnClickedButton1();
};
