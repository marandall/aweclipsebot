#pragma once
#include <list>
#include <vector>
#include "awsdk_citizen.h"

namespace CitizenLookup
{
	class LookupBase
	{
	public: // handle the return value
		LookupBase(int type = LOOKUP_BY_OTHER);
		virtual ~LookupBase() { }									// important to prevent memory leaks
		virtual void Result(AW::Citizen& Info, int Reason) { }
		
	public: // lookup information
		int		number;
		String	name;
		int		lookup_type;

	public: // identification states
		static const int LOOKUP_BY_NUMBER	= 0;
		static const int LOOKUP_BY_NAME		= 1;
		static const int LOOKUP_BY_OTHER	= 2;

	public: // test functions
		bool isLookupNumber() const { return lookup_type == LOOKUP_BY_NUMBER; }
		bool isLookupName()   const { return lookup_type == LOOKUP_BY_NAME; }
		bool isLookupOther()  const { return lookup_type == LOOKUP_BY_OTHER; }
	};

	class GroupLookupBase;
	class Kernel;

	class GroupLookupHelper : public LookupBase
	{
	public: // the backwards reference
		GroupLookupBase* pGroupBase;			// the GroupLookupBase parent
		AW::Citizen		 info;					// the citizen information
		int				 result;				// the reason code
	
	public: // handle the result
		void Result(AW::Citizen& Info, int Reason);
		GroupLookupHelper(GroupLookupBase* B) { pGroupBase = B; result = 0; }
		GroupLookupHelper() { result = 0; }
	};

	class GroupLookupBase : public Umbra::RemoteClass
	{
	public: // the list of lookups
		std::vector<GroupLookupHelper>	results;			// the list of lookups we use
		Kernel*						    kernel;				// the kernel object it is dispatched to
		int								outstanding;		// the number of queries left to return
		bool							opened;				// the opened result set
		bool							delete_children;	// delete the children objects

	public: // constructor
		GroupLookupBase(Kernel* K) { kernel = K; opened = false; outstanding = 0; delete_children = true; }
		virtual ~GroupLookupBase();

	public: // handle the data
		virtual void onComplete() { };
		virtual void processResult(GroupLookupHelper* Handler);

	public: // lookup routines
		int		lookupByNumber(int Number);
		int		lookupByName  (const char* Name);
		void	useDummyNumber(int Number, const char* Name);
		void	useDummyName  (const char* Name, int Number);

	public: // access
		GroupLookupHelper operator[](size_t Z) { return results[Z]; }
		size_t size() { return results.size(); }

	public: // opening and closed
		int	 getOutstanding() { return outstanding; }
		void open();
		void close(bool AutoDelete = true);							// may delete the object

	public: // delete children
		void deleteChildren() { delete_children = true; }
		void preserveChildren() { delete_children = false; }

	};

	class Kernel
	{
	protected: // management for simultainious lookups
		typedef std::list<LookupBase>	ActiveList;
		typedef ActiveList::iterator	iterator;
		typedef std::list<LookupBase*>	RequestList;
		typedef RequestList::iterator	rq_iterator;

	protected: // variable tracking
		ActiveList	m_pending;
		RequestList	m_requests;

	protected: // check for existing lookups
		bool lookup_exists(int Citizen);
		bool lookup_exists(String Name);

	public: // lookup processes
		int lookup_by_number		(int Number, LookupBase* pBase);
		int lookup_by_name			(String Name, LookupBase* pBase);
		int lookup_citizen_next		(int Citizen, LookupBase* pBase);
		int lookup_citizen_previous	(int Citizen, LookupBase* pBase);
		int handle_result			(int Result);
		int handle_close			(int Reason);

	public: // handle those lookups which have completed in-situ
		void purge();
	};

	extern Kernel g_Kernel;
}