// SelectUrlDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "SelectUrlDialog.h"


// SelectUrlDialog dialog

IMPLEMENT_DYNAMIC(SelectUrlDialog, CDialog)

SelectUrlDialog::SelectUrlDialog(CWnd* pParent /*=NULL*/)
	: CDialog(SelectUrlDialog::IDD, pParent)
	, v_Target(_T(""))
	, v_Url(_T(""))
	, v_Post(_T(""))
	, v_Target3D(FALSE)
{

}

SelectUrlDialog::~SelectUrlDialog()
{
}

void SelectUrlDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_URL, c_Url);
	DDX_CBString(pDX, IDC_TARGET, v_Target);
	DDX_Text(pDX, IDC_URL, v_Url);
	DDX_Text(pDX, IDC_URL2, v_Post);
	DDX_Check(pDX, IDC_TARget3D, v_Target3D);
}


BEGIN_MESSAGE_MAP(SelectUrlDialog, CDialog)
END_MESSAGE_MAP()


// SelectUrlDialog message handlers
