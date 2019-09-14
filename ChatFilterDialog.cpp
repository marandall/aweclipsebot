// hatFilterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "ChatFilterDialog.h"


// ChatFilterDialog dialog

IMPLEMENT_DYNAMIC(ChatFilterDialog, CDialog)

ChatFilterDialog::ChatFilterDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ChatFilterDialog::IDD, pParent)
	, v_List(_T(""))
	, v_Enabled(FALSE)
	, v_Duration(0)
	, v_Message(_T(""))
{

}

ChatFilterDialog::~ChatFilterDialog()
{
}

void ChatFilterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WORDS, v_List);
	DDX_Check(pDX, IDC_ENABLED, v_Enabled);
	DDX_Text(pDX, IDC_DUR, v_Duration);
	DDV_MinMaxInt(pDX, v_Duration, 0, 1200);
	DDX_Text(pDX, IDC_MESSAGE, v_Message);
}


BEGIN_MESSAGE_MAP(ChatFilterDialog, CDialog)
END_MESSAGE_MAP()


// ChatFilterDialog message handlers
