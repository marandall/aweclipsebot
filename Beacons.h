#pragma once
#include <list>
#include "umbra/umbra_remoteclass.h"
#include "globalumbra.h"
#include "autonumber.h"
#include "CIS_ConfigJumpPoints.h"

namespace Beacons    
{

	class Beacon : 
		public AW::Location,
		public AutoNumberIdentifier
	{
	public:
		int				owner;		// citizen number, -1 for console
		CStringA		name;
		CStringA		position;

	public:
		int			getOwner() const { return owner; }
		CStringA	getName() const { return name; }
		CStringA	getLocation() const { return position; }

	public: // handler functions
		void updatePosition(String pos)
		{
			/* gain location */
			AW::Coordinates crdLoc;
			crdLoc.FromString(pos);
			m_x = crdLoc.getX();
			m_y = crdLoc.getY();
			m_z = crdLoc.getZ();
			m_yaw = crdLoc.getYAW();
		}

	public:
		Beacon(int Owner, CStringA Name, LPCSTR Position) { 
			owner = Owner;
			name  = Name;
			position = Position;

			updatePosition(Position);
		}

		void Update(int Owner, CStringA Name, LPCSTR Position)
		{
			Beacon(Owner, Name, Position);
		}

		Beacon() 
		{ 
		}

	public: // umbra interface
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
			UMBRA_PROPERTY_RW(name,	name);
			UMBRA_PROPERTY_RW(owner,	owner);

			// subclass
			UMBRA_PROPERTY_REDIRECT(AW::Location);

			// nothing
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	//
	// A list of autonumbers for certain tasks
	//

	class ProcessingList
	{
	protected:
		std::vector<int> list;

	public: // handling functions
		void add(int X) { list.push_back(X); }
		void clear() { list.clear(); }

	public: // check and test
		bool includes(int X)
		{
			for (size_t i = 0; i < list.size(); i++)
				if (list[i] == X)
					return true;
			return false;
		}
	}
	
	// class methods
	typedef AutoIDProcessingList;

	//
	// Configuration Options
	//

	class Config : public CIS::Config::AutoTemplates::JumpPointsData
	{
	};

	//
	// Managing class for jump points
	//

	class Kernel : public Umbra::RemoteClass
	{
	protected:
		typedef std::list<Beacon>	List;
		
	public:
		List			list;
		Config		config;

	public:
		typedef List::iterator	iterator;
		typedef Umbra::CodeIterator<Kernel::List::iterator> ScriptIterator;
		iterator begin()	{ return list.begin();	}
		iterator end()		{ return list.end();	}

	public: // access indexes
		size_t		size() const { return list.size(); }
		Beacon&		operator[](size_t Index);
		iterator	erase(iterator Itr);

	protected: // signalling
		Umbra::IteratorSignalAgent _umbra_isa;
	
	public: // umbra interface
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
	
	public: // auto list operations
		void	transferOwnership(ProcessingList& ProcList, int NewOwner);
		void	deleteByProcessingList(ProcessingList& ProcList);

	public:
		Beacon* Search(LPCSTR Target);
		bool	Insert(int Owner, LPCSTR Name, LPCSTR Location);
		void	Erase();
		void	Delete(LPCSTR KeyId);
		int		RemoveBy(int Citizen);
		int		RemoveProximity(const AW::Location& lcPos, int Range);
		int		SearchOwner(int Owner, std::vector<Beacon*>& Array);
		int		GetUserBeaconCount(int Citizen);

		Beacon* FirstFirstCloserThan(const AW::Location& lcPos, int Range);
		Beacon*	getBeaconByProximity(const AW::Location& Pos, String Prefix);
		Beacon*	getByAutoID(int AutoID);

		void	deleteByAutoId(int AutoID);

	public:
		bool	load(String Path, bool ClearList = true);
		bool	save(LPCTSTR FilePath);

	public: // singleton instance
		static Kernel& getInst() { static Kernel k; return k; }
	};
}