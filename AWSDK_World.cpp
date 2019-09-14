#include "stdafx.h"

// required for inclusion
#include "awsdk_bot.h"
#include "awsdk_support.h"
#include "awsdk_world.h"
#include "awsdk_group_map.h"
#include "awsdk_colours.h"

namespace AW
{
	namespace World
	{	
		int interpolate_int(int Old, int New, float Prog)
		{
			if (Prog > 1.0) Prog = 1.0;
			else if (Prog < 0.0) Prog = 0.0;
			
			return (int)(Old + (New - Old) * Prog);
		}
		
		float interpolate_float(float Old, float New, float Prog)
		{
			if (Prog > 1.0) Prog = 1.0;
			else if (Prog < 0.0) Prog = 0.0;
			
			return (float)(Old + (New - Old) * Prog);
		}
		
		String interpolate_string(String Old, String New, float Prog)
		{
			if (Prog > 1.0) Prog = 1.0;
			else if (Prog < 0.0) Prog = 0.0;
			
			if (Prog <= 0.5) return Old;
			else return New;
		}
		
		COLORREF interpolate_rgb(DWORD Old, DWORD New, float Prog)
		{
			if (Prog > 1.0) Prog = 1.0;
			else if (Prog < 0.0) Prog = 0.0;
			
			// old values
			int ro = GetRValue(Old);
			int go = GetGValue(Old);
			int bo = GetBValue(Old);
			
			// new values
			int rn = GetRValue(New);
			int gn = GetGValue(New);
			int bn = GetBValue(New);
			
			// higher bounds checking
			if (rn > 0xff) rn = 0xff;
			if (gn > 0xff) gn = 0xff;
			if (bn > 0xff) bn = 0xff;
			
			// lower bounds checking
			if (rn < 0x00) rn = 0x00;
			if (gn < 0x00) gn = 0x00;
			if (bn < 0x00) bn = 0x00;
			
			// interpolate
			int rr = (int)(ro + (rn - ro) * Prog);
			int gr = (int)(go + (gn - go) * Prog);
			int br = (int)(bo + (bn - bo) * Prog);
			
			// return style
			return RGB(rr, gr, br);
		}
		
		// copy the attributes from the sdk to the database
		void Attributes::from_sdk(AW::Bot& Client)
		{
			dx_sdk(Client, AW::EXCHANGE_UPDATE_DATA);
		}
		
		// copy the data attributes to the sdk
		void Attributes::to_sdk(AW::Bot& Client)
		{
			dx_sdk(Client, AW::EXCHANGE_UPDATE_SDK);
		}
		
		// exchange mechanism		
		void Attributes::dx_sdk(AW::Bot& Client, bool ExchangeMode)
		{
			// colour triplet wrapper is used for transfering multiple colour sets
			ColourTripletWrapper ctw;
			
			Client._exchange(ExchangeMode, AW_WORLD_ENTER_RIGHT,                   enter_right                        );
			Client._exchange(ExchangeMode, AW_WORLD_BUILD_RIGHT,                   build_right                        );
			Client._exchange(ExchangeMode, AW_WORLD_VOIP_RIGHT,                    voip_right                         );
			Client._exchange(ExchangeMode, AW_WORLD_EMINENT_DOMAIN_RIGHT,          eminent_domain_right               );
			Client._exchange(ExchangeMode, AW_WORLD_SPECIAL_OBJECTS_RIGHT,         special_objects_right              );
			Client._exchange(ExchangeMode, AW_WORLD_PUBLIC_SPEAKER_RIGHT,          public_speaker_right               );
			Client._exchange(ExchangeMode, AW_WORLD_BOTS_RIGHT,                    bots_right                         );
			Client._exchange(ExchangeMode, AW_WORLD_EJECT_RIGHT,                   eject_right                        );
			Client._exchange(ExchangeMode, AW_WORLD_SPEAK_RIGHT,                   speak_right                        );
			Client._exchange(ExchangeMode, AW_WORLD_SPECIAL_COMMANDS_RIGHT,        special_commands_right             );
			Client._exchange(ExchangeMode, AW_WORLD_SPECIAL_COMMANDS,              special_commands                   );
			Client._exchange(ExchangeMode, AW_WORLD_V4_OBJECTS_RIGHT,              v4_objects_right                   );
			Client._exchange(ExchangeMode, AW_WORLD_TERRAIN_RIGHT,                 terrain_right                      );
			Client._exchange(ExchangeMode, AW_WORLD_TITLE,                         title                              );
			Client._exchange(ExchangeMode, AW_WORLD_HOME_PAGE,                     home_page                          );
			Client._exchange(ExchangeMode, AW_WORLD_RATING,                        rating                             );
			Client._exchange(ExchangeMode, AW_WORLD_WELCOME_MESSAGE,               welcome_message                    );
			Client._exchange(ExchangeMode, AW_WORLD_KEYWORDS,                      keywords                           );
			Client._exchange(ExchangeMode, AW_WORLD_GROUND,                        ground                             );
			Client._exchange(ExchangeMode, AW_WORLD_REPEATING_GROUND,              repeating_ground                   );
			Client._exchange(ExchangeMode, AW_WORLD_ENABLE_TERRAIN,                enable_terrain                     );
			Client._exchange(ExchangeMode, AW_WORLD_TERRAIN_AMBIENT,               terrain_ambient                    );
			Client._exchange(ExchangeMode, AW_WORLD_TERRAIN_DIFFUSE,               terrain_diffuse                    );
			Client._exchange(ExchangeMode, AW_WORLD_TERRAIN_OFFSET,                terrain_offset                     );
			Client._exchange(ExchangeMode, AW_WORLD_OBJECT_PATH,                   object_path                        );
			Client._exchange(ExchangeMode, AW_WORLD_OBJECT_REFRESH,                object_refresh                     );
			Client._exchange(ExchangeMode, AW_WORLD_OBJECT_PASSWORD,               object_password                    );
			Client._exchange(ExchangeMode, AW_WORLD_ENABLE_REFERER,                enable_referer                     );
			Client._exchange(ExchangeMode, AW_WORLD_CAV_OBJECT_PATH,               cav_object_path                    );
			Client._exchange(ExchangeMode, AW_WORLD_CAV_OBJECT_PASSWORD,           cav_object_password                );
			Client._exchange(ExchangeMode, AW_WORLD_CAV_OBJECT_REFRESH,            cav_object_refresh                 );
			Client._exchange(ExchangeMode, AW_WORLD_CELL_LIMIT,                    cell_limit                         );
			Client._exchange(ExchangeMode, AW_WORLD_RESTRICTED_RADIUS,             restricted_radius                  );
			Client._exchange(ExchangeMode, AW_WORLD_MAX_LIGHT_RADIUS,              max_light_radius                   );
			Client._exchange(ExchangeMode, AW_WORLD_DISABLE_CREATE_URL,            disable_create_url                 );
			Client._exchange(ExchangeMode, AW_WORLD_ALLOW_OBJECT_SELECT,           allow_object_select                );
			Client._exchange(ExchangeMode, AW_WORLD_ALLOW_3_AXIS_ROTATION,         allow_3_axis_rotation              );
			Client._exchange(ExchangeMode, AW_WORLD_ALLOW_TOURIST_BUILD,           allow_tourist_build                );
			Client._exchange(ExchangeMode, AW_WORLD_MOVER_EMPTY_RESET_TIMEOUT,     mover_empty_reset_timeout          );
			Client._exchange(ExchangeMode, AW_WORLD_MOVER_USED_RESET_TIMEOUT,      mover_used_reset_timeout           );
			Client._exchange(ExchangeMode, AW_WORLD_ENABLE_BUMP_EVENT,             enable_bump_event                  );
			Client._exchange(ExchangeMode, AW_WORLD_ENABLE_SYNC_EVENTS,            enable_sync_events                 );
			Client._exchange(ExchangeMode, AW_WORLD_FOG_ENABLE,                    fog_enable                         );
			Client._exchange(ExchangeMode, AW_WORLD_FOG_MINIMUM,                   fog_minimum                        );
			Client._exchange(ExchangeMode, AW_WORLD_FOG_MAXIMUM,                   fog_maximum                        );
			Client._exchange(ExchangeMode, AW_WORLD_FOG_TINTED,                    fog_tinted                         );
			Client._exchange(ExchangeMode, AW_WORLD_FOG_RED,                       ctw(fog)                           );
			Client._exchange(ExchangeMode, AW_WORLD_MINIMUM_VISIBILITY,            minimum_visibility                 );
			Client._exchange(ExchangeMode, AW_WORLD_LIGHT_X,                       light_x                            );
			Client._exchange(ExchangeMode, AW_WORLD_LIGHT_Y,                       light_y                            );
			Client._exchange(ExchangeMode, AW_WORLD_LIGHT_Z,                       light_z                            );
			Client._exchange(ExchangeMode, AW_WORLD_LIGHT_RED,                     ctw(light)                         );
			Client._exchange(ExchangeMode, AW_WORLD_AMBIENT_LIGHT_RED,             ctw(ambient_light)                 );
			Client._exchange(ExchangeMode, AW_WORLD_LIGHT_TEXTURE,                 light_texture                      );
			Client._exchange(ExchangeMode, AW_WORLD_LIGHT_MASK,                    light_mask                         );
			Client._exchange(ExchangeMode, AW_WORLD_LIGHT_DRAW_SIZE,               light_draw_size                    );
			Client._exchange(ExchangeMode, AW_WORLD_LIGHT_DRAW_FRONT,              light_draw_front                   );
			Client._exchange(ExchangeMode, AW_WORLD_LIGHT_DRAW_BRIGHT,             light_draw_bright                  );
			Client._exchange(ExchangeMode, AW_WORLD_LIGHT_SOURCE_USE_COLOR,        light_source_use_color             );
			Client._exchange(ExchangeMode, AW_WORLD_LIGHT_SOURCE_COLOR,            light_source_color                 .value);
			Client._exchange(ExchangeMode, AW_WORLD_DISABLE_SHADOWS,               disable_shadows                    );
			Client._exchange(ExchangeMode, AW_WORLD_ENTRY_POINT,                   entry_point                        );
			Client._exchange(ExchangeMode, AW_WORLD_ALLOW_PASSTHRU,                allow_passthru                     );
			Client._exchange(ExchangeMode, AW_WORLD_ALLOW_FLYING,                  allow_flying                       );
			Client._exchange(ExchangeMode, AW_WORLD_ALLOW_TELEPORT,                allow_teleport                     );
			Client._exchange(ExchangeMode, AW_WORLD_ALLOW_AVATAR_COLLISION,        allow_avatar_collision             );
			Client._exchange(ExchangeMode, AW_WORLD_GRAVITY,                       gravity                            );
			Client._exchange(ExchangeMode, AW_WORLD_FRICTION,                      friction                           );
			Client._exchange(ExchangeMode, AW_WORLD_SLOPESLIDE_ENABLED,            slopeslide_enabled                 );
			Client._exchange(ExchangeMode, AW_WORLD_SLOPESLIDE_MIN_ANGLE,          slopeslide_min_angle               );
			Client._exchange(ExchangeMode, AW_WORLD_SLOPESLIDE_MAX_ANGLE,          slopeslide_max_angle               );
			Client._exchange(ExchangeMode, AW_WORLD_ENABLE_CAMERA_COLLISION,       enable_camera_collision            );
			Client._exchange(ExchangeMode, AW_WORLD_CAMERA_ZOOM,                   camera_zoom                        );
			Client._exchange(ExchangeMode, AW_WORLD_BACKDROP,                      backdrop                           );
			Client._exchange(ExchangeMode, AW_WORLD_SKYBOX,                        skybox                             );
			Client._exchange(ExchangeMode, AW_WORLD_SKY_NORTH_RED,                 ctw(sky_north)                     );
			Client._exchange(ExchangeMode, AW_WORLD_SKY_SOUTH_RED,                 ctw(sky_south)                     );
			Client._exchange(ExchangeMode, AW_WORLD_SKY_EAST_RED,                  ctw(sky_east)                      );
			Client._exchange(ExchangeMode, AW_WORLD_SKY_WEST_RED,                  ctw(sky_west)                      );
			Client._exchange(ExchangeMode, AW_WORLD_SKY_TOP_RED,                   ctw(sky_top)                       );
			Client._exchange(ExchangeMode, AW_WORLD_SKY_BOTTOM_RED,                ctw(sky_bottom)                    );
			Client._exchange(ExchangeMode, AW_WORLD_CLOUDS_LAYER1_TEXTURE,         clouds_layer1_texture              );
			Client._exchange(ExchangeMode, AW_WORLD_CLOUDS_LAYER1_MASK,            clouds_layer1_mask                 );
			Client._exchange(ExchangeMode, AW_WORLD_CLOUDS_LAYER1_TILE,            clouds_layer1_tile                 );
			Client._exchange(ExchangeMode, AW_WORLD_CLOUDS_LAYER1_SPEED_X,         clouds_layer1_speed_x              );
			Client._exchange(ExchangeMode, AW_WORLD_CLOUDS_LAYER1_SPEED_Z,         clouds_layer1_speed_z              );
			Client._exchange(ExchangeMode, AW_WORLD_CLOUDS_LAYER1_OPACITY,         clouds_layer1_opacity              );
			Client._exchange(ExchangeMode, AW_WORLD_CLOUDS_LAYER2_TEXTURE,         clouds_layer2_texture              );
			Client._exchange(ExchangeMode, AW_WORLD_CLOUDS_LAYER2_MASK,            clouds_layer2_mask                 );
			Client._exchange(ExchangeMode, AW_WORLD_CLOUDS_LAYER2_TILE,            clouds_layer2_tile                 );
			Client._exchange(ExchangeMode, AW_WORLD_CLOUDS_LAYER2_SPEED_X,         clouds_layer2_speed_x              );
			Client._exchange(ExchangeMode, AW_WORLD_CLOUDS_LAYER2_SPEED_Z,         clouds_layer2_speed_z              );
			Client._exchange(ExchangeMode, AW_WORLD_CLOUDS_LAYER2_OPACITY,         clouds_layer2_opacity              );
			Client._exchange(ExchangeMode, AW_WORLD_CLOUDS_LAYER3_TEXTURE,         clouds_layer3_texture              );
			Client._exchange(ExchangeMode, AW_WORLD_CLOUDS_LAYER3_MASK,            clouds_layer3_mask                 );
			Client._exchange(ExchangeMode, AW_WORLD_CLOUDS_LAYER3_TILE,            clouds_layer3_tile                 );
			Client._exchange(ExchangeMode, AW_WORLD_CLOUDS_LAYER3_SPEED_X,         clouds_layer3_speed_x              );
			Client._exchange(ExchangeMode, AW_WORLD_CLOUDS_LAYER3_SPEED_Z,         clouds_layer3_speed_z              );
			Client._exchange(ExchangeMode, AW_WORLD_CLOUDS_LAYER3_OPACITY,         clouds_layer3_opacity              );
			Client._exchange(ExchangeMode, AW_WORLD_DISABLE_CHAT,                  disable_chat                       );
			Client._exchange(ExchangeMode, AW_WORLD_ALLOW_CITIZEN_WHISPER,         allow_citizen_whisper              );
			Client._exchange(ExchangeMode, AW_WORLD_ALLOW_TOURIST_WHISPER,         allow_tourist_whisper              );
			Client._exchange(ExchangeMode, AW_WORLD_ALWAYS_SHOW_NAMES,             always_show_names                  );
			Client._exchange(ExchangeMode, AW_WORLD_DISABLE_AVATAR_LIST,           disable_avatar_list                );
			Client._exchange(ExchangeMode, AW_WORLD_AVATAR_REFRESH_RATE,           avatar_refresh_rate                );
			Client._exchange(ExchangeMode, AW_WORLD_ENABLE_CAV,                    enable_cav                         );
			Client._exchange(ExchangeMode, AW_WORLD_ENABLE_PAV,                    enable_pav                         );
			Client._exchange(ExchangeMode, AW_WORLD_CHAT_DISABLE_URL_CLICKS,       chat_disable_url_clicks            );
			Client._exchange(ExchangeMode, AW_WORLD_DISABLE_MULTIPLE_MEDIA,        disable_multiple_media             );
			Client._exchange(ExchangeMode, AW_WORLD_BOTMENU_URL,                   botmenu_url                        );
			Client._exchange(ExchangeMode, AW_WORLD_VOIP_CONFERENCE_GLOBAL,        voip_conference_global             );
			Client._exchange(ExchangeMode, AW_WORLD_VOIP_MODERATE_GLOBAL,          voip_moderate_global               );
			Client._exchange(ExchangeMode, AW_WORLD_WATER_ENABLED,                 water_enabled                      );
			Client._exchange(ExchangeMode, AW_WORLD_WATER_TEXTURE,                 water_texture                      );
			Client._exchange(ExchangeMode, AW_WORLD_WATER_MASK,                    water_mask                         );
			Client._exchange(ExchangeMode, AW_WORLD_WATER_BOTTOM_TEXTURE,          water_bottom_texture               );
			Client._exchange(ExchangeMode, AW_WORLD_WATER_BOTTOM_MASK,             water_bottom_mask                  );
			Client._exchange(ExchangeMode, AW_WORLD_WATER_OPACITY,                 water_opacity                      );
			Client._exchange(ExchangeMode, AW_WORLD_WATER_VISIBILITY,              water_visibility                   );
			Client._exchange(ExchangeMode, AW_WORLD_WATER_RED,                     ctw(water)                         );
			Client._exchange(ExchangeMode, AW_WORLD_WATER_LEVEL,                   water_level                        );
			Client._exchange(ExchangeMode, AW_WORLD_WATER_SURFACE_MOVE,            water_surface_move                 );
			Client._exchange(ExchangeMode, AW_WORLD_WATER_WAVE_MOVE,               water_wave_move                    );
			Client._exchange(ExchangeMode, AW_WORLD_WATER_SPEED,                   water_speed                        );
			Client._exchange(ExchangeMode, AW_WORLD_BUOYANCY,                      buoyancy                           );
			Client._exchange(ExchangeMode, AW_WORLD_WATER_FRICTION,                water_friction                     );
			Client._exchange(ExchangeMode, AW_WORLD_WATER_UNDER_TERRAIN,           water_under_terrain                );
			Client._exchange(ExchangeMode, AW_WORLD_SOUND_FOOTSTEP,                sound_footstep                     );
			Client._exchange(ExchangeMode, AW_WORLD_SOUND_WATER_ENTER,             sound_water_enter                  );
			Client._exchange(ExchangeMode, AW_WORLD_SOUND_WATER_EXIT,              sound_water_exit                   );
			Client._exchange(ExchangeMode, AW_WORLD_SOUND_AMBIENT,                 sound_ambient                      );

		}
		
