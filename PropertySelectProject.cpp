// PropertySelectProject.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "PropertySelectProject.h"


// PropertySelectProject dialog

IMPLEMENT_DYNAMIC(PropertySelectProject, CDialog)

PropertySelectProject::PropertySelectProject(CWnd* pParent /*=NULL*/)
	: CDialog(PropertySelectProject::IDD, pParent)
{

}

PropertySelectProject::~PropertySelectProject()
{
}

void PropertySelectProject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROJECT_LIST, c_list);
}


BEGIN_MESSAGE_MAP(PropertySelectProject, CDialog)
	ON_BN_CLICKED(IDCANCEL, &PropertySelectProject::OnBnClickedCancel)
	ON_NOTIFY(NM_DBLCLK, IDC_PROJECT_LIST, &PropertySelectProject::OnList_DoubleClick)
END_MESSAGE_MAP()


// PropertySelectProject message handlers
#include "property_projects.h"
#include "property_project.h"

void PropertySelectProject::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

BOOL PropertySelectProject::OnInitDialog()
{
	CDialog::OnInitDialog();

	// setup the list
	c_list.InsertColumn(0, "Project Name", 0, 300);
	
	// add each item
	using namespace Property;
	for (Projects::iterator itr = g_Projects.begin(); itr != g_Projects.end(); itr++)
	{
		int nId = c_list.InsertItem(c_list.GetItemCount(), itr->id);
		c_list.SetItemData(nId, (DWORD_PTR)&*itr);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void PropertySelectProject::OnList_DoubleClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// get the item
	int nId = c_list.GetByHitTest();
	if (nId == -1) return;

	// select this item
	Property::Project* pProject = Property::g_Projects.checkProjectPtr((Property::Project*)c_list.GetItemData(nId));
	if (pProject)
	{
		selected_id = pProject->getId();
		OnOK();
	}

	*pResult = 0;
}
