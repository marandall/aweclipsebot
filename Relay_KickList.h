#pragma once
#include <vector>

class Avatar;

namespace Relays
{
	// a single entry for a target that has been kicked
	class KickEntry
	{
	public:
		Ascii	target;			// the pattern that was used
		Ascii	kicked_by;		// cid of the kicker
		Ascii	reason;			// reason for the kick
		time_t	timestamp;		// when the kick was made
		int		duration;		// -1 meaning forever
	};

	// maintains a list of people that have been kicked from the network + those that have been banned
	class KickList
	{
	protected:
		std::vector<KickEntry> m_list;
		time_t	last_purged;

	public: // standard vector operators
		size_t size() const { return m_list.size(); }
		KickEntry& operator[](size_t Index) { return m_list[Index]; }
		void clear() { m_list.clear(); }

	public: // mechanisms for kicking
		bool InsertElement(KickEntry& Entry);
		KickEntry* CheckStatus(Avatar& Target);
		bool RemoveElement(Ascii Target);
		void AutoPurge();

	public: // archiving
		bool Load(Ascii Path);
		bool Save(Ascii Path);
	};

	extern KickList g_KickList;
}