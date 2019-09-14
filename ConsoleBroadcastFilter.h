#pragma once
#include <vector>
#include <time.h>

namespace Comms
{
	class BroadcastFilter
	{
	protected:
		struct BroadcastMsg
		{
			__time64_t	msg_time;
			String		msg_id;
		};

		std::vector<BroadcastMsg>	m_list;

	public:
		void Add(String Broadcast)
		{
			if (IsFiltered(Broadcast))
				return;
			BroadcastMsg msg = { time64(0), Broadcast };
			m_list.push_back(msg);
		}

		bool IsFiltered(String Message)
		{
			for (size_t i = 0; i < m_list.size(); i++)
				if (m_list[i].msg_id == Message)
					return true;
			return false;
		}

		void Housekeeping()
		{
			__time64_t now = time64(0);
			for (int i = (int)m_list.size() - 1; i >= 0; i--)
				if (now - m_list[i].msg_time > 7)
					m_list.erase(m_list.begin() + i);
		}
	};

	extern BroadcastFilter g_BroadcastFilter;

}