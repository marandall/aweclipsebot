#include "stdafx.h"
#include "wa_group.h"

namespace World
{
	void ini_lx(AW::EnhancedINI& Ini, bool Direction, const char* Section, const char* Name, int& X)
	{ 
		Ini.Exchange(Direction, Section, Name, X); 
	}

	void ini_lx(AW::EnhancedINI& Ini, bool Direction, const char* Section, const char* Name, float& X)
	{ 
		Ini.Exchange(Direction, Section, Name, X); 
	}

	void ini_lx(AW::EnhancedINI& Ini, bool Direction, const char* Section, const char* Name, String& X)
	{ 
		Ini.Exchange(Direction, Section, Name, X); 
	}

	void ini_lx(AW::EnhancedINI& Ini, bool Direction, const char* Section, const char* Name, AW::Colour& X)
	{ 
		Ini.Exchange(Direction, Section, Name, X.value); 
	}

	errno_t	WA_Config::read_dump(const char* File)
	{
		// load the file
		AW::EnhancedINI ini;
		if (ini.Load(File) == false)
			return 1;

		// read the data
		ini_exchange(ini, AW::READ_FROM_INI);
		return 0;
	}
		
	errno_t	WA_Config::write_dump(const char* File)
	{
		// load the file
		AW::EnhancedINI ini;

		// add the headers
		ini.Write("__header", "type", "Eclipse Evolution 2 Worlds Attribute Dump");
		ini.Write("__header", "version", "3");

		ini_exchange(ini, AW::WRITE_TO_INI);
		return ini.Save(File) ? 0 : 1;
	}

	void WA_Config::sdk_exchange(AW::Bot& Client, bool UpdateMode)
	{
		light.		sdk_exchange(Client, UpdateMode);
		water.		sdk_exchange(Client, UpdateMode);
		sky.		sdk_exchange(Client, UpdateMode);
		clouds.		sdk_exchange(Client, UpdateMode);
		general.	sdk_exchange(Client, UpdateMode);
		movement.	sdk_exchange(Client, UpdateMode);
		rights.		sdk_exchange(Client, UpdateMode);
		building.	sdk_exchange(Client, UpdateMode);
		interaction.sdk_exchange(Client, UpdateMode);
		sounds.		sdk_exchange(Client, UpdateMode);
		ground.		sdk_exchange(Client, UpdateMode);
	}

	void WA_Config::ini_exchange(AW::EnhancedINI& Ini, bool Direction)
	{
		light.ini_exchange(Ini, Direction);
		water.ini_exchange(Ini, Direction);
		sky.ini_exchange(Ini, Direction);
		clouds.ini_exchange(Ini, Direction);
		general.ini_exchange(Ini, Direction);
		movement.ini_exchange(Ini, Direction);
		rights.ini_exchange(Ini, Direction);
		building.ini_exchange(Ini, Direction);
		interaction.ini_exchange(Ini, Direction);
		sounds.ini_exchange(Ini, Direction);
		ground.ini_exchange(Ini, Direction);
	}


