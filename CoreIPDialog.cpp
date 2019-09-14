// oreIPDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "CoreIPDialog.h"


// CoreIPDialog dialog

IMPLEMENT_DYNAMIC(CoreIPDialog, CDialog)

CoreIPDialog::CoreIPDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CoreIPDialog::IDD, pParent)
	, v_Enabled(FALSE)
	, v_Tourists(FALSE)
	, v_Citizens(FALSE)
	, v_Bots(FALSE)
	, v_TellSource(FALSE)
	, v_Others(_T(""))
{

}

CoreIPDialog::~CoreIPDialog()
{
}

void CoreIPDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_ENABLED, v_Enabled);
	DDX_Check(pDX, IDC_TELL_T, v_Tourists);
	DDX_Check(pDX, IDC_TELL_C, v_Citizens);
	DDX_Check(pDX, IDC_TELL_B, v_Bots);
	DDX_Check(pDX, IDC_TELL_SOURCE, v_TellSource);
	DDX_CBString(pDX, IDC_OTHERS, v_Others);
	DDX_Control(pDX, IDC_MESSAGE, c_Format);
}


BEGIN_MESSAGE_MAP(CoreIPDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CoreIPDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CoreIPDialog message handlers
bool CoreIPDialog::DET_ToDialog()
{ 
	return false; 
}

bool CoreIPDialog::DET_FromDialog()
{ 
	return false; 
}
void CoreIPDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
