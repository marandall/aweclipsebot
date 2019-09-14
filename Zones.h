#pragma once
#include "zone_regions.h"
#include "AWSDK_Support.h"
#include "zone_access_entry.h"
#include "tsb.h"
#include "serializer.h"
#include "umbra/umbra_remoteclass.h"
#include "zones_zone_ag.h"

#include <vector>
#include <list>

// the function for the between aspect of the square
#define BETWEEN(low, p, high) (low <= p && p <= high)

namespace XML { 
	class Node; 
}

namespace AW
{
	class Avatar;
}

namespace Zones
{
	class Zone;
	class Region;

	class ZoneList
	{
	public: // a linked list of zones using a custom iterator
		typedef std::list<Zone>				ListType;
		typedef ListType::iterator			iterator;
		typedef ListType::const_iterator	const_iterator;
		typedef ListType::reverse_iterator	reverse_iterator;
		typedef ListType::pointer			pointer;
		typedef ListType::reference			reference;

	public: // internal lists
		ListType			list;

	public: // access to the list
		iterator			begin(void)		  { return list.begin();	}
		iterator			end	 (void)		  { return list.end();	}
		const_iterator		begin(void)	const { return list.begin();	}
		const_iterator		end  (void) const { return list.end();	}
		reverse_iterator	rbegin(void)	  { return list.rbegin();	}
		reverse_iterator	rend (void)		  { return list.rend();	}
		size_t				size (void) const { return list.size();	}
		reference			back (void)		  { return list.back();	}
		void				clear(void)		  { list.clear();			}
		void				push_back(Zone& Z){ list.push_back(Z);	}
		void				erase(iterator Zi) { list.erase(Zi); }
	};

	template<class A>
	class rgl_template : public std::vector<A>
	{
	public: // additional functions to look up by a regional id
		Regions::Region*	GetRegion(int RegionId);
	};

	class RegionList
	{
	public: // type defenitions for the lists
		typedef rgl_template<Regions::Cylinder>		Cylinders;
		typedef rgl_template<Regions::Polygon>		Polygons;
		typedef rgl_template<Regions::Rectangle>	Rectangles;
		typedef rgl_template<Regions::Sphere>		Spheres;
		typedef std::vector<Regions::Region*>		Regions;

	public: // the local stores of zones
		Cylinders	cylinders;
		Polygons	polygons;
		Rectangles	rectangles;
		Spheres		spheres;
		Regions		regions;

	public: // insert the and remove the shapes into the dialog
		void	push_back(Zones::Regions::Cylinder& Shape);
		void	push_back(Zones::Regions::Polygon& Shape);
		void	push_back(Zones::Regions::Rectangle& Shape);
		void	push_back(Zones::Regions::Sphere& Shape);
		void	clear(void);
		size_t	GetCount() { return cylinders.size() + polygons.size() + rectangles.size() + spheres.size(); }
	};

	enum ZONE_ACCESS_PERMISSION
	{
		PERMISSION_EXPLICIT_GRANT,
		PERMISSION_EXPLICIT_DENY,
	};

	enum ZONE_ACCESS_BOOLEAN
	{
		ZAB_TRUE,
		ZAB_FALSE,
		ZAB_CHECK_PARENT
	};

	class Zone : 
		public Zones::AutoTemplates::ZoneData
	{
	public: // construction
		Zone(); 

	public: // umbra interface
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);

	public: // identification of the zone
		int			tracking_id;
		int			getTrackingID	(void) const		{ return tracking_id;	}
	
	public: // regions and internal zones
		RegionList	regions;
		ZoneList	children;
		Zone*		parent_zone;
		Zone*		getParent		(void) const		{ return parent_zone;	}
		Zone*		getParent		(void) 				{ return parent_zone;	}
		Zone*		search			(const char* Id);

	public: // access
		int			getChildCount()			const { return (int)children.size(); }

	public: // access control mechanisms
		bool		getCheckPermission(AccessEntry& ZAE);
		std::vector<String> getLogicalAccessList(void);

	public: // logical checks
		BOOL		getLogicalFly()			const { return (override_fly	   == B_EMPTY && parent_zone) ? getParent()->getLogicalFly()      : override_fly;			}
		BOOL		getLogicalShift()		const { return (override_shift	   == B_EMPTY && parent_zone) ? getParent()->getLogicalShift()	: override_shift;		}
		BOOL		getLogicalTeleport()	const { return (override_teleport  == B_EMPTY && parent_zone) ? getParent()->getLogicalTeleport() : override_teleport;	}
		String		getLogicalTemplate()	const { return (override_template  == ""      && parent_zone) ? getParent()->getLogicalTemplate() : override_template;	}
		String		getLogicalFinal()		const { return (override_final     == ""      && parent_zone) ? getParent()->getLogicalFinal()    : override_final;		}
	
	public: // logical communications
		BOOL		getLogicalChatRequiresPs() const { return (chat_requires_ps == B_EMPTY && parent_zone) ? getParent()->getLogicalChatRequiresPs() : chat_requires_ps; }
		String		getLogicalDenialArgument() const { return (denial_argument == "" && parent_zone) ? getParent()->getLogicalDenialArgument() : denial_argument;		}

	public:
		/* checker for position */
		Zone*		FindZone(const AW::Location&, Zone* pZone);
		bool		Contains(const AW::Location& pt)				const;
		bool		Authorised(long citizen)						const;
		String		getFullDescriptor(const char* seps = " > ")		const;
		Zone*		SearchID(int Zone_ID);
		void		update_parents();
		void		update_after_changes();

	public: // handling of deletion and editing
		bool		deleteChild(int ZoneID);

	public: // file io
		bool		serializedLoad(Serializer& SZ, int ZoneID);
		bool		serializedSave(Serializer& SZ);
		errno_t		save(String Path);
		errno_t		load(String Path);

	public: // communications
		String		getLogicalCommsChannel() { return chat_channel.GetLength() == 0 && parent_zone ? parent_zone->getLogicalCommsChannel() : chat_channel; }

	protected:
		void	Erase();
	};

	/* main control class for other zone stuff */
	class Controller : public Umbra::RemoteClass
	{
	protected:
		Zone	m_root;

	public: // umbra interface
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);

	public:
		Zone*	GetRoot(void) { return &m_root; }
		Zone*	getZone(const AW::Location&, Zone* CurZone);
		void	Clear(void);
		BOOL	Load(String FilePath);
		errno_t	Save(String FilePath);
	};

	extern Controller g_ZonesMgr;
}