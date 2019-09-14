#include "stdafx.h"
#include "awsdk_location.h"
#include "awsdk_bot.h"
#include "awsdk_avatar.h"
#include "awsdk_citizen.h"
#include "awsdk_objects_v3.h"
#include "awsdk_entity.h"
#include "awsdk_hudelement.h"

#ifdef _AWSDK_USE_UMBRA

using namespace Umbra;

namespace AW
{
	bool Location::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{ 
		// reflect get functions
		UMBRA_PROPERTY_R(coordinates,	(const char*)getCoordinates());
		
		// which state
		if (getAllowScriptedLocationChanges() == true)
		{
			UMBRA_PROPERTY_RW(x,				m_x);
			UMBRA_PROPERTY_RW(y,				m_y);
			UMBRA_PROPERTY_RW(z,				m_z);
			UMBRA_PROPERTY_RW(yaw,			m_yaw);
			UMBRA_PROPERTY_RW(tilt,			m_pitch);
			UMBRA_PROPERTY_RW(roll,			m_roll);
		}
		else
		{
			UMBRA_PROPERTY_R(x,					m_x);
			UMBRA_PROPERTY_R(y,					m_y);
			UMBRA_PROPERTY_R(z,					m_z);
			UMBRA_PROPERTY_R(yaw,				m_yaw);
			UMBRA_PROPERTY_R(tilt,			m_pitch);
			UMBRA_PROPERTY_R(roll,			m_roll);
		}

		UMBRA_PROPERTY_R(cell_x,		getCellX());
		UMBRA_PROPERTY_R(cell_z,		getCellZ());
		UMBRA_PROPERTY_R(sector_x,		getSectorX());
		UMBRA_PROPERTY_R(sector_z,		getSectorZ());

		// nothing found
		return Umbra::PropertyRequest::NOT_HANDLED;
	}

