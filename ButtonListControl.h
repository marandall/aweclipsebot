#pragma once
#include "ButtonListButtonControl.h"

// ButtonListControl dialog

class ButtonListControl : 
	public CDialog, 
	public EventBroadcaster
{
	DECLARE_DYNAMIC(ButtonListControl)

public: // the notification class
	class ClickNotificationMessage : public EventMessage
	{
	public:
		int clicked;

	public:
		ClickNotificationMessage(int Msg)
		{
			clicked = Msg;
		}
	};

public:
	ButtonListControl(CWnd* pParent = NULL);   // standard constructor
	virtual ~ButtonListControl();

	int			last_ctrl_id;
	int			max_height;
	CFont		header_font;
	int			scroll_pos;

public: // the event messages
	static const int LE_CLICKED = 1;

// Dialog Data
	enum { IDD = IDD_BUTTON_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public: // control options
	class Button
	{
	public:
		ButtonListButtonControl*	button;
		int							id;
	};

	class Category
	{
	public: // category name
		ButtonListButtonControl*	group_button;
		CStatic*					group_label;
		ButtonListControl*			parent;
		CStringA					name;
		bool						expanded;

	public: // group information
		typedef std::list<Button> ButtonList;
		ButtonList buttons;

	public: // deconstructor
		~Category();

	public: // add button
		ButtonListButtonControl* add(int Id, CString Name)
		{
			Button b;
			b.id	 = Id;

			// create the new button
			b.button = new ButtonListButtonControl();
			b.button->CreateEx(0, "BUTTON", NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_FLAT | BS_TEXT | BS_LEFT | BS_LEFTTEXT, 
							CRect(100, 0, 100, 16), parent, parent->last_ctrl_id++);
			b.button->SetFont(parent->GetFont());
			b.button->SetWindowText(CStringA(" - ") + Name);
			buttons.push_back(b);

			// return button
			return b.button;
		}
	};

protected:
	typedef std::list<Category>	CatList;
	CatList categories;

public: // categories
	Category& addCategory(CStringA Name, bool Expanded)
	{
		// return the category
		Category c;
		c.name			= Name;
		c.parent		= this;
		c.group_button	= 0;
		c.group_label	= 0;
		c.expanded		= Expanded;

		// create button
		c.group_button = new ButtonListButtonControl();
		c.group_button->CreateEx(0, "BUTTON", NULL, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_FLAT | BS_PUSHLIKE, 
						CRect(100, 0, 100, 16), c.parent, last_ctrl_id++);
		c.group_button->SetFont(&header_font);
		c.group_button->SetWindowText(Name);

		// add list
		categories.push_back(c);
		return categories.back();
	}

public: // expanding and collapsing
	void onButton(ButtonListButtonControl* B);
	void expandCategory(Category& C);
	void collapseCategory(Category& C);

	void resize();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
