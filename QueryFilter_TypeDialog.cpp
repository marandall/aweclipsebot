// QueryFilter_TypeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "QueryFilter_TypeDialog.h"
#include "QueryDialog.h"

// QueryFilter_TypeDialog dialog

IMPLEMENT_DYNAMIC(QueryFilter_TypeDialog, CDialog)

QueryFilter_TypeDialog::QueryFilter_TypeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(QueryFilter_TypeDialog::IDD, pParent)
{

}

QueryFilter_TypeDialog::~QueryFilter_TypeDialog()
{
}

void QueryFilter_TypeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBJECT_TYPE, c_Version3);
	DDX_Control(pDX, IDC_OBJECT_TYPE2, c_Zones);
	DDX_Control(pDX, IDC_OBJECT_TYPE3, c_Particles);
	DDX_Control(pDX, IDC_OBJECT_TYPE4, c_Movers);
	DDX_Control(pDX, IDC_OBJECT_TYPE5, c_Cameras);
}


BEGIN_MESSAGE_MAP(QueryFilter_TypeDialog, CDialog)
	ON_BN_CLICKED(IDC_CONFIRM2, &QueryFilter_TypeDialog::OnBnClickedConfirm2)
	ON_BN_CLICKED(IDC_UNSELECT3, &QueryFilter_TypeDialog::OnBnClickedUnselect3)
END_MESSAGE_MAP()


// QueryFilter_TypeDialog message handlers

void QueryFilter_TypeDialog::OnBnClickedConfirm2()
{
	bool types[MAX_OBJECT_TYPE];
	types[AW_OBJECT_TYPE_V3]		= c_Version3.	GetCheck() == TRUE;
	types[AW_OBJECT_TYPE_PARTICLES]	= c_Particles.	GetCheck() == TRUE;
	types[AW_OBJECT_TYPE_ZONE]		= c_Zones.		GetCheck() == TRUE;
	types[AW_OBJECT_TYPE_MOVER]		= c_Movers.		GetCheck() == TRUE;
	types[AW_OBJECT_TYPE_CAMERA]	= c_Cameras.	GetCheck() == TRUE;
	((QueryDialog*)GetParent())->Filter_SelectType(types, true);
}

void QueryFilter_TypeDialog::OnBnClickedUnselect3()
{
	bool types[MAX_OBJECT_TYPE];
	types[AW_OBJECT_TYPE_V3]		= c_Version3.	GetCheck() == TRUE;
	types[AW_OBJECT_TYPE_PARTICLES]	= c_Particles.	GetCheck() == TRUE;
	types[AW_OBJECT_TYPE_ZONE]		= c_Zones.		GetCheck() == TRUE;
	types[AW_OBJECT_TYPE_MOVER]		= c_Movers.		GetCheck() == TRUE;
	types[AW_OBJECT_TYPE_CAMERA]	= c_Cameras.	GetCheck() == TRUE;
	((QueryDialog*)GetParent())->Filter_SelectType(types, false);
}