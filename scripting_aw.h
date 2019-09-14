#pragma once
#include "umbra/umbra_remoteclass.h"

namespace ScriptingHost_ClassLib
{
	namespace AW
	{
		class D_Constate : public Umbra::RemoteClass
		{
			bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
			{
				UMBRA_PROPERTY_C(NOTHING, ConState::Nothing);
				UMBRA_PROPERTY_C(CREATING, ConState::Creating);
				UMBRA_PROPERTY_C(CREATED, ConState::Created);
				UMBRA_PROPERTY_C(CONNECTING, ConState::Connecting);
				UMBRA_PROPERTY_C(RECOVERING, ConState::Recovering);
				UMBRA_PROPERTY_C(CONNECTED, ConState::Connected);

				
				// unsupported element
				return Umbra::PropertyRequest::NOT_HANDLED; 
			}
		};

		class D_CameraType : public Umbra::RemoteClass
		{
		public: // umbra interfaces
			bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 
			{ 
				UMBRA_PROPERTY_C(TRACK_DEFAULT,		AW_CAMERA_TRACK_DEFAULT);
				UMBRA_PROPERTY_C(TRACK_AVATAR,		AW_CAMERA_TRACK_AVATAR);
				UMBRA_PROPERTY_C(TRACK_OBJECT,		AW_CAMERA_TRACK_OBJECT);
				UMBRA_PROPERTY_C(TRACK_AHEAD,			AW_CAMERA_TRACK_AHEAD);
				
				// unsupported element
				return Umbra::PropertyRequest::NOT_HANDLED;  
			}
		};

		// @class=Aw.AvatarState
		// 
		//
		class D_AvatarState : public Umbra::RemoteClass
		{
		public: // umbra interfaces
			bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 
			{
				UMBRA_PROPERTY_C(WALKING,			AW_AVATAR_STATE_WALKING);
				UMBRA_PROPERTY_C(RUNNING,			AW_AVATAR_STATE_RUNNING);
				UMBRA_PROPERTY_C(FLYING,			AW_AVATAR_STATE_FLYING);
				UMBRA_PROPERTY_C(SWIMMING,			AW_AVATAR_STATE_SWIMMING);
				UMBRA_PROPERTY_C(FALLING,			AW_AVATAR_STATE_FALLING);
				UMBRA_PROPERTY_C(JUMPING,			AW_AVATAR_STATE_JUMPING);
				UMBRA_PROPERTY_C(WARPING,			AW_AVATAR_STATE_WARPING);
				UMBRA_PROPERTY_C(RIDING,			AW_AVATAR_STATE_RIDING);
				UMBRA_PROPERTY_C(SLIDING1,			AW_AVATAR_STATE_SLIDING1);
				UMBRA_PROPERTY_C(SLIDING2,			AW_AVATAR_STATE_SLIDING2);
				UMBRA_PROPERTY_C(SLIDING3,			AW_AVATAR_STATE_SLIDING3);
				UMBRA_PROPERTY_C(CLIMBING,			AW_AVATAR_STATE_CLIMBING);
				
				// unsupported element
				return Umbra::PropertyRequest::NOT_HANDLED; 
			}
		};

		class D_WorldStatus : public Umbra::RemoteClass
		{
		public: // umbra interfaces
			bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 
			{
				UMBRA_PROPERTY_C(UNKNOWN,				AW_WORLDSTATUS_UNKNOWN);
				UMBRA_PROPERTY_C(PUBLIC,				AW_WORLDSTATUS_PUBLIC);
				UMBRA_PROPERTY_C(PRIVATE,				AW_WORLDSTATUS_PRIVATE);
				UMBRA_PROPERTY_C(STOPPED,				AW_WORLDSTATUS_STOPPED);
				
				// unsupported element
				return Umbra::PropertyRequest::NOT_HANDLED; 
			}
		};

		class D_ChatType : public Umbra::RemoteClass
		{
		public: // umbra interfaces
			bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 
			{ 
				UMBRA_PROPERTY_C(SAID,					AW_CHAT_SAID);
				UMBRA_PROPERTY_C(BROADCAST,				AW_CHAT_BROADCAST);
				UMBRA_PROPERTY_C(WHISPER,				AW_CHAT_WHISPER);
				
				// unsupported element
				return Umbra::PropertyRequest::NOT_HANDLED;  
			}
		};

		// Aw.EjectionEnum.ADDRESS
		class D_EjectionType : public Umbra::RemoteClass
		{		
		public: // umbra interfaces
			bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 
			{ 
				UMBRA_PROPERTY_C(ADDRESS,				AW_EJECT_BY_ADDRESS);
				UMBRA_PROPERTY_C(COMPUTER,				AW_EJECT_BY_COMPUTER);
				UMBRA_PROPERTY_C(CITIZEN,				AW_EJECT_BY_CITIZEN);
				
				// unsupported element
				return Umbra::PropertyRequest::NOT_HANDLED; 
			}
		};

		// Aw.ConsoleStyle
		class D_ConsoleStyle : public Umbra::RemoteClass
		{		
		public: // umbra interfaces
			bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 
			{ 
				UMBRA_PROPERTY_C(BOLD,					CM_BOLD);
				UMBRA_PROPERTY_C(ITALIC,				CM_ITALIC);
				UMBRA_PROPERTY_C(SUPPRESSED,			CM_SUPPRESSED);
				UMBRA_PROPERTY_C(BOLD_ITALIC,			CM_BOLD | CM_ITALIC);
				
				// unsupported element
				return Umbra::PropertyRequest::NOT_HANDLED; 
			}
		};

		class D_HudOriginType : public Umbra::RemoteClass
		{		
		public: // umbra interfaces
			bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 
			{ 
				UMBRA_PROPERTY_C(TOP_LFFT,				AW_HUD_ORIGIN_TOP_LEFT);
				UMBRA_PROPERTY_C(TOP,					AW_HUD_ORIGIN_TOP);
				UMBRA_PROPERTY_C(TOP_RIGHT,				AW_HUD_ORIGIN_TOP_RIGHT);
				UMBRA_PROPERTY_C(LEFT,					AW_HUD_ORIGIN_LEFT);
				UMBRA_PROPERTY_C(CENTER,				AW_HUD_ORIGIN_CENTER);
				UMBRA_PROPERTY_C(RIGHT,					AW_HUD_ORIGIN_RIGHT);
				UMBRA_PROPERTY_C(BOTTOM_LEFT,			AW_HUD_ORIGIN_BOTTOM_LEFT);
				UMBRA_PROPERTY_C(BOTTOM,				AW_HUD_ORIGIN_BOTTOM);
				UMBRA_PROPERTY_C(BOTTOM_RIGHT,			AW_HUD_ORIGIN_BOTTOM_RIGHT);
				
				// unsupported element
				return Umbra::PropertyRequest::NOT_HANDLED; 
			}
		};

		class D_HudTypes : public Umbra::RemoteClass
		{		
		public: // umbra interfaces
			bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 
			{ 
				UMBRA_PROPERTY_C(TEXT, AW_HUD_TYPE_TEXT);
				UMBRA_PROPERTY_C(IMAGE, AW_HUD_TYPE_IMAGE);
				UMBRA_PROPERTY_C(MODEL, AW_HUD_TYPE_MODEL);
				
				// unsupported element
				return Umbra::PropertyRequest::NOT_HANDLED; 
			}
		};

		class D_EnableCavEnum : public Umbra::RemoteClass
		{		
		public: // umbra interfaces
			bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 
			{ 
				UMBRA_PROPERTY_C(CITIZEN, AW_ENABLE_CAV_CITIZEN);
				UMBRA_PROPERTY_C(BOT, AW_ENABLE_CAV_BOT);
				UMBRA_PROPERTY_C(TOURIST, AW_ENABLE_CAV_TOURIST);
				UMBRA_PROPERTY_C(WORLD_CITIZEN, AW_ENABLE_CAV_WORLD_CITIZEN);
				UMBRA_PROPERTY_C(WORLD_BOT, AW_ENABLE_CAV_WORLD_BOT);
				UMBRA_PROPERTY_C(WORLD_TOURIST, AW_ENABLE_CAV_WORLD_TOURIST);
				
				// unsupported element
				return Umbra::PropertyRequest::NOT_HANDLED; 
			}
		};

		class D_ParticleFlags : public Umbra::RemoteClass
		{		
		public: // umbra interfaces
			bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 
			{ 
				UMBRA_PROPERTY_C(INTERPOLATE, AW_PARTICLE_FLAG_INTERPOLATE);
				UMBRA_PROPERTY_C(GRAVITY, AW_PARTICLE_FLAG_GRAVITY);
				UMBRA_PROPERTY_C(COLLIDE_ZONES, AW_PARTICLE_FLAG_COLLIDE_ZONES);
				UMBRA_PROPERTY_C(ZONE_EXCLUSIVE, AW_PARTICLE_FLAG_ZONE_EXCLUSIVE);
				UMBRA_PROPERTY_C(CAMERA_EMIT, AW_PARTICLE_FLAG_CAMERA_EMIT);
				UMBRA_PROPERTY_C(LINK, AW_PARTICLE_FLAG_MOVER_LINK);
				UMBRA_PROPERTY_C(DRAW_IN_FRONT, AW_PARTICLE_FLAG_DRAW_IN_FRONT);
				
				// unsupported element
				return Umbra::PropertyRequest::NOT_HANDLED; 
			}
		};

