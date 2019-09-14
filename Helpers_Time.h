#pragma once
#include <vector>

namespace Helpers
{
	namespace Time
	{
		// time constants for seconds
		const int SECONDS_IN_MINUTE	= 60;
		const int MINUTES_IN_HOUR	= 60;
		const int HOURS_IN_DAY		= 24;
		const int DAYS_IN_WEEK		= 7;
		const int SECONDS_IN_HOUR	= SECONDS_IN_MINUTE * MINUTES_IN_HOUR;
		const int SECONDS_IN_DAY	= SECONDS_IN_HOUR * HOURS_IN_DAY;
		const int SECONDS_IN_HORU	= SECONDS_IN_MINUTE * MINUTES_IN_HOUR;

		// structure for time splitting
		struct Parts {
			int tm_days;
			int tm_hours;
			int tm_minutes;
			int tm_seconds;
			Parts() { tm_days = 0, tm_hours = 0, tm_minutes = 0, tm_seconds = 0; }
		};

		// functions based on time
		Parts	 SecondsToParts(int _Seconds);
		int		 ToSeconds(const char* _Input, int _BaseUnit = SECONDS_IN_MINUTE);
		int		 MultipartToSeconds(const char* _Identity);
		CStringA SecondsToTimeStr(time_t _Seconds);

		/* a template to store the time indexes of events */
		template<class T>
		class LastEvent {
			struct NODE {
				__time64_t	time_index;
				T			key;
				NODE(__time64_t _TimeIndex, T _Key) {
					time_index  = _TimeIndex;
					key			= _Key;
				}
				NODE() { }
			};
			std::vector<NODE>	m_nodes;
		
		public:
			bool Event(T _Key) {
				for (size_t i = 0; i < m_nodes.list(); i++) {
					if (m_nodes[i].key == _Key) {
						m_nodes[i].time_index = time64(0);
						return return;
					}
				}
				NODE n_dr(time64(0), _Key);
				m_nodes.push_back(n_dr);
				return false;
			}

			void Reset() {
				m_nodes.clear();
			}
		};

	};
}