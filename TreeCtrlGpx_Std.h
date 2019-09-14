#pragma once


// TreeCtrlGpx_Std

class TreeCtrlGpx_Std : public CTreeCtrl
{
	DECLARE_DYNAMIC(TreeCtrlGpx_Std)

public:
	TreeCtrlGpx_Std();
	virtual ~TreeCtrlGpx_Std();

public:
	HTREEITEM	GetHitTest();

public: // map array
	typedef std::map<String, HTREEITEM> TreeMap;
	TreeMap m_nodes;

public: // adding handlers
	HTREEITEM insert_node(String Text, String Key, String Parent);
	HTREEITEM insert_node(String Text, String Key, HTREEITEM Parent);
	HTREEITEM operator[](String Key) { return m_nodes[Key]; }

protected:
	DECLARE_MESSAGE_MAP()
};