	void Light::sdk_exchange(AW::Bot& Client, bool UpdateMode)
	{
		AW::ColourTripletWrapper ctw;
		ctw.pClr = NULL;
		
		Client._exchange(UpdateMode, AW_WORLD_LIGHT_X, light_x);
		Client._exchange(UpdateMode, AW_WORLD_LIGHT_Y, light_y);
		Client._exchange(UpdateMode, AW_WORLD_LIGHT_Z, light_z);
		Client._exchange(UpdateMode, AW_WORLD_LIGHT_RED, ctw(light));
		Client._exchange(UpdateMode, AW_WORLD_AMBIENT_LIGHT_RED, ctw(ambient_light));
		Client._exchange(UpdateMode, AW_WORLD_LIGHT_TEXTURE, light_texture);
		Client._exchange(UpdateMode, AW_WORLD_LIGHT_MASK, light_mask);
		Client._exchange(UpdateMode, AW_WORLD_LIGHT_DRAW_SIZE, light_draw_size);
		Client._exchange(UpdateMode, AW_WORLD_LIGHT_DRAW_FRONT, light_draw_front);
		Client._exchange(UpdateMode, AW_WORLD_LIGHT_DRAW_BRIGHT, light_draw_bright);
		Client._exchange(UpdateMode, AW_WORLD_LIGHT_SOURCE_USE_COLOR, light_source_use_color);
		Client._exchange(UpdateMode, AW_WORLD_LIGHT_SOURCE_COLOR, light_source_color);
		Client._exchange(UpdateMode, AW_WORLD_FOG_RED, ctw(fog));
		Client._exchange(UpdateMode, AW_WORLD_FOG_ENABLE, fog_enable);
		Client._exchange(UpdateMode, AW_WORLD_FOG_MINIMUM, fog_minimum);
		Client._exchange(UpdateMode, AW_WORLD_FOG_MAXIMUM, fog_maximum);
		Client._exchange(UpdateMode, AW_WORLD_FOG_TINTED, fog_tinted);
	}

	void Light::ini_exchange(AW::EnhancedINI& Ini, bool Direction)
	{
		const char* section_id = "light";
		ini_lx(Ini, Direction, section_id, "light_x", light_x);
		ini_lx(Ini, Direction, section_id, "light_y", light_y);
		ini_lx(Ini, Direction, section_id, "light_z", light_z);
		ini_lx(Ini, Direction, section_id, "light", light);
		ini_lx(Ini, Direction, section_id, "ambient_light", ambient_light);
		ini_lx(Ini, Direction, section_id, "light_texture", light_texture);
		ini_lx(Ini, Direction, section_id, "light_mask", light_mask);
		ini_lx(Ini, Direction, section_id, "light_draw_size", light_draw_size);
		ini_lx(Ini, Direction, section_id, "light_draw_front", light_draw_front);
		ini_lx(Ini, Direction, section_id, "light_draw_bright", light_draw_bright);
		ini_lx(Ini, Direction, section_id, "light_source_use_color", light_source_use_color);
		ini_lx(Ini, Direction, section_id, "light_source_color", light_source_color);
		ini_lx(Ini, Direction, section_id, "fog", fog);
		ini_lx(Ini, Direction, section_id, "fog_enable", fog_enable);
		ini_lx(Ini, Direction, section_id, "fog_minimum", fog_minimum);
		ini_lx(Ini, Direction, section_id, "fog_maximum", fog_maximum);
		ini_lx(Ini, Direction, section_id, "fog_tinted", fog_tinted);
	}

	void Water::sdk_exchange(AW::Bot& Client, bool UpdateMode)
	{
		AW::ColourTripletWrapper ctw;
		ctw.pClr = NULL;
		
		Client._exchange(UpdateMode, AW_WORLD_WATER_TEXTURE, water_texture);
		Client._exchange(UpdateMode, AW_WORLD_WATER_MASK, water_mask);
		Client._exchange(UpdateMode, AW_WORLD_WATER_BOTTOM_TEXTURE, water_bottom_texture);
		Client._exchange(UpdateMode, AW_WORLD_WATER_BOTTOM_MASK, water_bottom_mask);
		Client._exchange(UpdateMode, AW_WORLD_WATER_OPACITY, water_opacity);
		Client._exchange(UpdateMode, AW_WORLD_WATER_RED, ctw(water));
		Client._exchange(UpdateMode, AW_WORLD_WATER_LEVEL, water_level);
		Client._exchange(UpdateMode, AW_WORLD_WATER_SURFACE_MOVE, water_surface_move);
		Client._exchange(UpdateMode, AW_WORLD_WATER_WAVE_MOVE, water_wave_move);
		Client._exchange(UpdateMode, AW_WORLD_WATER_SPEED, water_speed);
		Client._exchange(UpdateMode, AW_WORLD_WATER_ENABLED, water_enabled);
		Client._exchange(UpdateMode, AW_WORLD_WATER_VISIBILITY, water_visibility);
		Client._exchange(UpdateMode, AW_WORLD_WATER_UNDER_TERRAIN, water_under_terrain);
	}

