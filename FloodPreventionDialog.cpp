// FloodPreventionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "FloodPreventionDialog.h"


// FloodPreventionDialog dialog

IMPLEMENT_DYNAMIC(FloodPreventionDialog, CDialog)

FloodPreventionDialog::FloodPreventionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(FloodPreventionDialog::IDD, pParent)
	, v_MaxPer5(0)
	, v_MaxIdentical(0)
	, v_AutoMute(0)
	, v_AutoMuteDuration(0)
	, v_Enabled(FALSE)
{

}

FloodPreventionDialog::~FloodPreventionDialog()
{
}

void FloodPreventionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MAX_AVERAGE, v_MaxPer5);
	DDV_MinMaxInt(pDX, v_MaxPer5, 0, 50);
	DDX_Text(pDX, IDC_MAX_IDENTICAL, v_MaxIdentical);
	DDX_Text(pDX, IDC_AUTO_MUTE, v_AutoMute);
	DDX_Text(pDX, IDC_AUTO_MUTE2, v_AutoMuteDuration);
	DDX_Check(pDX, IDC_ENABLE, v_Enabled);
}


BEGIN_MESSAGE_MAP(FloodPreventionDialog, CDialog)
END_MESSAGE_MAP()


// FloodPreventionDialog message handlers
