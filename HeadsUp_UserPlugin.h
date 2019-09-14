#pragma once
#include "HeadsUp_User_AG.h"
#include "HeadsUp_Element.h"
#include "HeadsUp.h"


class Avatar;

namespace HeadsUp 
{ 
	//
	// FreeTray
	// @ Icons not bound by the conditions of the icon tray
	//

	class FreeTray : public Umbra::RemoteClass
	{
	public: // type def
		typedef std::list<Element> ListType;
		typedef ListType::iterator iterator;

	public: // list defenition
		ListType elements;

	public: // access
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);

	public: // helper
		Element* getElementById(int ID);
	};

	//
	// IconTray
	// @ Specific tray for a given users with auto formatting
	//

	class IconTray : 
		public AutoTemplates::UserData
	{
	protected: // pointers for the element and the class
		Avatar*			m_pUser;

	public: // constructors and deconstructors
		IconTray(Avatar* User);
		~IconTray();

	public: // editing functions
		void	expand();
		void	collapse();
		void	show();
		void	hide();

	public: // row changing elements
		Element	ic_row_up;
		Element	ic_row_down;

	protected: // maintain a list of the indexes
		typedef std::list<Element> List;
		std::vector<int>   visible_items;
		List	list;
		List	scripted_list;

	public: // visibility checking
		bool	isElementVisible(int Id);

	protected: // state tracking
		bool	is_dirty;
		bool	expanded;

	public: // modifications user.onZoneChange()
		void		hide_all			(void);
		void		reposition			(EclipseBot& Client);
		void		update_all			(void);
		void		update_element		(int ElementId);
		void		insert_at_position	(Element& EM, int Index);
		Element*	push_back(Element& Em);
		void		deleteItem(int ElementId);

	public: // searching
		Element*	search(std::string Key);
		Element*	lookup_id(int ElementId);	

	public: // element visibility
		bool		show_element				(int ElementId);
		bool		hide_element				(int ElementId);
		void		change_element_visibility	(int ElementId, bool Visible);
	};
}