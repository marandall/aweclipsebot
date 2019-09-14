// ZonesEditorCylinderDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "ZonesEditorCylinderDialog.h"


// ZonesEditorCylinderDialog dialog

IMPLEMENT_DYNAMIC(ZonesEditorCylinderDialog, CDialog)

ZonesEditorCylinderDialog::ZonesEditorCylinderDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ZonesEditorCylinderDialog::IDD, pParent)
	, v_Name(_T(""))
	, v_Base(_T(""))
	, v_Radius(0)
	, v_Height(0)
{

}

ZonesEditorCylinderDialog::~ZonesEditorCylinderDialog()
{
}

void ZonesEditorCylinderDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT7, v_Name);
	DDX_Text(pDX, IDC_EDIT1, v_Base);
	DDX_Text(pDX, IDC_EDIT6, v_Radius);
	DDX_Text(pDX, IDC_EDIT8, v_Height);
}


BEGIN_MESSAGE_MAP(ZonesEditorCylinderDialog, CDialog)
	ON_BN_CLICKED(IDDELETE, &ZonesEditorCylinderDialog::OnBnClickedDelete)
END_MESSAGE_MAP()


// ZonesEditorCylinderDialog message handlers

void ZonesEditorCylinderDialog::OnBnClickedDelete()
{
	EndDialog(IDNO);
}