	void Water::ini_exchange(AW::EnhancedINI& Ini, bool Direction)
	{
		const char* section_id = "water";
		ini_lx(Ini, Direction, section_id, "water_texture", water_texture);
		ini_lx(Ini, Direction, section_id, "water_mask", water_mask);
		ini_lx(Ini, Direction, section_id, "water_bottom_texture", water_bottom_texture);
		ini_lx(Ini, Direction, section_id, "water_bottom_mask", water_bottom_mask);
		ini_lx(Ini, Direction, section_id, "water_opacity", water_opacity);
		ini_lx(Ini, Direction, section_id, "water", water);
		ini_lx(Ini, Direction, section_id, "water_level", water_level);
		ini_lx(Ini, Direction, section_id, "water_surface_move", water_surface_move);
		ini_lx(Ini, Direction, section_id, "water_wave_move", water_wave_move);
		ini_lx(Ini, Direction, section_id, "water_speed", water_speed);
		ini_lx(Ini, Direction, section_id, "water_enabled", water_enabled);
		ini_lx(Ini, Direction, section_id, "water_visibility", water_visibility);
		ini_lx(Ini, Direction, section_id, "water_under_terrain", water_under_terrain);
	}

	void Sky::sdk_exchange(AW::Bot& Client, bool UpdateMode)
	{
		AW::ColourTripletWrapper ctw;
		ctw.pClr = NULL;
		
		Client._exchange(UpdateMode, AW_WORLD_SKYBOX, skybox);
		Client._exchange(UpdateMode, AW_WORLD_BACKDROP, backdrop);
		Client._exchange(UpdateMode, AW_WORLD_SKY_NORTH_RED, ctw(sky_north));
		Client._exchange(UpdateMode, AW_WORLD_SKY_SOUTH_RED, ctw(sky_south));
		Client._exchange(UpdateMode, AW_WORLD_SKY_EAST_RED, ctw(sky_east));
		Client._exchange(UpdateMode, AW_WORLD_SKY_WEST_RED, ctw(sky_west));
		Client._exchange(UpdateMode, AW_WORLD_SKY_TOP_RED, ctw(sky_top));
		Client._exchange(UpdateMode, AW_WORLD_SKY_BOTTOM_RED, ctw(sky_bottom));
	}

	void Sky::ini_exchange(AW::EnhancedINI& Ini, bool Direction)
	{
		const char* section_id = "sky";
		ini_lx(Ini, Direction, section_id, "skybox", skybox);
		ini_lx(Ini, Direction, section_id, "backdrop", backdrop);
		ini_lx(Ini, Direction, section_id, "sky_north", sky_north);
		ini_lx(Ini, Direction, section_id, "sky_south", sky_south);
		ini_lx(Ini, Direction, section_id, "sky_east", sky_east);
		ini_lx(Ini, Direction, section_id, "sky_west", sky_west);
		ini_lx(Ini, Direction, section_id, "sky_top", sky_top);
		ini_lx(Ini, Direction, section_id, "sky_bottom", sky_bottom);
	}