		// write the ini file to disk
		errno_t Attributes::write_file(String FilePath)
		{
			// create parser io object
			AW::EnhancedINI ini;
			
			// add versioning information
			ini.Write("__header__", "type",		"{xe2-world-attdump}");
			ini.Write("__header__", "version",	3);
			ini.Write("__header__", "date",		(int)time(NULL));

			dx_file(ini, AW::WRITE_TO_INI);
			return ini.Save(FilePath);
		}
		
		// read the ini file from disk
		errno_t	Attributes::read_file(String FilePath)
		{
			// create parser io object
			AW::EnhancedINI ini;
			if (ini.Load(FilePath) == false)
				return 1;
				
			// copy from file to sdk
			dx_file(ini, AW::READ_FROM_INI);
			return 0;
		}
		
		// exchange data with the file
		void Attributes::dx_file(AW::EnhancedINI& INI, bool ExchangeMode)
		{
		    const char* attributes_table = "attributes";
			INI.Exchange(ExchangeMode, attributes_table, "enter_right",                          enter_right                             );
			INI.Exchange(ExchangeMode, attributes_table, "build_right",                          build_right                             );
			INI.Exchange(ExchangeMode, attributes_table, "voip_right",                           voip_right                              );
			INI.Exchange(ExchangeMode, attributes_table, "eminent_domain_right",                 eminent_domain_right                    );
			INI.Exchange(ExchangeMode, attributes_table, "special_objects_right",                special_objects_right                   );
			INI.Exchange(ExchangeMode, attributes_table, "public_speaker_right",                 public_speaker_right                    );
			INI.Exchange(ExchangeMode, attributes_table, "bots_right",                           bots_right                              );
			INI.Exchange(ExchangeMode, attributes_table, "eject_right",                          eject_right                             );
			INI.Exchange(ExchangeMode, attributes_table, "speak_right",                          speak_right                             );
			INI.Exchange(ExchangeMode, attributes_table, "special_commands_right",               special_commands_right                  );
			INI.Exchange(ExchangeMode, attributes_table, "special_commands",                     special_commands                        );
			INI.Exchange(ExchangeMode, attributes_table, "v4_objects_right",                     v4_objects_right                        );
			INI.Exchange(ExchangeMode, attributes_table, "terrain_right",                        terrain_right                           );
			INI.Exchange(ExchangeMode, attributes_table, "title",                                title                                   );
			INI.Exchange(ExchangeMode, attributes_table, "home_page",                            home_page                               );
			INI.Exchange(ExchangeMode, attributes_table, "rating",                               rating                                  );
			INI.Exchange(ExchangeMode, attributes_table, "welcome_message",                      welcome_message                         );
			INI.Exchange(ExchangeMode, attributes_table, "keywords",                             keywords                                );
			INI.Exchange(ExchangeMode, attributes_table, "ground",                               ground                                  );
			INI.Exchange(ExchangeMode, attributes_table, "repeating_ground",                     repeating_ground                        );
			INI.Exchange(ExchangeMode, attributes_table, "enable_terrain",                       enable_terrain                          );
			INI.Exchange(ExchangeMode, attributes_table, "terrain_ambient",                      terrain_ambient                         );
			INI.Exchange(ExchangeMode, attributes_table, "terrain_diffuse",                      terrain_diffuse                         );
			INI.Exchange(ExchangeMode, attributes_table, "terrain_offset",                       terrain_offset                          );
			INI.Exchange(ExchangeMode, attributes_table, "object_path",                          object_path                             );
			INI.Exchange(ExchangeMode, attributes_table, "object_refresh",                       object_refresh                          );
			INI.Exchange(ExchangeMode, attributes_table, "object_password",                      object_password                         );
			INI.Exchange(ExchangeMode, attributes_table, "enable_referer",                       enable_referer                          );
			INI.Exchange(ExchangeMode, attributes_table, "cav_object_path",                      cav_object_path                         );
			INI.Exchange(ExchangeMode, attributes_table, "cav_object_password",                  cav_object_password                     );
			INI.Exchange(ExchangeMode, attributes_table, "cav_object_refresh",                   cav_object_refresh                      );
			INI.Exchange(ExchangeMode, attributes_table, "cell_limit",                           cell_limit                              );
			INI.Exchange(ExchangeMode, attributes_table, "restricted_radius",                    restricted_radius                       );
			INI.Exchange(ExchangeMode, attributes_table, "max_light_radius",                     max_light_radius                        );
			INI.Exchange(ExchangeMode, attributes_table, "disable_create_url",                   disable_create_url                      );
			INI.Exchange(ExchangeMode, attributes_table, "allow_object_select",                  allow_object_select                     );
			INI.Exchange(ExchangeMode, attributes_table, "allow_3_axis_rotation",                allow_3_axis_rotation                   );
			INI.Exchange(ExchangeMode, attributes_table, "allow_tourist_build",                  allow_tourist_build                     );
			INI.Exchange(ExchangeMode, attributes_table, "mover_empty_reset_timeout",            mover_empty_reset_timeout               );
			INI.Exchange(ExchangeMode, attributes_table, "mover_used_reset_timeout",             mover_used_reset_timeout                );
			INI.Exchange(ExchangeMode, attributes_table, "enable_bump_event",                    enable_bump_event                       );
			INI.Exchange(ExchangeMode, attributes_table, "enable_sync_events",                   enable_sync_events                      );
			INI.Exchange(ExchangeMode, attributes_table, "fog_enable",                           fog_enable                              );
			INI.Exchange(ExchangeMode, attributes_table, "fog_minimum",                          fog_minimum                             );
			INI.Exchange(ExchangeMode, attributes_table, "fog_maximum",                          fog_maximum                             );
			INI.Exchange(ExchangeMode, attributes_table, "fog_tinted",                           fog_tinted                              );
			INI.Exchange(ExchangeMode, attributes_table, "fog",                                  fog.value                               );
			INI.Exchange(ExchangeMode, attributes_table, "minimum_visibility",                   minimum_visibility                      );
			INI.Exchange(ExchangeMode, attributes_table, "light_x",                              light_x                                 );
			INI.Exchange(ExchangeMode, attributes_table, "light_y",                              light_y                                 );
			INI.Exchange(ExchangeMode, attributes_table, "light_z",                              light_z                                 );
			INI.Exchange(ExchangeMode, attributes_table, "light",                                light.value                             );
			INI.Exchange(ExchangeMode, attributes_table, "ambient_light",                        ambient_light.value                     );
			INI.Exchange(ExchangeMode, attributes_table, "light_texture",                        light_texture                           );
			INI.Exchange(ExchangeMode, attributes_table, "light_mask",                           light_mask                              );
			INI.Exchange(ExchangeMode, attributes_table, "light_draw_size",                      light_draw_size                         );
			INI.Exchange(ExchangeMode, attributes_table, "light_draw_front",                     light_draw_front                        );
			INI.Exchange(ExchangeMode, attributes_table, "light_draw_bright",                    light_draw_bright                       );
			INI.Exchange(ExchangeMode, attributes_table, "light_source_use_color",               light_source_use_color                  );
			INI.Exchange(ExchangeMode, attributes_table, "light_source_color",                   light_source_color.value                );
			INI.Exchange(ExchangeMode, attributes_table, "disable_shadows",                      disable_shadows                         );
			INI.Exchange(ExchangeMode, attributes_table, "entry_point",                          entry_point                             );
			INI.Exchange(ExchangeMode, attributes_table, "allow_passthru",                       allow_passthru                          );
			INI.Exchange(ExchangeMode, attributes_table, "allow_flying",                         allow_flying                            );
			INI.Exchange(ExchangeMode, attributes_table, "allow_teleport",                       allow_teleport                          );
			INI.Exchange(ExchangeMode, attributes_table, "allow_avatar_collision",               allow_avatar_collision                  );
			INI.Exchange(ExchangeMode, attributes_table, "gravity",                              gravity                                 );
			INI.Exchange(ExchangeMode, attributes_table, "friction",                             friction                                );
			INI.Exchange(ExchangeMode, attributes_table, "slopeslide_enabled",                   slopeslide_enabled                      );
			INI.Exchange(ExchangeMode, attributes_table, "slopeslide_min_angle",                 slopeslide_min_angle                    );
			INI.Exchange(ExchangeMode, attributes_table, "slopeslide_max_angle",                 slopeslide_max_angle                    );
			INI.Exchange(ExchangeMode, attributes_table, "enable_camera_collision",              enable_camera_collision                 );
			INI.Exchange(ExchangeMode, attributes_table, "camera_zoom",                          camera_zoom                             );
			INI.Exchange(ExchangeMode, attributes_table, "backdrop",                             backdrop                                );
			INI.Exchange(ExchangeMode, attributes_table, "skybox",                               skybox                                  );
			INI.Exchange(ExchangeMode, attributes_table, "sky_north",                            sky_north.value                         );
			INI.Exchange(ExchangeMode, attributes_table, "sky_south",                            sky_south.value                         );
			INI.Exchange(ExchangeMode, attributes_table, "sky_east",                             sky_east.value                          );
			INI.Exchange(ExchangeMode, attributes_table, "sky_west",                             sky_west.value                          );
			INI.Exchange(ExchangeMode, attributes_table, "sky_top",                              sky_top.value                           );
			INI.Exchange(ExchangeMode, attributes_table, "sky_bottom",                           sky_bottom.value                        );
			INI.Exchange(ExchangeMode, attributes_table, "clouds_layer1_texture",                clouds_layer1_texture                   );
			INI.Exchange(ExchangeMode, attributes_table, "clouds_layer1_mask",                   clouds_layer1_mask                      );
			INI.Exchange(ExchangeMode, attributes_table, "clouds_layer1_tile",                   clouds_layer1_tile                      );
			INI.Exchange(ExchangeMode, attributes_table, "clouds_layer1_speed_x",                clouds_layer1_speed_x                   );
			INI.Exchange(ExchangeMode, attributes_table, "clouds_layer1_speed_z",                clouds_layer1_speed_z                   );
			INI.Exchange(ExchangeMode, attributes_table, "clouds_layer1_opacity",                clouds_layer1_opacity                   );
			INI.Exchange(ExchangeMode, attributes_table, "clouds_layer2_texture",                clouds_layer2_texture                   );
			INI.Exchange(ExchangeMode, attributes_table, "clouds_layer2_mask",                   clouds_layer2_mask                      );
			INI.Exchange(ExchangeMode, attributes_table, "clouds_layer2_tile",                   clouds_layer2_tile                      );
			INI.Exchange(ExchangeMode, attributes_table, "clouds_layer2_speed_x",                clouds_layer2_speed_x                   );
			INI.Exchange(ExchangeMode, attributes_table, "clouds_layer2_speed_z",                clouds_layer2_speed_z                   );
			INI.Exchange(ExchangeMode, attributes_table, "clouds_layer2_opacity",                clouds_layer2_opacity                   );
			INI.Exchange(ExchangeMode, attributes_table, "clouds_layer3_texture",                clouds_layer3_texture                   );
			INI.Exchange(ExchangeMode, attributes_table, "clouds_layer3_mask",                   clouds_layer3_mask                      );
			INI.Exchange(ExchangeMode, attributes_table, "clouds_layer3_tile",                   clouds_layer3_tile                      );
			INI.Exchange(ExchangeMode, attributes_table, "clouds_layer3_speed_x",                clouds_layer3_speed_x                   );
			INI.Exchange(ExchangeMode, attributes_table, "clouds_layer3_speed_z",                clouds_layer3_speed_z                   );
			INI.Exchange(ExchangeMode, attributes_table, "clouds_layer3_opacity",                clouds_layer3_opacity                   );
			INI.Exchange(ExchangeMode, attributes_table, "disable_chat",                         disable_chat                            );
			INI.Exchange(ExchangeMode, attributes_table, "allow_citizen_whisper",                allow_citizen_whisper                   );
			INI.Exchange(ExchangeMode, attributes_table, "allow_tourist_whisper",                allow_tourist_whisper                   );
			INI.Exchange(ExchangeMode, attributes_table, "always_show_names",                    always_show_names                       );
			INI.Exchange(ExchangeMode, attributes_table, "disable_avatar_list",                  disable_avatar_list                     );
			INI.Exchange(ExchangeMode, attributes_table, "avatar_refresh_rate",                  avatar_refresh_rate                     );
			INI.Exchange(ExchangeMode, attributes_table, "enable_cav",                           enable_cav                              );
			INI.Exchange(ExchangeMode, attributes_table, "enable_pav",                           enable_pav                              );
			INI.Exchange(ExchangeMode, attributes_table, "chat_disable_url_clicks",              chat_disable_url_clicks                 );
			INI.Exchange(ExchangeMode, attributes_table, "disable_multiple_media",               disable_multiple_media                  );
			INI.Exchange(ExchangeMode, attributes_table, "botmenu_url",                          botmenu_url                             );
			INI.Exchange(ExchangeMode, attributes_table, "voip_conference_global",               voip_conference_global                  );
			INI.Exchange(ExchangeMode, attributes_table, "voip_moderate_global",                 voip_moderate_global                    );
			INI.Exchange(ExchangeMode, attributes_table, "water_enabled",                        water_enabled                           );
			INI.Exchange(ExchangeMode, attributes_table, "water_texture",                        water_texture                           );
			INI.Exchange(ExchangeMode, attributes_table, "water_mask",                           water_mask                              );
			INI.Exchange(ExchangeMode, attributes_table, "water_bottom_texture",                 water_bottom_texture                    );
			INI.Exchange(ExchangeMode, attributes_table, "water_bottom_mask",                    water_bottom_mask                       );
			INI.Exchange(ExchangeMode, attributes_table, "water_opacity",                        water_opacity                           );
			INI.Exchange(ExchangeMode, attributes_table, "water_visibility",                     water_visibility                        );
			INI.Exchange(ExchangeMode, attributes_table, "water",                                water.value                             );
			INI.Exchange(ExchangeMode, attributes_table, "water_level",                          water_level                             );
			INI.Exchange(ExchangeMode, attributes_table, "water_surface_move",                   water_surface_move                      );
			INI.Exchange(ExchangeMode, attributes_table, "water_wave_move",                      water_wave_move                         );
			INI.Exchange(ExchangeMode, attributes_table, "water_speed",                          water_speed                             );
			INI.Exchange(ExchangeMode, attributes_table, "buoyancy",                             buoyancy                                );
			INI.Exchange(ExchangeMode, attributes_table, "water_friction",                       water_friction                          );
			INI.Exchange(ExchangeMode, attributes_table, "water_under_terrain",                  water_under_terrain                     );
			INI.Exchange(ExchangeMode, attributes_table, "sound_footstep",                       sound_footstep                          );
			INI.Exchange(ExchangeMode, attributes_table, "sound_water_enter",                    sound_water_enter                       );
			INI.Exchange(ExchangeMode, attributes_table, "sound_water_exit",                     sound_water_exit                        );
			INI.Exchange(ExchangeMode, attributes_table, "sound_ambient",                        sound_ambient                           );

		}
		
