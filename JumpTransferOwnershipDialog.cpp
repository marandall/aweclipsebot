// JumpTransferOwnershipDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "JumpTransferOwnershipDialog.h"


// JumpTransferOwnershipDialog dialog

IMPLEMENT_DYNAMIC(JumpTransferOwnershipDialog, CDialog)

JumpTransferOwnershipDialog::JumpTransferOwnershipDialog(CWnd* pParent /*=NULL*/)
	: CDialog(JumpTransferOwnershipDialog::IDD, pParent)
	, v_citizen(0)
{

}

JumpTransferOwnershipDialog::~JumpTransferOwnershipDialog()
{
}

void JumpTransferOwnershipDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, v_citizen);
	DDV_MinMaxInt(pDX, v_citizen, 1, 1000000);
}


BEGIN_MESSAGE_MAP(JumpTransferOwnershipDialog, CDialog)
END_MESSAGE_MAP()


// JumpTransferOwnershipDialog message handlers
