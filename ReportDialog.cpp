// ReportDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "ReportDialog.h"


// ReportDialog dialog

IMPLEMENT_DYNAMIC(ReportDialog, CDialog)

ReportDialog::ReportDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ReportDialog::IDD, pParent)
	, v_output(_T(""))
{

}

ReportDialog::~ReportDialog()
{
}

void ReportDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, c_write);
	DDX_Text(pDX, IDC_EDIT1, v_output);
}


BEGIN_MESSAGE_MAP(ReportDialog, CDialog)
	ON_BN_CLICKED(IDOK, &ReportDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// ReportDialog message handlers

void ReportDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL ReportDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	_bodyFont.CreatePointFont(100, "Courier New");
	c_write.SetFont(&_bodyFont, TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
