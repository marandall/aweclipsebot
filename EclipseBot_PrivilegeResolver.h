#pragma once
#include "CitizenLookup_Kernel.h"

namespace CitizenLookup
{
	class PrivilegeLookupClass : public LookupBase
	{
	public:
		void Result(AW::Citizen& Info, int Reason)
		{
			for (size_t i = 0; i < avatars.size(); i++)
			{
				// lookup this privilege
				if (avatars[i].getPrivLUS() != LookupState::Resolving)
					continue;

				// lookup this user
				if (avatars[i].getPrivilege() != number)
					continue;

				// update
				avatars[i].EventCitizenLookup(Reason, Info.getCitizen(), Info.getName());
			}
		}
	};


}