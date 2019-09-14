#pragma once

namespace LaserTag
{
	class AvatarData;

	class AvatarClass
	{
	protected: /*  */
		AvatarData*	m_pLaserTag;

	public:
		AvatarClass();
		~AvatarClass();

	public: /* access method */
		AvatarData&	getLaserTag() { return *m_pLaserTag; }
	};
};