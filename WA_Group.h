#pragma once
#include "awsdk_support.h"
#include "awsdk_bot.h"
#include "awsdk_colours.h"

namespace AW { class Bot; }

namespace World
{
	class WA_Group
	{
	public: // sdk exchange mechanisms
		virtual void sdk_exchange(AW::Bot& Client, bool ReadSDK)			{};
		virtual void sdk_read    (AW::Bot& Client)	{ sdk_exchange(Client, AW::EXCHANGE_UPDATE_DATA); };
		virtual void sdk_write   (AW::Bot& Client)	{ sdk_exchange(Client, AW::EXCHANGE_UPDATE_SDK);  };

	public: // file exchange mechanisms
		virtual void ini_exchange(AW::EnhancedINI& Ini, bool Direction)		{};
		virtual void ini_read    (AW::EnhancedINI& Ini)	{ ini_exchange(Ini, AW::READ_FROM_INI); };
		virtual void ini_write   (AW::EnhancedINI& Ini)	{ ini_exchange(Ini, AW::WRITE_TO_INI);  };
	};

	class WA_Master
	{
	};

	class Light : public WA_Group
	{
	public: // attributes
		int	light_x;
		int	light_y;
		int	light_z;
		AW::Colour	light;
		AW::Colour	ambient_light;
		String	light_texture;
		String	light_mask;
		int	light_draw_size;
		BOOL	light_draw_front;
		BOOL	light_draw_bright;
		BOOL	light_source_use_color;
		int	light_source_color;
		AW::Colour	fog;
		BOOL	fog_enable;
		int	fog_minimum;
		int	fog_maximum;
		BOOL	fog_tinted;

	public: // sdk exchange mechanisms
		void sdk_exchange(AW::Bot& Client, bool UpdateMode);

	public: // file exchange mechanisms
		void ini_exchange(AW::EnhancedINI& Ini, bool Direction);

	public: // core writing
		void copy_to_core(WA_Master& Master);
	};


	class Water : public WA_Group
	{
	public: // attributes
		String	water_texture;
		String	water_mask;
		String	water_bottom_texture;
		String	water_bottom_mask;
		float	water_opacity;
		AW::Colour	water;
		float	water_level;
		float	water_surface_move;
		float	water_wave_move;
		float	water_speed;
		BOOL	water_enabled;
		int	water_visibility;
		BOOL	water_under_terrain;

	public: // sdk exchange mechanisms
		void sdk_exchange(AW::Bot& Client, bool UpdateMode);

	public: // file exchange mechanisms
		void ini_exchange(AW::EnhancedINI& Ini, bool Direction);

	public: // core writing
		void copy_to_core(WA_Master& Master);
	};


	class Sky : public WA_Group
	{
	public: // attributes
		String	skybox;
		String	backdrop;
		AW::Colour	sky_north;
		AW::Colour	sky_south;
		AW::Colour	sky_east;
		AW::Colour	sky_west;
		AW::Colour	sky_top;
		AW::Colour	sky_bottom;

	public: // sdk exchange mechanisms
		void sdk_exchange(AW::Bot& Client, bool UpdateMode);

	public: // file exchange mechanisms
		void ini_exchange(AW::EnhancedINI& Ini, bool Direction);

	public: // core writing
		void copy_to_core(WA_Master& Master);
	};


	class Clouds : public WA_Group
	{
	public: // attributes
		String	clouds_layer1_texture;
		String	clouds_layer1_mask;
		float	clouds_layer1_tile;
		float	clouds_layer1_speed_x;
		float	clouds_layer1_speed_z;
		float	clouds_layer1_opacity;
		String	clouds_layer2_texture;
		String	clouds_layer2_mask;
		float	clouds_layer2_tile;
		float	clouds_layer2_speed_x;
		float	clouds_layer2_speed_z;
		float	clouds_layer2_opacity;
		String	clouds_layer3_texture;
		String	clouds_layer3_mask;
		float	clouds_layer3_tile;
		float	clouds_layer3_speed_x;
		float	clouds_layer3_speed_z;
		float	clouds_layer3_opacity;

	public: // sdk exchange mechanisms
		void sdk_exchange(AW::Bot& Client, bool UpdateMode);

	public: // file exchange mechanisms
		void ini_exchange(AW::EnhancedINI& Ini, bool Direction);

	public: // core writing
		void copy_to_core(WA_Master& Master);
	};


	class General : public WA_Group
	{
	public: // attributes
		String	title;
		String	home_page;
		String	welcome_message;
		int	rating;
		String	keywords;
		String	botmenu_url;

	public: // sdk exchange mechanisms
		void sdk_exchange(AW::Bot& Client, bool UpdateMode);

	public: // file exchange mechanisms
		void ini_exchange(AW::EnhancedINI& Ini, bool Direction);