	void Clouds::sdk_exchange(AW::Bot& Client, bool UpdateMode)
	{
		AW::ColourTripletWrapper ctw;
		ctw.pClr = NULL;
		
		Client._exchange(UpdateMode, AW_WORLD_CLOUDS_LAYER1_TEXTURE, clouds_layer1_texture);
		Client._exchange(UpdateMode, AW_WORLD_CLOUDS_LAYER1_MASK, clouds_layer1_mask);
		Client._exchange(UpdateMode, AW_WORLD_CLOUDS_LAYER1_TILE, clouds_layer1_tile);
		Client._exchange(UpdateMode, AW_WORLD_CLOUDS_LAYER1_SPEED_X, clouds_layer1_speed_x);
		Client._exchange(UpdateMode, AW_WORLD_CLOUDS_LAYER1_SPEED_Z, clouds_layer1_speed_z);
		Client._exchange(UpdateMode, AW_WORLD_CLOUDS_LAYER1_OPACITY, clouds_layer1_opacity);
		Client._exchange(UpdateMode, AW_WORLD_CLOUDS_LAYER2_TEXTURE, clouds_layer2_texture);
		Client._exchange(UpdateMode, AW_WORLD_CLOUDS_LAYER2_MASK, clouds_layer2_mask);
		Client._exchange(UpdateMode, AW_WORLD_CLOUDS_LAYER2_TILE, clouds_layer2_tile);
		Client._exchange(UpdateMode, AW_WORLD_CLOUDS_LAYER2_SPEED_X, clouds_layer2_speed_x);
		Client._exchange(UpdateMode, AW_WORLD_CLOUDS_LAYER2_SPEED_Z, clouds_layer2_speed_z);
		Client._exchange(UpdateMode, AW_WORLD_CLOUDS_LAYER2_OPACITY, clouds_layer2_opacity);
		Client._exchange(UpdateMode, AW_WORLD_CLOUDS_LAYER3_TEXTURE, clouds_layer3_texture);
		Client._exchange(UpdateMode, AW_WORLD_CLOUDS_LAYER3_MASK, clouds_layer3_mask);
		Client._exchange(UpdateMode, AW_WORLD_CLOUDS_LAYER3_TILE, clouds_layer3_tile);
		Client._exchange(UpdateMode, AW_WORLD_CLOUDS_LAYER3_SPEED_X, clouds_layer3_speed_x);
		Client._exchange(UpdateMode, AW_WORLD_CLOUDS_LAYER3_SPEED_Z, clouds_layer3_speed_z);
		Client._exchange(UpdateMode, AW_WORLD_CLOUDS_LAYER3_OPACITY, clouds_layer3_opacity);
	}

	void Clouds::ini_exchange(AW::EnhancedINI& Ini, bool Direction)
	{
		const char* section_id = "clouds";
		ini_lx(Ini, Direction, section_id, "clouds_layer1_texture", clouds_layer1_texture);
		ini_lx(Ini, Direction, section_id, "clouds_layer1_mask", clouds_layer1_mask);
		ini_lx(Ini, Direction, section_id, "clouds_layer1_tile", clouds_layer1_tile);
		ini_lx(Ini, Direction, section_id, "clouds_layer1_speed_x", clouds_layer1_speed_x);
		ini_lx(Ini, Direction, section_id, "clouds_layer1_speed_z", clouds_layer1_speed_z);
		ini_lx(Ini, Direction, section_id, "clouds_layer1_opacity", clouds_layer1_opacity);
		ini_lx(Ini, Direction, section_id, "clouds_layer2_texture", clouds_layer2_texture);
		ini_lx(Ini, Direction, section_id, "clouds_layer2_mask", clouds_layer2_mask);
		ini_lx(Ini, Direction, section_id, "clouds_layer2_tile", clouds_layer2_tile);
		ini_lx(Ini, Direction, section_id, "clouds_layer2_speed_x", clouds_layer2_speed_x);
		ini_lx(Ini, Direction, section_id, "clouds_layer2_speed_z", clouds_layer2_speed_z);
		ini_lx(Ini, Direction, section_id, "clouds_layer2_opacity", clouds_layer2_opacity);
		ini_lx(Ini, Direction, section_id, "clouds_layer3_texture", clouds_layer3_texture);
		ini_lx(Ini, Direction, section_id, "clouds_layer3_mask", clouds_layer3_mask);
		ini_lx(Ini, Direction, section_id, "clouds_layer3_tile", clouds_layer3_tile);
		ini_lx(Ini, Direction, section_id, "clouds_layer3_speed_x", clouds_layer3_speed_x);
		ini_lx(Ini, Direction, section_id, "clouds_layer3_speed_z", clouds_layer3_speed_z);
		ini_lx(Ini, Direction, section_id, "clouds_layer3_opacity", clouds_layer3_opacity);
	}

