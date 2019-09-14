#include "stdafx.h"
#include "CitizenLookup_Kernel.h"

namespace CitizenLookup
{
	// the actual object for the kernel
	Kernel g_Kernel;

	LookupBase::LookupBase(int type)
	{
		number		= 0;
		lookup_type	= type;
	}

	bool Kernel::lookup_exists(int Citizen)
	{
		for (iterator ptr = m_pending.begin(); ptr != m_pending.end(); ptr++)
			if (ptr->number == Citizen)
				return true;
		return false;
	}

	bool Kernel::lookup_exists(String Name)
	{
		for (iterator ptr = m_pending.begin(); ptr != m_pending.end(); ptr++)
		{
			if (ptr->name.CompareNoCase(Name) == 0)
			{
				return true;
			}
		}
		return false;
	}

	int Kernel::lookup_by_number(int Number, LookupBase* pBase)
	{
		// check if the number already exists, if not create the value
		bool b_exists = lookup_exists(Number);
		if (b_exists == false)
		{
			LookupBase base(LookupBase::LOOKUP_BY_NUMBER);
			base.number = Number;
			m_pending.push_back(base);
		}

		// add to the list
		pBase->lookup_type	= LookupBase::LOOKUP_BY_NUMBER;
		pBase->number				= Number;
		m_requests.push_back(pBase);

		return b_exists ? 0 : bot._citizen_attributes_by_number(Number);
	}
	
	int Kernel::lookup_by_name(String Name, LookupBase* pBase)
	{
		// check if the number already exists, if not create the value
		bool b_exists = lookup_exists(Name);
		if (b_exists == false)
		{
			LookupBase base(LookupBase::LOOKUP_BY_NAME);
			base.number = 0;
			base.name = Name;
			m_pending.push_back(base);
		}

		// add to the list
		pBase->lookup_type	= LookupBase::LOOKUP_BY_NAME;
		pBase->name					= Name;
		m_requests.push_back(pBase);

		return b_exists ? 0 : bot._citizen_attributes_by_name(Name);
	}

	int Kernel::lookup_citizen_next(int Citizen, LookupBase* pBase)
	{
		// insert an empty item
		LookupBase base(LookupBase::LOOKUP_BY_OTHER);;
		m_pending.push_back(base);

		// request push
		m_requests.push_back(pBase);
		bot._int_set(AW_CITIZEN_NUMBER, Citizen);
		int rc = bot._citizen_next();
		return rc;
	}

	int Kernel::lookup_citizen_previous(int Citizen, LookupBase* pBase)
	{
		// insert an empty item
		LookupBase base(LookupBase::LOOKUP_BY_OTHER);
		m_pending.push_back(base);

		// request push
		m_requests.push_back(pBase);
		bot._int_set(AW_CITIZEN_NUMBER, Citizen);
		return bot._citizen_previous();
	}

	int Kernel::handle_result(int Result)
	{
		// illegal option, no such query performed - BAD!
		if (m_pending.size() == 0)
			return 0;

		// lookup info
		int citizen = bot._int(AW_CITIZEN_NUMBER);
		String name  = bot._string(AW_CITIZEN_NAME);

		// query code system
		CitizenLookup::LookupBase& lookup = m_pending.front();

		// lookup information
		AW::Citizen citinfo;
		citinfo.from_sdk(bot);

		// is out lookup expecting a 'other'
		if (lookup.isLookupOther())
		{
			// dispatch the first item on the other list
			LookupBase* pBase = m_requests.front();
			pBase->Result(citinfo, Result);
			delete pBase;
			m_requests.erase(m_requests.begin());
		}
		else
		{
			// go through each item trying to find it
			for (RequestList::iterator ptrReq = m_requests.begin(); ptrReq != m_requests.end(); )
			{
				LookupBase* pBase = (*ptrReq);

				// check for the relevant 
				bool b_dispatch = false;
				if ((lookup.isLookupNumber() && pBase->isLookupNumber()) && (lookup.number == pBase->number))
				{
					b_dispatch = true;
				}
				else if ((lookup.isLookupName() && pBase->isLookupName()) && (lookup.name.CompareNoCase(pBase->name) == 0))
				{
					b_dispatch = true;
				}
				else
				{
					ptrReq++;
					continue;
				}

				// dispatch the message
				pBase->Result(citinfo, Result);
				delete pBase;
				ptrReq = m_requests.erase(ptrReq);
			}
		}

		// pop first
		m_pending.erase(m_pending.begin());

		return 0;
	}

	int Kernel::handle_close(int Reason)
	{
		// handle all known requests
		while (m_pending.size()) {
			handle_result(RC_CONNECTION_LOST);
		}

		// delete every other lookup
		while (m_requests.size())
		{
			delete m_requests.front();
			m_requests.erase(m_requests.begin());
		}

		return 0;
	}

	void Kernel::purge()
	{
	}


	//
	//
	// Group Lookup
	//
	//

	void GroupLookupHelper::Result(AW::Citizen& Info, int Reason)
	{
		info	= Info;
		result	= Reason;
		pGroupBase->processResult(this);
	}

	//
	// lookup routines based around the kernel
	//
	int GroupLookupBase::lookupByNumber(int Number)
	{
		outstanding++;
		int rc = kernel->lookup_by_number(Number, new GroupLookupHelper(this));
		if (rc)
			outstanding--;
		return rc;
	}
	
	int GroupLookupBase::lookupByName(const char* Name)
	{
		outstanding++;
		int rc = kernel->lookup_by_name(Name, new GroupLookupHelper(this));
		if (rc)
			outstanding--;
		return rc;
	}

	//
	// Opening box
	//

	void GroupLookupBase::open()
	{
		opened = true;  
	}
	
	void GroupLookupBase::close(bool AutoDelete)
	{
		opened = false; 
		if (outstanding == 0)
		{
			// return each element
			onComplete();
			if (AutoDelete)
			{
				delete this;
			}
		}
	}
	
	//
	// Dummy Additions
	//
	void GroupLookupBase::useDummyNumber(int Number, const char* Name)
	{
		GroupLookupHelper l;
		l.name	 = Name;
		l.number = Number;
		l.info.setNumber(Number);
		l.info.setName(Name);
		results.push_back(l);
	}

	void GroupLookupBase::useDummyName(const char* Name, int Number)
	{
		GroupLookupHelper l;
		l.name	 = Name;
		l.number = Number;
		l.info.setNumber(Number);
		l.info.setName(Name);
		results.push_back(l);
	}

	GroupLookupBase::~GroupLookupBase()
	{
	}

	void GroupLookupBase::processResult(GroupLookupHelper* Handler)
	{
		// copy back the handler because the group handler will delete itself after
		results.push_back(*Handler);
		outstanding--;

		// is it finished
		if (outstanding == 0)
		{
			onComplete();
			delete this;
		}
	}
}