		// compare the whole set in a float aware case
		bool Attributes::attributes_equal(const Attributes& Attr)
		{
			if (enter_right != Attr.enter_right) return false;
			if (build_right != Attr.build_right) return false;
			if (voip_right != Attr.voip_right) return false;
			if (eminent_domain_right != Attr.eminent_domain_right) return false;
			if (special_objects_right != Attr.special_objects_right) return false;
			if (public_speaker_right != Attr.public_speaker_right) return false;
			if (bots_right != Attr.bots_right) return false;
			if (eject_right != Attr.eject_right) return false;
			if (speak_right != Attr.speak_right) return false;
			if (special_commands_right != Attr.special_commands_right) return false;
			if (special_commands != Attr.special_commands) return false;
			if (v4_objects_right != Attr.v4_objects_right) return false;
			if (terrain_right != Attr.terrain_right) return false;
			if (title != Attr.title) return false;
			if (home_page != Attr.home_page) return false;
			if (rating != Attr.rating) return false;
			if (welcome_message != Attr.welcome_message) return false;
			if (keywords != Attr.keywords) return false;
			if (ground != Attr.ground) return false;
			if (repeating_ground != Attr.repeating_ground) return false;
			if (enable_terrain != Attr.enable_terrain) return false;
			if (fabs(terrain_ambient - Attr.terrain_ambient) > 0.0001) return false;
			if (fabs(terrain_diffuse - Attr.terrain_diffuse) > 0.0001) return false;
			if (fabs(terrain_offset - Attr.terrain_offset) > 0.0001) return false;
			if (object_path != Attr.object_path) return false;
			if (object_refresh != Attr.object_refresh) return false;
			if (object_password != Attr.object_password) return false;
			if (enable_referer != Attr.enable_referer) return false;
			if (cav_object_path != Attr.cav_object_path) return false;
			if (cav_object_password != Attr.cav_object_password) return false;
			if (cav_object_refresh != Attr.cav_object_refresh) return false;
			if (cell_limit != Attr.cell_limit) return false;
			if (restricted_radius != Attr.restricted_radius) return false;
			if (max_light_radius != Attr.max_light_radius) return false;
			if (disable_create_url != Attr.disable_create_url) return false;
			if (allow_object_select != Attr.allow_object_select) return false;
			if (allow_3_axis_rotation != Attr.allow_3_axis_rotation) return false;
			if (allow_tourist_build != Attr.allow_tourist_build) return false;
			if (mover_empty_reset_timeout != Attr.mover_empty_reset_timeout) return false;
			if (mover_used_reset_timeout != Attr.mover_used_reset_timeout) return false;
			if (enable_bump_event != Attr.enable_bump_event) return false;
			if (enable_sync_events != Attr.enable_sync_events) return false;
			if (fog_enable != Attr.fog_enable) return false;
			if (fog_minimum != Attr.fog_minimum) return false;
			if (fog_maximum != Attr.fog_maximum) return false;
			if (fog_tinted != Attr.fog_tinted) return false;
			if (fog.value != Attr.fog.value) return false;
			if (minimum_visibility != Attr.minimum_visibility) return false;
			if (fabs(light_x - Attr.light_x) > 0.0001) return false;
			if (fabs(light_y - Attr.light_y) > 0.0001) return false;
			if (fabs(light_z - Attr.light_z) > 0.0001) return false;
			if (light.value != Attr.light.value) return false;
			if (ambient_light.value != Attr.ambient_light.value) return false;
			if (light_texture != Attr.light_texture) return false;
			if (light_mask != Attr.light_mask) return false;
			if (light_draw_size != Attr.light_draw_size) return false;
			if (light_draw_front != Attr.light_draw_front) return false;
			if (light_draw_bright != Attr.light_draw_bright) return false;
			if (light_source_use_color != Attr.light_source_use_color) return false;
			if (light_source_color.value != Attr.light_source_color.value) return false;
			if (disable_shadows != Attr.disable_shadows) return false;
			if (entry_point != Attr.entry_point) return false;
			if (allow_passthru != Attr.allow_passthru) return false;
			if (allow_flying != Attr.allow_flying) return false;
			if (allow_teleport != Attr.allow_teleport) return false;
			if (allow_avatar_collision != Attr.allow_avatar_collision) return false;
			if (fabs(gravity - Attr.gravity) > 0.0001) return false;
			if (fabs(friction - Attr.friction) > 0.0001) return false;
			if (slopeslide_enabled != Attr.slopeslide_enabled) return false;
			if (fabs(slopeslide_min_angle - Attr.slopeslide_min_angle) > 0.0001) return false;
			if (fabs(slopeslide_max_angle - Attr.slopeslide_max_angle) > 0.0001) return false;
			if (enable_camera_collision != Attr.enable_camera_collision) return false;
			if (fabs(camera_zoom - Attr.camera_zoom) > 0.0001) return false;
			if (backdrop != Attr.backdrop) return false;
			if (skybox != Attr.skybox) return false;
			if (sky_north.value != Attr.sky_north.value) return false;
			if (sky_south.value != Attr.sky_south.value) return false;
			if (sky_east.value != Attr.sky_east.value) return false;
			if (sky_west.value != Attr.sky_west.value) return false;
			if (sky_top.value != Attr.sky_top.value) return false;
			if (sky_bottom.value != Attr.sky_bottom.value) return false;
			if (clouds_layer1_texture != Attr.clouds_layer1_texture) return false;
			if (clouds_layer1_mask != Attr.clouds_layer1_mask) return false;
			if (fabs(clouds_layer1_tile - Attr.clouds_layer1_tile) > 0.0001) return false;
			if (fabs(clouds_layer1_speed_x - Attr.clouds_layer1_speed_x) > 0.0001) return false;
			if (fabs(clouds_layer1_speed_z - Attr.clouds_layer1_speed_z) > 0.0001) return false;
			if (clouds_layer1_opacity != Attr.clouds_layer1_opacity) return false;
			if (clouds_layer2_texture != Attr.clouds_layer2_texture) return false;
			if (clouds_layer2_mask != Attr.clouds_layer2_mask) return false;
			if (fabs(clouds_layer2_tile - Attr.clouds_layer2_tile) > 0.0001) return false;
			if (fabs(clouds_layer2_speed_x - Attr.clouds_layer2_speed_x) > 0.0001) return false;
			if (fabs(clouds_layer2_speed_z - Attr.clouds_layer2_speed_z) > 0.0001) return false;
			if (clouds_layer2_opacity != Attr.clouds_layer2_opacity) return false;
			if (clouds_layer3_texture != Attr.clouds_layer3_texture) return false;
			if (clouds_layer3_mask != Attr.clouds_layer3_mask) return false;
			if (fabs(clouds_layer3_tile - Attr.clouds_layer3_tile) > 0.0001) return false;
			if (fabs(clouds_layer3_speed_x - Attr.clouds_layer3_speed_x) > 0.0001) return false;
			if (fabs(clouds_layer3_speed_z - Attr.clouds_layer3_speed_z) > 0.0001) return false;
			if (clouds_layer3_opacity != Attr.clouds_layer3_opacity) return false;
			if (disable_chat != Attr.disable_chat) return false;
			if (allow_citizen_whisper != Attr.allow_citizen_whisper) return false;
			if (allow_tourist_whisper != Attr.allow_tourist_whisper) return false;
			if (always_show_names != Attr.always_show_names) return false;
			if (disable_avatar_list != Attr.disable_avatar_list) return false;
			if (avatar_refresh_rate != Attr.avatar_refresh_rate) return false;
			if (enable_cav != Attr.enable_cav) return false;
			if (enable_pav != Attr.enable_pav) return false;
			if (chat_disable_url_clicks != Attr.chat_disable_url_clicks) return false;
			if (disable_multiple_media != Attr.disable_multiple_media) return false;
			if (botmenu_url != Attr.botmenu_url) return false;
			if (voip_conference_global != Attr.voip_conference_global) return false;
			if (voip_moderate_global != Attr.voip_moderate_global) return false;
			if (water_enabled != Attr.water_enabled) return false;
			if (water_texture != Attr.water_texture) return false;
			if (water_mask != Attr.water_mask) return false;
			if (water_bottom_texture != Attr.water_bottom_texture) return false;
			if (water_bottom_mask != Attr.water_bottom_mask) return false;
			if (water_opacity != Attr.water_opacity) return false;
			if (water_visibility != Attr.water_visibility) return false;
			if (water.value != Attr.water.value) return false;
			if (fabs(water_level - Attr.water_level) > 0.0001) return false;
			if (fabs(water_surface_move - Attr.water_surface_move) > 0.0001) return false;
			if (fabs(water_wave_move - Attr.water_wave_move) > 0.0001) return false;
			if (fabs(water_speed - Attr.water_speed) > 0.0001) return false;
			if (fabs(buoyancy - Attr.buoyancy) > 0.0001) return false;
			if (fabs(water_friction - Attr.water_friction) > 0.0001) return false;
			if (water_under_terrain != Attr.water_under_terrain) return false;
			if (sound_footstep != Attr.sound_footstep) return false;
			if (sound_water_enter != Attr.sound_water_enter) return false;
			if (sound_water_exit != Attr.sound_water_exit) return false;
			if (sound_ambient != Attr.sound_ambient) return false;

			return true;
		}
		