	void General::sdk_exchange(AW::Bot& Client, bool UpdateMode)
	{
		AW::ColourTripletWrapper ctw;
		ctw.pClr = NULL;
		
		Client._exchange(UpdateMode, AW_WORLD_TITLE, title);
		Client._exchange(UpdateMode, AW_WORLD_HOME_PAGE, home_page);
		Client._exchange(UpdateMode, AW_WORLD_WELCOME_MESSAGE, welcome_message);
		Client._exchange(UpdateMode, AW_WORLD_RATING, rating);
		Client._exchange(UpdateMode, AW_WORLD_KEYWORDS, keywords);
		Client._exchange(UpdateMode, AW_WORLD_BOTMENU_URL, botmenu_url);
	}

	void General::ini_exchange(AW::EnhancedINI& Ini, bool Direction)
	{
		const char* section_id = "general";
		ini_lx(Ini, Direction, section_id, "title", title);
		ini_lx(Ini, Direction, section_id, "home_page", home_page);
		ini_lx(Ini, Direction, section_id, "welcome_message", welcome_message);
		ini_lx(Ini, Direction, section_id, "rating", rating);
		ini_lx(Ini, Direction, section_id, "keywords", keywords);
		ini_lx(Ini, Direction, section_id, "botmenu_url", botmenu_url);
	}

	void Movement::sdk_exchange(AW::Bot& Client, bool UpdateMode)
	{
		AW::ColourTripletWrapper ctw;
		ctw.pClr = NULL;
		
		Client._exchange(UpdateMode, AW_WORLD_ALLOW_PASSTHRU, allow_passthru);
		Client._exchange(UpdateMode, AW_WORLD_ALLOW_FLYING, allow_flying);
		Client._exchange(UpdateMode, AW_WORLD_ALLOW_TELEPORT, allow_teleport);
		Client._exchange(UpdateMode, AW_WORLD_GRAVITY, gravity);
		Client._exchange(UpdateMode, AW_WORLD_BUOYANCY, buoyancy);
		Client._exchange(UpdateMode, AW_WORLD_FRICTION, friction);
		Client._exchange(UpdateMode, AW_WORLD_WATER_FRICTION, water_friction);
		Client._exchange(UpdateMode, AW_WORLD_SLOPESLIDE_ENABLED, slopeslide_enabled);
		Client._exchange(UpdateMode, AW_WORLD_SLOPESLIDE_MIN_ANGLE, slopeslide_min_angle);
		Client._exchange(UpdateMode, AW_WORLD_SLOPESLIDE_MAX_ANGLE, slopeslide_max_angle);
		Client._exchange(UpdateMode, AW_WORLD_AVATAR_REFRESH_RATE, avatar_refresh_rate);
		Client._exchange(UpdateMode, AW_WORLD_ALLOW_AVATAR_COLLISION, allow_avatar_collision);
		Client._exchange(UpdateMode, AW_WORLD_MOVER_EMPTY_RESET_TIMEOUT, mover_empty_resettimeout);
		Client._exchange(UpdateMode, AW_WORLD_MOVER_USED_RESET_TIMEOUT, mover_used_resettimeout);
	}

