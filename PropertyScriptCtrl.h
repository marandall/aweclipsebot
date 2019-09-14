#pragma once
#include "resource.h"
#include "TextBox.h"
#include "colourbutton.h"
#include "ComboBox.h"
#include "awsdk_colours.h"
#include <vector>
#include <list>
#include <map>
#include "afxcmn.h"
#include "afxwin.h"
#include "OutputFormatCtrl.h"

// PropertyScriptCtrl dialog

class PropertyScriptCtrl : public CDialog
{
	DECLARE_DYNAMIC(PropertyScriptCtrl)

protected: // management controls
	// group element is a single control on the page
	class GroupElement
	{
	public:
		int			ctrl_id;	// the id for this control
		CString		text;		// the display text
		CWnd*		pWnd;		// the control window
		CStatic*	pLabel;		// the label element
		int			cType;		// the control type;
		int			height;		// the height of the control
		int			width;		// the width of the control
	};

	// group controls
	class GroupElements
	{
	public: // types
		typedef std::list<GroupElement>	List;
		typedef List::iterator			iterator;
		typedef List::reference			reference;
	
	public: // access
		List		list;
		iterator	begin() { return list.begin();	}
		iterator	end()	{ return list.end();	}
		size_t		size()	{ return list.size();	}
		void		push_back(GroupElement& E) { list.push_back(E); }

	public:
		CStringA	title;

	public:
		~GroupElements()
		{
			while (size())
			{
				list.front().pLabel	->DestroyWindow();
				if (list.front().pWnd)
				{
					list.front().pWnd	->DestroyWindow();
					delete list.front().pWnd;
				}
				delete list.front().pLabel;
				list.erase(list.begin());
			}
		}
	};

	
	typedef std::map<int, GroupElements>	 PageGroups;
	PageGroups m_groups;

	// create pages of controls
public:
	CStatic*					create_label	(int Group, CStringA Key, CStringA Text);
	TextBox*					create_edit		(int Group, CStringA Key, CStringA Text);
	ComboBox*					create_combo	(int Group, CStringA Key, CStringA Text);
	CButton*					create_check	(int Group, CStringA Key, CStringA Text);
	CButton*					create_tristate	(int Group, CStringA Key, CStringA Text);
	CWnd*							create_special	(int Group, CStringA Key, CWnd* SpecialObject, int Height, int Width, CStringA Text);
	ColourButton*			create_colour	(int Group, CStringA Key, CStringA Text);
	OutputFormatCtrl*	create_formmater(int Group, CStringA Key, CStringA Text);

	// display a page
	void			CreateAtPosition(CWnd& Parent, CWnd& Wnd);
	void			create_group(int Group, CStringA Name);
	void			finalise_groups();
	BOOL			show_group(int Group);
	int				current_group;
	int				getGroupHeight(int Group);

	// total height
	int				getHeight();

public:
	// setup counters for control creation
	int top					;
	int ELEMENT_SEPERATOR	;
	int ELEMENT_INSET		;
	int CONTROL_INSET		;


protected: // control fonts
	CFont		m_font;
	CFont		m_section_font;
	CFont		m_title_font;

protected: // scrolling
	CRect		m_rcOriginalRect;
	int			m_nScrollPos;
	int			m_nCurHeight;

public:
	PropertyScriptCtrl(CWnd* pParent = NULL);   // standard constructor
	virtual ~PropertyScriptCtrl();

// Dialog Data

	enum { IDD = IDD_PROPERTY_SCRIPT_CTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	CTabCtrl c_Tabs;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CStatic c_TitleText;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult);
};

class PropertyBuilderBase
{
public:
	virtual bool dialog_to_remote_attributes()
	{
		return true;
	}
		
	virtual void remote_attributes_to_dialog()
	{
	}

	virtual void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
	{
	}
};

class PropertyBuilderHelper
{
public:
	std::vector<PropertyBuilderBase*> list;			// the list of bases
	CTabCtrl*			tabs;						// the tab controls
	PropertyScriptCtrl*	window;						// the creation window
	CWnd*				tracking_rect;				// the CWnd of the tracking rectangle

	PropertyBuilderHelper()
	{
		window			= NULL;
		tabs			= NULL;
		tracking_rect	= NULL;
	}
	~PropertyBuilderHelper()
	{
		if (window)
		{
			window->DestroyWindow();
			delete window;
		}
	}

	void create_control(CWnd& Parent, CWnd& TrackingRect)
	{
		// create control
		window = new PropertyScriptCtrl(&Parent);
		window->Create(PropertyScriptCtrl::IDD, &Parent);
		window->ShowWindow(SW_SHOW);


		// find the tracking region
		tracking_rect = &TrackingRect;
		RECT rt;
		TrackingRect.GetWindowRect(&rt);
		Parent.ScreenToClient(&rt);
		window->MoveWindow(&rt);
	}

	void create_group(int Group, String Description, PropertyBuilderBase& Base)
	{
		// create the group with given description, add tab controls
		window->create_group(Group,	Description);
		if (tabs)
		{
			tabs->InsertItem(Group, CString(Description));
		}

		// create the group objects
		Base.create_objects(*window, Group);
		PropertyBuilderBase* pBase = &Base;
		list.push_back(pBase);
	}

	void attributes_to_dialog()
	{
		for (size_t i = 0; i < list.size(); i++)
			list[i]->remote_attributes_to_dialog();
	}

	void dialog_to_attributes()
	{
		for (size_t i = 0; i < list.size(); i++)
			list[i]->dialog_to_remote_attributes();
	}
};