// SerializationEditorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "SerializationEditorDialog.h"
#include "awsdk_group_map.h"

// SerializationEditorDialog dialog
const int UWM_CHECK_ALL = WM_USER + 304;

IMPLEMENT_DYNAMIC(SerializationEditorDialog, CDialog)

SerializationEditorDialog::SerializationEditorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(SerializationEditorDialog::IDD, pParent)
{
	m_group_map = NULL;
}

SerializationEditorDialog::~SerializationEditorDialog()
{
}

void SerializationEditorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, c_Tree);
	DDX_Control(pDX, IDC_FILE_REPRESENTATION, c_Actual);
}


BEGIN_MESSAGE_MAP(SerializationEditorDialog, CDialog)
	ON_BN_CLICKED(IDOK, &SerializationEditorDialog::OnBnClickedOk)
	ON_NOTIFY(TVN_KEYDOWN, IDC_TREE1, &SerializationEditorDialog::OnTree_KeyDown)
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &SerializationEditorDialog::OnTree_RightClick)
	ON_BN_CLICKED(IDC_CHECKALL, &SerializationEditorDialog::OnBnClickedCheckall)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &SerializationEditorDialog::OnTree_SelChanged)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &SerializationEditorDialog::OnTree_Clicked)
END_MESSAGE_MAP()


// SerializationEditorDialog message handlers
void SerializationEditorDialog::UpdateResult()
{
	AW::EnhancedINI eni;
	// go through each item, checking it
	for (GroupMap::iterator gPtr = m_map.begin(); 
		gPtr != m_map.end(); 
		gPtr++)
	{
		// children
		for (TrackerMap::iterator tPtr = gPtr->second.begin();
			tPtr != gPtr->second.end();
			tPtr++)
		{
			// only copied checked items
			if (c_Tree.GetCheck(tPtr->second.handle) != TRUE)
				continue;

			// site to site copy
			eni[tPtr->second.IniGroup][tPtr->second.Key]
				= m_source_data[tPtr->second.IniGroup][tPtr->second.Key]; 
		}
	}

	// return our new ini
	m_source_data = eni;
}

void SerializationEditorDialog::create_serialization_tree()
{
	// go through each of the external keys
	for (Serializer::SectionMap::iterator smPtr = m_source_data.list.begin();
		smPtr != m_source_data.list.end();
		smPtr++)
	{
		// insert the root node
		HTREEITEM hGroup = c_Tree.InsertItem((LPCSTR)smPtr->first);
		c_Tree.SetCheck(hGroup, TRUE);

		for (Serializer::KeyMap::iterator kpPtr = smPtr->second.keys.begin();
			kpPtr != smPtr->second.keys.end();
			smPtr++)
		{
			// insert the child node
			HTREEITEM hChild = c_Tree.InsertItem((LPCSTR)kpPtr->second, hGroup);
			c_Tree.SetCheck(hChild, TRUE);

			// insert item
			Tracker& tr = m_map[smPtr->first][kpPtr->first];
			tr.handle = hChild;
			tr.Key	  = kpPtr->first;
			tr.Group  = smPtr->first;
			tr.IniGroup = smPtr->first;
		}
	}
}

void SerializationEditorDialog::create_serialization_tree(AW::GroupMap& GM)
{
	for (AW::GroupMap::g_iterator gPtr = GM.g_begin(); 
		gPtr != GM.g_end();
		gPtr++)
	{
		// insert the root node
		HTREEITEM hGroup = c_Tree.InsertItem(*gPtr);
		c_Tree.SetCheck(hGroup, TRUE);

		// go through each child
		for (AW::GroupMap::iterator cPtr = GM.begin(); 
			cPtr != GM.end();
			cPtr++)
		{
			// check equal types
			if (cPtr->Group != *gPtr)
				continue;

			// insert the child node
			HTREEITEM hChild = c_Tree.InsertItem(cPtr->Label, hGroup);
			
			// insert item
			Tracker& tr = m_map[*gPtr][cPtr->Child];
			tr.handle = hChild;
			tr.Key	  = cPtr->Child;
			tr.Group  = *gPtr;
			tr.IniGroup = cPtr->IniGroup;
		}

		// expanderus
		c_Tree.Expand(hGroup, TVE_EXPAND);
	}

	// first visible
	c_Tree.EnsureVisible(c_Tree.GetRootItem());
}

void SerializationEditorDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL SerializationEditorDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// set tree style to include checkboxes
	SetWindowLong(c_Tree.m_hWnd, GWL_STYLE, 
			GetWindowLong(c_Tree.m_hWnd, GWL_STYLE) | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_CHECKBOXES); 

	// create default style
	if (m_group_map)
		create_serialization_tree(*m_group_map);
	else
		create_serialization_tree();

	// file data yeah
	String text = m_source_data.getFileData();
	text.Replace("\n", "\r\n");
	c_Actual.SetWindowTextA(text);

	// tell us to update
	PostMessageA(UWM_CHECK_ALL, 0, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT SerializationEditorDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UWM_CHECK_ALL)
	{
		OnBnClickedCheckall();
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void SerializationEditorDialog::OnTree_KeyDown(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);

	// on keydown event
	if (pTVKeyDown->wVKey == VK_SPACE)
	{
	}

	*pResult = 0;
}

void SerializationEditorDialog::OnTree_RightClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	// go through each child

}

void SerializationEditorDialog::OnBnClickedCheckall()
{
	// go through each item, checking it
	for (GroupMap::iterator gPtr = m_map.begin(); 
		gPtr != m_map.end(); 
		gPtr++)
	{
		// children
		for (TrackerMap::iterator tPtr = gPtr->second.begin();
			tPtr != gPtr->second.end();
			tPtr++)
		{
			c_Tree.SetCheck(tPtr->second.handle, TRUE);
		}
	}

	// check each root node
	HTREEITEM root	= c_Tree.GetRootItem();
	HTREEITEM hti 	= root; //c_Tree.GetNextItem(root, TVGN_CHILD);

	// go through each item
	while (hti)
	{
		c_Tree.SetCheck(hti, TRUE);
		hti = c_Tree.GetNextItem(hti, TVGN_NEXT);
	}
}

void SerializationEditorDialog::OnOK()
{
	UpdateResult();
	CDialog::OnOK();
}

void SerializationEditorDialog::OnTree_SelChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
}

void SerializationEditorDialog::OnTree_Clicked(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}
