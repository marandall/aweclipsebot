// TreeCtrlGpx.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "TreeCtrlGpx.h"


// TreeCtrlGpx

IMPLEMENT_DYNAMIC(TreeCtrlGpx, CTreeCtrl)

TreeCtrlGpx::TreeCtrlGpx()
{

}

TreeCtrlGpx::~TreeCtrlGpx()
{
}


BEGIN_MESSAGE_MAP(TreeCtrlGpx, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, &TreeCtrlGpx::OnTvnKeydown)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &TreeCtrlGpx::OnTvnSelchanged)
	ON_NOTIFY_REFLECT(TVN_SELCHANGING, &TreeCtrlGpx::OnTvnSelchanging)
	ON_NOTIFY_REFLECT(NM_CLICK, &TreeCtrlGpx::OnNMClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &TreeCtrlGpx::OnNMDblclk)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

HTREEITEM TreeCtrlGpx::GetHitTest()
{
	POINT point;
    GetCursorPos (&point);
	ScreenToClient(&point);
    TVHITTESTINFO HitTest;
    
	/* set up hit-test*/  
    HitTest.pt.x = point.x;
    HitTest.pt.y = point.y;

	HTREEITEM hti = this->HitTest(&HitTest);
	return hti;
}

HTREEITEM TreeCtrlGpx::insert_node(String Text, String Key, String Parent)
{
	HTREEITEM hti = InsertItem(Text, m_nodes[Parent]);
	m_nodes[Key] = hti;
	return hti;
}

HTREEITEM TreeCtrlGpx::insert_node(String Text, String Key, HTREEITEM Parent)
{
	HTREEITEM hti = InsertItem(Text, Parent);
	m_nodes[Key] = hti;
	return hti;
}

void TreeCtrlGpx::cascade_check(HTREEITEM HTI)
{
	// nothing
	if (HTI == NULL)
		return;

	// lookup the hit test
	HTREEITEM hti = HTI;
	HTREEITEM check = hti;

	if (hti == NULL)
		return;

	// switch to the first item
	hti = GetNextItem(hti, TVGN_CHILD);

	// no item
	if (hti == NULL)
		return;

	// checked
	BOOL checked = GetCheck(HTI) ? FALSE : TRUE;

	// check everything
	while (hti)
	{
		SetCheck(hti, checked);
		hti = GetNextItem(hti, TVGN_NEXT);
	}
}
// TreeCtrlGpx message handlers



void TreeCtrlGpx::OnTvnKeydown(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);
	*pResult = 0;

	// cascade check
	if (pTVKeyDown->wVKey == VK_SPACE)
	{
		cascade_check(GetSelectedItem());
	}
}

void TreeCtrlGpx::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;

	// check
	if (pNMTreeView->itemNew.hItem)
	{
		cascade_check(pNMTreeView->itemNew.hItem);
	}
}

void TreeCtrlGpx::OnTvnSelchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;

	// new item change
	if (pNMTreeView->itemNew.hItem)
		cascade_check(pNMTreeView->itemNew.hItem);
}

void TreeCtrlGpx::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	// checkbox
	HTREEITEM hti = GetHitTest();
	cascade_check(hti);
}

void TreeCtrlGpx::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void TreeCtrlGpx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	// CTreeCtrl::OnLButtonDblClk(nFlags, point);
}
