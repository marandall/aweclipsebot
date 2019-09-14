// MessageSelectionCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "MessageSelectionCtrl.h"
#include "MessageInfoDialog.h"

// MessageSelectionCtrl dialog

IMPLEMENT_DYNAMIC(MessageSelectionCtrl, CDialog)

MessageSelectionCtrl::MessageSelectionCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(MessageSelectionCtrl::IDD, pParent)
{

}

MessageSelectionCtrl::~MessageSelectionCtrl()
{
}

void MessageSelectionCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT, c_Edit);
}


BEGIN_MESSAGE_MAP(MessageSelectionCtrl, CDialog)
	ON_BN_CLICKED(IDC_EDIT, &MessageSelectionCtrl::OnBnClickedEdit)
END_MESSAGE_MAP()


// MessageSelectionCtrl message handlers

void MessageSelectionCtrl::OnBnClickedEdit()
{
}
