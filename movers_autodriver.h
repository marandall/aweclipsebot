#pragma once
#include <list>
#include "movement.h"

class CommonBot;

namespace Movers
{
	class AutoDriver : public Umbra::RemoteClass
	{
	public: // the tracking for the driver interface
		String	id;					// tag of this mover
		int		auto_id;			// auto incremented id
		Movement::Controller ctrl;	// the movement process

	public: // umbra interfaces
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
		{
			// not found
			//
			return Umbra::FunctionRequest::HAS_HANDLED;
		}

		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 
		{ 
			// application constants
			UMBRA_PROPERTY_R(movement, ctrl);
		
			// not found
			//
			return Umbra::PropertyRequest::HAS_HANDLED;
		}

	public: // constructor
		AutoDriver()
		{
			static int uid = 0;
			auto_id = uid++;
		}

	public:
		int HandleMove(float Resolution);
	};

	class AutoDriverKernel
	{
	public: // the list of drivers
		typedef std::list< AutoDriver >		ListType;
		typedef ListType::iterator			iterator;
		typedef ListType::const_iterator	const_iterator;

	public: // data
		ListType	list;
		iterator	begin() { return list.begin();	}
		iterator	end()	{ return list.end();	}
		size_t		size()	{ return list.size();	}

	public:
		AutoDriverKernel();
		~AutoDriverKernel();
		void			clear();

	public: 
		AutoDriver*		getEntryByName(String Id);
		AutoDriver*		getEntryByAuto(int Id);
		bool			process(float Resolution, CommonBot& Client);

	public:
		AutoDriver*		createNew(String Id);
		bool			Delete(String Id);

	public: // get driver kernel
		static AutoDriverKernel& getInstance();
	};

}