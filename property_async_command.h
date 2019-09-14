#pragma once
#include <list>

// forward references
namespace AW { class Bot; }

namespace Property
{
	// forward references
	class AsyncCallData;
	class AsyncRequest;

	enum ASYNC_RESULT_TYPES
	{
		ASYNC_CALL_PROJECT,
		ASYNC_CALL_COMMAND,
		ASYNC_CALL_SCRIPT
	};

	enum BUILD_ACTION_TYPES
	{
		BUILD_ACTION_ADD,
		BUILD_ACTION_CHANGE,
		BUILD_ACTION_DELETE
	};

	// AsyncCallData
	// ------------------------------------
	// all asyncresult result types must be of this base
	// class to provide the virtual destructor
	class AsyncCallData
	{
	public:
		virtual ~AsyncCallData() { };
		String	class_type;					// the type of class represented
	};

	// AsyncCallData_Project
	// ------------------------------------
	// contains the project id
	class AsyncCallData_Project : public AsyncCallData
	{
	public:	// information on the project id
		String			project_id;
		AsyncCallData_Project() { class_type = "project"; }
	};

	// AsyncCallback
	// ------------------------------------
	// function pointer types
	typedef void (*AsyncCallback)(AsyncRequest& Request);

	// AsyncRequest
	// ------------------------------------
	// the details of a request that we have made
	// that should handle the dispatching of our events
	class AsyncRequest
	{
	public:
		AsyncCallData*	result;				// allocated by the calling function, deleted after dispatching
		AsyncCallback	callback;			// the function to receive the notification
		AW::Bot*		bot_ptr;			// host bot
		int				call_from;			// one of ASYNC_RESULT_TYPES
		int				method;				// an BUILD_ACTION_TYPES
		int				cell_x, cell_z;		// the object cell x and z
		int				obj_number;			// the object number to expected
		int				rc;					// the response code
	};

	class AsyncMonitor
	{

	public: // type defenitions for a project
		typedef std::list<AsyncRequest>		ListType;
		typedef ListType::iterator			iterator;
		typedef ListType::const_iterator	const_iterator;
		typedef ListType::reverse_iterator	reverse_iterator;
		typedef ListType::pointer			pointer;

	public: // public lists
		ListType			m_list;

	public: // access to the list
		iterator			begin(void)		  { return m_list.begin();	}
		iterator			end	 (void)		  { return m_list.end();	}
		const_iterator		begin(void)	const { return m_list.begin();	}
		const_iterator		end  (void) const { return m_list.end();	}
		reverse_iterator	rbegin(void)	  { return m_list.rbegin();	}
		reverse_iterator	rend (void)		  { return m_list.rend();	}
		iterator			erase(iterator I) { return m_list.erase(I); }

	public: // operations
		void				push_back(AsyncRequest& RQ) { m_list.push_back(RQ); }
		bool				dispatch_result(int ReasonCode, AW::Bot& Client);
		void				purge_all(int ReasonCode, AW::Bot& Client);
	};

	// Function dispatching for the main types
	// ------------------------------------
	void AsyncHandler_Seed		(AsyncRequest& Request);
	void AsyncHandler_Project	(AsyncRequest& Request);
	void AsyncHandler_Command	(AsyncRequest& Request);

	// Global tracking
	// ------------------------------------
	extern AsyncMonitor g_AsyncMonitor;
}