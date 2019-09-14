#pragma once
#include <list>
#include <vector>
#include "awsdk_entity.h"
#include "s_ptr.h"
#include "scripting_iterator.h"


namespace AW
{
	class Bot;
}

namespace Movers
{
	class Mover : public AW::Entity
	{
	public:
		void setPositionAt(const AW::Location Here)
		{
			Location& lThis = *(dynamic_cast<AW::Location*>(this));
			lThis = Here;
		}

	public: // umbra interfaces
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);

	public: // connected movers
		std::vector<int> connected_avatars;
		size_t connectedCount() const { return connected_avatars.size(); }
	};

	class Tracker : public Umbra::RemoteClass
	{
	public: // construct / deconstruct
		Tracker();
		~Tracker();
		void			clear();

	public: // type defenitions
		typedef			std::list<Mover>				MoverList_T;
		typedef			std::list<Mover>::iterator		Iterator;

	public: // looping through
		std::list<Mover> m_list;
		size_t			 size()  const { return m_list.size(); }
		Iterator		 begin() { return m_list.begin(); }
		Iterator		 end()   { return m_list.end();   }

	public: // modification
		void			push_back		(Mover SourceEntity);
		void			remove_by_id	(int Mover_Id);

	public: // access
		Mover*			getById			(int Mover_Id);
		Mover*			getByName		(String Mover_Key);
		Mover*			getByModelId	(int ModelId);

	public: // umbra interfaces
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);

	protected: // signalling
		Umbra::IteratorSignalAgent _umbra_isa;
	};
	

	// external class
	extern Tracker		g_Movers;
}