	void Movement::ini_exchange(AW::EnhancedINI& Ini, bool Direction)
	{
		const char* section_id = "movement";
		ini_lx(Ini, Direction, section_id, "allow_passthru", allow_passthru);
		ini_lx(Ini, Direction, section_id, "allow_flying", allow_flying);
		ini_lx(Ini, Direction, section_id, "allow_teleport", allow_teleport);
		ini_lx(Ini, Direction, section_id, "gravity", gravity);
		ini_lx(Ini, Direction, section_id, "buoyancy", buoyancy);
		ini_lx(Ini, Direction, section_id, "friction", friction);
		ini_lx(Ini, Direction, section_id, "water_friction", water_friction);
		ini_lx(Ini, Direction, section_id, "slopeslide_enabled", slopeslide_enabled);
		ini_lx(Ini, Direction, section_id, "slopeslide_min_angle", slopeslide_min_angle);
		ini_lx(Ini, Direction, section_id, "slopeslide_max_angle", slopeslide_max_angle);
		ini_lx(Ini, Direction, section_id, "avatar_refresh_rate", avatar_refresh_rate);
		ini_lx(Ini, Direction, section_id, "allow_avatar_collision", allow_avatar_collision);
		ini_lx(Ini, Direction, section_id, "mover_empty_resettimeout", mover_empty_resettimeout);
		ini_lx(Ini, Direction, section_id, "mover_used_resettimeout", mover_used_resettimeout);
	}

	void Rights::sdk_exchange(AW::Bot& Client, bool UpdateMode)
	{
		AW::ColourTripletWrapper ctw;
		ctw.pClr = NULL;
		
		Client._exchange(UpdateMode, AW_WORLD_EMINENT_DOMAIN_RIGHT, eminent_domain_right);
		Client._exchange(UpdateMode, AW_WORLD_PUBLIC_SPEAKER_RIGHT, public_speaker_right);
		Client._exchange(UpdateMode, AW_WORLD_SPECIAL_OBJECTS_RIGHT, special_objects_right);
		Client._exchange(UpdateMode, AW_WORLD_BUILD_RIGHT, build_right);
		Client._exchange(UpdateMode, AW_WORLD_ENTER_RIGHT, enter_right);
		Client._exchange(UpdateMode, AW_WORLD_EJECT_RIGHT, eject_right);
		Client._exchange(UpdateMode, AW_WORLD_BOTS_RIGHT, bots_right);
		Client._exchange(UpdateMode, AW_WORLD_SPECIAL_COMMANDS_RIGHT, special_commands_right);
		Client._exchange(UpdateMode, AW_WORLD_SPEAK_RIGHT, speak_right);
		Client._exchange(UpdateMode, AW_WORLD_VOIP_RIGHT, voip_right);
	}

	void Rights::ini_exchange(AW::EnhancedINI& Ini, bool Direction)
	{
		const char* section_id = "rights";
		ini_lx(Ini, Direction, section_id, "eminent_domain_right", eminent_domain_right);
		ini_lx(Ini, Direction, section_id, "public_speaker_right", public_speaker_right);
		ini_lx(Ini, Direction, section_id, "special_objects_right", special_objects_right);
		ini_lx(Ini, Direction, section_id, "build_right", build_right);
		ini_lx(Ini, Direction, section_id, "enter_right", enter_right);
		ini_lx(Ini, Direction, section_id, "eject_right", eject_right);
		ini_lx(Ini, Direction, section_id, "bots_right", bots_right);
		ini_lx(Ini, Direction, section_id, "special_commands_right", special_commands_right);
		ini_lx(Ini, Direction, section_id, "speak_right", speak_right);
		ini_lx(Ini, Direction, section_id, "voip_right", voip_right);
	}

