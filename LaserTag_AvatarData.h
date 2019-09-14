#pragma once

namespace LaserTag
{
	class AvatarData
	{
	public:
		int		team;
		bool	shielded;
		
	public: /* scorecard */
		int		health;
		int		kills;
		int		deaths;

	public: /* helper functions */
		float	getRatio()	{ return (float)kills/deaths; }
	};
}