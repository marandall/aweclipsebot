#pragma once

#include <vector>

// ListCtrlGpx

class ListCtrlGpx : public CListCtrl
{
	DECLARE_DYNAMIC(ListCtrlGpx)

public:
	ListCtrlGpx();
	virtual ~ListCtrlGpx();

protected:
	DECLARE_MESSAGE_MAP()

public:
	int		GetByItemData	(UINT_PTR ItemData);
	int		EditItem		(int ItemId, int SubItemId, const TCHAR* Text, int Icon = -1);
	int		GetByHitTest	();

public: // sorting class
	class Sorter;
	struct SortingParam
	{
		ListCtrlGpx* ctrl;
		Sorter*			 sorter;
	};

	class Sorter
	{
	public: // checksum
		virtual int sort(LPARAM Item1, LPARAM Item2, SortingParam* Options) = 0;
		static  int CommonInt(int X1, int X2)
		{
			if (X1 == X2) 
				return 0;
			else
				return X1 < X2 ? -1 : 1;
		}
	};

public: // static sorting routing
	static int StaticSorter(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

public: // sorting directions
	static const int SORT_ASCENDING = 0;
	static const int SORT_DECENDING	= 1;

public: // sorting parameters
	int	sort_on_col;							// which col to operate the sort on
	int sort_order;								// which direction
	std::vector<Sorter*> sorters;	// array of sorting routines

public: // sorting functions
	void sort(int Col, int Direction);
	void sortToggle(int Col);
	void resort();
	void addSorter(int Col, Sorter* Base);

protected:
	CImageList	m_imgList;
	struct ImageItem {
		String	id;
		int		index; };
	std::vector<ImageItem> m_Images;

public:
	DWORD	InitIcons(int Type = ILC_COLOR24);
	DWORD	InsertIcon(const TCHAR* FilePath, const char* Name = "");
	DWORD	InsertIconByHandle(HICON IconHandle, const char* Name = "");
	DWORD	InsertIconByBitmapResource(UINT BitmapRes, const char* Name = "");
	DWORD	InsertIconByIconResource(UINT BitmapRes, const char* Name = "");
	
	int		getIconKey(const char* Key);

public: // style
	ListCtrlGpx& useFullRowSelect()
	{
		SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);
		return *this;
	}

	ListCtrlGpx& useGridlines()
	{
		SetExtendedStyle(GetExtendedStyle() | LVS_EX_GRIDLINES);
		return *this;
	}

	
};