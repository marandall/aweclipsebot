#pragma once
#include "RelayPrivateChannels.h"
#include "listctrlgpx.h"

// RelayChannelsConfigDialog dialog

class RelayChannelsConfigDialog : public CDialog
{
	DECLARE_DYNAMIC(RelayChannelsConfigDialog)

public:
	RelayChannelsConfigDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~RelayChannelsConfigDialog();

// Dialog Data
	enum { IDD = IDD_RELAY_CHANNELS_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	void resetUsers();

public: // copy information
	ChatControllers::Channels::Channel::PermissionList perms;
	BOOL v_public;
	virtual BOOL OnInitDialog();
	ListCtrlGpx c_acl;
	afx_msg void OnBnClickedOk();
	afx_msg void OnACL_DoubleClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnACL_CreateUser();
};
