#pragma once
#include "listctrlgpx.h"


// RelayChannelsDialog dialog

class RelayChannelsDialog : public CDialog
{
	DECLARE_DYNAMIC(RelayChannelsDialog)

public:
	RelayChannelsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~RelayChannelsDialog();

// Dialog Data
	enum { IDD = IDD_RELAY_CHANNELS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	ListCtrlGpx c_list;
	afx_msg void OnBnClickedOk();
	afx_msg void OnList_DoubleClick(NMHDR *pNMHDR, LRESULT *pResult);
	ListCtrlGpx c_members;
	afx_msg void OnList_ItemActivate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnList_ItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
};
