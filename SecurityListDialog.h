#pragma once
#include "listctrlgpx.h"

// SecurityListDialog dialog
namespace DynamicRights
{
	class Permissions;
}

class SecurityListDialog : 
	public CDialog
{
	DECLARE_DYNAMIC(SecurityListDialog)

public:
	SecurityListDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~SecurityListDialog();

// Dialog Data
	enum { IDD = IDD_SECURITY_LIST };

	DynamicRights::Permissions* m_pPermissions;
	String	v_Title;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	ListCtrlGpx c_List;
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
