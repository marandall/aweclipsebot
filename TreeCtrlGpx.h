#pragma once
#include <map>

// TreeCtrlGpx

class TreeCtrlGpx : public CTreeCtrl
{
	DECLARE_DYNAMIC(TreeCtrlGpx)

public:
	TreeCtrlGpx();
	virtual ~TreeCtrlGpx();

	HTREEITEM	GetHitTest();
	void		cascade_check(HTREEITEM HTI);

public: // map array
	typedef std::map<String, HTREEITEM> TreeMap;
	TreeMap m_nodes;

public: // adding handlers
	HTREEITEM insert_node(String Text, String Key, String Parent);
	HTREEITEM insert_node(String Text, String Key, HTREEITEM Parent);
	HTREEITEM operator[](String Key) { return m_nodes[Key]; }

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTvnKeydown(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


