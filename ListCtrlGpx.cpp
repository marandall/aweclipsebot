// ListCtrlGpx.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "ListCtrlGpx.h"


// ListCtrlGpx

IMPLEMENT_DYNAMIC(ListCtrlGpx, CListCtrl)

ListCtrlGpx::ListCtrlGpx()
{
}

ListCtrlGpx::~ListCtrlGpx()
{
}


BEGIN_MESSAGE_MAP(ListCtrlGpx, CListCtrl)
END_MESSAGE_MAP()

int CALLBACK StaticSorter(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	ListCtrlGpx::SortingParam* param = (ListCtrlGpx::SortingParam*)lParamSort;
	
	int way = param->sorter->sort(lParam1, lParam2, param);
	if (param->ctrl->sort_order != ListCtrlGpx::SORT_ASCENDING)
	{
		way*= -1;
	}

	// sorting solution is good
	return way;
}

void ListCtrlGpx::sort(int Col, int Direction)
{
	// get the sorter
	if (Col >= sorters.size())
		return;

	Sorter* pSorter = sorters[Col];
	if (pSorter == 0)
		return;
	
	// alloc arrange
	sort_on_col = Col;
	sort_order = Direction;

	// setup sorting object
	SortingParam param;
	param.ctrl = this;
	param.sorter = pSorter;

	// call sorting procedure
	SortItems(&::StaticSorter, (DWORD_PTR)&param);
}

void ListCtrlGpx::sortToggle(int Col)
{
	if (Col == sort_on_col)
	{
		sort(Col, sort_order == SORT_ASCENDING ? SORT_DECENDING : SORT_ASCENDING);
	}
	else
	{
		sort(Col, SORT_ASCENDING);
	}
}

void ListCtrlGpx::resort()
{
	sort(sort_on_col, sort_order);
}

void ListCtrlGpx::addSorter(int Col, ListCtrlGpx::Sorter* Base)
{
	// expand the sorting routine if needs be
	if (Col >= sorters.size())
		sorters.resize(Col + 1);

	// push into the block
	sorters[Col] = Base;
}

// ListCtrlGpx message handlers
int	ListCtrlGpx::GetByItemData(UINT_PTR ItemData) {
	int count = GetItemCount();
	for (int i = 0; i < count; i++)
		if (GetItemData(i) == ItemData)
			return i;
	return -1;
}

DWORD ListCtrlGpx::InitIcons(int Type)
{
	m_imgList.Create(16, 16, ILC_COLOR24, 20, 5);
	SetImageList(&m_imgList, LVSIL_SMALL);
	return 0;
}

int ListCtrlGpx::EditItem(int ItemId, int SubItemId, const TCHAR* Text, int Icon) {
	/* variables */
	LV_ITEM aLv;

	/* for each item*/
	aLv.iItem		= ItemId;
	aLv.iSubItem	= SubItemId;
	aLv.mask		= LVIF_TEXT | (Icon == -1 ? 0 : LVIF_IMAGE);
	aLv.iImage		= Icon;
	aLv.pszText		= (LPSTR)Text;

	/* set each item */
	SetItem(&aLv);
	return 0;
}

int ListCtrlGpx::GetByHitTest()
{
	POINT point;
    GetCursorPos (&point);
	ScreenToClient(&point);
    HDHITTESTINFO HitTest;
    
	/* set up hit-test*/  
    HitTest.pt.x = point.x;
    HitTest.pt.y = point.y;

	return this->HitTest(point);
}

DWORD ListCtrlGpx::InsertIconByHandle(HICON IconHandle, const char* Name)
{
	int index = m_imgList.Add(IconHandle);

	/* create an item for this */
	ImageItem imgItem = { Name, index };
	m_Images.push_back(imgItem);
	return index;
}

DWORD	ListCtrlGpx::InsertIconByIconResource(UINT BitmapRes, const char* Name)
{
	int index = m_imgList.Add(AfxGetApp()->LoadIconA(BitmapRes));
	ImageItem imgItem = { Name, index };
	m_Images.push_back(imgItem);
	return index;
}

DWORD	ListCtrlGpx::InsertIconByBitmapResource(UINT BitmapRes, const char* Name)
{
	CBitmap b;
	BOOL bc = b.LoadBitmapA(BitmapRes);

	int index = m_imgList.Add(&b, RGB(0x0, 0x0, 0x0));

	/* create an item for this */
	ImageItem imgItem = { Name, index };
	m_Images.push_back(imgItem);
	return index;
}

DWORD ListCtrlGpx::InsertIcon(const TCHAR* FilePath, const char* Name) {
	/* create the bitmap */
	CBitmap bmp;
	HANDLE hnd = LoadImageA(NULL, FilePath, IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);

	/* check he handle for failure */
	if (hnd == NULL)
		return GetLastError();
	else
		bmp.Attach(hnd);

	/* add a null icon or a proper icon, don't really care */
	int index = m_imgList.Add(&bmp, (COLORREF)0x0);

	/* create an item for this */
	ImageItem imgItem = { Name, index };
	m_Images.push_back(imgItem);
	return index;
}

int	ListCtrlGpx::getIconKey(const char* Key)
{
	for(size_t i = 0; i < m_Images.size(); i++)
		if (m_Images[i].id == Key)
			return m_Images[i].index;
	return 0;
}