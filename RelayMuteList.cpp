#include "stdafx.h"
#include "RelayMuteList.h"

namespace ChatControllers
{
	void MuteList::load(Serializer& Sz)
	{
		// clear the existing list
		m_list.clear();

		// count the mutes
		int mutes = atoi(Sz["muting"]["count"]);
		int count = 0;
		String key;

		// load each key
		for (int count = 0; count < mutes; count++)
		{
			key.Format("muted%d", count++);
			push_back(Sz["muting"][key]);
		}
	}

	void MuteList::save(Serializer& Sz)
	{
		// set the number of mutes
		Sz["muting"]["count"] = ToString((int)m_list.size());
		int count = 0;

		// write the dtaa
		String key;
		for (iterator ptr = begin(); ptr != end(); ptr++)
		{
			key.Format("muted%d", count++);
			Sz["muting"][key] = *ptr;
		}
	}

	bool MuteList::is_filtered(Avatar& User) const
	{
		for (const_iterator ptr = begin(); ptr != end(); ptr++)
			if (User.IsPattern(*ptr) == true)
				return true;
		return false;
	}

	bool MuteList::key_exists(String Key) const
	{
		for (const_iterator ptr = begin(); ptr != end(); ptr++)
			if ((*ptr).CompareNoCase(Key) == 0)
				return true;
		return false;
	}
	
	void MuteList::remove_key(String Key)
	{
		for (iterator ptr = begin(); ptr != end(); ptr++)
		{
			if ((*ptr).CompareNoCase(Key) != 0)
				continue;
			m_list.erase(ptr);
			return;
		}
	}
}