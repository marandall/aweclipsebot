#include "stdafx.h"
#include "HeadsUp_UserPlugin.h"
#include <algorithm>

namespace HeadsUp
{
	//
	//
	//
	//

	bool FreeTray::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		return false;
	}

	bool FreeTray::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		return false;
	}

	Element* FreeTray::getElementById(int ID)
	{
		for (iterator i2 = elements.begin(); i2 != elements.end(); i2++)
			if (i2->id == ID)
				return &*i2;
		return 0;
	}

	//
	//
	//
	//

	IconTray::IconTray(Avatar* User)
	{
		m_pUser = User;
	}

	IconTray::~IconTray()
	{
	}

	void IconTray::expand()
	{
		setExpanded(true);
		update_all();
	}
	
	void IconTray::collapse()
	{
		setExpanded(false);
		update_all();
	}

	void IconTray::show()
	{
		setVisible(true);
		update_all();
	}
	
	void IconTray::hide()
	{
		setVisible(false);
		update_all();
	}

	void IconTray::hide_all()
	{
		for (List::iterator ptrEm = list.begin(); ptrEm != list.end(); ptrEm++)
		{
			if (ptrEm->is_visible)
			{
				ptrEm->is_visible = false;
				m_pUser->DestroyHUD(ptrEm->id);
			}
		}
	}

	Element* IconTray::lookup_id(int ElementId)
	{
		for (List::iterator ptrEm = list.begin(); ptrEm != list.end(); ptrEm++)
			if (ptrEm->id == ElementId)
				return &(*ptrEm);
		return NULL;
	}

	bool IconTray::isElementVisible(int Id)
	{
		for (size_t i = 0; 
			i < visible_items.size(); 
			i++)
		{
			if (visible_items[i] == Id)
				return true;
		}
		return false;
	}

	void IconTray::insert_at_position(Element& EM, int Index)
	{
		// insert at given position
		EM.line_index = Index;
		
		// no items
		if (list.size() == 0)
		{
			list.push_back(EM);
			return;
		}
		else if (list.back().line_index < Index)
		{
			list.push_back(EM);
		}
		else
		{
			bool use_next = false;
			for (List::iterator ptrEm = list.begin(); ptrEm != list.end(); ptrEm++)
			{
				if (use_next)
				{
					list.insert(ptrEm, EM);
					is_dirty = true;
					return;
				}

				// if the next line, set to use next
				if (Index >= ptrEm->line_index)
					use_next = true;
			}

			// insert item at the back
			list.push_back(EM);
			is_dirty = true;
		}
	}

	Element* IconTray::push_back(Element& Em)
	{
		list.push_back(Em);
		return &list.back();
	}

	void IconTray::deleteItem(int ElementId)
	{
		for (List::iterator ptrEm = list.begin(); ptrEm != list.end(); ptrEm++)
		{
			if (ptrEm->id == ElementId)
			{
				list.erase(ptrEm);
				return;
			}
		}
	}

	Element* IconTray::search(std::string Key)
	{
		for (List::iterator ptrEm = list.begin(); ptrEm != list.end(); ptrEm++)
		{
			const char* k2 = ptrEm->key; 
			if (Key.compare(k2) == 0)
				return &(*ptrEm);
		}
		return 0;
	}

	bool SortElementIndex ( HeadsUp::Element* E1, HeadsUp::Element* E2 )
	{
	   return E1->index > E2->index;
	}

	void IconTray::reposition(EclipseBot& Client)
	{
		// no ct, abord
		if (bot.HasCT() == false)
			return;

		// is this a bot, if so it may be ignored
		if (m_pUser->IsBot() == true)
			return;

		// boundary of the icons
		const int ICON_SEPERATOR	= HeadsUp::Config::getInstance().getSpacing();
		const int x_padding				= HeadsUp::Config::getInstance().getXEdgePadding();
		const int y_padding				= HeadsUp::Config::getInstance().getYEdgePadding();

		// head up element
		std::vector<HeadsUp::Element*> build_list;
		std::vector<HeadsUp::Element*> delete_list;

		// expansion is relative
		bool actual_expansion	= tsb_check(getExpanded(), HeadsUp::Config::getInstance().getDefaultExpansion());
		bool actual_visibility	= tsb_check(getVisible(), HeadsUp::Config::getInstance().getDefaultVisibility());

		// calculate each size
		int width_required = 0;
		for (List::iterator ptrEm = list.begin(); ptrEm != list.end(); ptrEm++)
		{
			Element& em = *ptrEm;
			bool to_be_vis = 
				(actual_expansion  && em.row == row && em.visible) || 
				(actual_expansion  && em.row == ".all") ||
				(actual_expansion  && em.click_method == HeadsUp::Element::CLICK_METHOD_COLLAPSE) ||
				(!actual_expansion && em.click_method == HeadsUp::Element::CLICK_METHOD_EXPAND); 

			// if visible, add it to the required width but the list
			if (to_be_vis && visible != B_FALSE)
			{
				width_required+= em.size_x + ICON_SEPERATOR;
				build_list.push_back(&em);
			}
			else if (ptrEm->is_visible == true)
			{
				em.is_visible = false;
				m_pUser->DestroyHUD(em.id);
			}
		}

		// sort the indexes
		std::sort(build_list.begin(), build_list.end(), SortElementIndex);
		visible_items.clear();

		// only if this is enabled to do we rebuild it
		if (HeadsUp::Config::getInstance().getEnabled() == false || getVisible() == false)
			return;

		// horizontal setup loop
		int left = 0;
		int pos  = 0;
		switch (HeadsUp::Config::getInstance().getPosition())
		{
			case HeadsUp::Config::POSITION_TOP:
				left = -width_required / 2;
				pos  = AW_HUD_ORIGIN_TOP;
				break;

			case HeadsUp::Config::POSITION_TOP_LEFT:
				left = x_padding;
				pos  = AW_HUD_ORIGIN_TOP_LEFT;
				break;

			case HeadsUp::Config::POSITION_TOP_RIGHT:
				left = -width_required - x_padding;
				pos  = AW_HUD_ORIGIN_TOP_RIGHT;
				break;

			case HeadsUp::Config::POSITION_BOTTOM:
				left = -width_required / 2;
				pos  = AW_HUD_ORIGIN_BOTTOM;
				break;

			case HeadsUp::Config::POSITION_BOTTOM_LEFT:
				pos  = AW_HUD_ORIGIN_BOTTOM_LEFT;
				left = x_padding;
				break;

			case HeadsUp::Config::POSITION_BOTTOM_RIGHT:
				pos  = AW_HUD_ORIGIN_BOTTOM_RIGHT;
				left = -width_required - x_padding;
				break;
		}

		// vertical position loop
		switch (HeadsUp::Config::getInstance().getPosition())
		{
			case HeadsUp::Config::POSITION_TOP:
			case HeadsUp::Config::POSITION_TOP_LEFT:
			case HeadsUp::Config::POSITION_TOP_RIGHT:
				for (size_t i = 0; i < build_list.size(); i++)
				{
					Element& em = *build_list[i];
					em.x		= left;
					em.y		= y_padding;
					left		+= em.size_x + ICON_SEPERATOR;
				}
				break;

			case HeadsUp::Config::POSITION_BOTTOM:
			case HeadsUp::Config::POSITION_BOTTOM_LEFT:
			case HeadsUp::Config::POSITION_BOTTOM_RIGHT:
				for (size_t i = 0; i < build_list.size(); i++)
				{
					Element& em = *build_list[i];
					em.x		= left;
					em.y		= -y_padding - em.size_y;
					left		+= em.size_x + ICON_SEPERATOR;
				}
				break;
		};

		// create the additional HUDs
		for (size_t i = 0; i < build_list.size(); i++)
		{
			Element& em = *build_list[i];
			int flags = 0;
			if (em.click_method != Element::CLICK_METHOD_NONE)
				flags = AW_HUD_ELEMENT_FLAG_TRANSITION | AW_HUD_ELEMENT_FLAG_CLICKS; 
			else
				flags = AW_HUD_ELEMENT_FLAG_TRANSITION;

			// assign additional flags
			em.origin			= pos;
			em.flags	    = flags;
			em.opacity		= HeadsUp::Config::getInstance().getOpacity();
			em.is_visible	= true;
			m_pUser->CreateHUD(em);
		}

		// no longer dirty
		is_dirty = false;
	}

	void IconTray::update_all()
	{
		reposition(bot);
	}

	bool IconTray::hide_element(int ID)
	{
		for (List::iterator ptrEm = list.begin(); ptrEm != list.end(); ptrEm++)
		{
			if (ptrEm->visible && ptrEm->id == ID)
			{
				ptrEm->visible = false;
				is_dirty = true;
				return true;
			}
		}

		return false;
	}

	bool IconTray::show_element(int ID)
	{
		for (List::iterator ptrEm = list.begin(); ptrEm != list.end(); ptrEm++)
		{
			if (ptrEm->visible == false && ptrEm->id == ID)
			{
				ptrEm->visible = true;
				is_dirty = true;
				return true;
			}
		}
		return false;
	}

	void IconTray::change_element_visibility(int ID, bool Vis)
	{
		if (Vis) show_element(ID);
		else hide_element(ID);
	}
	
	void IconTray::update_element(int ID)
	{
	}
}