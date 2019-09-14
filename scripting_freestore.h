#pragma once
#include "umbra/umbra_remoteclass.h"
#include "umbra/umbra_variant.h"
#include <list>

namespace ScriptingHost_ClassLib
{
	class FreeStore : public Umbra::RemoteClass
	{
	protected: // element storage
		struct Em
		{
			String  key;
			Umbra::Variant value;
		};

	public: // type defenition
		typedef std::list<Em> KeyPairs;
		typedef KeyPairs::iterator iterator;
		KeyPairs ems;

	protected: // helper search
		Em* getEm(String Key)
		{
			for (iterator i2 = ems.begin(); i2 != ems.end(); i2++)
				if (i2->key == Key)
					return &(*i2);
			return 0;
		}

	public: // handler
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 
		{ 
			// support lookup
			if (R.isGet() == true)
			{
				Em* pEm = getEm(R.id.c_str());
				if (pEm == 0)
				{
					Em em;
					em.key = R.id.c_str();
					em.value = Umbra::Variant();
					ems.push_back(em);

					R.explicit_reference = true;
					V = &ems.back().value.getVM();
				}
				else
				{
					R.explicit_reference = true;
					V = &pEm->value.getVM();
				}

				return Umbra::PropertyRequest::HAS_HANDLED;
			}

			// unsupported element
			return Umbra::PropertyRequest::NOT_HANDLED;  
		}
	};
}