		// interpolate between 2 sets of values
		void Attributes::interpolate(Attributes& Old, Attributes& New, float Prog)
		{
			enter_right = interpolate_string(Old.enter_right, New.enter_right, Prog);
			build_right = interpolate_string(Old.build_right, New.build_right, Prog);
			voip_right = interpolate_string(Old.voip_right, New.voip_right, Prog);
			eminent_domain_right = interpolate_string(Old.eminent_domain_right, New.eminent_domain_right, Prog);
			special_objects_right = interpolate_string(Old.special_objects_right, New.special_objects_right, Prog);
			public_speaker_right = interpolate_string(Old.public_speaker_right, New.public_speaker_right, Prog);
			bots_right = interpolate_string(Old.bots_right, New.bots_right, Prog);
			eject_right = interpolate_string(Old.eject_right, New.eject_right, Prog);
			speak_right = interpolate_string(Old.speak_right, New.speak_right, Prog);
			special_commands_right = interpolate_string(Old.special_commands_right, New.special_commands_right, Prog);
			special_commands = interpolate_string(Old.special_commands, New.special_commands, Prog);
			v4_objects_right = interpolate_string(Old.v4_objects_right, New.v4_objects_right, Prog);
			terrain_right = interpolate_string(Old.terrain_right, New.terrain_right, Prog);
			title = interpolate_string(Old.title, New.title, Prog);
			home_page = interpolate_string(Old.home_page, New.home_page, Prog);
			rating = interpolate_int(Old.rating, New.rating, Prog);
			welcome_message = interpolate_string(Old.welcome_message, New.welcome_message, Prog);
			keywords = interpolate_string(Old.keywords, New.keywords, Prog);
			ground = interpolate_string(Old.ground, New.ground, Prog);
			// unspecified repeating_ground
			// unspecified enable_terrain
			terrain_ambient = interpolate_float(Old.terrain_ambient, New.terrain_ambient, Prog);
			terrain_diffuse = interpolate_float(Old.terrain_diffuse, New.terrain_diffuse, Prog);
			terrain_offset = interpolate_float(Old.terrain_offset, New.terrain_offset, Prog);
			object_path = interpolate_string(Old.object_path, New.object_path, Prog);
			object_refresh = interpolate_int(Old.object_refresh, New.object_refresh, Prog);
			object_password = interpolate_string(Old.object_password, New.object_password, Prog);
			// unspecified enable_referer
			cav_object_path = interpolate_string(Old.cav_object_path, New.cav_object_path, Prog);
			cav_object_password = interpolate_string(Old.cav_object_password, New.cav_object_password, Prog);
			cav_object_refresh = interpolate_string(Old.cav_object_refresh, New.cav_object_refresh, Prog);
			cell_limit = interpolate_int(Old.cell_limit, New.cell_limit, Prog);
			restricted_radius = interpolate_int(Old.restricted_radius, New.restricted_radius, Prog);
			max_light_radius = interpolate_int(Old.max_light_radius, New.max_light_radius, Prog);
			// unspecified disable_create_url
			// unspecified allow_object_select
			// unspecified allow_3_axis_rotation
			// unspecified allow_tourist_build
			mover_empty_reset_timeout = interpolate_int(Old.mover_empty_reset_timeout, New.mover_empty_reset_timeout, Prog);
			mover_used_reset_timeout = interpolate_int(Old.mover_used_reset_timeout, New.mover_used_reset_timeout, Prog);
			// unspecified enable_bump_event
			// unspecified enable_sync_events
			// unspecified fog_enable
			fog_minimum = interpolate_int(Old.fog_minimum, New.fog_minimum, Prog);
			fog_maximum = interpolate_int(Old.fog_maximum, New.fog_maximum, Prog);
			// unspecified fog_tinted
			fog = interpolate_rgb(Old.fog.getColour(), New.fog.getColour(), Prog);
			minimum_visibility = interpolate_int(Old.minimum_visibility, New.minimum_visibility, Prog);
			light_x = interpolate_float(Old.light_x, New.light_x, Prog);
			light_y = interpolate_float(Old.light_y, New.light_y, Prog);
			light_z = interpolate_float(Old.light_z, New.light_z, Prog);
			light = interpolate_rgb(Old.light.getColour(), New.light.getColour(), Prog);
			ambient_light = interpolate_rgb(Old.ambient_light.getColour(), New.ambient_light.getColour(), Prog);
			light_texture = interpolate_string(Old.light_texture, New.light_texture, Prog);
			light_mask = interpolate_string(Old.light_mask, New.light_mask, Prog);
			light_draw_size = interpolate_int(Old.light_draw_size, New.light_draw_size, Prog);
			// unspecified light_draw_front
			// unspecified light_draw_bright
			// unspecified light_source_use_color
			light_source_color = interpolate_rgb(Old.light_source_color.getColour(), New.light_source_color.getColour(), Prog);
			// unspecified disable_shadows
			entry_point = interpolate_string(Old.entry_point, New.entry_point, Prog);
			// unspecified allow_passthru
			// unspecified allow_flying
			// unspecified allow_teleport
			// unspecified allow_avatar_collision
			gravity = interpolate_float(Old.gravity, New.gravity, Prog);
			friction = interpolate_float(Old.friction, New.friction, Prog);
			// unspecified slopeslide_enabled
			slopeslide_min_angle = interpolate_float(Old.slopeslide_min_angle, New.slopeslide_min_angle, Prog);
			slopeslide_max_angle = interpolate_float(Old.slopeslide_max_angle, New.slopeslide_max_angle, Prog);
			// unspecified enable_camera_collision
			camera_zoom = interpolate_float(Old.camera_zoom, New.camera_zoom, Prog);
			backdrop = interpolate_string(Old.backdrop, New.backdrop, Prog);
			skybox = interpolate_string(Old.skybox, New.skybox, Prog);
			sky_north = interpolate_rgb(Old.sky_north.getColour(), New.sky_north.getColour(), Prog);
			sky_south = interpolate_rgb(Old.sky_south.getColour(), New.sky_south.getColour(), Prog);
			sky_east = interpolate_rgb(Old.sky_east.getColour(), New.sky_east.getColour(), Prog);
			sky_west = interpolate_rgb(Old.sky_west.getColour(), New.sky_west.getColour(), Prog);
			sky_top = interpolate_rgb(Old.sky_top.getColour(), New.sky_top.getColour(), Prog);
			sky_bottom = interpolate_rgb(Old.sky_bottom.getColour(), New.sky_bottom.getColour(), Prog);
			clouds_layer1_texture = interpolate_string(Old.clouds_layer1_texture, New.clouds_layer1_texture, Prog);
			clouds_layer1_mask = interpolate_string(Old.clouds_layer1_mask, New.clouds_layer1_mask, Prog);
			clouds_layer1_tile = interpolate_float(Old.clouds_layer1_tile, New.clouds_layer1_tile, Prog);
			clouds_layer1_speed_x = interpolate_float(Old.clouds_layer1_speed_x, New.clouds_layer1_speed_x, Prog);
			clouds_layer1_speed_z = interpolate_float(Old.clouds_layer1_speed_z, New.clouds_layer1_speed_z, Prog);
			clouds_layer1_opacity = interpolate_int(Old.clouds_layer1_opacity, New.clouds_layer1_opacity, Prog);
			clouds_layer2_texture = interpolate_string(Old.clouds_layer2_texture, New.clouds_layer2_texture, Prog);
			clouds_layer2_mask = interpolate_string(Old.clouds_layer2_mask, New.clouds_layer2_mask, Prog);
			clouds_layer2_tile = interpolate_float(Old.clouds_layer2_tile, New.clouds_layer2_tile, Prog);
			clouds_layer2_speed_x = interpolate_float(Old.clouds_layer2_speed_x, New.clouds_layer2_speed_x, Prog);
			clouds_layer2_speed_z = interpolate_float(Old.clouds_layer2_speed_z, New.clouds_layer2_speed_z, Prog);
			clouds_layer2_opacity = interpolate_int(Old.clouds_layer2_opacity, New.clouds_layer2_opacity, Prog);
			clouds_layer3_texture = interpolate_string(Old.clouds_layer3_texture, New.clouds_layer3_texture, Prog);
			clouds_layer3_mask = interpolate_string(Old.clouds_layer3_mask, New.clouds_layer3_mask, Prog);
			clouds_layer3_tile = interpolate_float(Old.clouds_layer3_tile, New.clouds_layer3_tile, Prog);
			clouds_layer3_speed_x = interpolate_float(Old.clouds_layer3_speed_x, New.clouds_layer3_speed_x, Prog);
			clouds_layer3_speed_z = interpolate_float(Old.clouds_layer3_speed_z, New.clouds_layer3_speed_z, Prog);
			clouds_layer3_opacity = interpolate_int(Old.clouds_layer3_opacity, New.clouds_layer3_opacity, Prog);
			// unspecified disable_chat
			// unspecified allow_citizen_whisper
			// unspecified allow_tourist_whisper
			// unspecified always_show_names
			// unspecified disable_avatar_list
			avatar_refresh_rate = interpolate_int(Old.avatar_refresh_rate, New.avatar_refresh_rate, Prog);
			// unspecified enable_cav
			// unspecified enable_pav
			// unspecified chat_disable_url_clicks
			// unspecified disable_multiple_media
			botmenu_url = interpolate_string(Old.botmenu_url, New.botmenu_url, Prog);
			// unspecified voip_conference_global
			// unspecified voip_moderate_global
			// unspecified water_enabled
			water_texture = interpolate_string(Old.water_texture, New.water_texture, Prog);
			water_mask = interpolate_string(Old.water_mask, New.water_mask, Prog);
			water_bottom_texture = interpolate_string(Old.water_bottom_texture, New.water_bottom_texture, Prog);
			water_bottom_mask = interpolate_string(Old.water_bottom_mask, New.water_bottom_mask, Prog);
			water_opacity = interpolate_int(Old.water_opacity, New.water_opacity, Prog);
			water_visibility = interpolate_int(Old.water_visibility, New.water_visibility, Prog);
			water = interpolate_rgb(Old.water.getColour(), New.water.getColour(), Prog);
			water_level = interpolate_float(Old.water_level, New.water_level, Prog);
			water_surface_move = interpolate_float(Old.water_surface_move, New.water_surface_move, Prog);
			water_wave_move = interpolate_float(Old.water_wave_move, New.water_wave_move, Prog);
			water_speed = interpolate_float(Old.water_speed, New.water_speed, Prog);
			buoyancy = interpolate_float(Old.buoyancy, New.buoyancy, Prog);
			water_friction = interpolate_float(Old.water_friction, New.water_friction, Prog);
			// unspecified water_under_terrain
			sound_footstep = interpolate_string(Old.sound_footstep, New.sound_footstep, Prog);
			sound_water_enter = interpolate_string(Old.sound_water_enter, New.sound_water_enter, Prog);
			sound_water_exit = interpolate_string(Old.sound_water_exit, New.sound_water_exit, Prog);
			sound_ambient = interpolate_string(Old.sound_ambient, New.sound_ambient, Prog);

		}
		
		// overloaded functions to convert strings to their respective values
		String temp_string;

		// convert from one string to return the same string
		String convertTypeToString(String Type)
		{
			return Type;
		}
		
		// convert from an integer to a type
		String convertTypeToString(int Type)	
		{
			temp_string.Format("%d", Type); return temp_string;
		}
		
		// convert from floating point		
		String convertTypeToString(float Type)
		{
			temp_string.Format("%f", Type); temp_string.TrimRight("0"); return temp_string;
		}
		
		// convert from a colour to a string
		String convertTypeToString(AW::Colour Type)
		{
			temp_string.Format("#%02x%02x%02x", Type.GetRed(), Type.GetGreen(), Type.GetBlue()); return temp_string;
		}
		
		// converts from a string into a string
		void convertStringToType(String Source, String& Type)
		{
			Type = Source;
		}
		
