// ZonesEditorSphereDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "ZonesEditorSphereDialog.h"


// ZonesEditorSphereDialog dialog

IMPLEMENT_DYNAMIC(ZonesEditorSphereDialog, CDialog)

ZonesEditorSphereDialog::ZonesEditorSphereDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ZonesEditorSphereDialog::IDD, pParent)
	, v_Name(_T(""))
	, v_Centre(_T(""))
	, v_Radius(0)
{

}

ZonesEditorSphereDialog::~ZonesEditorSphereDialog()
{
}

void ZonesEditorSphereDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT7, v_Name);
	DDX_Text(pDX, IDC_EDIT1, v_Centre);
	DDX_Text(pDX, IDC_EDIT6, v_Radius);
}


BEGIN_MESSAGE_MAP(ZonesEditorSphereDialog, CDialog)
	ON_BN_CLICKED(IDDELETE, &ZonesEditorSphereDialog::OnBnClickedDelete)
END_MESSAGE_MAP()


// ZonesEditorSphereDialog message handlers

void ZonesEditorSphereDialog::OnBnClickedDelete()
{
	EndDialog(IDNO);
}
