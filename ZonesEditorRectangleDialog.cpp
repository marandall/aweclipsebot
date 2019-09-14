// ZonesEditorRectangleDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "ZonesEditorRectangleDialog.h"


// ZonesEditorRectangleDialog dialog

IMPLEMENT_DYNAMIC(ZonesEditorRectangleDialog, CDialog)

ZonesEditorRectangleDialog::ZonesEditorRectangleDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ZonesEditorRectangleDialog::IDD, pParent)
	, v_Name(_T(""))
	, v_Top(_T(""))
	, v_Bottom(_T(""))
{

}

ZonesEditorRectangleDialog::~ZonesEditorRectangleDialog()
{
}

void ZonesEditorRectangleDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT7, v_Name);
	DDX_Text(pDX, IDC_EDIT1, v_Top);
	DDX_Text(pDX, IDC_EDIT6, v_Bottom);
}


BEGIN_MESSAGE_MAP(ZonesEditorRectangleDialog, CDialog)
	ON_BN_CLICKED(IDDELETE, &ZonesEditorRectangleDialog::OnBnClickedDelete)
END_MESSAGE_MAP()


// ZonesEditorRectangleDialog message handlers

void ZonesEditorRectangleDialog::OnBnClickedDelete()
{
	EndDialog(IDNO);
}