		// converts from a string into a float
		void convertStringToType(String Source, float& Type)
		{
			Type = (float)atof(Source);
		}
		
		// convert from a string into an integer
		void convertStringToType(String Source, int& Type)
		{
			Type = atoi(Source);
		}
		
		// convert from a string into a colour
		void convertStringToType(String Source, AW::Colour& Type)
		{
			char* pEnd = NULL;
			Type.value = Colour::LookupString(Source);
		}
		
		String Attributes::get_by_name(String AttributeID)
		{
			if (strcmp("enter_right",                 AttributeID) == 0) return convertTypeToString(enter_right);
			if (strcmp("build_right",                 AttributeID) == 0) return convertTypeToString(build_right);
			if (strcmp("voip_right",                  AttributeID) == 0) return convertTypeToString(voip_right);
			if (strcmp("eminent_domain_right",        AttributeID) == 0) return convertTypeToString(eminent_domain_right);
			if (strcmp("special_objects_right",       AttributeID) == 0) return convertTypeToString(special_objects_right);
			if (strcmp("public_speaker_right",        AttributeID) == 0) return convertTypeToString(public_speaker_right);
			if (strcmp("bots_right",                  AttributeID) == 0) return convertTypeToString(bots_right);
			if (strcmp("eject_right",                 AttributeID) == 0) return convertTypeToString(eject_right);
			if (strcmp("speak_right",                 AttributeID) == 0) return convertTypeToString(speak_right);
			if (strcmp("special_commands_right",      AttributeID) == 0) return convertTypeToString(special_commands_right);
			if (strcmp("special_commands",            AttributeID) == 0) return convertTypeToString(special_commands);
			if (strcmp("v4_objects_right",            AttributeID) == 0) return convertTypeToString(v4_objects_right);
			if (strcmp("terrain_right",               AttributeID) == 0) return convertTypeToString(terrain_right);
			if (strcmp("title",                       AttributeID) == 0) return convertTypeToString(title);
			if (strcmp("home_page",                   AttributeID) == 0) return convertTypeToString(home_page);
			if (strcmp("rating",                      AttributeID) == 0) return convertTypeToString(rating);
			if (strcmp("welcome_message",             AttributeID) == 0) return convertTypeToString(welcome_message);
			if (strcmp("keywords",                    AttributeID) == 0) return convertTypeToString(keywords);
			if (strcmp("ground",                      AttributeID) == 0) return convertTypeToString(ground);
			if (strcmp("repeating_ground",            AttributeID) == 0) return convertTypeToString(repeating_ground);
			if (strcmp("enable_terrain",              AttributeID) == 0) return convertTypeToString(enable_terrain);
			if (strcmp("terrain_ambient",             AttributeID) == 0) return convertTypeToString(terrain_ambient);
			if (strcmp("terrain_diffuse",             AttributeID) == 0) return convertTypeToString(terrain_diffuse);
			if (strcmp("terrain_offset",              AttributeID) == 0) return convertTypeToString(terrain_offset);
			if (strcmp("object_path",                 AttributeID) == 0) return convertTypeToString(object_path);
			if (strcmp("object_refresh",              AttributeID) == 0) return convertTypeToString(object_refresh);
			if (strcmp("object_password",             AttributeID) == 0) return convertTypeToString(object_password);
			if (strcmp("enable_referer",              AttributeID) == 0) return convertTypeToString(enable_referer);
			if (strcmp("cav_object_path",             AttributeID) == 0) return convertTypeToString(cav_object_path);
			if (strcmp("cav_object_password",         AttributeID) == 0) return convertTypeToString(cav_object_password);
			if (strcmp("cav_object_refresh",          AttributeID) == 0) return convertTypeToString(cav_object_refresh);
			if (strcmp("cell_limit",                  AttributeID) == 0) return convertTypeToString(cell_limit);
			if (strcmp("restricted_radius",           AttributeID) == 0) return convertTypeToString(restricted_radius);
			if (strcmp("max_light_radius",            AttributeID) == 0) return convertTypeToString(max_light_radius);
			if (strcmp("disable_create_url",          AttributeID) == 0) return convertTypeToString(disable_create_url);
			if (strcmp("allow_object_select",         AttributeID) == 0) return convertTypeToString(allow_object_select);
			if (strcmp("allow_3_axis_rotation",       AttributeID) == 0) return convertTypeToString(allow_3_axis_rotation);
			if (strcmp("allow_tourist_build",         AttributeID) == 0) return convertTypeToString(allow_tourist_build);
			if (strcmp("mover_empty_reset_timeout",   AttributeID) == 0) return convertTypeToString(mover_empty_reset_timeout);
			if (strcmp("mover_used_reset_timeout",    AttributeID) == 0) return convertTypeToString(mover_used_reset_timeout);
			if (strcmp("enable_bump_event",           AttributeID) == 0) return convertTypeToString(enable_bump_event);
			if (strcmp("enable_sync_events",          AttributeID) == 0) return convertTypeToString(enable_sync_events);
			if (strcmp("fog_enable",                  AttributeID) == 0) return convertTypeToString(fog_enable);
			if (strcmp("fog_minimum",                 AttributeID) == 0) return convertTypeToString(fog_minimum);
			if (strcmp("fog_maximum",                 AttributeID) == 0) return convertTypeToString(fog_maximum);
			if (strcmp("fog_tinted",                  AttributeID) == 0) return convertTypeToString(fog_tinted);
			if (strcmp("fog",                         AttributeID) == 0) return convertTypeToString(fog);
			if (strcmp("minimum_visibility",          AttributeID) == 0) return convertTypeToString(minimum_visibility);
			if (strcmp("light_x",                     AttributeID) == 0) return convertTypeToString(light_x);
			if (strcmp("light_y",                     AttributeID) == 0) return convertTypeToString(light_y);
			if (strcmp("light_z",                     AttributeID) == 0) return convertTypeToString(light_z);
			if (strcmp("light",                       AttributeID) == 0) return convertTypeToString(light);
			if (strcmp("ambient_light",               AttributeID) == 0) return convertTypeToString(ambient_light);
			if (strcmp("light_texture",               AttributeID) == 0) return convertTypeToString(light_texture);
			if (strcmp("light_mask",                  AttributeID) == 0) return convertTypeToString(light_mask);
			if (strcmp("light_draw_size",             AttributeID) == 0) return convertTypeToString(light_draw_size);
			if (strcmp("light_draw_front",            AttributeID) == 0) return convertTypeToString(light_draw_front);
			if (strcmp("light_draw_bright",           AttributeID) == 0) return convertTypeToString(light_draw_bright);
			if (strcmp("light_source_use_color",      AttributeID) == 0) return convertTypeToString(light_source_use_color);
			if (strcmp("light_source_color",          AttributeID) == 0) return convertTypeToString(light_source_color);
			if (strcmp("disable_shadows",             AttributeID) == 0) return convertTypeToString(disable_shadows);
			if (strcmp("entry_point",                 AttributeID) == 0) return convertTypeToString(entry_point);
			if (strcmp("allow_passthru",              AttributeID) == 0) return convertTypeToString(allow_passthru);
			if (strcmp("allow_flying",                AttributeID) == 0) return convertTypeToString(allow_flying);
			if (strcmp("allow_teleport",              AttributeID) == 0) return convertTypeToString(allow_teleport);
			if (strcmp("allow_avatar_collision",      AttributeID) == 0) return convertTypeToString(allow_avatar_collision);
			if (strcmp("gravity",                     AttributeID) == 0) return convertTypeToString(gravity);
			if (strcmp("friction",                    AttributeID) == 0) return convertTypeToString(friction);
			if (strcmp("slopeslide_enabled",          AttributeID) == 0) return convertTypeToString(slopeslide_enabled);
			if (strcmp("slopeslide_min_angle",        AttributeID) == 0) return convertTypeToString(slopeslide_min_angle);
			if (strcmp("slopeslide_max_angle",        AttributeID) == 0) return convertTypeToString(slopeslide_max_angle);
			if (strcmp("enable_camera_collision",     AttributeID) == 0) return convertTypeToString(enable_camera_collision);
			if (strcmp("camera_zoom",                 AttributeID) == 0) return convertTypeToString(camera_zoom);
			if (strcmp("backdrop",                    AttributeID) == 0) return convertTypeToString(backdrop);
			if (strcmp("skybox",                      AttributeID) == 0) return convertTypeToString(skybox);
			if (strcmp("sky_north",                   AttributeID) == 0) return convertTypeToString(sky_north);
			if (strcmp("sky_south",                   AttributeID) == 0) return convertTypeToString(sky_south);
			if (strcmp("sky_east",                    AttributeID) == 0) return convertTypeToString(sky_east);
			if (strcmp("sky_west",                    AttributeID) == 0) return convertTypeToString(sky_west);
			if (strcmp("sky_top",                     AttributeID) == 0) return convertTypeToString(sky_top);
			if (strcmp("sky_bottom",                  AttributeID) == 0) return convertTypeToString(sky_bottom);
			if (strcmp("clouds_layer1_texture",       AttributeID) == 0) return convertTypeToString(clouds_layer1_texture);
			if (strcmp("clouds_layer1_mask",          AttributeID) == 0) return convertTypeToString(clouds_layer1_mask);
			if (strcmp("clouds_layer1_tile",          AttributeID) == 0) return convertTypeToString(clouds_layer1_tile);
			if (strcmp("clouds_layer1_speed_x",       AttributeID) == 0) return convertTypeToString(clouds_layer1_speed_x);
			if (strcmp("clouds_layer1_speed_z",       AttributeID) == 0) return convertTypeToString(clouds_layer1_speed_z);
			if (strcmp("clouds_layer1_opacity",       AttributeID) == 0) return convertTypeToString(clouds_layer1_opacity);
			if (strcmp("clouds_layer2_texture",       AttributeID) == 0) return convertTypeToString(clouds_layer2_texture);
			if (strcmp("clouds_layer2_mask",          AttributeID) == 0) return convertTypeToString(clouds_layer2_mask);
			if (strcmp("clouds_layer2_tile",          AttributeID) == 0) return convertTypeToString(clouds_layer2_tile);
			if (strcmp("clouds_layer2_speed_x",       AttributeID) == 0) return convertTypeToString(clouds_layer2_speed_x);
			if (strcmp("clouds_layer2_speed_z",       AttributeID) == 0) return convertTypeToString(clouds_layer2_speed_z);
			if (strcmp("clouds_layer2_opacity",       AttributeID) == 0) return convertTypeToString(clouds_layer2_opacity);
			if (strcmp("clouds_layer3_texture",       AttributeID) == 0) return convertTypeToString(clouds_layer3_texture);
			if (strcmp("clouds_layer3_mask",          AttributeID) == 0) return convertTypeToString(clouds_layer3_mask);
			if (strcmp("clouds_layer3_tile",          AttributeID) == 0) return convertTypeToString(clouds_layer3_tile);
			if (strcmp("clouds_layer3_speed_x",       AttributeID) == 0) return convertTypeToString(clouds_layer3_speed_x);
			if (strcmp("clouds_layer3_speed_z",       AttributeID) == 0) return convertTypeToString(clouds_layer3_speed_z);
			if (strcmp("clouds_layer3_opacity",       AttributeID) == 0) return convertTypeToString(clouds_layer3_opacity);
			if (strcmp("disable_chat",                AttributeID) == 0) return convertTypeToString(disable_chat);
			if (strcmp("allow_citizen_whisper",       AttributeID) == 0) return convertTypeToString(allow_citizen_whisper);
			if (strcmp("allow_tourist_whisper",       AttributeID) == 0) return convertTypeToString(allow_tourist_whisper);
			if (strcmp("always_show_names",           AttributeID) == 0) return convertTypeToString(always_show_names);
			if (strcmp("disable_avatar_list",         AttributeID) == 0) return convertTypeToString(disable_avatar_list);
			if (strcmp("avatar_refresh_rate",         AttributeID) == 0) return convertTypeToString(avatar_refresh_rate);
			if (strcmp("enable_cav",                  AttributeID) == 0) return convertTypeToString(enable_cav);
			if (strcmp("enable_pav",                  AttributeID) == 0) return convertTypeToString(enable_pav);
			if (strcmp("chat_disable_url_clicks",     AttributeID) == 0) return convertTypeToString(chat_disable_url_clicks);
			if (strcmp("disable_multiple_media",      AttributeID) == 0) return convertTypeToString(disable_multiple_media);
			if (strcmp("botmenu_url",                 AttributeID) == 0) return convertTypeToString(botmenu_url);
			if (strcmp("voip_conference_global",      AttributeID) == 0) return convertTypeToString(voip_conference_global);
			if (strcmp("voip_moderate_global",        AttributeID) == 0) return convertTypeToString(voip_moderate_global);
			if (strcmp("water_enabled",               AttributeID) == 0) return convertTypeToString(water_enabled);
			if (strcmp("water_texture",               AttributeID) == 0) return convertTypeToString(water_texture);
			if (strcmp("water_mask",                  AttributeID) == 0) return convertTypeToString(water_mask);
			if (strcmp("water_bottom_texture",        AttributeID) == 0) return convertTypeToString(water_bottom_texture);
			if (strcmp("water_bottom_mask",           AttributeID) == 0) return convertTypeToString(water_bottom_mask);
			if (strcmp("water_opacity",               AttributeID) == 0) return convertTypeToString(water_opacity);
			if (strcmp("water_visibility",            AttributeID) == 0) return convertTypeToString(water_visibility);
			if (strcmp("water",                       AttributeID) == 0) return convertTypeToString(water);
			if (strcmp("water_level",                 AttributeID) == 0) return convertTypeToString(water_level);
			if (strcmp("water_surface_move",          AttributeID) == 0) return convertTypeToString(water_surface_move);
			if (strcmp("water_wave_move",             AttributeID) == 0) return convertTypeToString(water_wave_move);
			if (strcmp("water_speed",                 AttributeID) == 0) return convertTypeToString(water_speed);
			if (strcmp("buoyancy",                    AttributeID) == 0) return convertTypeToString(buoyancy);
			if (strcmp("water_friction",              AttributeID) == 0) return convertTypeToString(water_friction);
			if (strcmp("water_under_terrain",         AttributeID) == 0) return convertTypeToString(water_under_terrain);
			if (strcmp("sound_footstep",              AttributeID) == 0) return convertTypeToString(sound_footstep);
			if (strcmp("sound_water_enter",           AttributeID) == 0) return convertTypeToString(sound_water_enter);
			if (strcmp("sound_water_exit",            AttributeID) == 0) return convertTypeToString(sound_water_exit);
			if (strcmp("sound_ambient",               AttributeID) == 0) return convertTypeToString(sound_ambient);

			return "";
		}
		
