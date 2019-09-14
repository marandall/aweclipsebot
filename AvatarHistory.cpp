#include "stdafx.h"
#include "AvatarHistory.h"

namespace History
{
	void AvatarArticle::begin(const Avatar& Source)
	{
		// effective initialisation
		m_enter_time = 0;
		m_exit_time	 = 0;

		// autonumber
		static int unique_id = 0;
		m_unique_id = (unique_id++);

		// copy routunes
		CopyData(Source);
		m_enter_time	 = time64(0);
		m_enter_position = Source;
	}

	void AvatarArticle::update(const Avatar& Source)
	{
		CopyData(Source);
	}
	
	void AvatarArticle::finalise(const Avatar& Source)
	{
		CopyData(Source);
		m_exit_time		 = time64(0);
		m_exit_position	 = Source;
	}

	//
	//
	// Avatars Database
	//
	//

	AvatarHistory& AvatarHistory::getInstance()
	{
		static AvatarHistory ah;
		return ah;
	}

	AvatarArticle* AvatarHistory::search(int UniqueId)
	{
		for (iterator itr = begin(); itr != end(); itr++)
			if (itr->getUniqueId() == UniqueId)
				return &*itr;
		return NULL;
	}

	int AvatarHistory::begin(const Avatar& Source)
	{
		AvatarArticle av;
		av.begin(Source);
		list.push_back(av);
		return av.getUniqueId();
	}

	void AvatarHistory::update(const Avatar& Source)
	{
		AvatarArticle* pArt = search(Source.getHistoricId());
		if (pArt) pArt->update(Source);
	}
	
	void AvatarHistory::finalise(const Avatar& Source)
	{
		AvatarArticle* pArt = search(Source.getHistoricId());
		if (pArt) pArt->finalise(Source);
	}

	//
	// Query Mechanisms
	//

	AvatarHistory::ResultSet AvatarHistory::queryIp(int IP)
	{
		// operational query against each row
		ResultSet rs;
		for (iterator itr = begin(); itr != end(); itr++)
			if (itr->getAddressLng() == IP)
				rs.push_back(&*itr);

		return rs;
	}

	AvatarHistory::ResultSet AvatarHistory::queryCitizen(int Citizen)
	{
		// operational query against each row
		ResultSet rs;
		for (iterator itr = begin(); itr != end(); itr++)
			if (itr->getCitizen() == Citizen)
				rs.push_back(&*itr);

		return rs;
	}
	
	AvatarHistory::ResultSet AvatarHistory::queryPrivilege(int Privilege)
	{
		// operational query against each row
		ResultSet rs;
		for (iterator itr = begin(); itr != end(); itr++)
			if (itr->getPrivilege() == Privilege)
				rs.push_back(&*itr);

		return rs;
	}
}