	bool Location::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R) 
	{
		//
		// distance finding
		//

		UMBRA_FUNCTION_BEGIN(distanceTo) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Location,Umbra::VT_CLASS);

			// required class
			vaLocation;
			const type_info& ci = typeid(vaLocation.toRemoteClass());

			UMBRA_FUNCTION_CC(Location, AW::Location);

			// code information break
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( pLocation->distanceTo(*this) );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// distance finding over the ground
		//

		UMBRA_FUNCTION_BEGIN(distanceTo2D) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Location,Umbra::VT_CLASS);

			// required class
			UMBRA_FUNCTION_CC(Location, AW::Location);

			// code information break
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( pLocation->distanceToGD(*this) );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// is a point within a rect
		//

		UMBRA_FUNCTION_BEGIN(withinRect) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Top, Umbra::VT_CLASS);
			UMBRA_FUNCTION_DA(1, Bottom, Umbra::VT_CLASS);

			// required class
			UMBRA_FUNCTION_CC(Top, AW::Location);
			UMBRA_FUNCTION_CC(Bottom, AW::Location);

			// code information break
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( PointInRect(*pTop, *pBottom) );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// the angle between 2 locations in radians
		//

		UMBRA_FUNCTION_BEGIN(angleBetween) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Location, Umbra::VT_CLASS);

			// required class
			UMBRA_FUNCTION_CC(Location, AW::Location);

			// code information break
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( AngleBetweenR(*pLocation) );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// the new position when run along an offset
		//

		UMBRA_FUNCTION_BEGIN(getOffsetLocation) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Distance, Umbra::VT_NUMBER);
			UMBRA_FUNCTION_DA(1, Altitude, Umbra::VT_NUMBER);

			// code information break
			UMBRA_FUNCTION_CS() {
				Location* pLoc = new Location(getOffsetPosition(vaDistance.toInt32(), vaAltitude.toInt32()));
				pLoc->setUmbraGarbageCollected();
				UMBRA_FUNCTION_RET( *pLoc );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// editing parameters
		//
		
		if (getAllowScriptedLocationChanges() == true)
		{
			String s = R.id.c_str();
			UMBRA_FUNCTION_BEGIN(useLocation) {
				// required arguments
				UMBRA_FUNCTION_DA(0, Location, Umbra::VT_CLASS);
				UMBRA_FUNCTION_CC(Location, AW::Location);

				// code information break
				UMBRA_FUNCTION_CS() {
					*this = *pLocation;
					UMBRA_FUNCTION_RET( *this );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();
		}

		// nothing found
		return Umbra::PropertyRequest::NOT_HANDLED;
	}

	bool Citizen::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		// reflected functions
		UMBRA_PROPERTY_R(number,				getNumber());
		UMBRA_PROPERTY_R(name,					getName());
		UMBRA_PROPERTY_R(password,				getPassword());
		UMBRA_PROPERTY_R(email,					getEmail());
		UMBRA_PROPERTY_R(time_left,				getTimeLeft());
		UMBRA_PROPERTY_R(privilege_password,	getPrivilegePassword());
		UMBRA_PROPERTY_R(immigration_time,		getImmigrationTime());
		UMBRA_PROPERTY_R(expiration_time,		getExpirationTime());
		UMBRA_PROPERTY_R(beta,					getBeta());
		UMBRA_PROPERTY_R(last_login,			getLastLogin());
		UMBRA_PROPERTY_R(bot_limit,				getBotLimit());
		UMBRA_PROPERTY_R(total_time,			getTotalTime());
		UMBRA_PROPERTY_R(enabled,				getEnabled());
		UMBRA_PROPERTY_R(comment,				getComment());
		UMBRA_PROPERTY_R(url,					getUrl());
		UMBRA_PROPERTY_R(privacy,				getPrivacy());
		UMBRA_PROPERTY_R(trial,					getTrial());
		UMBRA_PROPERTY_R(cav_enabled,			getCavEnabled());
		UMBRA_PROPERTY_R(cav_template,			getCavTemplate());
		UMBRA_PROPERTY_R(last_address,			getLastAddress());

		// nothing found
		return Umbra::PropertyRequest::NOT_HANDLED;
	}

	bool AvatarData::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		// the properties
		UMBRA_PROPERTY_R(name,					getName());
		UMBRA_PROPERTY_R(session,				getSession());	
		UMBRA_PROPERTY_R(citizen,				getCitizen());
		UMBRA_PROPERTY_R(privilege,				getPrivilege());
		UMBRA_PROPERTY_R(priv_name,				getPrivName());
		UMBRA_PROPERTY_R(citizen_info,			getCitizenInfo());

		// ip information
		UMBRA_PROPERTY_R(address,				getAddress());
		UMBRA_PROPERTY_R(address_lus,			getAddressLUS());
		UMBRA_PROPERTY_R(address_lng,			(int)getAddressLng());
		UMBRA_PROPERTY_R(dns,					getDns());
		UMBRA_PROPERTY_R(dns_lus,				getDnsLUS());

		// avatar state information
		UMBRA_PROPERTY_R(avatar,				getAvatar());
		UMBRA_PROPERTY_R(state,					getState());
		UMBRA_PROPERTY_R(gesture,				getGesture());

		// lower functions
		UMBRA_PROPERTY_REDIRECT(Location);

		// nothing found
		return Umbra::PropertyRequest::NOT_HANDLED;
	}

	bool ObjectData::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		// strings
		UMBRA_PROPERTY_RW(model,				model);
		UMBRA_PROPERTY_RW(description,	description);
		UMBRA_PROPERTY_RW(action,				action);
		
		// build info
		UMBRA_PROPERTY_R(name,					name);
		UMBRA_PROPERTY_R(id,						id);
		UMBRA_PROPERTY_R(number,				number);
		UMBRA_PROPERTY_RW(owner,				owner);
		UMBRA_PROPERTY_RW(timestamp,		timestamp);
		UMBRA_PROPERTY_R (type,					type);							// read only

		// type checks on the object
		UMBRA_PROPERTY_R(is_v3,					isV3());
		UMBRA_PROPERTY_R(is_v4,					isZone());
		UMBRA_PROPERTY_R(is_particle,		isParticle());
		UMBRA_PROPERTY_R(is_mover,			isMover());
		UMBRA_PROPERTY_R(is_camera,			isCamera());

		// check base classes
		UMBRA_PROPERTY_REDIRECT(Location);
	
		// nothing found
		return Umbra::PropertyRequest::NOT_HANDLED;
	}

	bool ObjectData::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		UMBRA_FUNCTION_REDIRECT(AW::Location);

		//
		// zone data
		//

		UMBRA_FUNCTION_BEGIN(getZone) {
			// code information break
			UMBRA_FUNCTION_CS() {
				if (isZone() == false)
				{
					UMBRA_FUNCTION_RET( Umbra::Variant() );
				}
				else
				{
					Object_Zone* pObj = new Object_Zone(&binary_array);
					pObj->setUmbraGarbageCollected();
					UMBRA_FUNCTION_RET( *pObj );
				}
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(getParticle) {
			// code information break
			UMBRA_FUNCTION_CS() {
				if (isParticle() == false)
				{
					UMBRA_FUNCTION_RET( Umbra::Variant() );
				}
				else
				{
					Object_Particle* pObj = new Object_Particle(&binary_array);
					pObj->setUmbraGarbageCollected();
					UMBRA_FUNCTION_RET( *pObj );
				}
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(getCamera) {
			// code information break
			UMBRA_FUNCTION_CS() {
				if (isCamera() == false)
				{
					UMBRA_FUNCTION_RET( Umbra::Variant() );
				}
				else
				{
					Object_Camera* pObj = new Object_Camera(&binary_array);
					pObj->setUmbraGarbageCollected();
					UMBRA_FUNCTION_RET( *pObj );
				}
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(getMover) {
			// code information break
			UMBRA_FUNCTION_CS() {
				if (isMover() == false)
				{
					UMBRA_FUNCTION_RET( Umbra::Variant() );
				}
				else
				{
					Object_Mover* pObj = new Object_Mover(&binary_array);
					pObj->setUmbraGarbageCollected();
					UMBRA_FUNCTION_RET( *pObj );
				}
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// nothing found
		return Umbra::FunctionRequest::NOT_HANDLED;
	}

	bool Entity::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		// reflections
		UMBRA_PROPERTY_R(type,					getType());	// <func id="getType()" about="Returns the entity type" />
		UMBRA_PROPERTY_R(id,					getId());
		UMBRA_PROPERTY_R(state,					getState());
		UMBRA_PROPERTY_R(flags,					getFlags());
		UMBRA_PROPERTY_R(owner_session,			getOwnerSession());
		UMBRA_PROPERTY_R(model_number,			getModelNum());
		UMBRA_PROPERTY_R(owner_citizen,			getOwnerCitizen());
		UMBRA_PROPERTY_R(name,					getName());

		// state checking	
		UMBRA_PROPERTY_R(is_idle,				isStateIdle());
		UMBRA_PROPERTY_R(is_start,				isStateStart());
		UMBRA_PROPERTY_R(is_move,				isStateMove());
		UMBRA_PROPERTY_R(is_paused,				isStatePause());
		UMBRA_PROPERTY_R(is_continue,			isStateContinue());
		UMBRA_PROPERTY_R(is_stop,				isStateStop());
		UMBRA_PROPERTY_R(is_reset,				isStateReset());

		// redirect to location
		UMBRA_PROPERTY_REDIRECT(AW::Location);

		// nothing found
		return Umbra::PropertyRequest::NOT_HANDLED;
	}

	int flags_update(int& F, int Flag, bool Use)
	{
		if (Use) return F | Flag;
		else	 return F & (~Flag);
	}

	bool HUD_Element::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		// hud elements
		UMBRA_PROPERTY_RW(type,					type);
		UMBRA_PROPERTY_RW(id,					id);
		UMBRA_PROPERTY_RW(origin,				origin);
		UMBRA_PROPERTY_RW(x,					x);
		UMBRA_PROPERTY_RW(y,					y);
		UMBRA_PROPERTY_RW(z,					z);
		UMBRA_PROPERTY_RW(flags,				flags);
		UMBRA_PROPERTY_RW(text,					text);
		UMBRA_PROPERTY_RW(color,				color);
		UMBRA_PROPERTY_RW(opacity,				opacity);
		UMBRA_PROPERTY_RW(size_x,				size_x);
		UMBRA_PROPERTY_RW(size_y,				size_y);
		UMBRA_PROPERTY_RW(size_z,				size_z);
		UMBRA_PROPERTY_RW(texture_offset_x,		texture_offsetx);
		UMBRA_PROPERTY_RW(texture_offset_y,		texture_offsety);

		// read access flags
		UMBRA_PROPERTY_R(flag_clicks,			(flags & AW_HUD_ELEMENT_FLAG_CLICKS)		!= 0);
		UMBRA_PROPERTY_R(flag_stretch,			(flags & AW_HUD_ELEMENT_FLAG_STRETCH)		!= 0);
		UMBRA_PROPERTY_R(flag_additive,			(flags & AW_HUD_ELEMENT_FLAG_ADDITIVE)		!= 0);
		UMBRA_PROPERTY_R(flag_size_percent,		(flags & AW_HUD_ELEMENT_FLAG_SIZE_PERCENT)	!= 0);
		UMBRA_PROPERTY_R(flag_transition,		(flags & AW_HUD_ELEMENT_FLAG_TRANSITION)	!= 0);
		UMBRA_PROPERTY_R(flag_temporary,		(flags & AW_HUD_ELEMENT_FLAG_TEMPORARY)		!= 0);
		UMBRA_PROPERTY_R(flag_universe_path,	(flags & AW_HUD_ELEMENT_FLAG_UNIVERSE_PATH) != 0);
		UMBRA_PROPERTY_R(flag_clamp,			(flags & AW_HUD_ELEMENT_FLAG_CLAMP)			!= 0);
		UMBRA_PROPERTY_R(flag_highlight,		(flags & AW_HUD_ELEMENT_FLAG_HIGHLIGHT)		!= 0);

		// write access to the individual flag elements
		UMBRA_PROPERTY_WE(flag_clicks,				flags = flags_update(flags, AW_HUD_ELEMENT_FLAG_CLICKS,			V.toBool()));
		UMBRA_PROPERTY_WE(flag_stretch,				flags = flags_update(flags, AW_HUD_ELEMENT_FLAG_STRETCH,		V.toBool()));
		UMBRA_PROPERTY_WE(flag_additive,			flags = flags_update(flags, AW_HUD_ELEMENT_FLAG_ADDITIVE,		V.toBool()));
		UMBRA_PROPERTY_WE(flag_size_percent,	flags = flags_update(flags, AW_HUD_ELEMENT_FLAG_SIZE_PERCENT,	V.toBool()));
		UMBRA_PROPERTY_WE(flag_transition,		flags = flags_update(flags, AW_HUD_ELEMENT_FLAG_TRANSITION,		V.toBool()));
		UMBRA_PROPERTY_WE(flag_temporary,			flags = flags_update(flags, AW_HUD_ELEMENT_FLAG_TEMPORARY,		V.toBool()));
		UMBRA_PROPERTY_WE(flag_universe_path,	flags = flags_update(flags, AW_HUD_ELEMENT_FLAG_UNIVERSE_PATH,	V.toBool()));
		UMBRA_PROPERTY_WE(flag_clamp,					flags = flags_update(flags, AW_HUD_ELEMENT_FLAG_CLAMP,			V.toBool()));
		UMBRA_PROPERTY_WE(flag_highlight,			flags = flags_update(flags, AW_HUD_ELEMENT_FLAG_HIGHLIGHT,		V.toBool()));

		// nothing found
		return Umbra::PropertyRequest::NOT_HANDLED;
	}

	bool Object_Zone::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		UMBRA_PROPERTY_RW(footsteps,		footstep);
		UMBRA_PROPERTY_RW(ambient,			ambient);
		UMBRA_PROPERTY_RW(camera,				camera);
		UMBRA_PROPERTY_RW(target_cursor,targetcur);
		UMBRA_PROPERTY_RW(voip_rights,	voip_rights);
		UMBRA_PROPERTY_RW(name,					name);

		// structure
		aw_object_data_zone& z = getStruct();
	
		// size dimensions
		UMBRA_PROPERTY_RW(size_x,			z.size.x);
		UMBRA_PROPERTY_RW(size_y,			z.size.y);
		UMBRA_PROPERTY_RW(size_z,			z.size.z);

		// additional properties
		UMBRA_PROPERTY_RW(version,		z.version);
		UMBRA_PROPERTY_RW(shape,			z.shape);
		UMBRA_PROPERTY_RW(priority,		z.priority);
		UMBRA_PROPERTY_RW(gravity,		z.gravity);
		UMBRA_PROPERTY_RW(friction,		z.friction);
		UMBRA_PROPERTY_RW(flags,			z.flags);
		UMBRA_PROPERTY_RW(color,			z.color);
		UMBRA_PROPERTY_RW(fog_min,		z.fog_min);
		UMBRA_PROPERTY_RW(fog_max,		z.fog_max);

		// nothing found
		return R.NOT_HANDLED;
	}

	bool Object_Zone::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		// nothing found
		return R.NOT_HANDLED;
	}

	bool Object_Particle::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		// structure
		aw_object_data_particles& p = getStruct();

		// volume automated code
		UMBRA_PROPERTY_RW(volume_min_x, p.volume.min.x);
		UMBRA_PROPERTY_RW(volume_min_y, p.volume.min.y);
		UMBRA_PROPERTY_RW(volume_min_z, p.volume.min.z);
		UMBRA_PROPERTY_RW(volume_max_x, p.volume.max.x);
		UMBRA_PROPERTY_RW(volume_max_y, p.volume.max.y);
		UMBRA_PROPERTY_RW(volume_max_z, p.volume.max.z);

		// speed automated code
		UMBRA_PROPERTY_RW(speed_min_x, p.speed.min.x);
		UMBRA_PROPERTY_RW(speed_min_y, p.speed.min.y);
		UMBRA_PROPERTY_RW(speed_min_z, p.speed.min.z);
		UMBRA_PROPERTY_RW(speed_max_x, p.speed.max.x);
		UMBRA_PROPERTY_RW(speed_max_y, p.speed.max.y);
		UMBRA_PROPERTY_RW(speed_max_z, p.speed.max.z);

		// accel automated code
		UMBRA_PROPERTY_RW(accel_min_x, p.accel.min.x);
		UMBRA_PROPERTY_RW(accel_min_y, p.accel.min.y);
		UMBRA_PROPERTY_RW(accel_min_z, p.accel.min.z);
		UMBRA_PROPERTY_RW(accel_max_x, p.accel.max.x);
		UMBRA_PROPERTY_RW(accel_max_y, p.accel.max.y);
		UMBRA_PROPERTY_RW(accel_max_z, p.accel.max.z);

		// angle automated code
		UMBRA_PROPERTY_RW(angle_min_x, p.angle.min.x);
		UMBRA_PROPERTY_RW(angle_min_y, p.angle.min.y);
		UMBRA_PROPERTY_RW(angle_min_z, p.angle.min.z);
		UMBRA_PROPERTY_RW(angle_max_x, p.angle.max.x);
		UMBRA_PROPERTY_RW(angle_max_y, p.angle.max.y);
		UMBRA_PROPERTY_RW(angle_max_z, p.angle.max.z);

		// spin automated code
		UMBRA_PROPERTY_RW(spin_min_x, p.spin.min.x);
		UMBRA_PROPERTY_RW(spin_min_y, p.spin.min.y);
		UMBRA_PROPERTY_RW(spin_min_z, p.spin.min.z);
		UMBRA_PROPERTY_RW(spin_max_x, p.spin.max.x);
		UMBRA_PROPERTY_RW(spin_max_y, p.spin.max.y);
		UMBRA_PROPERTY_RW(spin_max_z, p.spin.max.z);

		// size automated code
		UMBRA_PROPERTY_RW(size_min_x, p.size.min.x);
		UMBRA_PROPERTY_RW(size_min_y, p.size.min.y);
		UMBRA_PROPERTY_RW(size_min_z, p.size.min.z);
		UMBRA_PROPERTY_RW(size_max_x, p.size.max.x);
		UMBRA_PROPERTY_RW(size_max_y, p.size.max.y);
		UMBRA_PROPERTY_RW(size_max_z, p.size.max.z);

		// other attributes setting
		UMBRA_PROPERTY_RW(version, p.version);
		UMBRA_PROPERTY_RW(release_min, p.release_min);
		UMBRA_PROPERTY_RW(release_max, p.release_max);
		UMBRA_PROPERTY_RW(release_size, p.release_size);
		UMBRA_PROPERTY_RW(lifespan, p.lifespan);
		UMBRA_PROPERTY_RW(emitter_lifespan, p.emitter_lifespan);
		UMBRA_PROPERTY_RW(fade_in, p.fade_in);
		UMBRA_PROPERTY_RW(fade_out, p.fade_out);
		UMBRA_PROPERTY_RW(color_start, p.color_start);
		UMBRA_PROPERTY_RW(color_end, p.color_end);
		UMBRA_PROPERTY_RW(opacity, p.opacity);
		UMBRA_PROPERTY_RW(render_style, p.render_style);
		UMBRA_PROPERTY_RW(flags, p.flags);
		UMBRA_PROPERTY_RW(style, p.style);

		// strings
		UMBRA_PROPERTY_RW(asset_list, assetlist);
		UMBRA_PROPERTY_RW(name, name);

		// nothing found
		return R.NOT_HANDLED;
	}

	bool Object_Particle::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		// structure
		aw_object_data_particles& p = getStruct();

		UMBRA_FUNCTION_BEGIN(setVolumeRange) {
			UMBRA_FUNCTION_DA(0, X1, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(1, Y1, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(2, Z1, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(3, X2, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(4, Y2, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(5, Z2, Umbra::VT_DOUBLE);
			// code information break
			UMBRA_FUNCTION_CS() {
				p.volume.min.x = vaX1.toFloat();
				p.volume.min.y = vaY1.toFloat();
				p.volume.min.z = vaZ1.toFloat();
				p.volume.max.x = vaX2.toFloat();
				p.volume.max.y = vaY2.toFloat();
				p.volume.max.z = vaZ2.toFloat();
				UMBRA_FUNCTION_RET( *this );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(setSpeedRange) {
			UMBRA_FUNCTION_DA(0, X1, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(1, Y1, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(2, Z1, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(3, X2, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(4, Y2, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(5, Z2, Umbra::VT_DOUBLE);
			// code information break
			UMBRA_FUNCTION_CS() {
				p.speed.min.x = vaX1.toFloat();
				p.speed.min.y = vaY1.toFloat();
				p.speed.min.z = vaZ1.toFloat();
				p.speed.max.x = vaX2.toFloat();
				p.speed.max.y = vaY2.toFloat();
				p.speed.max.z = vaZ2.toFloat();
				UMBRA_FUNCTION_RET( *this );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(setAccelRange) {
			UMBRA_FUNCTION_DA(0, X1, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(1, Y1, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(2, Z1, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(3, X2, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(4, Y2, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(5, Z2, Umbra::VT_DOUBLE);
			// code information break
			UMBRA_FUNCTION_CS() {
				p.accel.min.x = vaX1.toFloat();
				p.accel.min.y = vaY1.toFloat();
				p.accel.min.z = vaZ1.toFloat();
				p.accel.max.x = vaX2.toFloat();
				p.accel.max.y = vaY2.toFloat();
				p.accel.max.z = vaZ2.toFloat();
				UMBRA_FUNCTION_RET( *this );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(setAngleRange) {
			UMBRA_FUNCTION_DA(0, X1, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(1, Y1, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(2, Z1, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(3, X2, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(4, Y2, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(5, Z2, Umbra::VT_DOUBLE);
			// code information break
			UMBRA_FUNCTION_CS() {
				p.angle.min.x = vaX1.toFloat();
				p.angle.min.y = vaY1.toFloat();
				p.angle.min.z = vaZ1.toFloat();
				p.angle.max.x = vaX2.toFloat();
				p.angle.max.y = vaY2.toFloat();
				p.angle.max.z = vaZ2.toFloat();
				UMBRA_FUNCTION_RET( *this );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(setSpinRange) {
			UMBRA_FUNCTION_DA(0, X1, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(1, Y1, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(2, Z1, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(3, X2, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(4, Y2, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(5, Z2, Umbra::VT_DOUBLE);
			// code information break
			UMBRA_FUNCTION_CS() {
				p.spin.min.x = vaX1.toFloat();
				p.spin.min.y = vaY1.toFloat();
				p.spin.min.z = vaZ1.toFloat();
				p.spin.max.x = vaX2.toFloat();
				p.spin.max.y = vaY2.toFloat();
				p.spin.max.z = vaZ2.toFloat();
				UMBRA_FUNCTION_RET( *this );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(setSizeRange) {
			UMBRA_FUNCTION_DA(0, X1, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(1, Y1, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(2, Z1, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(3, X2, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(4, Y2, Umbra::VT_DOUBLE);
			UMBRA_FUNCTION_DA(5, Z2, Umbra::VT_DOUBLE);
			// code information break
			UMBRA_FUNCTION_CS() {
				p.size.min.x = vaX1.toFloat();
				p.size.min.y = vaY1.toFloat();
				p.size.min.z = vaZ1.toFloat();
				p.size.max.x = vaX2.toFloat();
				p.size.max.y = vaY2.toFloat();
				p.size.max.z = vaZ2.toFloat();
				UMBRA_FUNCTION_RET( *this );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// nothing found
		return R.NOT_HANDLED;
	}
}

#endif