#pragma once
#include <map>
#include "stringenum.h"

// ComboBox

class ComboBox : public CComboBox
{
	DECLARE_DYNAMIC(ComboBox)

public:
	ComboBox();
	virtual ~ComboBox();

protected:
	DECLARE_MESSAGE_MAP()

public: // insert and selection functions
	int InsertWithData(CStringA Text, int ItemData);
	int InsertByMap(std::map<int, CStringA>& MapData);
	int Insert(StringEnum& E);
	int SelectByItemData(int ID);
};


