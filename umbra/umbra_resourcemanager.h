#pragma once
#include "umbra_typedef.h"
#include <vector>
#include <map>

namespace Umbra
{
	class RemoteClass;

	// constants needed for the resource manager
	const int RESOURCE_GARBAGE_COLLECTED	= 1;	// 
	
	class ResourceIndex
	{
	public:
		int				ref_count;
		int				resource_mode;
		RemoteClass*	ptr;

	public:
		int				increment();
		int				decrement();

	public: 
		ResourceIndex()
		{ 
			ref_count = 0;
			resource_mode = 0; 
			ptr = 0;
		}
		
		~ResourceIndex() 
		{ 
		}
	};

	class ResourceFrame
	{
	public:
		typedef std::map<int, ResourceIndex>	MapType;
		typedef MapType::iterator				iterator;
		typedef MapType::reference				reference;

	public: // the list of indexes
		MapType	indexes;

	public: // clear the items of the list
		void empty();
	};

	class ResourceManager
	{
	public: // static information
		static const int MAX_RES_FRAMES = 5000;

	public: // handling  information
		ResourceFrame	frames[MAX_RES_FRAMES];
		bool			running;
		int				resource_count;

	public: // access the shared resource manager
		static ResourceManager& GetRM();

	public: // lifetime operators
		ResourceManager()
		{
			running			= true;
			resource_count	= 0;
		}

		~ResourceManager()
		{
		}

	public: // resource creation and lookups
		int	register_resource(Umbra::RemoteClass& Class);
		RemoteClass* search(int ResourceId);

	public:
		// cleanup routines
		void	empty_resources();
	};
}