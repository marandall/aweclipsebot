#include "stdafx.h"
#include "umbra_remoteclass.h"
#include "umbra_resourcemanager.h"

// memory allocation checking
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace Umbra
{
	ResourceManager mgr;

	ResourceManager& ResourceManager::GetRM()
	{
		return mgr;
	}

	int	ResourceIndex::increment()
	{
		return ++ref_count;
	}
	
	int	ResourceIndex::decrement()
	{
		return --ref_count;
	}

	void ResourceFrame::empty()
	{
	}

	int	ResourceManager::register_resource(Umbra::RemoteClass& Class)
	{
		// increment the resource count and set the resource count
		resource_count++;
		int slot = resource_count % MAX_RES_FRAMES;

		// create resource index
		ResourceIndex r_id;
		r_id.ptr = dynamic_cast<RemoteClass*>(&Class);

		// insert into frame
		frames[slot].indexes[resource_count] = r_id;
		Class._umbra_res_id = resource_count;

		// reset reference count
		return resource_count;
	}

	RemoteClass* ResourceManager::search(int ResourceId)
	{
		int index = ResourceId % MAX_RES_FRAMES;
			
		// access the iterator index for this item
		ResourceFrame::iterator ri = frames[index].indexes.find(ResourceId);
		if (ri != frames[index].indexes.end())
		{
			return ri->second.ptr;
		}
		else
		{
			// printf("RM unable to find resource %d\n", ResourceId);
			return 0;
		}
	}
	
	void ResourceManager::empty_resources()
	{
	}
}