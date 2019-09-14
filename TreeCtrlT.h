#pragma once
#include <vector>

// CTreeCtrlT

template<typename TYPE>
class TreeCtrlT
{
private:
	CTreeCtrl*	m_pTree;

public:
	TreeCtrlT(CTreeCtrl& x) { m_pTree = &x; }
	void Attach(CTreeCtrl& x) { m_pTree = &x; }
	TreeCtrlT() { }
	virtual ~TreeCtrlT() { };

private:
	struct TreeNode {
		TYPE		key_id;
		HTREEITEM	node_id; };
	std::vector<TreeNode> m_nodes;

private:
	void Allocate(TYPE Key, HTREEITEM Handle) {
		TreeNode node;
		node.key_id		= Key;
		node.node_id	= Handle;
		m_nodes.push_back(node);
	}

	TreeNode* SearchNode(TYPE Key) {
		for (size_t i = 0; i < m_nodes.size(); i++)
			if (m_nodes[i].key_id == Key)
				return &m_nodes[i];
		return NULL;
	}

public:
	HTREEITEM InsertNode(TYPE Key, LPCSTR Text, 
					int Icon = 0) {
		HTREEITEM hItem = m_pTree->InsertItem(Text, TVI_ROOT);
		if (!hItem)
			return NULL;
		Allocate(Key, hItem);
		return hItem;
	}

	bool InsertNode(TYPE Key, TYPE Parent, 
					LPCSTR Text, int Icon = 0) {
		TreeNode* pParent = SearchNode(Parent);
		if (pParent == NULL)
			return false;
		HTREEITEM hItem = m_pTree->InsertItem(Text, pParent->node_id);
		if (!hItem)
			return false;
		Allocate(Key, hItem);
		return true;
	}

	bool DeleteNode(TYPE Key) {
		TreeNode* pParent = SearchNode(Key);
		if (!pParent)
			return false;
		m_pTree->DeleteItem(pParent->node_id);
		return true;
	}

	void DeleteAll() {
		m_pTree->DeleteAllItems();
		m_nodes.clear();
	}

	bool NodeChecked(TYPE Key) {
		TreeNode* pParent = SearchNode(Key);
		if (!pParent)
			return false;
		return m_pTree->GetCheck(pParent->node_id) ? true : false;
	}

	bool setNodeChecked(TYPE Key, bool Checked) {
		TreeNode* pParent = SearchNode(Key);
		if (!pParent)
			return false;
		return m_pTree->SetCheck(pParent->node_id, (Checked == true ? TRUE : FALSE)) ? TRUE : FALSE;
	}

	bool SearchHandle(HTREEITEM Handle, TYPE& input) {
		for (size_t i = 0; i < m_nodes.size(); i++)
			if (m_nodes[i].node_id == Handle) {
				input = m_nodes[i].key_id;
				return true;
			}
		return false;
	}
};


