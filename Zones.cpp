#include "stdafx.h"
#include "zones.h"
#include "zone_regions.h"
#include "zoneConnector.h"
#include "awsdk_support.h"
#include "zone_access_entry.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

namespace Zones
{
	Controller	g_ZonesMgr;

	bool AccessEntry::ZAE_KeyConforms(CStringA Key)
	{
		return false;
	}

	/* a zone constructor */
	Zone::Zone() 
	{ 
		static int uTracker = 0;
		tracking_id = ++uTracker;
	}

	void Zone::update_parents()
	{
		// try and search children
		for (ZoneList::iterator zonePtr = children.begin(); zonePtr != children.end(); zonePtr++)
		{
			zonePtr->parent_zone = this;
			zonePtr->update_parents();
		}
	}

	void Zone::update_after_changes()
	{
		parent_zone = NULL;
		update_parents();
	}


	/* zone based searches and queries */
	Zone* Zone::FindZone(const AW::Location& pt, Zone* pZone) 
	{
		Regions::Region*	pRegion = NULL;
		Zone* pLoopZone = NULL;

		if (Contains(pt)) 
		{
			pZone = const_cast<Zone*>(this);

			for (ZoneList::iterator zonePtr = children.begin(); zonePtr != children.end(); zonePtr++)
				if (zonePtr->Contains(pt))
					pZone = zonePtr->FindZone(pt, pZone);
		} 

		/* return our updated string */
		return pZone;
	}

	bool Zone::Contains(const AW::Location& pt) const 
	{
		// must lookup
		if (getUseForLookups() == false)
			return false;

		/* variables */
		Regions::Region*	pRegion = NULL;

		/* are we are global zone in which case we are always true */
		if (id == ".") 
			return true;
		
		/* loop through each region */
		for (size_t i = 0; i < regions.rectangles.size(); i++)
			if (regions.rectangles[i].Contains(pt)) 
				return true;

		for (size_t i = 0; i < regions.spheres.size(); i++)
			if (regions.spheres[i].Contains(pt)) 
				return true;

		for (size_t i = 0; i < regions.cylinders.size(); i++)
			if (regions.cylinders[i].Contains(pt)) 
				return true;

		for (size_t i = 0; i < regions.polygons.size(); i++)
			if (regions.polygons[i].Contains(pt)) 
				return true;
		
		/* sorry, nothing home */
		return false;
	}

	String Zone::getFullDescriptor(const char* seps) const 
	{
		const void* pEx = this;

		String	tTemp	= name;
		Zone*	pLevel	= parent_zone;
		
		/* if our top level is null (parent) we abort */
		if (pLevel == NULL) return tTemp;
		
		/* while we are not at the top and our id is not a single period*/
		while ((pLevel != NULL) && (pLevel->id != ".")) 
		{
			CString bt = tTemp;
			tTemp.Format("%s%s%s", pLevel->getName(), seps, bt);
			pLevel = pLevel->parent_zone;
		}
		return tTemp;
	}

	Zone* Zone::SearchID(int Zone_ID)
	{
		// if we found this id, return it
		if (tracking_id == Zone_ID)
			return this;

		// try and search children
		for (ZoneList::iterator zonePtr = children.begin(); zonePtr != children.end(); zonePtr++)
		{
			Zone* zPtr = zonePtr->SearchID(Zone_ID);
			if (zPtr) return zPtr;
		}

		// nothing found
		return NULL;
	}

	bool Zone::deleteChild(int ZoneID)
	{
		Zone* pParent = getParent();
		for (ZoneList::iterator zonePtr = children.begin(); zonePtr != children.end(); zonePtr++)
		{
			if (zonePtr->tracking_id == ZoneID)
			{
				children.list.erase(zonePtr);
				return true;
			}
		}

		return false;
	}


	/* erases everything in the zones recursivly */
	void Zone::Erase()
	{
		regions.clear();
		children.clear();
	}

