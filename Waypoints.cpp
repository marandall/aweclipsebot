#include "stdafx.h"
#include "movement_waypoints.h"
#include "fileloader.h"

using namespace std;

namespace Movement
{
	Waypoints::Waypoints()
	{
		m_cur_line			= 0;			// current command line
		m_next_target		= "";			// coords value of next target
		
		m_speed				= 1000;			// how fast to move in cm/sec
		m_auto_pitch		= false;		// should the pitch go up and down
		m_last_tick			= 0;			// the time the waypoints were paused
		m_pause_start		= 0;			// the time of pausing
		m_hit_exact			= true;			// should each waypoint be hit exactly

		m_offsetx			= 0;			// x offset
		m_offsety			= 0;			// y offset
		m_offsetz			= 0;			// z offset

		m_acceleration		= 0;			// acceleration in m/s/s
		m_targetspeed		= 0;			// the target speed
		m_roll_rate_d		= 0;			// the rate at which we roll
		m_max_yaw_rate_d	= 0;			// the rate at which it can turn
		m_turn_roll_ratio	= 0;			// the roll ratio
	}

	bool Waypoints::StringToBool(String Value)
	{
		Value.MakeLower();
		return Value == "true" || Value == "on" || Value == "yes";
	}

	bool Waypoints::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		// Property Requests 
		//
		UMBRA_PROPERTY_RW(current_line,			m_cur_line);
		UMBRA_PROPERTY_R (next_target,			m_next_target);
		UMBRA_PROPERTY_RW(fixed_yaw_target,		m_fixed_yaw_target);
		UMBRA_PROPERTY_R (count,				(int)path_list.size());

		UMBRA_PROPERTY_RW(speed,				m_speed);
		UMBRA_PROPERTY_RW(auto_pitch,			m_auto_pitch);
		UMBRA_PROPERTY_RW(last_tick,			m_last_tick);
		UMBRA_PROPERTY_RW(pause_start,			m_pause_start);
		UMBRA_PROPERTY_RW(hit_exact,			m_hit_exact);

		UMBRA_PROPERTY_RW(offset_x,				m_offsetx);
		UMBRA_PROPERTY_RW(offset_y,				m_offsety);
		UMBRA_PROPERTY_RW(offset_z,				m_offsetz);

		UMBRA_PROPERTY_RW(acceleration,			m_acceleration);
		UMBRA_PROPERTY_RW(target_speed,			m_targetspeed);
		UMBRA_PROPERTY_RW(roll_rate,			m_roll_rate_d);
		UMBRA_PROPERTY_RW(max_yaw_rate,			m_max_yaw_rate_d);
		UMBRA_PROPERTY_RW(turn_roll_ratio,		m_turn_roll_ratio);
		UMBRA_PROPERTY_RW(yaw_adjustment,		m_yaw_adjustment);

