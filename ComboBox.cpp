// ComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "ComboBox.h"


// ComboBox

IMPLEMENT_DYNAMIC(ComboBox, CComboBox)

ComboBox::ComboBox()
{

}

ComboBox::~ComboBox()
{
}


BEGIN_MESSAGE_MAP(ComboBox, CComboBox)
END_MESSAGE_MAP()



// ComboBox message handlers
int ComboBox::InsertWithData(CStringA Text, int ItemData)
{
	int id = InsertString(GetCount(), Text);
	SetItemData(id, (DWORD_PTR)ItemData);
	return id;
}

int ComboBox::InsertByMap(std::map<int, CStringA>& MapData)
{
	typedef std::map<int, CStringA> mm2s;
	for (mm2s::iterator i2sPtr = MapData.begin(); i2sPtr != MapData.end(); i2sPtr++)
		InsertWithData(i2sPtr->second, i2sPtr->first);
	return 0;
}

int ComboBox::Insert(StringEnum& E)
{
	for (StringEnum::iterator itr = E.begin(); itr != E.end(); itr++)
		InsertWithData(itr->second, itr->first);
	return 0;
}

int ComboBox::SelectByItemData(int ID)
{
	// search for the controls
	for (int i = 0; i < GetCount(); i++)
	{
		int itemData = GetItemData(i);
		if (itemData == ID)
			return SetCurSel(i);
	}

	// create the insert
	CString def;
	def.Format(_T("Default (%d)"), ID);
	return -1;
}