	std::vector<String> Zone::getLogicalAccessList(void)
	{
		std::list<String> lister;
		Zone* pZone = this;

		// jump up to the very top
		String text;
		do {
			// nothing above
			if (pZone->getParent() != 0)
			{
				String next = pZone->access_list + String(",") + text;
				text = next;
				pZone = pZone->getParent();
			}
			else
			{
				String next;
				next.Format("%s,%s", pZone->access_list.GetLength() == 0 ? "any()" : pZone->access_list, text);
				text = next;
				pZone = pZone->getParent();
			}
		} while (pZone);

		// split it, return
		SplitString items(text, ",", -1);
		std::vector<String> result;
		for (size_t i = 0; i < items.size(); i++)
		{
			String item = items[i];
			item.Trim();

			// previous void
			if (item == "new()")
			{
				result.clear();
			}
			else if (item.GetLength() > 0)
			{
				result.push_back(item);
			}
		}

		// return the completed list
		return result;
	}

	bool Zone::getCheckPermission(AccessEntry& ZAE)
	{
		std::vector<String> access = getLogicalAccessList();

		// format codes
		//   @ * - access to all users
		//   @ reset() - clear the items previously in the list
		//   @ pattern

		bool b_permitted	= false;
		bool b_access_state	= true;

		// go through each item
		for (size_t i = 0; i < access.size(); i++)
		{
			String& em = access[i];

			// check mode changes
			if (em == "begin_permit()")
				b_access_state = true;
			else if (em == "begin_deny()")
				b_access_state = false;

			// when the combination matches
			if (ZAE.ZAE_KeyConforms(access[i]) == true)
				b_permitted = b_access_state;
		}

		return b_permitted;
	}

	//
	//
	Zone* Zone::search(const char* Id)	
	{
		// check this element
		if (getId().Compare(Id) == 0)
			return this;

		// search each element
		for (ZoneList::iterator zonePtr = children.begin(); zonePtr != children.end(); zonePtr++)
		{
			Zone* pThis = zonePtr->search(Id);
			if (pThis) return pThis;
		}

		// not found
		return 0;
	}

	/* erases everything tracked by this controller */
	void Controller::Clear() 
	{
	}

	/* code for the controller interface */
	BOOL Controller::Load(String FilePath) 
	{
		/* clear the main information */
		Clear();

		/* select primary zone root */
		if (m_root.load(FilePath) == false)
			return false;

		/* count */
		String logText;
		logText.Format("Total Zones loaded: %d", static_cast<int>(m_root.children.size()));
		WriteMsg(logText, 0, OT_DEBUG | MF_FBOLD | OM_TIME);
		return TRUE;
	}

	/* extrapolates the current zone from this one */
	Zone* Controller::getZone(const AW::Location& pt, Zone* CurZone) 
	{
		/* if the zone is not valid */
		if (CurZone) {
			if (CurZone->Contains(pt)) {
				return CurZone->FindZone(pt, CurZone);
			} }
		return m_root.FindZone(pt, &m_root);
	}

	/* zone connector desconstrutors */
	ZoneConnector::~ZoneConnector(void) 
	{ 

	}

	/* zone connecctor code */
	bool ZoneConnector::ZoneModifyPos(const AW::Location& position)
	{
		/* Work out where we are now */
		Zone* new_zone = g_ZonesMgr.getZone(position, m_pThisZone);
		
		/* are they now different? */
		if (m_pThisZone != new_zone) {
			Zone* pOldZone = m_pThisZone;
			m_pThisZone = new_zone;
			onEventZoneChanged(pOldZone, new_zone);
			return true;
		}
		return false;
	}

	String ZoneConnector::getZoneId(void) const 
	{
		if (!m_pThisZone) return ".";
		else return m_pThisZone->getId();
	}

	String ZoneConnector::getZoneDescriptor(const char* seps) const 
	{
		if (!m_pThisZone) return ".";
		else return m_pThisZone->getFullDescriptor(seps);
	}
};;