		class D_MoverFlags : public Umbra::RemoteClass
		{		
		public: // umbra interfaces
			bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 
			{ 
				UMBRA_PROPERTY_C(LOOP, AW_MOVER_FLAG_LOOP);
				UMBRA_PROPERTY_C(USE_OBJTAG, AW_MOVER_FLAG_USE_OBJTAG);
				UMBRA_PROPERTY_C(BUMP_ADD, AW_MOVER_FLAG_BUMP_ADD);
				UMBRA_PROPERTY_C(CLICK_START, AW_MOVER_FLAG_CLICK_START);
				UMBRA_PROPERTY_C(CLICK_PAUSE, AW_MOVER_FLAG_CLICK_PAUSE);
				UMBRA_PROPERTY_C(LOCK_POSITION, AW_MOVER_FLAG_LOCK_POSITION);
				UMBRA_PROPERTY_C(LOCK_YAW, AW_MOVER_FLAG_LOCK_YAW);
				UMBRA_PROPERTY_C(LOCK_PITCH, AW_MOVER_FLAG_LOCK_PITCH);
				UMBRA_PROPERTY_C(SINGLE_RIDER, AW_MOVER_FLAG_SINGLE_RIDER);
				UMBRA_PROPERTY_C(NO_AUTO_YAW, AW_MOVER_FLAG_NO_AUTO_YAW);
				UMBRA_PROPERTY_C(EMPTY_RESET, AW_MOVER_FLAG_EMPTY_RESET);
				UMBRA_PROPERTY_C(DISABLE_FLYING, AW_MOVER_FLAG_DISABLE_FLYING);
				UMBRA_PROPERTY_C(KEEP_ON_WATER, AW_MOVER_FLAG_KEEP_ON_WATER);
				UMBRA_PROPERTY_C(DISABLE_UPWARDS, AW_MOVER_FLAG_DISABLE_UPWARDS);
				UMBRA_PROPERTY_C(INVISIBLE_AV, AW_MOVER_FLAG_INVISIBLE_AV);
				UMBRA_PROPERTY_C(EXIT_EJECT_UP, AW_MOVER_FLAG_EXIT_EJECT_UP);
				UMBRA_PROPERTY_C(EXIT_NONE_SOLID, AW_MOVER_FLAG_EXIT_NON_SOLID);
				UMBRA_PROPERTY_C(AV_APPLY_TILT_X, AW_MOVER_FLAG_AV_APPLY_TILT_X);
				UMBRA_PROPERTY_C(AV_APPLY_TILT_Z, AW_MOVER_FLAG_AV_APPLY_TILT_Z);
				UMBRA_PROPERTY_C(OBJECT_LINK, AW_MOVER_FLAG_OBJECT_LINK);
				UMBRA_PROPERTY_C(DISABLE_MOVE, AW_MOVER_FLAG_DISABLE_MOVE);
				UMBRA_PROPERTY_C(TILT_BY_TERRAIN, AW_MOVER_FLAG_TILT_BY_TERRAIN);
				UMBRA_PROPERTY_C(DISABLE_EXPL_SEQS, AW_MOVER_FLAG_DISABLE_EXPL_SEQS);
				UMBRA_PROPERTY_C(DISABLE_TURN_ON_PLACE, AW_MOVER_FLAG_DISABLE_TURN_ON_PLACE);
				UMBRA_PROPERTY_C(DISABLE_TELEPORTS, AW_MOVER_FLAG_DISABLE_TELEPORTS);
				UMBRA_PROPERTY_C(DETATCH_ON_TELEPORT, AW_MOVER_FLAG_DETACH_ON_TELEPORT);
				
				// unsupported element
				return Umbra::PropertyRequest::NOT_HANDLED; 
			}
		};

		class D_ZoneFlags : public Umbra::RemoteClass
		{		
		public: // umbra interfaces
			bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 
			{ 
				UMBRA_PROPERTY_C(WATER, AW_ZONE_FLAG_WATER);
				UMBRA_PROPERTY_C(BLOCK_PARTICLES, AW_ZONE_FLAG_BLOCK_PARTICLES);
				UMBRA_PROPERTY_C(BLOCK_LIGHTS, AW_ZONE_FLAG_BLOCK_LIGHTS);
				UMBRA_PROPERTY_C(BLOCK_WORLD_LIGHT, AW_ZONE_FLAG_BLOCK_WORLD_LIGHT);
				UMBRA_PROPERTY_C(BLOCK_CHAT, AW_ZONE_FLAG_BLOCK_CHAT);
				UMBRA_PROPERTY_C(VISIBLE, AW_ZONE_FLAG_VISIBLE);
				UMBRA_PROPERTY_C(BLOCK_JOIN, AW_ZONE_FLAG_BLOCK_JOIN);
				UMBRA_PROPERTY_C(VOIP, AW_ZONE_FLAG_VOIP);
				
				// unsupported element
				return Umbra::PropertyRequest::NOT_HANDLED; 
			}
		};


		// awsdk reason codes
		// 
		class D_ReasonCodes : public Umbra::RemoteClass
		{
		public: // umbra interfaces
			bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 			
			{ 				
				// unsupported element
				return Umbra::PropertyRequest::NOT_HANDLED;  
			}
		};

		// namespaces
		class Namespace : public Umbra::RemoteClass
		{
		private: // the variables
			D_AvatarState			avatar_state;
			D_WorldStatus			world_status;
			D_ChatType				chat_type;
			D_EjectionType		ejection_type;
			D_HudOriginType		hud_origin_type;
			D_CameraType			camera_type;
			D_ConsoleStyle		console_style;
			D_EnableCavEnum		_enableCavEnum;
			D_HudTypes				_hudTypes;

		private: // object flaggs
			D_ParticleFlags		_particleFlags;
			D_MoverFlags			_moverFlags;
			D_ZoneFlags				_zoneFlags;

