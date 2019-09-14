// SecurityListDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "SecurityListDialog.h"
#include "DynamicRights.h"

// SecurityListDialog dialog

IMPLEMENT_DYNAMIC(SecurityListDialog, CDialog)

SecurityListDialog::SecurityListDialog(CWnd* pParent /*=NULL*/)
	: CDialog(SecurityListDialog::IDD, pParent)
{

}

SecurityListDialog::~SecurityListDialog()
{
}

void SecurityListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, c_List);
}


BEGIN_MESSAGE_MAP(SecurityListDialog, CDialog)
	ON_BN_CLICKED(IDOK, &SecurityListDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// SecurityListDialog message handlers

void SecurityListDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL SecurityListDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	/* add cols */
	c_List.InsertColumn(0, _T("Right"),			0, 120);
	c_List.InsertColumn(1, _T("Granted By"),	0, 400);

	/* show all of the permissions */
	if (m_pPermissions) {
		for (size_t i = 0; i < m_pPermissions->size(); i++) {
			int id = c_List.InsertItem(c_List.GetItemCount(), (*m_pPermissions)[i].key_id);
			String list = (*m_pPermissions)[i].inherit_from;
			list.Trim(", ");
			c_List.SetItemText(id, 1, list);
		}
	}

	/* set title */
	SetWindowText(v_Title);

	return TRUE;
}