		// Nothing found
		//
		return Umbra::PropertyRequest::HAS_HANDLED;
	}

	bool Waypoints::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		// 
		// load a waypoints file
		//

		UMBRA_FUNCTION_BEGIN(loadWaypoints) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Key, Umbra::VT_STRING);

			// code information break
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( LoadWaypoints(vaKey.toString().c_str()) );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// Clear the waypoints file
		//

		UMBRA_FUNCTION_BEGIN(clear) {
			// code information break
			UMBRA_FUNCTION_CS() {
				path_list.clear();
				m_cur_line = 0;
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// Clear the waypoints file after the current file
		//

		UMBRA_FUNCTION_BEGIN(clearAfter) {
			UMBRA_FUNCTION_DA(0, Index, Umbra::VT_INDEX);

			// code information break
			UMBRA_FUNCTION_CS() {
				size_t index = (size_t)vaIndex.toInt32();
				if (index >= 0 && index < path_list.size())
				{
					path_list.erase(path_list.begin() + index, path_list.end());
				}
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// 
		// add a waypoint
		//

		UMBRA_FUNCTION_BEGIN(add) {
			// required arguments
			UMBRA_FUNCTION_DA(0, String, Umbra::VT_STRING);

			// code information break
			UMBRA_FUNCTION_CS() {
				path_list.push_back( vaString.toString().c_str() );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// 
		// get the index
		//

		UMBRA_FUNCTION_BEGIN(line) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Index, Umbra::VT_INDEX);

			// code information break
			UMBRA_FUNCTION_CS() {
				size_t index = (size_t)vaIndex.toInt32();
				if (index >= 0 && index < path_list.size())
				{
					UMBRA_FUNCTION_RET( path_list[index] );
				}
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// nothing
		//

		return Umbra::FunctionRequest::HAS_HANDLED;
	}

	bool Waypoints::LoadWaypoints(const char* File)
	{
		FileReader file(File);
		if (!file.IsOK())
			return false;

		m_cur_line = 0;
		
		// load all of the line datas
		path_list.clear();
		for(size_t i = 0; i < file.size(); i++)
			if (file[i].GetLength())
				path_list.push_back(file[i]);
		return true;
	}

	int Waypoints::getNextChange(Controller* Control, AW::Location Current, AW::Location& Result, float Resolution)
	{
		// check cancel
		if (path_list.size() == 0)
			return DO_NOTHING;

		// item too large
		if (m_cur_line >= path_list.size())
			m_cur_line = 0;

		// handle each of the data
		size_t count_loop = 0;
		String current_string;
		while (true)
		{
			if (count_loop++ > path_list.size())
				return DO_NOTHING;

			// split the command and the arguments
			AW::SplitString s_data(path_list[m_cur_line], " ", 2);
			String		s_cmd = s_data[0]; s_cmd.MakeLower(); s_cmd.Trim();
			String		s_arg = s_data[1]; s_arg.MakeLower(); s_arg.Trim();

			// switch the command style
			if (s_cmd[0] == '#')
			{
				goNext();
				continue;
			}

			// set the velocity
			else if (s_cmd == "speed")
			{
				float f_res		= static_cast<float>(atof(s_arg)) * 100.0f;		// in meters per second
				m_speed			= (int)(f_res);
				m_targetspeed	= m_speed;
			}

			// the roll rate
			else if (s_cmd == "roll_rate")
			{
				m_roll_rate_d = atoi(s_arg) * 10;			// in tenths of degree a second
			}

			// the ration with which we roll
			else if (s_cmd == "roll_ratio")
			{
				m_turn_roll_ratio = atoi(s_arg) * 10;		// in tenths of degree
			}

			// limit the rate at which the mover can yaw
			else if (s_cmd == "yaw_rate")
			{
				m_max_yaw_rate_d = atoi(s_arg) * 10;	// in tenths of degree
			}

			// fixed yaw direction
			else if (s_cmd == "fix_yaw")
			{
				if (s_arg == "null")
					m_fixed_yaw_target = "";	
				else
					m_fixed_yaw_target = s_arg;
			}

			// if we want to stop the bot immediately
			else if (s_cmd == "stop")
			{
				return DO_NOTHING;
			}

			// do we want pitch to be calculated
			else if (s_cmd == "auto_pitch")
			{
				m_auto_pitch = StringToBool(s_arg);
			}

			// do we want to pass through each wapoint exactly
			else if (s_cmd == "hit_exact")
			{
				m_hit_exact = StringToBool(s_arg);
			}

			// change the offset
			else if (s_cmd.Left(7) == "offset_")
			{
				// calculate coordinates
				AW::Coordinates crds;
				crds.FromString(s_arg);

				// which offset is it?
				String offsettype = s_cmd.Mid(8);
				if (offsettype == "x")
				{	
					m_offsetx = crds.getX();
				}
				else if (offsettype == "y")
				{
					m_offsety = crds.getY();
				}
				else if (offsettype == "z")
				{
					m_offsetz = crds.getZ();
				}
			}

			// accelerate speed
			else if (s_cmd == "accelerate")
			{
				SplitString s_args2(s_arg, ",", 2);
				if (s_args2.size() == 2)	
				{
					// trim the argument strings down
					String s_mss = s_args2[0]; s_mss.Trim();
					String s_ac2 = s_args2[1]; s_ac2.Trim();

					// accelerate in meters per second upto given velocity
					m_acceleration	= (int)(atof(s_mss) * 100);		// meters per second
					m_targetspeed	= (int)(atof(s_ac2) * 100);		// meters per second
				}
			}

			// pausing holds off incrementing for x period
			else if (s_cmd == "pause")
			{
				int pause_time = (int)(atof(s_arg) * 1000);
				int expire_at  = m_pause_start + pause_time;
				int cur_time   = GetTickCount();

				int gtc_mps = GetTickCount() - m_pause_start;

				if (m_pause_start == 0)
				{
					m_pause_start = GetTickCount();	// indicate the start of paused
					return DO_PAUSE;
				}
				else if (gtc_mps >= pause_time)
				{
					m_pause_start = 0;				// reset pause start time
					m_cur_line = (++m_cur_line) % (int)path_list.size();
					continue;						// move on to doing next thing
				}
				else
				{
					return DO_PAUSE;		// we are still paused
				}
			}
			else if (s_cmd == "goto")
			{
				// search for the symbo
				// message:
				CStringA searchParam = s_arg + ":";

				// loop for the element to jump to
				for (size_t i = 0; i < path_list.size(); i++)
				{
					if (path_list[i].CompareNoCase(searchParam) == 0)
					{
						m_cur_line = (int)i;
						continue;
					}
				}
			}
			else if (s_cmd == "end")
			{
				continue;
			}
			else if (s_cmd == "teleport")
			{
				//Location next(s_arg);
				//NextMove = next.getThisLocation();

				// next time we execute another step
				goNext();
			}

			else if (s_cmd == "resolve")
			{
				current_string = s_arg;
				break;
			}

			// is it some other kind of command, implying coordinates
			else if (isdigit((int)s_cmd[0]))
			{
				current_string = path_list[m_cur_line];
				break;
			}

			// skip to the next line
			goNext();
		}

		/*
			Acceleration
			-------------------------------
				- Changes the speed up to or down to a given value
				  while-ever the bot is actually moving
		*/
		if (m_targetspeed != m_speed)
		{
			bool sign = m_speed > m_targetspeed;
			
			// increase the speed by the acceleration times resolution
			m_speed+= static_cast<int>(m_acceleration * Resolution);
			
			// if the sign has switched, ensure its set exactly to the speed
			if ((m_speed > m_targetspeed) != sign)
				m_speed = m_targetspeed;
		}





		/*
			Lookup Location
			-------------------------------
				- Uses classic lookup styles
				- Updated to use the AW location
		*/
		::Location crdTarget(current_string, NULL);

		/*
			Calculations
			-------------------------------
				- the distance to the target
				- the target height
		*/
		int		distance_gd		= static_cast<int>(Current.distanceToGD(crdTarget));
		int		distance_3d		= static_cast<int>(Current.distanceTo(crdTarget));
		int		targetheight	= crdTarget.getY() + m_offsety;
		float	vertical_pitch	= static_cast<float>(
										atan2((float)(targetheight - Current.getY()), (float)distance_gd ));

		// angle between targets
		float targetyaw_rads	= (float)Current.AngleBetweenR(crdTarget);
		float current_yaw_rads	= Current.GetRadianYAW();
		float yaw_change_rads	= targetyaw_rads - current_yaw_rads;

		// convert the maximum yaw rate into radians 
		float max_yaw_rate_rads	= AW::_XConvert::aw_angle_to_radians(m_max_yaw_rate_d) * Resolution;

		/*
			Target YAW limiting
			-------------------------------
				- target yaw is limited by the
				  maximum rate of which it turns
		*/
		if (m_max_yaw_rate_d != 0)
		{
			// bounding
			if (yaw_change_rads > max_yaw_rate_rads)
				yaw_change_rads = max_yaw_rate_rads;
			else if (yaw_change_rads < (-max_yaw_rate_rads))
				yaw_change_rads = (-max_yaw_rate_rads);

			// set the new angle to be that which we have changed to
			targetyaw_rads = current_yaw_rads + yaw_change_rads;
		}

		/*
			Speed Limiting
			-------------------------------
				- checks if we have hit, therefore gone through, our target
				  and should move on
			    - reduces speed if we want to hit exactly
		*/
		int speed		= static_cast<int>(m_speed * Resolution);		// portion of a seconds motion
		bool has_passed = false;
		
		// do we need to hit our target exactly or more importantly have we passed it?
		if (speed > distance_3d) 
		{
			if (m_hit_exact)
				speed	= distance_3d;
			has_passed	= true;
		}
		
		// x z speed available
		float vel_xz	= speed * cos(vertical_pitch);
		float vel_y		= speed * sin(vertical_pitch);

		// sanity check; should be constant
		float vCheck	= pow(
						pow((float)vel_xz, 2.0f) + pow((float)vel_y, 2.0f),
						0.5f);

		/*
			Establish cooridnates
			-------------------------------
		*/
		int	new_x		= static_cast<int>(Current.getX() + sin(targetyaw_rads) * vel_xz);
		int	new_z		= static_cast<int>(Current.getZ() + cos(targetyaw_rads) * vel_xz);
		int	new_y		= static_cast<int>(Current.getY() + vel_y);
		int new_yaw		= static_cast<int>(AW::_XConvert::aw_radians_to_angle(targetyaw_rads));
		int new_pitch	= 0;
		int new_roll	= 0;
		int delta_y		= static_cast<int>(sin(vertical_pitch) * vel_y);

		/*
			Automatic Pitching
			-------------------------------
				- Points the nose up or down depending on if the setting is true
		*/
		if (m_auto_pitch == true)
		{
			new_pitch = static_cast<int>(AW::_XConvert::aw_radians_to_angle(vertical_pitch));
		}

		/*
			Automatic Rolling
			-------------------------------
				- Rolls the mover a given angle left or right 
				  depending on the rate of turn
			    - ignored if there is no roll ratio
		*/
		if (m_turn_roll_ratio != 0)
		{
			// change the radian change into an integer change
			int		turn_rate_degs	= static_cast<int>(AW::_XConvert::aw_radians_to_angle(yaw_change_rads));
			
			// bounds checking
			if (turn_rate_degs < -900)
				turn_rate_degs = -900;
			
			else if (turn_rate_degs > 900) 
				turn_rate_degs = 900;

			// turn rate equals the ratio
			double ratio = 1.0f / 900. * m_turn_roll_ratio;
			new_roll = static_cast<int>(-turn_rate_degs * ratio);
		}

		/*
			YAW Fixing
			-------------------------------
				- Makes the position always face a certain direction (@<value>)
				- Makes the position always face a certain direction (
		*/
		if (m_fixed_yaw_target.GetLength() > 0)
		{
			// constant pitch
			if (m_fixed_yaw_target.Left(6) == "const ")
			{
				::Location	aim(m_fixed_yaw_target.Mid(6), NULL);
				new_yaw = aim.getYAW();
			}

			// explicit direction 
			else
			{
				::Location		aim(m_fixed_yaw_target, NULL);
				AW::Location	cur_pos(new_x, new_y, new_z, new_yaw);
				new_yaw = cur_pos.AngleBetween(aim);
			}
		}

		/*
			set Location Object
			-------------------------------
				- creates a new location object
		*/
		AW::LocationEx lEx;
		lEx.set(new_x, new_y, new_z, new_yaw);
		lEx.setTilt(-new_pitch);
		lEx.SetRoll(new_roll);
		Result = lEx;


		// if we are at the next location jump forward
		if (has_passed || Result.distanceTo(crdTarget) < 50)
			goNext();
		
		return DO_MOVE;
	}
}