	public: // core writing
		void copy_to_core(WA_Master& Master);
	};


	class Movement : public WA_Group
	{
	public: // attributes
		BOOL	allow_passthru;
		BOOL	allow_flying;
		BOOL	allow_teleport;
		float	gravity;
		float	buoyancy;
		float	friction;
		float	water_friction;
		BOOL	slopeslide_enabled;
		float	slopeslide_min_angle;
		float	slopeslide_max_angle;
		BOOL	avatar_refresh_rate;
		BOOL	allow_avatar_collision;
		int	mover_empty_resettimeout;
		int	mover_used_resettimeout;

	public: // sdk exchange mechanisms
		void sdk_exchange(AW::Bot& Client, bool UpdateMode);

	public: // file exchange mechanisms
		void ini_exchange(AW::EnhancedINI& Ini, bool Direction);

	public: // core writing
		void copy_to_core(WA_Master& Master);
	};


	class Rights : public WA_Group
	{
	public: // attributes
		String	eminent_domain_right;
		String	public_speaker_right;
		String	special_objects_right;
		String	build_right;
		String	enter_right;
		String	eject_right;
		String	bots_right;
		String	special_commands_right;
		String	speak_right;
		String	voip_right;

	public: // sdk exchange mechanisms
		void sdk_exchange(AW::Bot& Client, bool UpdateMode);

	public: // file exchange mechanisms
		void ini_exchange(AW::EnhancedINI& Ini, bool Direction);

	public: // core writing
		void copy_to_core(WA_Master& Master);
	};


	class Building : public WA_Group
	{
	public: // attributes
		String	object_path;
		int	object_refresh;
		String	object_password;
		int	restricted_radius;
		BOOL	disable_create_url;
		BOOL	enable_bump_event;
		BOOL	enable_sync_events;
		int	max_light_radius;
		BOOL	allow_object_select;
		int	cell_limit;
		BOOL	allow_3_axis_rotation;
		BOOL	allow_tourist_build;
		BOOL	disable_multiple_media;
		BOOL	enable_referer;

	public: // sdk exchange mechanisms
		void sdk_exchange(AW::Bot& Client, bool UpdateMode);

	public: // file exchange mechanisms
		void ini_exchange(AW::EnhancedINI& Ini, bool Direction);

	public: // core writing
		void copy_to_core(WA_Master& Master);
	};


	class Interaction : public WA_Group
	{
	public: // attributes
		BOOL	allow_citizen_whisper;
		BOOL	allow_tourist_whisper;
		BOOL	disable_chat;
		BOOL	chat_disable_url_clicks;
		BOOL	enable_cav;
		BOOL	enable_pav;
		BOOL	always_show_names;
		BOOL	disable_avatar_list;
		String	entry_point;
		int	minimum_visibility;

	public: // sdk exchange mechanisms
		void sdk_exchange(AW::Bot& Client, bool UpdateMode);

	public: // file exchange mechanisms
		void ini_exchange(AW::EnhancedINI& Ini, bool Direction);

	public: // core writing
		void copy_to_core(WA_Master& Master);
	};


	class Sounds : public WA_Group
	{
	public: // attributes
		String	sound_footstep;
		String	sound_water_enter;
		String	sound_water_exit;
		String	sound_ambient;

	public: // sdk exchange mechanisms
		void sdk_exchange(AW::Bot& Client, bool UpdateMode);

	public: // file exchange mechanisms
		void ini_exchange(AW::EnhancedINI& Ini, bool Direction);

	public: // core writing
		void copy_to_core(WA_Master& Master);
	};


	class Ground : public WA_Group
	{
	public: // attributes
		String	ground;
		BOOL	repeating_ground;
		BOOL	enable_terrain;
		float	terrain_ambient;
		float	terrain_diffuse;
		float	terrain_offset;

	public: // sdk exchange mechanisms
		void sdk_exchange(AW::Bot& Client, bool UpdateMode);

	public: // file exchange mechanisms
		void ini_exchange(AW::EnhancedINI& Ini, bool Direction);

	public: // core writing
		void copy_to_core(WA_Master& Master);
	};


	class WA_Config : public WA_Group
	{
	public: // sdk exchange mechanisms
		void sdk_exchange(AW::Bot& Client, bool UpdateMode);
		void ini_exchange(AW::EnhancedINI& Ini, bool Direction);

	public: // read and write to propdump file
		errno_t	read_dump(const char* File);
		errno_t	write_dump(const char* File);

	public: // groups
		Light		light;
		Water		water;
		Sky			sky;
		Clouds		clouds;
		General		general;
		Movement	movement;
		Rights		rights;
		Building	building;
		Interaction	interaction;
		Sounds		sounds;
		Ground		ground;
	};
}