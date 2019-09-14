#pragma once
#include "umbra/umbra_remoteclass.h"
#include "movement_interface.h"


namespace Movement
{
	class CircleKernel : public IMovement, public Umbra::RemoteClass
	{
	protected: // tracking
		float	m_radians;			// the radian rotation
		String	m_center;			// the current position
		float	m_radius;			// how far from the position

	public: // read access
		float	getRadians()	const { return m_radians;	}
		String	getCenter()		const { return m_center;	}
		float	getRadius()		const { return m_radius;	}
	
	public: // write access
		void	setup(String Pos, int Radius);

	public: // conversion
		int		getNextChange(Controller* Control, AW::Location Current, AW::Location& Result, float Resolution);
	};
}