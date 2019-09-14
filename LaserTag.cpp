#include "stdafx.h"
#include "LaserTag_AvatarClass.h"
#include "LaserTag_AvatarData.h"
#include "LaserTag_settings.h"

namespace LaserTag
{
	AvatarClass::AvatarClass()
	{
		m_pLaserTag = new AvatarData();
	}
	
	AvatarClass::~AvatarClass()
	{
		if (m_pLaserTag) 
			delete m_pLaserTag;
	}





}