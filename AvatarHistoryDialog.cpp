// AvatarHistoryDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "AvatarHistoryDialog.h"
#include "AvatarHistory.h"

// AvatarHistoryDialog dialog

IMPLEMENT_DYNAMIC(AvatarHistoryDialog, CDialog)

AvatarHistoryDialog::AvatarHistoryDialog(CWnd* pParent /*=NULL*/)
	: CDialog(AvatarHistoryDialog::IDD, pParent)
{

}

AvatarHistoryDialog::~AvatarHistoryDialog()
{
}

void AvatarHistoryDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, c_DataList);
	DDX_Control(pDX, IDC_HISTORY, c_History);
	DDX_Control(pDX, IDC_ATTRIBUTE, c_SelectedText);
}


BEGIN_MESSAGE_MAP(AvatarHistoryDialog, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_HISTORY, &AvatarHistoryDialog::OnHistoryList_ItemChanged)
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()


// AvatarHistoryDialog message handlers
using namespace History;

void AvatarHistoryDialog::OnHistoryList_ItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	if (!((pNMLV->uChanged & LVIF_STATE) && (pNMLV->uNewState & LVIS_SELECTED)))
		return;

	/* find header */
	AvatarArticle* pItem = History::AvatarHistory::getInstance().search(pNMLV->iItem);
	if (pItem)
		DisplayItem(*pItem);
}

void AvatarHistoryDialog::DisplayItem(AvatarArticle& Item)
{
	c_DataList.SetItemText(m_si_name,			1,	Item.getName());
	c_DataList.SetItemText(m_si_session,		1,	ToString(Item.getSession()));

	c_DataList.SetItemText(m_si_entered,		1,	MakeGuiTime(Item.getEnterTime(), "%#c"));
	c_DataList.SetItemText(m_si_entered_pos,	1,	Item.getEnterPosition().getCoordinates());

	c_DataList.SetItemText(m_si_citizen,		1,	ToString(Item.getCitizen()));
	c_DataList.SetItemText(m_si_privilege,		1,	ToString(Item.getPrivilege()));
	c_DataList.SetItemText(m_si_privname,		1,	Item.getPrivName());
	c_DataList.SetItemText(m_si_ip,				1,	Item.getAddress());
	c_DataList.SetItemText(m_si_dns,			1,	Item.getDns());

	if (Item.getExitTime() == 0) {
		c_DataList.SetItemText(m_si_exited,		1,	"");
		c_DataList.SetItemText(m_si_exited_pos,	1,	"");
	} else {
		c_DataList.SetItemText(m_si_exited,		1,	MakeGuiTime(Item.getExitTime(), "%#c"));
		c_DataList.SetItemText(m_si_exited_pos,	1,	Item.getExitPosition().getCoordinates());
	}
}

BOOL AvatarHistoryDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	c_History.InsertColumn(0, _T("Avatar"), 0, 80);
	c_History.InsertColumn(1, _T("Time"),   0, 100);

	History::AvatarHistory& history = History::AvatarHistory::getInstance();

	// iterate each element
	for (AvatarHistory::iterator itr = history.begin(); itr != history.end(); itr++)
	{
		AvatarArticle& aa = *itr;
		int item = c_History.InsertItem(c_History.GetItemCount(),  aa.getName());
		c_History.SetItemText(item, 1, MakeGuiTime(aa.getEnterTime()));
		c_History.SetItemData(item, (DWORD_PTR)aa.getUniqueId());
	}

	/* set list items */
	int pos = 0;
	c_DataList.InsertColumn(0, _T("Type"), 0, 120);
	c_DataList.InsertColumn(1, _T("Data"), 0, 220);
	m_si_name		 = c_DataList.InsertItem(pos++,	_T("Name"));		
	m_si_session	 = c_DataList.InsertItem(pos++,	_T("Session"));
	m_si_entered	 = c_DataList.InsertItem(pos++,	_T("Timestamp"));
	m_si_entered_pos = c_DataList.InsertItem(pos++,	_T("Enter Position"));
	m_si_citizen	 = c_DataList.InsertItem(pos++,	_T("Citizen"));
	m_si_privilege	 = c_DataList.InsertItem(pos++,	_T("Privilege"));
	m_si_privname	 = c_DataList.InsertItem(pos++,	_T("Privilege Name"));
	m_si_ip			 = c_DataList.InsertItem(pos++,	_T("IP Address"));
	m_si_dns		 = c_DataList.InsertItem(pos++,	_T("Reverse DNS"));
	m_si_exited		= c_DataList.InsertItem(pos++,	_T("Exit Timestamp"));
	m_si_exited_pos = c_DataList.InsertItem(pos++,	_T("Exit Position"));


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void AvatarHistoryDialog::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	delete this;
}