		bool Attributes::set_by_name(String AttributeID, String Value)
		{
			if (strcmp("enter_right",                 AttributeID) == 0) { convertStringToType(Value, enter_right                   ); return true; }
			if (strcmp("build_right",                 AttributeID) == 0) { convertStringToType(Value, build_right                   ); return true; }
			if (strcmp("voip_right",                  AttributeID) == 0) { convertStringToType(Value, voip_right                    ); return true; }
			if (strcmp("eminent_domain_right",        AttributeID) == 0) { convertStringToType(Value, eminent_domain_right          ); return true; }
			if (strcmp("special_objects_right",       AttributeID) == 0) { convertStringToType(Value, special_objects_right         ); return true; }
			if (strcmp("public_speaker_right",        AttributeID) == 0) { convertStringToType(Value, public_speaker_right          ); return true; }
			if (strcmp("bots_right",                  AttributeID) == 0) { convertStringToType(Value, bots_right                    ); return true; }
			if (strcmp("eject_right",                 AttributeID) == 0) { convertStringToType(Value, eject_right                   ); return true; }
			if (strcmp("speak_right",                 AttributeID) == 0) { convertStringToType(Value, speak_right                   ); return true; }
			if (strcmp("special_commands_right",      AttributeID) == 0) { convertStringToType(Value, special_commands_right        ); return true; }
			if (strcmp("special_commands",            AttributeID) == 0) { convertStringToType(Value, special_commands              ); return true; }
			if (strcmp("v4_objects_right",            AttributeID) == 0) { convertStringToType(Value, v4_objects_right              ); return true; }
			if (strcmp("terrain_right",               AttributeID) == 0) { convertStringToType(Value, terrain_right                 ); return true; }
			if (strcmp("title",                       AttributeID) == 0) { convertStringToType(Value, title                         ); return true; }
			if (strcmp("home_page",                   AttributeID) == 0) { convertStringToType(Value, home_page                     ); return true; }
			if (strcmp("rating",                      AttributeID) == 0) { convertStringToType(Value, rating                        ); return true; }
			if (strcmp("welcome_message",             AttributeID) == 0) { convertStringToType(Value, welcome_message               ); return true; }
			if (strcmp("keywords",                    AttributeID) == 0) { convertStringToType(Value, keywords                      ); return true; }
			if (strcmp("ground",                      AttributeID) == 0) { convertStringToType(Value, ground                        ); return true; }
			if (strcmp("repeating_ground",            AttributeID) == 0) { convertStringToType(Value, repeating_ground              ); return true; }
			if (strcmp("enable_terrain",              AttributeID) == 0) { convertStringToType(Value, enable_terrain                ); return true; }
			if (strcmp("terrain_ambient",             AttributeID) == 0) { convertStringToType(Value, terrain_ambient               ); return true; }
			if (strcmp("terrain_diffuse",             AttributeID) == 0) { convertStringToType(Value, terrain_diffuse               ); return true; }
			if (strcmp("terrain_offset",              AttributeID) == 0) { convertStringToType(Value, terrain_offset                ); return true; }
			if (strcmp("object_path",                 AttributeID) == 0) { convertStringToType(Value, object_path                   ); return true; }
			if (strcmp("object_refresh",              AttributeID) == 0) { convertStringToType(Value, object_refresh                ); return true; }
			if (strcmp("object_password",             AttributeID) == 0) { convertStringToType(Value, object_password               ); return true; }
			if (strcmp("enable_referer",              AttributeID) == 0) { convertStringToType(Value, enable_referer                ); return true; }
			if (strcmp("cav_object_path",             AttributeID) == 0) { convertStringToType(Value, cav_object_path               ); return true; }
			if (strcmp("cav_object_password",         AttributeID) == 0) { convertStringToType(Value, cav_object_password           ); return true; }
			if (strcmp("cav_object_refresh",          AttributeID) == 0) { convertStringToType(Value, cav_object_refresh            ); return true; }
			if (strcmp("cell_limit",                  AttributeID) == 0) { convertStringToType(Value, cell_limit                    ); return true; }
			if (strcmp("restricted_radius",           AttributeID) == 0) { convertStringToType(Value, restricted_radius             ); return true; }
			if (strcmp("max_light_radius",            AttributeID) == 0) { convertStringToType(Value, max_light_radius              ); return true; }
			if (strcmp("disable_create_url",          AttributeID) == 0) { convertStringToType(Value, disable_create_url            ); return true; }
			if (strcmp("allow_object_select",         AttributeID) == 0) { convertStringToType(Value, allow_object_select           ); return true; }
			if (strcmp("allow_3_axis_rotation",       AttributeID) == 0) { convertStringToType(Value, allow_3_axis_rotation         ); return true; }
			if (strcmp("allow_tourist_build",         AttributeID) == 0) { convertStringToType(Value, allow_tourist_build           ); return true; }
			if (strcmp("mover_empty_reset_timeout",   AttributeID) == 0) { convertStringToType(Value, mover_empty_reset_timeout     ); return true; }
			if (strcmp("mover_used_reset_timeout",    AttributeID) == 0) { convertStringToType(Value, mover_used_reset_timeout      ); return true; }
			if (strcmp("enable_bump_event",           AttributeID) == 0) { convertStringToType(Value, enable_bump_event             ); return true; }
			if (strcmp("enable_sync_events",          AttributeID) == 0) { convertStringToType(Value, enable_sync_events            ); return true; }
			if (strcmp("fog_enable",                  AttributeID) == 0) { convertStringToType(Value, fog_enable                    ); return true; }
			if (strcmp("fog_minimum",                 AttributeID) == 0) { convertStringToType(Value, fog_minimum                   ); return true; }
			if (strcmp("fog_maximum",                 AttributeID) == 0) { convertStringToType(Value, fog_maximum                   ); return true; }
			if (strcmp("fog_tinted",                  AttributeID) == 0) { convertStringToType(Value, fog_tinted                    ); return true; }
			if (strcmp("fog",                         AttributeID) == 0) { convertStringToType(Value, fog                           ); return true; }
			if (strcmp("minimum_visibility",          AttributeID) == 0) { convertStringToType(Value, minimum_visibility            ); return true; }
			if (strcmp("light_x",                     AttributeID) == 0) { convertStringToType(Value, light_x                       ); return true; }
			if (strcmp("light_y",                     AttributeID) == 0) { convertStringToType(Value, light_y                       ); return true; }
			if (strcmp("light_z",                     AttributeID) == 0) { convertStringToType(Value, light_z                       ); return true; }
			if (strcmp("light",                       AttributeID) == 0) { convertStringToType(Value, light                         ); return true; }
			if (strcmp("ambient_light",               AttributeID) == 0) { convertStringToType(Value, ambient_light                 ); return true; }
			if (strcmp("light_texture",               AttributeID) == 0) { convertStringToType(Value, light_texture                 ); return true; }
			if (strcmp("light_mask",                  AttributeID) == 0) { convertStringToType(Value, light_mask                    ); return true; }
			if (strcmp("light_draw_size",             AttributeID) == 0) { convertStringToType(Value, light_draw_size               ); return true; }
			if (strcmp("light_draw_front",            AttributeID) == 0) { convertStringToType(Value, light_draw_front              ); return true; }
			if (strcmp("light_draw_bright",           AttributeID) == 0) { convertStringToType(Value, light_draw_bright             ); return true; }
			if (strcmp("light_source_use_color",      AttributeID) == 0) { convertStringToType(Value, light_source_use_color        ); return true; }
			if (strcmp("light_source_color",          AttributeID) == 0) { convertStringToType(Value, light_source_color            ); return true; }
			if (strcmp("disable_shadows",             AttributeID) == 0) { convertStringToType(Value, disable_shadows               ); return true; }
			if (strcmp("entry_point",                 AttributeID) == 0) { convertStringToType(Value, entry_point                   ); return true; }
			if (strcmp("allow_passthru",              AttributeID) == 0) { convertStringToType(Value, allow_passthru                ); return true; }
			if (strcmp("allow_flying",                AttributeID) == 0) { convertStringToType(Value, allow_flying                  ); return true; }
			if (strcmp("allow_teleport",              AttributeID) == 0) { convertStringToType(Value, allow_teleport                ); return true; }
			if (strcmp("allow_avatar_collision",      AttributeID) == 0) { convertStringToType(Value, allow_avatar_collision        ); return true; }
			if (strcmp("gravity",                     AttributeID) == 0) { convertStringToType(Value, gravity                       ); return true; }
			if (strcmp("friction",                    AttributeID) == 0) { convertStringToType(Value, friction                      ); return true; }
			if (strcmp("slopeslide_enabled",          AttributeID) == 0) { convertStringToType(Value, slopeslide_enabled            ); return true; }
			if (strcmp("slopeslide_min_angle",        AttributeID) == 0) { convertStringToType(Value, slopeslide_min_angle          ); return true; }
			if (strcmp("slopeslide_max_angle",        AttributeID) == 0) { convertStringToType(Value, slopeslide_max_angle          ); return true; }
			if (strcmp("enable_camera_collision",     AttributeID) == 0) { convertStringToType(Value, enable_camera_collision       ); return true; }
			if (strcmp("camera_zoom",                 AttributeID) == 0) { convertStringToType(Value, camera_zoom                   ); return true; }
			if (strcmp("backdrop",                    AttributeID) == 0) { convertStringToType(Value, backdrop                      ); return true; }
			if (strcmp("skybox",                      AttributeID) == 0) { convertStringToType(Value, skybox                        ); return true; }
			if (strcmp("sky_north",                   AttributeID) == 0) { convertStringToType(Value, sky_north                     ); return true; }
			if (strcmp("sky_south",                   AttributeID) == 0) { convertStringToType(Value, sky_south                     ); return true; }
			if (strcmp("sky_east",                    AttributeID) == 0) { convertStringToType(Value, sky_east                      ); return true; }
			if (strcmp("sky_west",                    AttributeID) == 0) { convertStringToType(Value, sky_west                      ); return true; }
			if (strcmp("sky_top",                     AttributeID) == 0) { convertStringToType(Value, sky_top                       ); return true; }
			if (strcmp("sky_bottom",                  AttributeID) == 0) { convertStringToType(Value, sky_bottom                    ); return true; }
			if (strcmp("clouds_layer1_texture",       AttributeID) == 0) { convertStringToType(Value, clouds_layer1_texture         ); return true; }
			if (strcmp("clouds_layer1_mask",          AttributeID) == 0) { convertStringToType(Value, clouds_layer1_mask            ); return true; }
			if (strcmp("clouds_layer1_tile",          AttributeID) == 0) { convertStringToType(Value, clouds_layer1_tile            ); return true; }
			if (strcmp("clouds_layer1_speed_x",       AttributeID) == 0) { convertStringToType(Value, clouds_layer1_speed_x         ); return true; }
			if (strcmp("clouds_layer1_speed_z",       AttributeID) == 0) { convertStringToType(Value, clouds_layer1_speed_z         ); return true; }
			if (strcmp("clouds_layer1_opacity",       AttributeID) == 0) { convertStringToType(Value, clouds_layer1_opacity         ); return true; }
			if (strcmp("clouds_layer2_texture",       AttributeID) == 0) { convertStringToType(Value, clouds_layer2_texture         ); return true; }
			if (strcmp("clouds_layer2_mask",          AttributeID) == 0) { convertStringToType(Value, clouds_layer2_mask            ); return true; }
			if (strcmp("clouds_layer2_tile",          AttributeID) == 0) { convertStringToType(Value, clouds_layer2_tile            ); return true; }
			if (strcmp("clouds_layer2_speed_x",       AttributeID) == 0) { convertStringToType(Value, clouds_layer2_speed_x         ); return true; }
			if (strcmp("clouds_layer2_speed_z",       AttributeID) == 0) { convertStringToType(Value, clouds_layer2_speed_z         ); return true; }
			if (strcmp("clouds_layer2_opacity",       AttributeID) == 0) { convertStringToType(Value, clouds_layer2_opacity         ); return true; }
			if (strcmp("clouds_layer3_texture",       AttributeID) == 0) { convertStringToType(Value, clouds_layer3_texture         ); return true; }
			if (strcmp("clouds_layer3_mask",          AttributeID) == 0) { convertStringToType(Value, clouds_layer3_mask            ); return true; }
			if (strcmp("clouds_layer3_tile",          AttributeID) == 0) { convertStringToType(Value, clouds_layer3_tile            ); return true; }
			if (strcmp("clouds_layer3_speed_x",       AttributeID) == 0) { convertStringToType(Value, clouds_layer3_speed_x         ); return true; }
			if (strcmp("clouds_layer3_speed_z",       AttributeID) == 0) { convertStringToType(Value, clouds_layer3_speed_z         ); return true; }
			if (strcmp("clouds_layer3_opacity",       AttributeID) == 0) { convertStringToType(Value, clouds_layer3_opacity         ); return true; }
			if (strcmp("disable_chat",                AttributeID) == 0) { convertStringToType(Value, disable_chat                  ); return true; }
			if (strcmp("allow_citizen_whisper",       AttributeID) == 0) { convertStringToType(Value, allow_citizen_whisper         ); return true; }
			if (strcmp("allow_tourist_whisper",       AttributeID) == 0) { convertStringToType(Value, allow_tourist_whisper         ); return true; }
			if (strcmp("always_show_names",           AttributeID) == 0) { convertStringToType(Value, always_show_names             ); return true; }
			if (strcmp("disable_avatar_list",         AttributeID) == 0) { convertStringToType(Value, disable_avatar_list           ); return true; }
			if (strcmp("avatar_refresh_rate",         AttributeID) == 0) { convertStringToType(Value, avatar_refresh_rate           ); return true; }
			if (strcmp("enable_cav",                  AttributeID) == 0) { convertStringToType(Value, enable_cav                    ); return true; }
			if (strcmp("enable_pav",                  AttributeID) == 0) { convertStringToType(Value, enable_pav                    ); return true; }
			if (strcmp("chat_disable_url_clicks",     AttributeID) == 0) { convertStringToType(Value, chat_disable_url_clicks       ); return true; }
			if (strcmp("disable_multiple_media",      AttributeID) == 0) { convertStringToType(Value, disable_multiple_media        ); return true; }
			if (strcmp("botmenu_url",                 AttributeID) == 0) { convertStringToType(Value, botmenu_url                   ); return true; }
			if (strcmp("voip_conference_global",      AttributeID) == 0) { convertStringToType(Value, voip_conference_global        ); return true; }
			if (strcmp("voip_moderate_global",        AttributeID) == 0) { convertStringToType(Value, voip_moderate_global          ); return true; }
			if (strcmp("water_enabled",               AttributeID) == 0) { convertStringToType(Value, water_enabled                 ); return true; }
			if (strcmp("water_texture",               AttributeID) == 0) { convertStringToType(Value, water_texture                 ); return true; }
			if (strcmp("water_mask",                  AttributeID) == 0) { convertStringToType(Value, water_mask                    ); return true; }
			if (strcmp("water_bottom_texture",        AttributeID) == 0) { convertStringToType(Value, water_bottom_texture          ); return true; }
			if (strcmp("water_bottom_mask",           AttributeID) == 0) { convertStringToType(Value, water_bottom_mask             ); return true; }
			if (strcmp("water_opacity",               AttributeID) == 0) { convertStringToType(Value, water_opacity                 ); return true; }
			if (strcmp("water_visibility",            AttributeID) == 0) { convertStringToType(Value, water_visibility              ); return true; }
			if (strcmp("water",                       AttributeID) == 0) { convertStringToType(Value, water                         ); return true; }
			if (strcmp("water_level",                 AttributeID) == 0) { convertStringToType(Value, water_level                   ); return true; }
			if (strcmp("water_surface_move",          AttributeID) == 0) { convertStringToType(Value, water_surface_move            ); return true; }
			if (strcmp("water_wave_move",             AttributeID) == 0) { convertStringToType(Value, water_wave_move               ); return true; }
			if (strcmp("water_speed",                 AttributeID) == 0) { convertStringToType(Value, water_speed                   ); return true; }
			if (strcmp("buoyancy",                    AttributeID) == 0) { convertStringToType(Value, buoyancy                      ); return true; }
			if (strcmp("water_friction",              AttributeID) == 0) { convertStringToType(Value, water_friction                ); return true; }
			if (strcmp("water_under_terrain",         AttributeID) == 0) { convertStringToType(Value, water_under_terrain           ); return true; }
			if (strcmp("sound_footstep",              AttributeID) == 0) { convertStringToType(Value, sound_footstep                ); return true; }
			if (strcmp("sound_water_enter",           AttributeID) == 0) { convertStringToType(Value, sound_water_enter             ); return true; }
			if (strcmp("sound_water_exit",            AttributeID) == 0) { convertStringToType(Value, sound_water_exit              ); return true; }
			if (strcmp("sound_ambient",               AttributeID) == 0) { convertStringToType(Value, sound_ambient                 ); return true; }

			return false;
		}
		
