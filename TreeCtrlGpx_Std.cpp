// TreeCtrlGpx_Std.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "TreeCtrlGpx_Std.h"


// TreeCtrlGpx_Std

IMPLEMENT_DYNAMIC(TreeCtrlGpx_Std, CTreeCtrl)

TreeCtrlGpx_Std::TreeCtrlGpx_Std()
{

}

TreeCtrlGpx_Std::~TreeCtrlGpx_Std()
{
}


BEGIN_MESSAGE_MAP(TreeCtrlGpx_Std, CTreeCtrl)
END_MESSAGE_MAP()



// TreeCtrlGpx_Std message handlers
HTREEITEM TreeCtrlGpx_Std::GetHitTest()
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

HTREEITEM TreeCtrlGpx_Std::insert_node(String Text, String Key, String Parent)
{
	HTREEITEM hti = InsertItem(Text, m_nodes[Parent]);
	m_nodes[Key] = hti;
	return hti;
}

HTREEITEM TreeCtrlGpx_Std::insert_node(String Text, String Key, HTREEITEM Parent)
{
	HTREEITEM hti = InsertItem(Text, Parent);
	m_nodes[Key] = hti;
	return hti;
}

