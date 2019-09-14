// QueryFilter_RegionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "QueryFilter_RegionDialog.h"


// QueryFilter_RegionDialog dialog

IMPLEMENT_DYNAMIC(QueryFilter_RegionDialog, CDialog)

QueryFilter_RegionDialog::QueryFilter_RegionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(QueryFilter_RegionDialog::IDD, pParent)
{

}

QueryFilter_RegionDialog::~QueryFilter_RegionDialog()
{
}

void QueryFilter_RegionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NORTH, c_Top);
	DDX_Control(pDX, IDC_SOUTH, c_Bottom);
}


BEGIN_MESSAGE_MAP(QueryFilter_RegionDialog, CDialog)
END_MESSAGE_MAP()


// QueryFilter_RegionDialog message handlers
