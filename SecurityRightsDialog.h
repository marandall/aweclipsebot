#pragma once
#include "listctrlgpx.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "TreeCtrlT.h"
#include "dynamicrights.h"


class SecurityRightsDialog : public CDialog
{
	DECLARE_DYNAMIC(SecurityRightsDialog)

public:
	SecurityRightsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~SecurityRightsDialog();

// Dialog Data
	enum { IDD = IDD_SECURITY_CONSOLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	int		cur_user_id;
	bool	can_show;
	void	ShowUser(DynamicRights::User& User);
	void	SaveUser(DynamicRights::User& User);
	void	InitAll();

public: // the dynamic set
	DynamicRights::Provider provider;

public:
	ListCtrlGpx c_Users;
	CEdit c_UserId;
	CEdit c_FullId;
	CEdit c_Description;
	CComboBox c_MemberOf;
	CTreeCtrl c_RightsList;
	TreeCtrlT<String>	m_RightsTree;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnUserList_ItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual void OnOK();
	afx_msg void OnBnClickedOk();
	afx_msg void OnUsers_New();
public:
	afx_msg void OnUsers_Delete();
	afx_msg void OnRightsResetcustomrights();
	afx_msg void OnRights_Import();
	afx_msg void OnRights_Export();
	afx_msg void OnAccounts_NewGroup();
};
