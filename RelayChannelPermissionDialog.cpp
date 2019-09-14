// RelayChannelPermissionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "RelayChannelPermissionDialog.h"


// RelayChannelPermissionDialog dialog

IMPLEMENT_DYNAMIC(RelayChannelPermissionDialog, CDialog)

RelayChannelPermissionDialog::RelayChannelPermissionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(RelayChannelPermissionDialog::IDD, pParent)
	, v_user(_T(""))
	, v_accessType(0)
{

}

RelayChannelPermissionDialog::~RelayChannelPermissionDialog()
{
}

void RelayChannelPermissionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, v_user);
	DDX_CBIndex(pDX, IDC_COMBO1, v_accessType);
	DDX_Control(pDX, IDC_COMBO1, c_accessList);
}


BEGIN_MESSAGE_MAP(RelayChannelPermissionDialog, CDialog)
	ON_BN_CLICKED(IDOK, &RelayChannelPermissionDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// RelayChannelPermissionDialog message handlers

#include "relayrelay.h"
BOOL RelayChannelPermissionDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	using namespace ChatControllers::Channels;
	c_accessList.InsertString(0, "NONE");
	c_accessList.InsertString(1, "MEMBER");
	c_accessList.InsertString(2, "MODERATOR");
	c_accessList.InsertString(3, "ADMINISTRATOR");

	// select
	c_accessList.SetCurSel(v_accessType);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void RelayChannelPermissionDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
