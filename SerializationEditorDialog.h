#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Serializer.h"
#include "awsdk_support.h"
#include <map>
#include "TreeCtrlGpx.h"

// SerializationEditorDialog dialog
namespace AW
{
	class GroupMap;
	class EnhancedINI;
}

class SerializationEditorDialog : public CDialog
{
	DECLARE_DYNAMIC(SerializationEditorDialog)

public:
	SerializationEditorDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~SerializationEditorDialog();

// Dialog Data
	enum { IDD = IDD_SERIALIZATION_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public: // tracking data
	struct Tracker { HTREEITEM handle; String Key, Group, IniGroup; };
	typedef			std::map<String, Tracker>	TrackerMap;
	typedef			std::map<String, TrackerMap>	GroupMap;
	GroupMap		m_map;

public: // the serialization data
	AW::EnhancedINI	m_source_data;
	AW::GroupMap*	m_group_map;

public: // setup
	void setup(AW::EnhancedINI& SZ, AW::GroupMap* GM = NULL) 
	{
		m_source_data	= SZ;
		m_group_map		= GM;
	}

	void UpdateResult();
	AW::EnhancedINI& Result() { return m_source_data; }

protected:
	void		create_serialization_tree();
	void		create_serialization_tree(AW::GroupMap& GM);

public:
	TreeCtrlGpx c_Tree;
	CEdit c_Actual;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnTree_KeyDown(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTree_RightClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckall();
protected:
	virtual void OnOK();
public:
	afx_msg void OnTree_SelChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTree_Clicked(NMHDR *pNMHDR, LRESULT *pResult);
};