	void Building::sdk_exchange(AW::Bot& Client, bool UpdateMode)
	{
		AW::ColourTripletWrapper ctw;
		ctw.pClr = NULL;
		
		Client._exchange(UpdateMode, AW_WORLD_OBJECT_PATH, object_path);
		Client._exchange(UpdateMode, AW_WORLD_OBJECT_REFRESH, object_refresh);
		Client._exchange(UpdateMode, AW_WORLD_OBJECT_PASSWORD, object_password);
		Client._exchange(UpdateMode, AW_WORLD_RESTRICTED_RADIUS, restricted_radius);
		Client._exchange(UpdateMode, AW_WORLD_DISABLE_CREATE_URL, disable_create_url);
		Client._exchange(UpdateMode, AW_WORLD_ENABLE_BUMP_EVENT, enable_bump_event);
		Client._exchange(UpdateMode, AW_WORLD_ENABLE_SYNC_EVENTS, enable_sync_events);
		Client._exchange(UpdateMode, AW_WORLD_MAX_LIGHT_RADIUS, max_light_radius);
		Client._exchange(UpdateMode, AW_WORLD_ALLOW_OBJECT_SELECT, allow_object_select);
		Client._exchange(UpdateMode, AW_WORLD_CELL_LIMIT, cell_limit);
		Client._exchange(UpdateMode, AW_WORLD_ALLOW_3_AXIS_ROTATION, allow_3_axis_rotation);
		Client._exchange(UpdateMode, AW_WORLD_ALLOW_TOURIST_BUILD, allow_tourist_build);
		Client._exchange(UpdateMode, AW_WORLD_DISABLE_MULTIPLE_MEDIA, disable_multiple_media);
		Client._exchange(UpdateMode, AW_WORLD_ENABLE_REFERER, enable_referer);
	}

	void Building::ini_exchange(AW::EnhancedINI& Ini, bool Direction)
	{
		const char* section_id = "building";
		ini_lx(Ini, Direction, section_id, "object_path", object_path);
		ini_lx(Ini, Direction, section_id, "object_refresh", object_refresh);
		ini_lx(Ini, Direction, section_id, "object_password", object_password);
		ini_lx(Ini, Direction, section_id, "restricted_radius", restricted_radius);
		ini_lx(Ini, Direction, section_id, "disable_create_url", disable_create_url);
		ini_lx(Ini, Direction, section_id, "enable_bump_event", enable_bump_event);
		ini_lx(Ini, Direction, section_id, "enable_sync_events", enable_sync_events);
		ini_lx(Ini, Direction, section_id, "max_light_radius", max_light_radius);
		ini_lx(Ini, Direction, section_id, "allow_object_select", allow_object_select);
		ini_lx(Ini, Direction, section_id, "cell_limit", cell_limit);
		ini_lx(Ini, Direction, section_id, "allow_3_axis_rotation", allow_3_axis_rotation);
		ini_lx(Ini, Direction, section_id, "allow_tourist_build", allow_tourist_build);
		ini_lx(Ini, Direction, section_id, "disable_multiple_media", disable_multiple_media);
		ini_lx(Ini, Direction, section_id, "enable_referer", enable_referer);
	}

	void Interaction::sdk_exchange(AW::Bot& Client, bool UpdateMode)
	{
		AW::ColourTripletWrapper ctw;
		ctw.pClr = NULL;
		
		Client._exchange(UpdateMode, AW_WORLD_ALLOW_CITIZEN_WHISPER, allow_citizen_whisper);
		Client._exchange(UpdateMode, AW_WORLD_ALLOW_TOURIST_WHISPER, allow_tourist_whisper);
		Client._exchange(UpdateMode, AW_WORLD_DISABLE_CHAT, disable_chat);
		Client._exchange(UpdateMode, AW_WORLD_CHAT_DISABLE_URL_CLICKS, chat_disable_url_clicks);
		Client._exchange(UpdateMode, AW_WORLD_ENABLE_CAV, enable_cav);
		Client._exchange(UpdateMode, AW_WORLD_ENABLE_PAV, enable_pav);
		Client._exchange(UpdateMode, AW_WORLD_ALWAYS_SHOW_NAMES, always_show_names);
		Client._exchange(UpdateMode, AW_WORLD_DISABLE_AVATAR_LIST, disable_avatar_list);
		Client._exchange(UpdateMode, AW_WORLD_ENTRY_POINT, entry_point);
		Client._exchange(UpdateMode, AW_WORLD_MINIMUM_VISIBILITY, minimum_visibility);
	}

