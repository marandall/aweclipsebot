// PropertyDetailDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "PropertyDetailDialog.h"
#include "property_object.h"
#include "property_objects.h"


// PropertyDetailDialog dialog

IMPLEMENT_DYNAMIC(PropertyDetailDialog, CDialog)

PropertyDetailDialog::PropertyDetailDialog(CWnd* pParent /*=NULL*/)
	: CDialog(PropertyDetailDialog::IDD, pParent)
{

}

PropertyDetailDialog::~PropertyDetailDialog()
{
}

void PropertyDetailDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, c_list);
}


BEGIN_MESSAGE_MAP(PropertyDetailDialog, CDialog)
	ON_BN_CLICKED(IDOK, &PropertyDetailDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// PropertyDetailDialog message handlers

void PropertyDetailDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL PropertyDetailDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// the model information
	c_list.InsertColumn(0, _T("Owner"),			0, 80);
	c_list.InsertColumn(1, _T("Model"),			0, 100);
	c_list.InsertColumn(2, _T("Position"),		0, 150);
	c_list.InsertColumn(3, _T("About"),			0, 400);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void PropertyDetailDialog::viewList(Property::Objects& ObjList)
{
	for (Property::Objects::iterator ptr = ObjList.begin(); ptr != ObjList.end(); ptr++)
	{
		int id = c_list.InsertItem(c_list.GetItemCount(), ToString(ptr->getOwner()));
		c_list.EditItem(id, 1, ptr->getModelDescriptor());
		c_list.EditItem(id, 2, ptr->getCoordinates());
		c_list.EditItem(id, 3, ptr->getDetails());
	}
}
