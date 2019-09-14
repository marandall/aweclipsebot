#include "stdafx.h"
#include "umbra_variant.h"
#include "umbra_resourcemanager.h"
#include "umbra_remoteclass.h"

// debug code using CRT.
#ifdef _DEBUG

#endif

namespace Umbra
{
	VariantMemory::VariantMemory()
	{
		type			= 0;
		ref_count		= 0;
		break_on_change = false;
	}

	VariantMemory::~VariantMemory()
	{
		int type = getType();
		if (type == Types::Resource || type == Types::String)
		{
			// lookup pointer and delete reference
			RemoteClass* pRemote = ResourceManager::GetRM().search(du.v_resource);
			if (pRemote)
			{
				pRemote->doUmbraDeleteRef();
			}
		}	
	}

	VariantMemory& VariantMemory::operator=(const VariantMemory& VM)
	{
		// type check for cleanup
		if (type == Types::Resource || type == Types::Resource)
		{
			// lookup pointer and delete reference
			RemoteClass* pRemote = ResourceManager::GetRM().search(du.v_resource);
			if (pRemote)
			{
				pRemote->doUmbraDeleteRef();
			}
		}

		// clean up ourselves first
		switch (VM.getType())
		{
			case Types::Resource:
			case Types::String:
			{
				// the remote class gets incremented, oh the pain
				RemoteClass* pRemote = ResourceManager::GetRM().search(VM.du.v_resource);
				if (pRemote != 0)
				{
					pRemote->doUmbraAddRef();
				}
			}

			default:
			{
				// copy as standard
				du			= VM.du;
				type		= VM.type;
			}
		}

		// all is safe
		return *this;
	}

	void VariantMemory::addRef()
	{ 
		ref_count++; 
	}

	void VariantMemory::decRef() 
	{ 
		if (--ref_count == 0) 
		{
			delete this;
		}
	}

}