		protected: // handler
			bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 
			{ 
				UMBRA_PROPERTY_R(AvatarState,			avatar_state);
				UMBRA_PROPERTY_R(WorldStatus,			world_status);
				UMBRA_PROPERTY_R(ChatType,				chat_type);
				UMBRA_PROPERTY_R(EjectionType,		ejection_type);
				UMBRA_PROPERTY_R(HudOrigin,				hud_origin_type);
				UMBRA_PROPERTY_R(HudTypes,				_hudTypes);
				UMBRA_PROPERTY_R(CameraType,			camera_type);
				UMBRA_PROPERTY_R(ConsoleStyle,		console_style);
				UMBRA_PROPERTY_C(EnableCavFlags,	_particleFlags);

				// object flags
				UMBRA_PROPERTY_C(ParticleFlags,		_particleFlags);
				UMBRA_PROPERTY_C(MoverFlags,			_moverFlags);
				UMBRA_PROPERTY_C(ZoneFlags,				_zoneFlags);

				// attributes
				UMBRA_PROPERTY_R(UNIVERSE_BROWSER_MINIMUM, AW_UNIVERSE_BROWSER_MINIMUM)
				UMBRA_PROPERTY_R(UNIVERSE_BROWSER_RELEASE, AW_UNIVERSE_BROWSER_RELEASE)
				UMBRA_PROPERTY_R(UNIVERSE_BROWSER_BETA, AW_UNIVERSE_BROWSER_BETA)
				UMBRA_PROPERTY_R(UNIVERSE_WORLD_MINIMUM, AW_UNIVERSE_WORLD_MINIMUM)
				UMBRA_PROPERTY_R(UNIVERSE_WORLD_START, AW_UNIVERSE_WORLD_START)
				UMBRA_PROPERTY_R(UNIVERSE_REGISTRATION_REQUIRED, AW_UNIVERSE_REGISTRATION_REQUIRED)
				UMBRA_PROPERTY_R(UNIVERSE_BUILD_NUMBER, AW_UNIVERSE_BUILD_NUMBER)
				UMBRA_PROPERTY_R(UNIVERSE_MONTHLY_CHARGE, AW_UNIVERSE_MONTHLY_CHARGE)
				UMBRA_PROPERTY_R(UNIVERSE_ANNUAL_CHARGE, AW_UNIVERSE_ANNUAL_CHARGE)
				UMBRA_PROPERTY_R(UNIVERSE_REGISTER_METHOD, AW_UNIVERSE_REGISTER_METHOD)
				UMBRA_PROPERTY_R(UNIVERSE_TIME, AW_UNIVERSE_TIME)
				UMBRA_PROPERTY_R(UNIVERSE_CITIZEN_CHANGES_ALLOWED, AW_UNIVERSE_CITIZEN_CHANGES_ALLOWED)
				UMBRA_PROPERTY_R(UNIVERSE_BROWSER_RELEASE_22, AW_UNIVERSE_BROWSER_RELEASE_22)
				UMBRA_PROPERTY_R(UNIVERSE_WELCOME_MESSAGE, AW_UNIVERSE_WELCOME_MESSAGE)
				UMBRA_PROPERTY_R(UNIVERSE_WORLD_RELEASE, AW_UNIVERSE_WORLD_RELEASE)
				UMBRA_PROPERTY_R(UNIVERSE_WORLD_BETA, AW_UNIVERSE_WORLD_BETA)
				UMBRA_PROPERTY_R(UNIVERSE_ALLOW_TOURISTS, AW_UNIVERSE_ALLOW_TOURISTS)
				UMBRA_PROPERTY_R(UNIVERSE_SEARCH_URL, AW_UNIVERSE_SEARCH_URL)
				UMBRA_PROPERTY_R(UNIVERSE_NOTEPAD_URL, AW_UNIVERSE_NOTEPAD_URL)
				UMBRA_PROPERTY_R(UNIVERSE_NAME, AW_UNIVERSE_NAME)
				UMBRA_PROPERTY_R(UNIVERSE_USER_LIST_ENABLED, AW_UNIVERSE_USER_LIST_ENABLED)
				UMBRA_PROPERTY_R(CITIZEN_NUMBER, AW_CITIZEN_NUMBER)
				UMBRA_PROPERTY_R(CITIZEN_NAME, AW_CITIZEN_NAME)
				UMBRA_PROPERTY_R(CITIZEN_PASSWORD, AW_CITIZEN_PASSWORD)
				UMBRA_PROPERTY_R(CITIZEN_EMAIL, AW_CITIZEN_EMAIL)
				UMBRA_PROPERTY_R(CITIZEN_TIME_LEFT, AW_CITIZEN_TIME_LEFT)
				UMBRA_PROPERTY_R(CITIZEN_PRIVILEGE_PASSWORD, AW_CITIZEN_PRIVILEGE_PASSWORD)
				UMBRA_PROPERTY_R(CITIZEN_IMMIGRATION_TIME, AW_CITIZEN_IMMIGRATION_TIME)
				UMBRA_PROPERTY_R(CITIZEN_EXPIRATION_TIME, AW_CITIZEN_EXPIRATION_TIME)
				UMBRA_PROPERTY_R(CITIZEN_BETA, AW_CITIZEN_BETA)
				UMBRA_PROPERTY_R(CITIZEN_LAST_LOGIN, AW_CITIZEN_LAST_LOGIN)
				UMBRA_PROPERTY_R(CITIZEN_BOT_LIMIT, AW_CITIZEN_BOT_LIMIT)
				UMBRA_PROPERTY_R(CITIZEN_TOTAL_TIME, AW_CITIZEN_TOTAL_TIME)
				UMBRA_PROPERTY_R(CITIZEN_ENABLED, AW_CITIZEN_ENABLED)
				UMBRA_PROPERTY_R(CITIZEN_COMMENT, AW_CITIZEN_COMMENT)
				UMBRA_PROPERTY_R(CITIZEN_URL, AW_CITIZEN_URL)
				UMBRA_PROPERTY_R(WORLD_NAME, AW_WORLD_NAME)
				UMBRA_PROPERTY_R(WORLD_TITLE, AW_WORLD_TITLE)
				UMBRA_PROPERTY_R(WORLD_BACKDROP, AW_WORLD_BACKDROP)
				UMBRA_PROPERTY_R(WORLD_GROUND, AW_WORLD_GROUND)
				UMBRA_PROPERTY_R(WORLD_OBJECT_PATH, AW_WORLD_OBJECT_PATH)
				UMBRA_PROPERTY_R(WORLD_OBJECT_REFRESH, AW_WORLD_OBJECT_REFRESH)
				UMBRA_PROPERTY_R(WORLD_BUILD_RIGHT, AW_WORLD_BUILD_RIGHT)
				UMBRA_PROPERTY_R(WORLD_EMINENT_DOMAIN_RIGHT, AW_WORLD_EMINENT_DOMAIN_RIGHT)
				UMBRA_PROPERTY_R(WORLD_ENTER_RIGHT, AW_WORLD_ENTER_RIGHT)
				UMBRA_PROPERTY_R(WORLD_SPECIAL_OBJECTS_RIGHT, AW_WORLD_SPECIAL_OBJECTS_RIGHT)
				UMBRA_PROPERTY_R(WORLD_FOG_RED, AW_WORLD_FOG_RED)
				UMBRA_PROPERTY_R(WORLD_FOG_GREEN, AW_WORLD_FOG_GREEN)
				UMBRA_PROPERTY_R(WORLD_FOG_BLUE, AW_WORLD_FOG_BLUE)
				UMBRA_PROPERTY_R(WORLD_CARETAKER_CAPABILITY, AW_WORLD_CARETAKER_CAPABILITY)
				UMBRA_PROPERTY_R(WORLD_RESTRICTED_RADIUS, AW_WORLD_RESTRICTED_RADIUS)
				UMBRA_PROPERTY_R(WORLD_PUBLIC_SPEAKER_CAPABILITY, AW_WORLD_PUBLIC_SPEAKER_CAPABILITY)
				UMBRA_PROPERTY_R(WORLD_PUBLIC_SPEAKER_RIGHT, AW_WORLD_PUBLIC_SPEAKER_RIGHT)
				UMBRA_PROPERTY_R(WORLD_CREATION_TIMESTAMP, AW_WORLD_CREATION_TIMESTAMP)
				UMBRA_PROPERTY_R(WORLD_HOME_PAGE, AW_WORLD_HOME_PAGE)
				UMBRA_PROPERTY_R(WORLD_BUILD_NUMBER, AW_WORLD_BUILD_NUMBER)
				UMBRA_PROPERTY_R(WORLD_OBJECT_PASSWORD, AW_WORLD_OBJECT_PASSWORD)
				UMBRA_PROPERTY_R(WORLD_DISABLE_CREATE_URL, AW_WORLD_DISABLE_CREATE_URL)
				UMBRA_PROPERTY_R(WORLD_RATING, AW_WORLD_RATING)
				UMBRA_PROPERTY_R(WORLD_WELCOME_MESSAGE, AW_WORLD_WELCOME_MESSAGE)
				UMBRA_PROPERTY_R(WORLD_EJECT_RIGHT, AW_WORLD_EJECT_RIGHT)
				UMBRA_PROPERTY_R(WORLD_EJECT_CAPABILITY, AW_WORLD_EJECT_CAPABILITY)
				UMBRA_PROPERTY_R(WORLD_CELL_LIMIT, AW_WORLD_CELL_LIMIT)
				UMBRA_PROPERTY_R(WORLD_BUILD_CAPABILITY, AW_WORLD_BUILD_CAPABILITY)
				UMBRA_PROPERTY_R(WORLD_ALLOW_PASSTHRU, AW_WORLD_ALLOW_PASSTHRU)
				UMBRA_PROPERTY_R(WORLD_ALLOW_FLYING, AW_WORLD_ALLOW_FLYING)
				UMBRA_PROPERTY_R(WORLD_ALLOW_TELEPORT, AW_WORLD_ALLOW_TELEPORT)
				UMBRA_PROPERTY_R(WORLD_ALLOW_OBJECT_SELECT, AW_WORLD_ALLOW_OBJECT_SELECT)
				UMBRA_PROPERTY_R(WORLD_BOTS_RIGHT, AW_WORLD_BOTS_RIGHT)
				UMBRA_PROPERTY_R(WORLD_SPEAK_CAPABILITY, AW_WORLD_SPEAK_CAPABILITY)
				UMBRA_PROPERTY_R(WORLD_SPEAK_RIGHT, AW_WORLD_SPEAK_RIGHT)
				UMBRA_PROPERTY_R(WORLD_ALLOW_TOURIST_WHISPER, AW_WORLD_ALLOW_TOURIST_WHISPER)
				UMBRA_PROPERTY_R(WORLD_LIGHT_X, AW_WORLD_LIGHT_X)
				UMBRA_PROPERTY_R(WORLD_LIGHT_Y, AW_WORLD_LIGHT_Y)
				UMBRA_PROPERTY_R(WORLD_LIGHT_Z, AW_WORLD_LIGHT_Z)
				UMBRA_PROPERTY_R(WORLD_LIGHT_RED, AW_WORLD_LIGHT_RED)
				UMBRA_PROPERTY_R(WORLD_LIGHT_GREEN, AW_WORLD_LIGHT_GREEN)
				UMBRA_PROPERTY_R(WORLD_LIGHT_BLUE, AW_WORLD_LIGHT_BLUE)
				UMBRA_PROPERTY_R(WORLD_AMBIENT_LIGHT_RED, AW_WORLD_AMBIENT_LIGHT_RED)
				UMBRA_PROPERTY_R(WORLD_AMBIENT_LIGHT_GREEN, AW_WORLD_AMBIENT_LIGHT_GREEN)
				UMBRA_PROPERTY_R(WORLD_AMBIENT_LIGHT_BLUE, AW_WORLD_AMBIENT_LIGHT_BLUE)
				UMBRA_PROPERTY_R(WORLD_ALLOW_AVATAR_COLLISION, AW_WORLD_ALLOW_AVATAR_COLLISION)
				UMBRA_PROPERTY_R(WORLD_FOG_ENABLE, AW_WORLD_FOG_ENABLE)
				UMBRA_PROPERTY_R(WORLD_FOG_MINIMUM, AW_WORLD_FOG_MINIMUM)
				UMBRA_PROPERTY_R(WORLD_FOG_MAXIMUM, AW_WORLD_FOG_MAXIMUM)
				UMBRA_PROPERTY_R(WORLD_FOG_TINTED, AW_WORLD_FOG_TINTED)
				UMBRA_PROPERTY_R(WORLD_MAX_USERS, AW_WORLD_MAX_USERS)
				UMBRA_PROPERTY_R(WORLD_SIZE, AW_WORLD_SIZE)
				UMBRA_PROPERTY_R(WORLD_OBJECT_COUNT, AW_WORLD_OBJECT_COUNT)
				UMBRA_PROPERTY_R(WORLD_EXPIRATION, AW_WORLD_EXPIRATION)
				UMBRA_PROPERTY_R(WORLD_SPECIAL_COMMANDS_RIGHT, AW_WORLD_SPECIAL_COMMANDS_RIGHT)
				UMBRA_PROPERTY_R(WORLD_MAX_LIGHT_RADIUS, AW_WORLD_MAX_LIGHT_RADIUS)
				UMBRA_PROPERTY_R(WORLD_SKYBOX, AW_WORLD_SKYBOX)
				UMBRA_PROPERTY_R(WORLD_MINIMUM_VISIBILITY, AW_WORLD_MINIMUM_VISIBILITY)
				UMBRA_PROPERTY_R(WORLD_REPEATING_GROUND, AW_WORLD_REPEATING_GROUND)
				UMBRA_PROPERTY_R(WORLD_KEYWORDS, AW_WORLD_KEYWORDS)
				UMBRA_PROPERTY_R(WORLD_ENABLE_TERRAIN, AW_WORLD_ENABLE_TERRAIN)
				UMBRA_PROPERTY_R(WORLD_ALLOW_3_AXIS_ROTATION, AW_WORLD_ALLOW_3_AXIS_ROTATION)
				UMBRA_PROPERTY_R(WORLD_TERRAIN_TIMESTAMP, AW_WORLD_TERRAIN_TIMESTAMP)
				UMBRA_PROPERTY_R(WORLD_ENTRY_POINT, AW_WORLD_ENTRY_POINT)
				UMBRA_PROPERTY_R(WORLD_SKY_NORTH_RED, AW_WORLD_SKY_NORTH_RED)
				UMBRA_PROPERTY_R(WORLD_SKY_NORTH_GREEN, AW_WORLD_SKY_NORTH_GREEN)
				UMBRA_PROPERTY_R(WORLD_SKY_NORTH_BLUE, AW_WORLD_SKY_NORTH_BLUE)
				UMBRA_PROPERTY_R(WORLD_SKY_SOUTH_RED, AW_WORLD_SKY_SOUTH_RED)
				UMBRA_PROPERTY_R(WORLD_SKY_SOUTH_GREEN, AW_WORLD_SKY_SOUTH_GREEN)
				UMBRA_PROPERTY_R(WORLD_SKY_SOUTH_BLUE, AW_WORLD_SKY_SOUTH_BLUE)
				UMBRA_PROPERTY_R(WORLD_SKY_EAST_RED, AW_WORLD_SKY_EAST_RED)
				UMBRA_PROPERTY_R(WORLD_SKY_EAST_GREEN, AW_WORLD_SKY_EAST_GREEN)
				UMBRA_PROPERTY_R(WORLD_SKY_EAST_BLUE, AW_WORLD_SKY_EAST_BLUE)
				UMBRA_PROPERTY_R(WORLD_SKY_WEST_RED, AW_WORLD_SKY_WEST_RED)
				UMBRA_PROPERTY_R(WORLD_SKY_WEST_GREEN, AW_WORLD_SKY_WEST_GREEN)
				UMBRA_PROPERTY_R(WORLD_SKY_WEST_BLUE, AW_WORLD_SKY_WEST_BLUE)
				UMBRA_PROPERTY_R(WORLD_SKY_TOP_RED, AW_WORLD_SKY_TOP_RED)
				UMBRA_PROPERTY_R(WORLD_SKY_TOP_GREEN, AW_WORLD_SKY_TOP_GREEN)
				UMBRA_PROPERTY_R(WORLD_SKY_TOP_BLUE, AW_WORLD_SKY_TOP_BLUE)
				UMBRA_PROPERTY_R(WORLD_SKY_BOTTOM_RED, AW_WORLD_SKY_BOTTOM_RED)
				UMBRA_PROPERTY_R(WORLD_SKY_BOTTOM_GREEN, AW_WORLD_SKY_BOTTOM_GREEN)
				UMBRA_PROPERTY_R(WORLD_SKY_BOTTOM_BLUE, AW_WORLD_SKY_BOTTOM_BLUE)
				UMBRA_PROPERTY_R(WORLD_CLOUDS_LAYER1_TEXTURE, AW_WORLD_CLOUDS_LAYER1_TEXTURE)
				UMBRA_PROPERTY_R(WORLD_CLOUDS_LAYER1_MASK, AW_WORLD_CLOUDS_LAYER1_MASK)
				UMBRA_PROPERTY_R(WORLD_CLOUDS_LAYER1_TILE, AW_WORLD_CLOUDS_LAYER1_TILE)
				UMBRA_PROPERTY_R(WORLD_CLOUDS_LAYER1_SPEED_X, AW_WORLD_CLOUDS_LAYER1_SPEED_X)
				UMBRA_PROPERTY_R(WORLD_CLOUDS_LAYER1_SPEED_Z, AW_WORLD_CLOUDS_LAYER1_SPEED_Z)
				UMBRA_PROPERTY_R(WORLD_CLOUDS_LAYER1_OPACITY, AW_WORLD_CLOUDS_LAYER1_OPACITY)
				UMBRA_PROPERTY_R(WORLD_CLOUDS_LAYER2_TEXTURE, AW_WORLD_CLOUDS_LAYER2_TEXTURE)
				UMBRA_PROPERTY_R(WORLD_CLOUDS_LAYER2_MASK, AW_WORLD_CLOUDS_LAYER2_MASK)
				UMBRA_PROPERTY_R(WORLD_CLOUDS_LAYER2_TILE, AW_WORLD_CLOUDS_LAYER2_TILE)
				UMBRA_PROPERTY_R(WORLD_CLOUDS_LAYER2_SPEED_X, AW_WORLD_CLOUDS_LAYER2_SPEED_X)
				UMBRA_PROPERTY_R(WORLD_CLOUDS_LAYER2_SPEED_Z, AW_WORLD_CLOUDS_LAYER2_SPEED_Z)
				UMBRA_PROPERTY_R(WORLD_CLOUDS_LAYER2_OPACITY, AW_WORLD_CLOUDS_LAYER2_OPACITY)
				UMBRA_PROPERTY_R(WORLD_CLOUDS_LAYER3_TEXTURE, AW_WORLD_CLOUDS_LAYER3_TEXTURE)
				UMBRA_PROPERTY_R(WORLD_CLOUDS_LAYER3_MASK, AW_WORLD_CLOUDS_LAYER3_MASK)
				UMBRA_PROPERTY_R(WORLD_CLOUDS_LAYER3_TILE, AW_WORLD_CLOUDS_LAYER3_TILE)
				UMBRA_PROPERTY_R(WORLD_CLOUDS_LAYER3_SPEED_X, AW_WORLD_CLOUDS_LAYER3_SPEED_X)
				UMBRA_PROPERTY_R(WORLD_CLOUDS_LAYER3_SPEED_Z, AW_WORLD_CLOUDS_LAYER3_SPEED_Z)
				UMBRA_PROPERTY_R(WORLD_CLOUDS_LAYER3_OPACITY, AW_WORLD_CLOUDS_LAYER3_OPACITY)
				UMBRA_PROPERTY_R(WORLD_DISABLE_CHAT, AW_WORLD_DISABLE_CHAT)
				UMBRA_PROPERTY_R(WORLD_ALLOW_CITIZEN_WHISPER, AW_WORLD_ALLOW_CITIZEN_WHISPER)
				UMBRA_PROPERTY_R(WORLD_ALWAYS_SHOW_NAMES, AW_WORLD_ALWAYS_SHOW_NAMES)
				UMBRA_PROPERTY_R(WORLD_DISABLE_AVATAR_LIST, AW_WORLD_DISABLE_AVATAR_LIST)
				UMBRA_PROPERTY_R(WORLD_AVATAR_REFRESH_RATE, AW_WORLD_AVATAR_REFRESH_RATE)
				UMBRA_PROPERTY_R(WORLD_WATER_TEXTURE, AW_WORLD_WATER_TEXTURE)
				UMBRA_PROPERTY_R(WORLD_WATER_MASK, AW_WORLD_WATER_MASK)
				UMBRA_PROPERTY_R(WORLD_WATER_BOTTOM_TEXTURE, AW_WORLD_WATER_BOTTOM_TEXTURE)
				UMBRA_PROPERTY_R(WORLD_WATER_BOTTOM_MASK, AW_WORLD_WATER_BOTTOM_MASK)
				UMBRA_PROPERTY_R(WORLD_WATER_OPACITY, AW_WORLD_WATER_OPACITY)
				UMBRA_PROPERTY_R(WORLD_WATER_RED, AW_WORLD_WATER_RED)
				UMBRA_PROPERTY_R(WORLD_WATER_GREEN, AW_WORLD_WATER_GREEN)
				UMBRA_PROPERTY_R(WORLD_WATER_BLUE, AW_WORLD_WATER_BLUE)
				UMBRA_PROPERTY_R(WORLD_WATER_LEVEL, AW_WORLD_WATER_LEVEL)
				UMBRA_PROPERTY_R(WORLD_WATER_SURFACE_MOVE, AW_WORLD_WATER_SURFACE_MOVE)
				UMBRA_PROPERTY_R(WORLD_WATER_WAVE_MOVE, AW_WORLD_WATER_WAVE_MOVE)
				UMBRA_PROPERTY_R(WORLD_WATER_SPEED, AW_WORLD_WATER_SPEED)
				UMBRA_PROPERTY_R(WORLD_WATER_ENABLED, AW_WORLD_WATER_ENABLED)
				UMBRA_PROPERTY_R(WORLD_EMINENT_DOMAIN_CAPABILITY, AW_WORLD_EMINENT_DOMAIN_CAPABILITY)
				UMBRA_PROPERTY_R(WORLD_LIGHT_TEXTURE, AW_WORLD_LIGHT_TEXTURE)
				UMBRA_PROPERTY_R(WORLD_LIGHT_MASK, AW_WORLD_LIGHT_MASK)
				UMBRA_PROPERTY_R(WORLD_LIGHT_DRSIZE, AW_WORLD_LIGHT_DRAW_SIZE)
				UMBRA_PROPERTY_R(WORLD_LIGHT_DRFRONT, AW_WORLD_LIGHT_DRAW_FRONT)
				UMBRA_PROPERTY_R(WORLD_LIGHT_DRBRIGHT, AW_WORLD_LIGHT_DRAW_BRIGHT)
				UMBRA_PROPERTY_R(WORLD_LIGHT_SOURCE_USE_COLOR, AW_WORLD_LIGHT_SOURCE_USE_COLOR)
				UMBRA_PROPERTY_R(WORLD_LIGHT_SOURCE_COLOR, AW_WORLD_LIGHT_SOURCE_COLOR)
				UMBRA_PROPERTY_R(WORLD_TERRAIN_AMBIENT, AW_WORLD_TERRAIN_AMBIENT)
				UMBRA_PROPERTY_R(WORLD_TERRAIN_DIFFUSE, AW_WORLD_TERRAIN_DIFFUSE)
				UMBRA_PROPERTY_R(WORLD_WATER_VISIBILITY, AW_WORLD_WATER_VISIBILITY)
				UMBRA_PROPERTY_R(WORLD_SOUND_FOOTSTEP, AW_WORLD_SOUND_FOOTSTEP)
				UMBRA_PROPERTY_R(WORLD_SOUND_WATER_ENTER, AW_WORLD_SOUND_WATER_ENTER)
				UMBRA_PROPERTY_R(WORLD_SOUND_WATER_EXIT, AW_WORLD_SOUND_WATER_EXIT)
				UMBRA_PROPERTY_R(WORLD_SOUND_AMBIENT, AW_WORLD_SOUND_AMBIENT)
				UMBRA_PROPERTY_R(WORLD_GRAVITY, AW_WORLD_GRAVITY)
				UMBRA_PROPERTY_R(WORLD_BUOYANCY, AW_WORLD_BUOYANCY)
				UMBRA_PROPERTY_R(WORLD_FRICTION, AW_WORLD_FRICTION)
				UMBRA_PROPERTY_R(WORLD_WATER_FRICTION, AW_WORLD_WATER_FRICTION)
				UMBRA_PROPERTY_R(WORLD_SLOPESLIDE_ENABLED, AW_WORLD_SLOPESLIDE_ENABLED)
				UMBRA_PROPERTY_R(WORLD_SLOPESLIDE_MIN_ANGLE, AW_WORLD_SLOPESLIDE_MIN_ANGLE)
				UMBRA_PROPERTY_R(WORLD_SLOPESLIDE_MAX_ANGLE, AW_WORLD_SLOPESLIDE_MAX_ANGLE)
				UMBRA_PROPERTY_R(WORLD_ALLOW_TOURIST_BUILD, AW_WORLD_ALLOW_TOURIST_BUILD)
				UMBRA_PROPERTY_R(WORLD_ENABLE_REFERER, AW_WORLD_ENABLE_REFERER)
				UMBRA_PROPERTY_R(WORLD_WATER_UNDER_TERRAIN, AW_WORLD_WATER_UNDER_TERRAIN)
				UMBRA_PROPERTY_R(WORLD_TERRAIN_OFFSET, AW_WORLD_TERRAIN_OFFSET)
				UMBRA_PROPERTY_R(WORLD_VOIP_RIGHT, AW_WORLD_VOIP_RIGHT)
				UMBRA_PROPERTY_R(WORLD_DISABLE_MULTIPLE_MEDIA, AW_WORLD_DISABLE_MULTIPLE_MEDIA)
				UMBRA_PROPERTY_R(WORLD_BOTMENU_URL, AW_WORLD_BOTMENU_URL)
				UMBRA_PROPERTY_R(WORLD_ENABLE_BUMP_EVENT, AW_WORLD_ENABLE_BUMP_EVENT)
				UMBRA_PROPERTY_R(WORLD_ENABLE_SYNC_EVENTS, AW_WORLD_ENABLE_SYNC_EVENTS)
				UMBRA_PROPERTY_R(WORLD_ENABLE_CAV, AW_WORLD_ENABLE_CAV)
				UMBRA_PROPERTY_R(WORLD_ENABLE_PAV, AW_WORLD_ENABLE_PAV)
				UMBRA_PROPERTY_R(WORLD_CHAT_DISABLE_URL_CLICKS, AW_WORLD_CHAT_DISABLE_URL_CLICKS)
				UMBRA_PROPERTY_R(WORLD_MOVER_EMPTY_RESET_TIMEOUT, AW_WORLD_MOVER_EMPTY_RESET_TIMEOUT)
				UMBRA_PROPERTY_R(WORLD_MOVER_USED_RESET_TIMEOUT, AW_WORLD_MOVER_USED_RESET_TIMEOUT)
				UMBRA_PROPERTY_R(MY_X, AW_MY_X)
				UMBRA_PROPERTY_R(MY_Y, AW_MY_Y)
				UMBRA_PROPERTY_R(MY_Z, AW_MY_Z)
				UMBRA_PROPERTY_R(MY_YAW, AW_MY_YAW)
				UMBRA_PROPERTY_R(MY_PITCH, AW_MY_PITCH)
				UMBRA_PROPERTY_R(MY_TYPE, AW_MY_TYPE)
				UMBRA_PROPERTY_R(MY_GESTURE, AW_MY_GESTURE)
				UMBRA_PROPERTY_R(MY_STATE, AW_MY_STATE)
				UMBRA_PROPERTY_R(AVATAR_SESSION, AW_AVATAR_SESSION)
				UMBRA_PROPERTY_R(AVATAR_NAME, AW_AVATAR_NAME)
				UMBRA_PROPERTY_R(AVATAR_X, AW_AVATAR_X)
				UMBRA_PROPERTY_R(AVATAR_Y, AW_AVATAR_Y)
				UMBRA_PROPERTY_R(AVATAR_Z, AW_AVATAR_Z)
				UMBRA_PROPERTY_R(AVATAR_YAW, AW_AVATAR_YAW)
				UMBRA_PROPERTY_R(AVATAR_PITCH, AW_AVATAR_PITCH)
				UMBRA_PROPERTY_R(AVATAR_TYPE, AW_AVATAR_TYPE)
				UMBRA_PROPERTY_R(AVATAR_GESTURE, AW_AVATAR_GESTURE)
				UMBRA_PROPERTY_R(AVATAR_STATE, AW_AVATAR_STATE)
				UMBRA_PROPERTY_R(AVATAR_ADDRESS, AW_AVATAR_ADDRESS)
				UMBRA_PROPERTY_R(AVATAR_VERSION, AW_AVATAR_VERSION)
				UMBRA_PROPERTY_R(AVATAR_CITIZEN, AW_AVATAR_CITIZEN)
				UMBRA_PROPERTY_R(AVATAR_PRIVILEGE, AW_AVATAR_PRIVILEGE)
				UMBRA_PROPERTY_R(AVATAR_LOCK, AW_AVATAR_LOCK)
				UMBRA_PROPERTY_R(AVATAR_FLAGS, AW_AVATAR_FLAGS)
				UMBRA_PROPERTY_R(CHAT_SESSION, AW_CHAT_SESSION)
				UMBRA_PROPERTY_R(CHAT_MESSAGE, AW_CHAT_MESSAGE)
				UMBRA_PROPERTY_R(CELL_X, AW_CELL_X)
				UMBRA_PROPERTY_R(CELL_Z, AW_CELL_Z)
				UMBRA_PROPERTY_R(CELL_SEQUENCE, AW_CELL_SEQUENCE)
				UMBRA_PROPERTY_R(CELL_SIZE, AW_CELL_SIZE)
				UMBRA_PROPERTY_R(CELL_ITERATOR, AW_CELL_ITERATOR)
				UMBRA_PROPERTY_R(CELL_COMBINE, AW_CELL_COMBINE)
				UMBRA_PROPERTY_R(OBJECT_ID, AW_OBJECT_ID)
				UMBRA_PROPERTY_R(OBJECT_NUMBER, AW_OBJECT_NUMBER)
				UMBRA_PROPERTY_R(OBJECT_X, AW_OBJECT_X)
				UMBRA_PROPERTY_R(OBJECT_Y, AW_OBJECT_Y)
				UMBRA_PROPERTY_R(OBJECT_Z, AW_OBJECT_Z)
				UMBRA_PROPERTY_R(OBJECT_YAW, AW_OBJECT_YAW)
				UMBRA_PROPERTY_R(OBJECT_TILT, AW_OBJECT_TILT)
				UMBRA_PROPERTY_R(OBJECT_ROLL, AW_OBJECT_ROLL)
				UMBRA_PROPERTY_R(OBJECT_MODEL, AW_OBJECT_MODEL)
				UMBRA_PROPERTY_R(OBJECT_DESCRIPTION, AW_OBJECT_DESCRIPTION)
				UMBRA_PROPERTY_R(OBJECT_ACTION, AW_OBJECT_ACTION)
				UMBRA_PROPERTY_R(OBJECT_OLD_NUMBER, AW_OBJECT_OLD_NUMBER)
				UMBRA_PROPERTY_R(OBJECT_OLD_X, AW_OBJECT_OLD_X)
				UMBRA_PROPERTY_R(OBJECT_OLD_Z, AW_OBJECT_OLD_Z)
				UMBRA_PROPERTY_R(OBJECT_OWNER, AW_OBJECT_OWNER)
				UMBRA_PROPERTY_R(OBJECT_SESSION, AW_OBJECT_SESSION)
				UMBRA_PROPERTY_R(OBJECT_BUILD_TIMESTAMP, AW_OBJECT_BUILD_TIMESTAMP)
				UMBRA_PROPERTY_R(OBJECT_SYNC, AW_OBJECT_SYNC)
				UMBRA_PROPERTY_R(OBJECT_TYPE, AW_OBJECT_TYPE)
				UMBRA_PROPERTY_R(OBJECT_DATA, AW_OBJECT_DATA)
				UMBRA_PROPERTY_R(QUERY_COMPLETE, AW_QUERY_COMPLETE)
				UMBRA_PROPERTY_R(CHAT_TYPE, AW_CHAT_TYPE)
				UMBRA_PROPERTY_R(LICENSE_NAME, AW_LICENSE_NAME)
				UMBRA_PROPERTY_R(LICENSE_PASSWORD, AW_LICENSE_PASSWORD)
				UMBRA_PROPERTY_R(LICENSE_USERS, AW_LICENSE_USERS)
				UMBRA_PROPERTY_R(LICENSE_RANGE, AW_LICENSE_RANGE)
				UMBRA_PROPERTY_R(LICENSE_EMAIL, AW_LICENSE_EMAIL)
				UMBRA_PROPERTY_R(LICENSE_COMMENT, AW_LICENSE_COMMENT)
				UMBRA_PROPERTY_R(LICENSE_CREATION_TIME, AW_LICENSE_CREATION_TIME)
				UMBRA_PROPERTY_R(LICENSE_EXPIRATION_TIME, AW_LICENSE_EXPIRATION_TIME)
				UMBRA_PROPERTY_R(LICENSE_LAST_START, AW_LICENSE_LAST_START)
				UMBRA_PROPERTY_R(LICENSE_LAST_ADDRESS, AW_LICENSE_LAST_ADDRESS)
				UMBRA_PROPERTY_R(LICENSE_HIDDEN, AW_LICENSE_HIDDEN)
				UMBRA_PROPERTY_R(LICENSE_ALLOW_TOURISTS, AW_LICENSE_ALLOW_TOURISTS)
				UMBRA_PROPERTY_R(LICENSE_VOIP, AW_LICENSE_VOIP)
				UMBRA_PROPERTY_R(LICENSE_PLUGINS, AW_LICENSE_PLUGINS)
				UMBRA_PROPERTY_R(WORLDLIST_NAME, AW_WORLDLIST_NAME)
				UMBRA_PROPERTY_R(WORLDLIST_STATUS, AW_WORLDLIST_STATUS)
				UMBRA_PROPERTY_R(WORLDLIST_USERS, AW_WORLDLIST_USERS)
				UMBRA_PROPERTY_R(WORLDLIST_RATING, AW_WORLDLIST_RATING)
				UMBRA_PROPERTY_R(WORLDLIST_MORE, AW_WORLDLIST_MORE)
				UMBRA_PROPERTY_R(EJECT_SESSION, AW_EJECT_SESSION)
				UMBRA_PROPERTY_R(EJECT_DURATION, AW_EJECT_DURATION)
				UMBRA_PROPERTY_R(EJECTION_TYPE, AW_EJECTION_TYPE)
				UMBRA_PROPERTY_R(EJECTION_ADDRESS, AW_EJECTION_ADDRESS)
				UMBRA_PROPERTY_R(EJECTION_EXPIRATION_TIME, AW_EJECTION_EXPIRATION_TIME)
				UMBRA_PROPERTY_R(EJECTION_CREATION_TIME, AW_EJECTION_CREATION_TIME)
				UMBRA_PROPERTY_R(EJECTION_COMMENT, AW_EJECTION_COMMENT)
				UMBRA_PROPERTY_R(DISCONNECT_REASON, AW_DISCONNECT_REASON)
				UMBRA_PROPERTY_R(FILE_RECIPIENT, AW_FILE_RECIPIENT)
				UMBRA_PROPERTY_R(FILE_SENDER, AW_FILE_SENDER)
				UMBRA_PROPERTY_R(FILE_SENDER_NAME, AW_FILE_SENDER_NAME)
				UMBRA_PROPERTY_R(FILE_SESSION, AW_FILE_SESSION)
				UMBRA_PROPERTY_R(FILE_ADDRESS, AW_FILE_ADDRESS)
				UMBRA_PROPERTY_R(FILE_PORT, AW_FILE_PORT)
				UMBRA_PROPERTY_R(CLICKED_SESSION, AW_CLICKED_SESSION)
				UMBRA_PROPERTY_R(CLICKED_NAME, AW_CLICKED_NAME)
				UMBRA_PROPERTY_R(URL_NAME, AW_URL_NAME)
				UMBRA_PROPERTY_R(URL_POST, AW_URL_POST)
				UMBRA_PROPERTY_R(URL_TARGET, AW_URL_TARGET)
				UMBRA_PROPERTY_R(URL_TARGET_3D, AW_URL_TARGET_3D)
				UMBRA_PROPERTY_R(TELEPORT_WORLD, AW_TELEPORT_WORLD)
				UMBRA_PROPERTY_R(TELEPORT_X, AW_TELEPORT_X)
				UMBRA_PROPERTY_R(TELEPORT_Y, AW_TELEPORT_Y)
				UMBRA_PROPERTY_R(TELEPORT_Z, AW_TELEPORT_Z)
				UMBRA_PROPERTY_R(TELEPORT_YAW, AW_TELEPORT_YAW)
				UMBRA_PROPERTY_R(TELEPORT_WARP, AW_TELEPORT_WARP)
				UMBRA_PROPERTY_R(SERVER_BUILD, AW_SERVER_BUILD)
				UMBRA_PROPERTY_R(SERVER_NAME, AW_SERVER_NAME)
				UMBRA_PROPERTY_R(SERVER_PASSWORD, AW_SERVER_PASSWORD)
				UMBRA_PROPERTY_R(SERVER_REGISTRY, AW_SERVER_REGISTRY)
				UMBRA_PROPERTY_R(SERVER_CARETAKERS, AW_SERVER_CARETAKERS)
				UMBRA_PROPERTY_R(SERVER_ID, AW_SERVER_ID)
				UMBRA_PROPERTY_R(SERVER_INSTANCE, AW_SERVER_INSTANCE)
				UMBRA_PROPERTY_R(SERVER_ENABLED, AW_SERVER_ENABLED)
				UMBRA_PROPERTY_R(SERVER_STATE, AW_SERVER_STATE)
				UMBRA_PROPERTY_R(SERVER_USERS, AW_SERVER_USERS)
				UMBRA_PROPERTY_R(SERVER_MAX_USERS, AW_SERVER_MAX_USERS)
				UMBRA_PROPERTY_R(SERVER_OBJECTS, AW_SERVER_OBJECTS)
				UMBRA_PROPERTY_R(SERVER_SIZE, AW_SERVER_SIZE)
				UMBRA_PROPERTY_R(SERVER_EXPIRATION, AW_SERVER_EXPIRATION)
				UMBRA_PROPERTY_R(SERVER_START_RC, AW_SERVER_START_RC)
				UMBRA_PROPERTY_R(SERVER_MORE, AW_SERVER_MORE)
				UMBRA_PROPERTY_R(SERVER_TERRAIN_NODES, AW_SERVER_TERRAIN_NODES)
				UMBRA_PROPERTY_R(TERRAIN_X, AW_TERRAIN_X)
				UMBRA_PROPERTY_R(TERRAIN_Z, AW_TERRAIN_Z)
				UMBRA_PROPERTY_R(TERRAIN_PAGE_X, AW_TERRAIN_PAGE_X)
				UMBRA_PROPERTY_R(TERRAIN_PAGE_Z, AW_TERRAIN_PAGE_Z)
				UMBRA_PROPERTY_R(TERRAIN_NODE_X, AW_TERRAIN_NODE_X)
				UMBRA_PROPERTY_R(TERRAIN_NODE_Z, AW_TERRAIN_NODE_Z)
				UMBRA_PROPERTY_R(TERRAIN_NODE_SIZE, AW_TERRAIN_NODE_SIZE)
				UMBRA_PROPERTY_R(TERRAIN_NODE_TEXTURE_COUNT, AW_TERRAIN_NODE_TEXTURE_COUNT)
				UMBRA_PROPERTY_R(TERRAIN_NODE_HEIGHT_COUNT, AW_TERRAIN_NODE_HEIGHT_COUNT)
				UMBRA_PROPERTY_R(TERRAIN_NODE_TEXTURES, AW_TERRAIN_NODE_TEXTURES)
				UMBRA_PROPERTY_R(TERRAIN_NODE_HEIGHTS, AW_TERRAIN_NODE_HEIGHTS)
				UMBRA_PROPERTY_R(TERRAIN_SEQUENCE, AW_TERRAIN_SEQUENCE)
				UMBRA_PROPERTY_R(TERRAIN_COMPLETE, AW_TERRAIN_COMPLETE)
				UMBRA_PROPERTY_R(TERRAIN_VERSION_NEEDED, AW_TERRAIN_VERSION_NEEDED)
				UMBRA_PROPERTY_R(ENTER_GLOBAL, AW_ENTER_GLOBAL)
				UMBRA_PROPERTY_R(CONSOLE_RED, AW_CONSOLE_RED)
				UMBRA_PROPERTY_R(CONSOLE_GREEN, AW_CONSOLE_GREEN)
				UMBRA_PROPERTY_R(CONSOLE_BLUE, AW_CONSOLE_BLUE)
				UMBRA_PROPERTY_R(CONSOLE_BOLD, AW_CONSOLE_BOLD)
				UMBRA_PROPERTY_R(CONSOLE_ITALICS, AW_CONSOLE_ITALICS)
				UMBRA_PROPERTY_R(CONSOLE_MESSAGE, AW_CONSOLE_MESSAGE)
				UMBRA_PROPERTY_R(BOTGRAM_TO, AW_BOTGRAM_TO)
				UMBRA_PROPERTY_R(BOTGRAM_FROM, AW_BOTGRAM_FROM)
				UMBRA_PROPERTY_R(BOTGRAM_FROM_NAME, AW_BOTGRAM_FROM_NAME)
				UMBRA_PROPERTY_R(BOTGRAM_TYPE, AW_BOTGRAM_TYPE)
				UMBRA_PROPERTY_R(BOTGRAM_TEXT, AW_BOTGRAM_TEXT)
				UMBRA_PROPERTY_R(TOOLBAR_ID, AW_TOOLBAR_ID)
				UMBRA_PROPERTY_R(TOOLBAR_SESSION, AW_TOOLBAR_SESSION)
				UMBRA_PROPERTY_R(USERLIST_MORE, AW_USERLIST_MORE)
				UMBRA_PROPERTY_R(USERLIST_NAME, AW_USERLIST_NAME)
				UMBRA_PROPERTY_R(USERLIST_WORLD, AW_USERLIST_WORLD)
				UMBRA_PROPERTY_R(USERLIST_EMAIL, AW_USERLIST_EMAIL)
				UMBRA_PROPERTY_R(USERLIST_CITIZEN, AW_USERLIST_CITIZEN)
				UMBRA_PROPERTY_R(USERLIST_PRIVILEGE, AW_USERLIST_PRIVILEGE)
				UMBRA_PROPERTY_R(USERLIST_STATE, AW_USERLIST_STATE)
				UMBRA_PROPERTY_R(USERLIST_ADDRESS, AW_USERLIST_ADDRESS)
				UMBRA_PROPERTY_R(USERLIST_ID, AW_USERLIST_ID)
				UMBRA_PROPERTY_R(SOUND_NAME, AW_SOUND_NAME)
				UMBRA_PROPERTY_R(CAMERA_LOCATION_TYPE, AW_CAMERA_LOCATION_TYPE)
				UMBRA_PROPERTY_R(CAMERA_LOCATION_OBJECT, AW_CAMERA_LOCATION_OBJECT)
				UMBRA_PROPERTY_R(CAMERA_LOCATION_SESSION, AW_CAMERA_LOCATION_SESSION)
				UMBRA_PROPERTY_R(CAMERA_TARGET_TYPE, AW_CAMERA_TARGET_TYPE)
				UMBRA_PROPERTY_R(CAMERA_TARGET_OBJECT, AW_CAMERA_TARGET_OBJECT)
				UMBRA_PROPERTY_R(CAMERA_TARGET_SESSION, AW_CAMERA_TARGET_SESSION)
				UMBRA_PROPERTY_R(PLUGIN_STRING, AW_PLUGIN_STRING)
				UMBRA_PROPERTY_R(BOTMENU_TO_SESSION, AW_BOTMENU_TO_SESSION)
				UMBRA_PROPERTY_R(BOTMENU_FROM_NAME, AW_BOTMENU_FROM_NAME)
				UMBRA_PROPERTY_R(BOTMENU_FROM_SESSION, AW_BOTMENU_FROM_SESSION)
				UMBRA_PROPERTY_R(BOTMENU_QUESTION, AW_BOTMENU_QUESTION)
				UMBRA_PROPERTY_R(BOTMENU_ANSWER, AW_BOTMENU_ANSWER)
				UMBRA_PROPERTY_R(CONTACT_NUMBER, AW_CONTACT_NUMBER)
				UMBRA_PROPERTY_R(CONTACT_STATUS, AW_CONTACT_STATUS)
				UMBRA_PROPERTY_R(CONTACT_NAME, AW_CONTACT_NAME)
				UMBRA_PROPERTY_R(CONTACT_WORLD, AW_CONTACT_WORLD)
				UMBRA_PROPERTY_R(CONTACT_MORE, AW_CONTACT_MORE)
				UMBRA_PROPERTY_R(CONTACT_OPTIONS, AW_CONTACT_OPTIONS)
				UMBRA_PROPERTY_R(TELEGRAM_TO, AW_TELEGRAM_TO)
				UMBRA_PROPERTY_R(TELEGRAM_FROM, AW_TELEGRAM_FROM)
				UMBRA_PROPERTY_R(TELEGRAM_TEXT, AW_TELEGRAM_TEXT)
				UMBRA_PROPERTY_R(TELEGRAM_SENT, AW_TELEGRAM_SENT)
				UMBRA_PROPERTY_R(TELEGRAM_MORE, AW_TELEGRAM_MORE)
				UMBRA_PROPERTY_R(JOIN_NAME, AW_JOIN_NAME)
				UMBRA_PROPERTY_R(JOIN_CITIZEN, AW_JOIN_CITIZEN)
				UMBRA_PROPERTY_R(JOIN_WORLD, AW_JOIN_WORLD)
				UMBRA_PROPERTY_R(JOIN_X, AW_JOIN_X)
				UMBRA_PROPERTY_R(JOIN_Y, AW_JOIN_Y)
				UMBRA_PROPERTY_R(JOIN_Z, AW_JOIN_Z)
				UMBRA_PROPERTY_R(JOIN_YAW, AW_JOIN_YAW)
				UMBRA_PROPERTY_R(REGISTER_CC_NAME, AW_REGISTER_CC_NAME)
				UMBRA_PROPERTY_R(REGISTER_CC_NUMBER, AW_REGISTER_CC_NUMBER)
				UMBRA_PROPERTY_R(REGISTER_CC_MONTH, AW_REGISTER_CC_MONTH)
				UMBRA_PROPERTY_R(REGISTER_CC_YEAR, AW_REGISTER_CC_YEAR)
				UMBRA_PROPERTY_R(REGISTER_ADDRESS, AW_REGISTER_ADDRESS)
				UMBRA_PROPERTY_R(REGISTER_CITY, AW_REGISTER_CITY)
				UMBRA_PROPERTY_R(REGISTER_STATE, AW_REGISTER_STATE)
				UMBRA_PROPERTY_R(REGISTER_ZIP, AW_REGISTER_ZIP)
				UMBRA_PROPERTY_R(REGISTER_PHONE_NUMBER, AW_REGISTER_PHONE_NUMBER)
				UMBRA_PROPERTY_R(REGISTER_BUSINESS_NAME, AW_REGISTER_BUSINESS_NAME)
				UMBRA_PROPERTY_R(REGISTER_VENDOR, AW_REGISTER_VENDOR)
				UMBRA_PROPERTY_R(REGISTER_RESULT, AW_REGISTER_RESULT)
				UMBRA_PROPERTY_R(REGISTER_METHOD, AW_REGISTER_METHOD)
				UMBRA_PROPERTY_R(VOIP_DATA, AW_VOIP_DATA)
				UMBRA_PROPERTY_R(UNIVERSE_CAV_PATH, AW_UNIVERSE_CAV_PATH)
				UMBRA_PROPERTY_R(CITIZEN_CAV_ENABLED, AW_CITIZEN_CAV_ENABLED)
				UMBRA_PROPERTY_R(ENTITY_TYPE, AW_ENTITY_TYPE)
				UMBRA_PROPERTY_R(ENTITY_ID, AW_ENTITY_ID)
				UMBRA_PROPERTY_R(ENTITY_STATE, AW_ENTITY_STATE)
				UMBRA_PROPERTY_R(ENTITY_FLAGS, AW_ENTITY_FLAGS)
				UMBRA_PROPERTY_R(ENTITY_X, AW_ENTITY_X)
				UMBRA_PROPERTY_R(ENTITY_Y, AW_ENTITY_Y)
				UMBRA_PROPERTY_R(ENTITY_Z, AW_ENTITY_Z)
				UMBRA_PROPERTY_R(ENTITY_YAW, AW_ENTITY_YAW)
				UMBRA_PROPERTY_R(ENTITY_PITCH, AW_ENTITY_PITCH)
				UMBRA_PROPERTY_R(ENTITY_ROLL, AW_ENTITY_ROLL)
				UMBRA_PROPERTY_R(ENTITY_OWNER_SESSION, AW_ENTITY_OWNER_SESSION)
				UMBRA_PROPERTY_R(ENTITY_OWNER_CITIZEN, AW_ENTITY_OWNER_CITIZEN)
				UMBRA_PROPERTY_R(AVATAR_DISTANCE, AW_AVATAR_DISTANCE)
				UMBRA_PROPERTY_R(AVATAR_ANGLE, AW_AVATAR_ANGLE)
				UMBRA_PROPERTY_R(AVATAR_Y_DELTA, AW_AVATAR_Y_DELTA)
				UMBRA_PROPERTY_R(AVATAR_YDELTA, AW_AVATAR_YAW_DELTA)
				UMBRA_PROPERTY_R(AVATAR_PITCH_DELTA, AW_AVATAR_PITCH_DELTA)
				UMBRA_PROPERTY_R(AVATAR_WORLD_INSTANCE, AW_AVATAR_WORLD_INSTANCE)
				UMBRA_PROPERTY_R(ATTRIB_SENDER_SESSION, AW_ATTRIB_SENDER_SESSION)
				UMBRA_PROPERTY_R(ENTITY_MODEL_NUM, AW_ENTITY_MODEL_NUM)
				UMBRA_PROPERTY_R(WORLD_V4_OBJECTS_RIGHT, AW_WORLD_V4_OBJECTS_RIGHT)
				UMBRA_PROPERTY_R(CITIZEN_LAST_ADDRESS, AW_CITIZEN_LAST_ADDRESS)
				UMBRA_PROPERTY_R(HUD_ELEMENT_TYPE, AW_HUD_ELEMENT_TYPE)
				UMBRA_PROPERTY_R(HUD_ELEMENT_ID, AW_HUD_ELEMENT_ID)
				UMBRA_PROPERTY_R(HUD_ELEMENT_SESSION, AW_HUD_ELEMENT_SESSION)
				UMBRA_PROPERTY_R(HUD_ELEMENT_ORIGIN, AW_HUD_ELEMENT_ORIGIN)
				UMBRA_PROPERTY_R(HUD_ELEMENT_X, AW_HUD_ELEMENT_X)
				UMBRA_PROPERTY_R(HUD_ELEMENT_Y, AW_HUD_ELEMENT_Y)
				UMBRA_PROPERTY_R(HUD_ELEMENT_Z, AW_HUD_ELEMENT_Z)
				UMBRA_PROPERTY_R(HUD_ELEMENT_FLAGS, AW_HUD_ELEMENT_FLAGS)
				UMBRA_PROPERTY_R(HUD_ELEMENT_TEXT, AW_HUD_ELEMENT_TEXT)
				UMBRA_PROPERTY_R(HUD_ELEMENT_COLOR, AW_HUD_ELEMENT_COLOR)
				UMBRA_PROPERTY_R(HUD_ELEMENT_OPACITY, AW_HUD_ELEMENT_OPACITY)
				UMBRA_PROPERTY_R(HUD_ELEMENT_SIZE_X, AW_HUD_ELEMENT_SIZE_X)
				UMBRA_PROPERTY_R(HUD_ELEMENT_SIZE_Y, AW_HUD_ELEMENT_SIZE_Y)
				UMBRA_PROPERTY_R(HUD_ELEMENT_SIZE_Z, AW_HUD_ELEMENT_SIZE_Z)
				UMBRA_PROPERTY_R(HUD_ELEMENT_CLICK_X, AW_HUD_ELEMENT_CLICK_X)
				UMBRA_PROPERTY_R(HUD_ELEMENT_CLICK_Y, AW_HUD_ELEMENT_CLICK_Y)
				UMBRA_PROPERTY_R(HUD_ELEMENT_CLICK_Z, AW_HUD_ELEMENT_CLICK_Z)
				UMBRA_PROPERTY_R(HUD_ELEMENT_TEXTURE_OFFSET_X, AW_HUD_ELEMENT_TEXTURE_OFFSET_X)
				UMBRA_PROPERTY_R(HUD_ELEMENT_TEXTURE_OFFSET_Y, AW_HUD_ELEMENT_TEXTURE_OFFSET_Y)
				UMBRA_PROPERTY_R(CITIZEN_PRIVACY, AW_CITIZEN_PRIVACY)
				UMBRA_PROPERTY_R(CITIZEN_TRIAL, AW_CITIZEN_TRIAL)
				UMBRA_PROPERTY_R(UNIVERSE_CAV_PATH2, AW_UNIVERSE_CAV_PATH2)
				UMBRA_PROPERTY_R(WORLD_DISABLE_SHADOWS, AW_WORLD_DISABLE_SHADOWS)
				UMBRA_PROPERTY_R(WORLD_ENABLE_CAMERA_COLLISION, AW_WORLD_ENABLE_CAMERA_COLLISION)
				UMBRA_PROPERTY_R(WORLD_SPECIAL_COMMANDS, AW_WORLD_SPECIAL_COMMANDS)

				// unsupported element
				return Umbra::PropertyRequest::NOT_HANDLED;  
			}

			bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
			{
				//
				// create a location from xyz position
				//

				UMBRA_FUNCTION_BEGIN(xyzLocation) {
					// required arguments
					UMBRA_FUNCTION_DA(0, X,		Umbra::VT_INTEGER);
					UMBRA_FUNCTION_DA(1, Y,		Umbra::VT_INTEGER);
					UMBRA_FUNCTION_DA(2, Z,		Umbra::VT_INTEGER);
					UMBRA_FUNCTION_OA(3, Yaw,	Umbra::VT_INTEGER, 0);

					// code information break
					UMBRA_FUNCTION_CS() {
						int x = vaX.toInt32();
						int y = vaY.toInt32();
						int z = vaZ.toInt32();
						::AW::Location* pLoc = new ::AW::Location(x, y, z, vaYaw.toInt32());
						pLoc->setUmbraGarbageCollected();
						UMBRA_FUNCTION_RET( *pLoc );
					} UMBRA_FUNCTION_CE();
				} UMBRA_FUNCTION_END();

				//
				// create a location from string position
				//

				UMBRA_FUNCTION_BEGIN(parseLocation) {
					// required arguments
					UMBRA_FUNCTION_DA(0, Pos, Umbra::VT_STRING);

					// code information break
					UMBRA_FUNCTION_CS() {
						Location loc(vaPos.toString().c_str());
						UMBRA_FUNCTION_RET( (new Location(loc))->setUmbraGarbageCollected()  );
					} UMBRA_FUNCTION_CE();
				} UMBRA_FUNCTION_END();

				//
				// Cannot Find
				//

				return Umbra::FunctionRequest::NOT_HANDLED;
			}

		public: // static interface
			static Namespace& getInstance() { static Namespace s; return s; }
		};
	}
}