	void Interaction::ini_exchange(AW::EnhancedINI& Ini, bool Direction)
	{
		const char* section_id = "interaction";
		ini_lx(Ini, Direction, section_id, "allow_citizen_whisper", allow_citizen_whisper);
		ini_lx(Ini, Direction, section_id, "allow_tourist_whisper", allow_tourist_whisper);
		ini_lx(Ini, Direction, section_id, "disable_chat", disable_chat);
		ini_lx(Ini, Direction, section_id, "chat_disable_url_clicks", chat_disable_url_clicks);
		ini_lx(Ini, Direction, section_id, "enable_cav", enable_cav);
		ini_lx(Ini, Direction, section_id, "enable_pav", enable_pav);
		ini_lx(Ini, Direction, section_id, "always_show_names", always_show_names);
		ini_lx(Ini, Direction, section_id, "disable_avatar_list", disable_avatar_list);
		ini_lx(Ini, Direction, section_id, "entry_point", entry_point);
		ini_lx(Ini, Direction, section_id, "minimum_visibility", minimum_visibility);
	}

	void Sounds::sdk_exchange(AW::Bot& Client, bool UpdateMode)
	{
		AW::ColourTripletWrapper ctw;
		ctw.pClr = NULL;
		
		Client._exchange(UpdateMode, AW_WORLD_SOUND_FOOTSTEP, sound_footstep);
		Client._exchange(UpdateMode, AW_WORLD_SOUND_WATER_ENTER, sound_water_enter);
		Client._exchange(UpdateMode, AW_WORLD_SOUND_WATER_EXIT, sound_water_exit);
		Client._exchange(UpdateMode, AW_WORLD_SOUND_AMBIENT, sound_ambient);
	}

	void Sounds::ini_exchange(AW::EnhancedINI& Ini, bool Direction)
	{
		const char* section_id = "sounds";
		ini_lx(Ini, Direction, section_id, "sound_footstep", sound_footstep);
		ini_lx(Ini, Direction, section_id, "sound_water_enter", sound_water_enter);
		ini_lx(Ini, Direction, section_id, "sound_water_exit", sound_water_exit);
		ini_lx(Ini, Direction, section_id, "sound_ambient", sound_ambient);
	}

	void Ground::sdk_exchange(AW::Bot& Client, bool UpdateMode)
	{
		AW::ColourTripletWrapper ctw;
		ctw.pClr = NULL;
		
		Client._exchange(UpdateMode, AW_WORLD_GROUND, ground);
		Client._exchange(UpdateMode, AW_WORLD_REPEATING_GROUND, repeating_ground);
		Client._exchange(UpdateMode, AW_WORLD_ENABLE_TERRAIN, enable_terrain);
		Client._exchange(UpdateMode, AW_WORLD_TERRAIN_AMBIENT, terrain_ambient);
		Client._exchange(UpdateMode, AW_WORLD_TERRAIN_DIFFUSE, terrain_diffuse);
		Client._exchange(UpdateMode, AW_WORLD_TERRAIN_OFFSET, terrain_offset);
	}

	void Ground::ini_exchange(AW::EnhancedINI& Ini, bool Direction)
	{
		const char* section_id = "ground";
		ini_lx(Ini, Direction, section_id, "ground", ground);
		ini_lx(Ini, Direction, section_id, "repeating_ground", repeating_ground);
		ini_lx(Ini, Direction, section_id, "enable_terrain", enable_terrain);
		ini_lx(Ini, Direction, section_id, "terrain_ambient", terrain_ambient);
		ini_lx(Ini, Direction, section_id, "terrain_diffuse", terrain_diffuse);
		ini_lx(Ini, Direction, section_id, "terrain_offset", terrain_offset);
	}
}