		GroupMap& Attributes::StaticGroupMap()
		{
			static GroupMap gm;
			if (gm.size() == 0)
			{
					gm.push_back("wag_right", "attributes", "enter_right", "Enter Right");
				gm.push_back("wag_right", "attributes", "build_right", "Build Right");
				gm.push_back("wag_right", "attributes", "voip_right", "Voip Right");
				gm.push_back("wag_right", "attributes", "eminent_domain_right", "Eminent Domain Right");
				gm.push_back("wag_right", "attributes", "special_objects_right", "Special Objects Right");
				gm.push_back("wag_right", "attributes", "public_speaker_right", "Public Speaker Right");
				gm.push_back("wag_right", "attributes", "bots_right", "Bots Right");
				gm.push_back("wag_right", "attributes", "eject_right", "Eject Right");
				gm.push_back("wag_right", "attributes", "speak_right", "Speak Right");
				gm.push_back("wag_right", "attributes", "special_commands_right", "Special Commands Right");
				gm.push_back("wag_right", "attributes", "special_commands", "Special Commands");
				gm.push_back("wag_right", "attributes", "v4_objects_right", "V4 Objects Right");
				gm.push_back("wag_right", "attributes", "terrain_right", "Terrain Right");
				gm.push_back("wag_general", "attributes", "title", "Title");
				gm.push_back("wag_general", "attributes", "home_page", "Home Page");
				gm.push_back("wag_general", "attributes", "rating", "Rating");
				gm.push_back("wag_general", "attributes", "welcome_message", "Welcome Message");
				gm.push_back("wag_general", "attributes", "keywords", "Keywords");
				gm.push_back("wag_ground", "attributes", "ground", "Ground");
				gm.push_back("wag_ground", "attributes", "repeating_ground", "Repeating Ground");
				gm.push_back("wag_ground", "attributes", "enable_terrain", "Enable Terrain");
				gm.push_back("wag_ground", "attributes", "terrain_ambient", "Terrain Ambient");
				gm.push_back("wag_ground", "attributes", "terrain_diffuse", "Terrain Diffuse");
				gm.push_back("wag_ground", "attributes", "terrain_offset", "Terrain Offset");
				gm.push_back("wag_object_path", "attributes", "object_path", "Object Path");
				gm.push_back("wag_object_path", "attributes", "object_refresh", "Object Refresh");
				gm.push_back("wag_object_path", "attributes", "object_password", "Object Password");
				gm.push_back("wag_object_path", "attributes", "enable_referer", "Enable Referer");
				gm.push_back("wag_object_path", "attributes", "cav_object_path", "Cav Object Path");
				gm.push_back("wag_object_path", "attributes", "cav_object_password", "Cav Object Password");
				gm.push_back("wag_object_path", "attributes", "cav_object_refresh", "Cav Object Refresh");
				gm.push_back("wag_building", "attributes", "cell_limit", "Cell Limit");
				gm.push_back("wag_building", "attributes", "restricted_radius", "Restricted Radius");
				gm.push_back("wag_building", "attributes", "max_light_radius", "Max Light Radius");
				gm.push_back("wag_building", "attributes", "disable_create_url", "Disable Create Url");
				gm.push_back("wag_building", "attributes", "allow_object_select", "Allow Object Select");
				gm.push_back("wag_building", "attributes", "allow_3_axis_rotation", "Allow 3 Axis Rotation");
				gm.push_back("wag_building", "attributes", "allow_tourist_build", "Allow Tourist Build");
				gm.push_back("wag_building", "attributes", "mover_empty_reset_timeout", "Mover Empty Reset Timeout");
				gm.push_back("wag_building", "attributes", "mover_used_reset_timeout", "Mover Used Reset Timeout");
				gm.push_back("wag_building", "attributes", "enable_bump_event", "Enable Bump Event");
				gm.push_back("wag_building", "attributes", "enable_sync_events", "Enable Sync Events");
				gm.push_back("wag_fog", "attributes", "fog_enable", "Fog Enable");
				gm.push_back("wag_fog", "attributes", "fog_minimum", "Fog Minimum");
				gm.push_back("wag_fog", "attributes", "fog_maximum", "Fog Maximum");
				gm.push_back("wag_fog", "attributes", "fog_tinted", "Fog Tinted");
				gm.push_back("wag_fog", "attributes", "fog", "Fog");
				gm.push_back("wag_lighting", "attributes", "minimum_visibility", "Minimum Visibility");
				gm.push_back("wag_lighting", "attributes", "light_x", "Light X");
				gm.push_back("wag_lighting", "attributes", "light_y", "Light Y");
				gm.push_back("wag_lighting", "attributes", "light_z", "Light Z");
				gm.push_back("wag_lighting", "attributes", "light", "Light");
				gm.push_back("wag_lighting", "attributes", "ambient_light", "Ambient Light");
				gm.push_back("wag_lighting", "attributes", "light_texture", "Light Texture");
				gm.push_back("wag_lighting", "attributes", "light_mask", "Light Mask");
				gm.push_back("wag_lighting", "attributes", "light_draw_size", "Light Draw Size");
				gm.push_back("wag_lighting", "attributes", "light_draw_front", "Light Draw Front");
				gm.push_back("wag_lighting", "attributes", "light_draw_bright", "Light Draw Bright");
				gm.push_back("wag_lighting", "attributes", "light_source_use_color", "Light Source Use Color");
				gm.push_back("wag_lighting", "attributes", "light_source_color", "Light Source Color");
				gm.push_back("wag_lighting", "attributes", "disable_shadows", "Disable Shadows");
				gm.push_back("wag_movement", "attributes", "entry_point", "Entry Point");
				gm.push_back("wag_movement", "attributes", "allow_passthru", "Allow Passthru");
				gm.push_back("wag_movement", "attributes", "allow_flying", "Allow Flying");
				gm.push_back("wag_movement", "attributes", "allow_teleport", "Allow Teleport");
				gm.push_back("wag_movement", "attributes", "allow_avatar_collision", "Allow Avatar Collision");
				gm.push_back("wag_movement", "attributes", "gravity", "Gravity");
				gm.push_back("wag_movement", "attributes", "friction", "Friction");
				gm.push_back("wag_movement", "attributes", "slopeslide_enabled", "Slopeslide Enabled");
				gm.push_back("wag_movement", "attributes", "slopeslide_min_angle", "Slopeslide Min Angle");
				gm.push_back("wag_movement", "attributes", "slopeslide_max_angle", "Slopeslide Max Angle");
				gm.push_back("wag_movement", "attributes", "enable_camera_collision", "Enable Camera Collision");
				gm.push_back("wag_movement", "attributes", "camera_zoom", "Camera Zoom");
				gm.push_back("wag_sky", "attributes", "backdrop", "Backdrop");
				gm.push_back("wag_sky", "attributes", "skybox", "Skybox");
				gm.push_back("wag_sky", "attributes", "sky_north", "Sky North");
				gm.push_back("wag_sky", "attributes", "sky_south", "Sky South");
				gm.push_back("wag_sky", "attributes", "sky_east", "Sky East");
				gm.push_back("wag_sky", "attributes", "sky_west", "Sky West");
				gm.push_back("wag_sky", "attributes", "sky_top", "Sky Top");
				gm.push_back("wag_sky", "attributes", "sky_bottom", "Sky Bottom");
				gm.push_back("wag_clouds_1", "attributes", "clouds_layer1_texture", "Layer1 Texture");
				gm.push_back("wag_clouds_1", "attributes", "clouds_layer1_mask", "Layer1 Mask");
				gm.push_back("wag_clouds_1", "attributes", "clouds_layer1_tile", "Layer1 Tile");
				gm.push_back("wag_clouds_1", "attributes", "clouds_layer1_speed_x", "Layer1 Speed X");
				gm.push_back("wag_clouds_1", "attributes", "clouds_layer1_speed_z", "Layer1 Speed Z");
				gm.push_back("wag_clouds_1", "attributes", "clouds_layer1_opacity", "Layer1 Opacity");
				gm.push_back("wag_clouds_2", "attributes", "clouds_layer2_texture", "Layer2 Texture");
				gm.push_back("wag_clouds_2", "attributes", "clouds_layer2_mask", "Layer2 Mask");
				gm.push_back("wag_clouds_2", "attributes", "clouds_layer2_tile", "Layer2 Tile");
				gm.push_back("wag_clouds_2", "attributes", "clouds_layer2_speed_x", "Layer2 Speed X");
				gm.push_back("wag_clouds_2", "attributes", "clouds_layer2_speed_z", "Layer2 Speed Z");
				gm.push_back("wag_clouds_2", "attributes", "clouds_layer2_opacity", "Layer2 Opacity");
				gm.push_back("wag_clouds_3", "attributes", "clouds_layer3_texture", "Layer3 Texture");
				gm.push_back("wag_clouds_3", "attributes", "clouds_layer3_mask", "Layer3 Mask");
				gm.push_back("wag_clouds_3", "attributes", "clouds_layer3_tile", "Layer3 Tile");
				gm.push_back("wag_clouds_3", "attributes", "clouds_layer3_speed_x", "Layer3 Speed X");
				gm.push_back("wag_clouds_3", "attributes", "clouds_layer3_speed_z", "Layer3 Speed Z");
				gm.push_back("wag_clouds_3", "attributes", "clouds_layer3_opacity", "Layer3 Opacity");
				gm.push_back("wag_communications", "attributes", "disable_chat", "Disable Chat");
				gm.push_back("wag_communications", "attributes", "allow_citizen_whisper", "Allow Citizen Whisper");
				gm.push_back("wag_communications", "attributes", "allow_tourist_whisper", "Allow Tourist Whisper");
				gm.push_back("wag_communications", "attributes", "always_show_names", "Always Show Names");
				gm.push_back("wag_avatars", "attributes", "disable_avatar_list", "Disable Avatar List");
				gm.push_back("wag_avatars", "attributes", "avatar_refresh_rate", "Avatar Refresh Rate");
				gm.push_back("wag_avatars", "attributes", "enable_cav", "Enable Cav");
				gm.push_back("wag_avatars", "attributes", "enable_pav", "Enable Pav");
				gm.push_back("wag_communications", "attributes", "chat_disable_url_clicks", "Chat Disable Url Clicks");
				gm.push_back("wag_communications", "attributes", "disable_multiple_media", "Disable Multiple Media");
				gm.push_back("wag_communications", "attributes", "botmenu_url", "Botmenu Url");
				gm.push_back("wag_communications", "attributes", "voip_conference_global", "Voip Conference Global");
				gm.push_back("wag_communications", "attributes", "voip_moderate_global", "Voip Moderate Global");
				gm.push_back("wag_water", "attributes", "water_enabled", "Enabled");
				gm.push_back("wag_water", "attributes", "water_texture", "Texture");
				gm.push_back("wag_water", "attributes", "water_mask", "Mask");
				gm.push_back("wag_water", "attributes", "water_bottom_texture", "Bottom Texture");
				gm.push_back("wag_water", "attributes", "water_bottom_mask", "Bottom Mask");
				gm.push_back("wag_water", "attributes", "water_opacity", "Opacity");
				gm.push_back("wag_water", "attributes", "water_visibility", "Visibility");
				gm.push_back("wag_water", "attributes", "water", "");
				gm.push_back("wag_water", "attributes", "water_level", "Level");
				gm.push_back("wag_water", "attributes", "water_surface_move", "Surface Move");
				gm.push_back("wag_water", "attributes", "water_wave_move", "Wave Move");
				gm.push_back("wag_water", "attributes", "water_speed", "Speed");
				gm.push_back("wag_water", "attributes", "buoyancy", "Buoyancy");
				gm.push_back("wag_water", "attributes", "water_friction", "Friction");
				gm.push_back("wag_water", "attributes", "water_under_terrain", "Under Terrain");
				gm.push_back("wag_sound", "attributes", "sound_footstep", "Sound Footstep");
				gm.push_back("wag_sound", "attributes", "sound_water_enter", "Sound  Enter");
				gm.push_back("wag_sound", "attributes", "sound_water_exit", "Sound  Exit");
				gm.push_back("wag_sound", "attributes", "sound_ambient", "Sound Ambient");

			}
			
			return gm;
		}
		
