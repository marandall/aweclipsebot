// ZonesEditorPolygonDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "ZonesEditorPolygonDialog.h"


// ZonesEditorPolygonDialog dialog

IMPLEMENT_DYNAMIC(ZonesEditorPolygonDialog, CDialog)

ZonesEditorPolygonDialog::ZonesEditorPolygonDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ZonesEditorPolygonDialog::IDD, pParent)
	, v_base(0)
	, v_height(0)
	, v_map(_T(""))
{

}

ZonesEditorPolygonDialog::~ZonesEditorPolygonDialog()
{
}

void ZonesEditorPolygonDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT7, v_name);
	DDX_Text(pDX, IDC_EDIT6, v_base);
	DDX_Text(pDX, IDC_EDIT8, v_height);
	DDX_Text(pDX, IDC_EDIT9, v_map);
}


BEGIN_MESSAGE_MAP(ZonesEditorPolygonDialog, CDialog)
	ON_BN_CLICKED(IDOK, &ZonesEditorPolygonDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDDELETE, &ZonesEditorPolygonDialog::OnBnClickedDelete)
END_MESSAGE_MAP()


// ZonesEditorPolygonDialog message handlers

void ZonesEditorPolygonDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void ZonesEditorPolygonDialog::OnBnClickedDelete()
{
	EndDialog(IDDELETE);
}
