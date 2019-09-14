#pragma once
#include "afxwin.h"


// RelayChannelPermissionDialog dialog

class RelayChannelPermissionDialog : public CDialog
{
	DECLARE_DYNAMIC(RelayChannelPermissionDialog)

public:
	RelayChannelPermissionDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~RelayChannelPermissionDialog();

// Dialog Data
	enum { IDD = IDD_RELAY_CHANNELS_PERMISSIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString v_user;
	int v_accessType;
	virtual BOOL OnInitDialog();
	CComboBox c_accessList;
	afx_msg void OnBnClickedOk();
};