		AW::EnhancedINI Attributes::getEnhancedINI()
		{
			AW::EnhancedINI ini;
			dx_file(ini, AW::WRITE_TO_INI);
			return ini;
		}
		
		void Attributes::setEnhancedINI(AW::EnhancedINI& INI)
		{
			dx_file(INI, AW::READ_FROM_INI);
		}
		
		bool Attributes::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
			UMBRA_PROPERTY_RW(enter_right, enter_right);
			UMBRA_PROPERTY_RW(build_right, build_right);
			UMBRA_PROPERTY_RW(voip_right, voip_right);
			UMBRA_PROPERTY_RW(eminent_domain_right, eminent_domain_right);
			UMBRA_PROPERTY_RW(special_objects_right, special_objects_right);
			UMBRA_PROPERTY_RW(public_speaker_right, public_speaker_right);
			UMBRA_PROPERTY_RW(bots_right, bots_right);
			UMBRA_PROPERTY_RW(eject_right, eject_right);
			UMBRA_PROPERTY_RW(speak_right, speak_right);
			UMBRA_PROPERTY_RW(special_commands_right, special_commands_right);
			UMBRA_PROPERTY_RW(special_commands, special_commands);
			UMBRA_PROPERTY_RW(v4_objects_right, v4_objects_right);
			UMBRA_PROPERTY_RW(terrain_right, terrain_right);
			UMBRA_PROPERTY_RW(title, title);
			UMBRA_PROPERTY_RW(home_page, home_page);
			UMBRA_PROPERTY_RW(rating, rating);
			UMBRA_PROPERTY_RW(welcome_message, welcome_message);
			UMBRA_PROPERTY_RW(keywords, keywords);
			UMBRA_PROPERTY_RW(ground, ground);
			UMBRA_PROPERTY_RW(repeating_ground, repeating_ground);
			UMBRA_PROPERTY_RW(enable_terrain, enable_terrain);
			UMBRA_PROPERTY_RW(terrain_ambient, terrain_ambient);
			UMBRA_PROPERTY_RW(terrain_diffuse, terrain_diffuse);
			UMBRA_PROPERTY_RW(terrain_offset, terrain_offset);
			UMBRA_PROPERTY_RW(object_path, object_path);
			UMBRA_PROPERTY_RW(object_refresh, object_refresh);
			UMBRA_PROPERTY_RW(object_password, object_password);
			UMBRA_PROPERTY_RW(enable_referer, enable_referer);
			UMBRA_PROPERTY_RW(cav_object_path, cav_object_path);
			UMBRA_PROPERTY_RW(cav_object_password, cav_object_password);
			UMBRA_PROPERTY_RW(cav_object_refresh, cav_object_refresh);
			UMBRA_PROPERTY_RW(cell_limit, cell_limit);
			UMBRA_PROPERTY_RW(restricted_radius, restricted_radius);
			UMBRA_PROPERTY_RW(max_light_radius, max_light_radius);
			UMBRA_PROPERTY_RW(disable_create_url, disable_create_url);
			UMBRA_PROPERTY_RW(allow_object_select, allow_object_select);
			UMBRA_PROPERTY_RW(allow_3_axis_rotation, allow_3_axis_rotation);
			UMBRA_PROPERTY_RW(allow_tourist_build, allow_tourist_build);
			UMBRA_PROPERTY_RW(mover_empty_reset_timeout, mover_empty_reset_timeout);
			UMBRA_PROPERTY_RW(mover_used_reset_timeout, mover_used_reset_timeout);
			UMBRA_PROPERTY_RW(enable_bump_event, enable_bump_event);
			UMBRA_PROPERTY_RW(enable_sync_events, enable_sync_events);
			UMBRA_PROPERTY_RW(fog_enable, fog_enable);
			UMBRA_PROPERTY_RW(fog_minimum, fog_minimum);
			UMBRA_PROPERTY_RW(fog_maximum, fog_maximum);
			UMBRA_PROPERTY_RW(fog_tinted, fog_tinted);
			UMBRA_PROPERTY_RW(fog, fog);
			UMBRA_PROPERTY_RW(minimum_visibility, minimum_visibility);
			UMBRA_PROPERTY_RW(light_x, light_x);
			UMBRA_PROPERTY_RW(light_y, light_y);
			UMBRA_PROPERTY_RW(light_z, light_z);
			UMBRA_PROPERTY_RW(light, light);
			UMBRA_PROPERTY_RW(ambient_light, ambient_light);
			UMBRA_PROPERTY_RW(light_texture, light_texture);
			UMBRA_PROPERTY_RW(light_mask, light_mask);
			UMBRA_PROPERTY_RW(light_draw_size, light_draw_size);
			UMBRA_PROPERTY_RW(light_draw_front, light_draw_front);
			UMBRA_PROPERTY_RW(light_draw_bright, light_draw_bright);
			UMBRA_PROPERTY_RW(light_source_use_color, light_source_use_color);
			UMBRA_PROPERTY_RW(light_source_color, light_source_color);
			UMBRA_PROPERTY_RW(disable_shadows, disable_shadows);
			UMBRA_PROPERTY_RW(entry_point, entry_point);
			UMBRA_PROPERTY_RW(allow_passthru, allow_passthru);
			UMBRA_PROPERTY_RW(allow_flying, allow_flying);
			UMBRA_PROPERTY_RW(allow_teleport, allow_teleport);
			UMBRA_PROPERTY_RW(allow_avatar_collision, allow_avatar_collision);
			UMBRA_PROPERTY_RW(gravity, gravity);
			UMBRA_PROPERTY_RW(friction, friction);
			UMBRA_PROPERTY_RW(slopeslide_enabled, slopeslide_enabled);
			UMBRA_PROPERTY_RW(slopeslide_min_angle, slopeslide_min_angle);
			UMBRA_PROPERTY_RW(slopeslide_max_angle, slopeslide_max_angle);
			UMBRA_PROPERTY_RW(enable_camera_collision, enable_camera_collision);
			UMBRA_PROPERTY_RW(camera_zoom, camera_zoom);
			UMBRA_PROPERTY_RW(backdrop, backdrop);
			UMBRA_PROPERTY_RW(skybox, skybox);
			UMBRA_PROPERTY_RW(sky_north, sky_north);
			UMBRA_PROPERTY_RW(sky_south, sky_south);
			UMBRA_PROPERTY_RW(sky_east, sky_east);
			UMBRA_PROPERTY_RW(sky_west, sky_west);
			UMBRA_PROPERTY_RW(sky_top, sky_top);
			UMBRA_PROPERTY_RW(sky_bottom, sky_bottom);
			UMBRA_PROPERTY_RW(clouds_layer1_texture, clouds_layer1_texture);
			UMBRA_PROPERTY_RW(clouds_layer1_mask, clouds_layer1_mask);
			UMBRA_PROPERTY_RW(clouds_layer1_tile, clouds_layer1_tile);
			UMBRA_PROPERTY_RW(clouds_layer1_speed_x, clouds_layer1_speed_x);
			UMBRA_PROPERTY_RW(clouds_layer1_speed_z, clouds_layer1_speed_z);
			UMBRA_PROPERTY_RW(clouds_layer1_opacity, clouds_layer1_opacity);
			UMBRA_PROPERTY_RW(clouds_layer2_texture, clouds_layer2_texture);
			UMBRA_PROPERTY_RW(clouds_layer2_mask, clouds_layer2_mask);
			UMBRA_PROPERTY_RW(clouds_layer2_tile, clouds_layer2_tile);
			UMBRA_PROPERTY_RW(clouds_layer2_speed_x, clouds_layer2_speed_x);
			UMBRA_PROPERTY_RW(clouds_layer2_speed_z, clouds_layer2_speed_z);
			UMBRA_PROPERTY_RW(clouds_layer2_opacity, clouds_layer2_opacity);
			UMBRA_PROPERTY_RW(clouds_layer3_texture, clouds_layer3_texture);
			UMBRA_PROPERTY_RW(clouds_layer3_mask, clouds_layer3_mask);
			UMBRA_PROPERTY_RW(clouds_layer3_tile, clouds_layer3_tile);
			UMBRA_PROPERTY_RW(clouds_layer3_speed_x, clouds_layer3_speed_x);
			UMBRA_PROPERTY_RW(clouds_layer3_speed_z, clouds_layer3_speed_z);
			UMBRA_PROPERTY_RW(clouds_layer3_opacity, clouds_layer3_opacity);
			UMBRA_PROPERTY_RW(disable_chat, disable_chat);
			UMBRA_PROPERTY_RW(allow_citizen_whisper, allow_citizen_whisper);
			UMBRA_PROPERTY_RW(allow_tourist_whisper, allow_tourist_whisper);
			UMBRA_PROPERTY_RW(always_show_names, always_show_names);
			UMBRA_PROPERTY_RW(disable_avatar_list, disable_avatar_list);
			UMBRA_PROPERTY_RW(avatar_refresh_rate, avatar_refresh_rate);
			UMBRA_PROPERTY_RW(enable_cav, enable_cav);
			UMBRA_PROPERTY_RW(enable_pav, enable_pav);
			UMBRA_PROPERTY_RW(chat_disable_url_clicks, chat_disable_url_clicks);
			UMBRA_PROPERTY_RW(disable_multiple_media, disable_multiple_media);
			UMBRA_PROPERTY_RW(botmenu_url, botmenu_url);
			UMBRA_PROPERTY_RW(voip_conference_global, voip_conference_global);
			UMBRA_PROPERTY_RW(voip_moderate_global, voip_moderate_global);
			UMBRA_PROPERTY_RW(water_enabled, water_enabled);
			UMBRA_PROPERTY_RW(water_texture, water_texture);
			UMBRA_PROPERTY_RW(water_mask, water_mask);
			UMBRA_PROPERTY_RW(water_bottom_texture, water_bottom_texture);
			UMBRA_PROPERTY_RW(water_bottom_mask, water_bottom_mask);
			UMBRA_PROPERTY_RW(water_opacity, water_opacity);
			UMBRA_PROPERTY_RW(water_visibility, water_visibility);
			UMBRA_PROPERTY_RW(water, water);
			UMBRA_PROPERTY_RW(water_level, water_level);
			UMBRA_PROPERTY_RW(water_surface_move, water_surface_move);
			UMBRA_PROPERTY_RW(water_wave_move, water_wave_move);
			UMBRA_PROPERTY_RW(water_speed, water_speed);
			UMBRA_PROPERTY_RW(buoyancy, buoyancy);
			UMBRA_PROPERTY_RW(water_friction, water_friction);
			UMBRA_PROPERTY_RW(water_under_terrain, water_under_terrain);
			UMBRA_PROPERTY_RW(sound_footstep, sound_footstep);
			UMBRA_PROPERTY_RW(sound_water_enter, sound_water_enter);
			UMBRA_PROPERTY_RW(sound_water_exit, sound_water_exit);
			UMBRA_PROPERTY_RW(sound_ambient, sound_ambient);

			
			throw Umbra::UnsupportedPropertyException(R);
		}
	}
}