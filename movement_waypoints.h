#pragma once
#include <vector>
#include <math.h>
#include "umbra/umbra_remoteclass.h"
#include "awsdk_location.h"
#include "awsdk_support.h"
#include "movement_interface.h"

/**
	Waypoints File Format
	------------------------------------

	INIT SEQ
	MODE STATIC, NOT_RELATIVE

	MOVE   27.685n 23.999e -909.7a 180
	MOVE   31.362n 24.04e -903.2a 181
	MOVE   33.253n 20.298e -861.8a 181
	MOVE   27.507n 20.459e -874.0a 226
	PAUSE  5

*/

using namespace std;

namespace Movement
{
	class Waypoints : public IMovement, public Umbra::RemoteClass
	{
	public: // umbra interface
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);

	public: // variable data
		vector<String> path_list;			// the list of action
		int			m_cur_line;				// current command line
		String		m_next_target;			// coords value of next target
		String		m_fixed_yaw_target;		// should we use a fixed yaw, if so where

		int			m_speed;				// how fast to move in cm/sec
		bool		m_auto_pitch;			// should the pitch go up and down
		int			m_last_tick;			// the time the waypoints were paused
		int			m_pause_start;			// the time of pausing
		bool		m_hit_exact;			// should each waypoint be hit exactly

		int			m_offsetx;				// x offset
		int			m_offsety;				// y offset
		int			m_offsetz;				// z offset

		int			m_acceleration;			// acceleration in m/s/s
		int			m_targetspeed;			// the target speed
		int			m_roll_rate_d;			// the rate at which we roll
		int			m_max_yaw_rate_d;		// the rate at which it can turn
		int			m_turn_roll_ratio;		// the roll ratio
		int			m_yaw_adjustment;		// the yaw adjustment (10ths of degree)

	public: // index read access
		int			getCurrentLine()		const { return m_cur_line;		}
		String		getCurrentWaypoint()	const { return path_list[m_cur_line]; }

	public: // read access
		int			getSpeed()				const { return m_speed;			}
		bool		getAutoPitch()			const { return m_auto_pitch;	}
		int			getPauseStart()			const { return m_pause_start;	}
		bool		getHitExact()			const { return m_hit_exact;		}
		int			getOffsetX()			const { return m_offsetx;		}
		int			getOffsetY()			const { return m_offsety;		}
		int			getOffsetZ()			const { return m_offsetz;		}

	public: // write access
		void		changeSpeed(int Speed)	{ m_speed = Speed; }


	public: // operations
		void		goNext() { m_cur_line = ++m_cur_line % (int)path_list.size(); }
		void		reset()  { m_cur_line = 0; }

	public: // the different modes of control
		const static int METHOD_COORDINATES	= 1;
		const static int METHOD_DIRECTION	= 2;

	public: // takeoff routines
		int			takeoff_routine;

	public: // paused state
		bool		paused;
		bool		IsPaused() const { return paused;	}
		void		Pause()			 { paused = true;	}
		void		Continue()		 { paused = false;	}

	public: // readers
		size_t		size() const	 { return path_list.size(); }
		int			getCurLine() const { return m_cur_line; }

	public: // io
		bool		LoadWaypoints(const char* File);

	public:
		Waypoints();
		bool	StringToBool(String Value);

	public: // next kernel
		int		getNextChange(Controller* Control, AW::Location Current, AW::Location& Result, float Resolution);;
	};
}