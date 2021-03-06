#pragma once
#include "awsdk_colours.h"
#include "awsdk_support.h"
#include "awsdk_group_map.h"

namespace AW
{
	class Bot;
	class GroupMap;
	
	namespace World
	{
		class Attributes
		{
		public: // world attributes
			Ascii     enter_right;                            // attribute: AW_WORLD_ENTER_RIGHT
			Ascii     build_right;                            // attribute: AW_WORLD_BUILD_RIGHT
			Ascii     voip_right;                             // attribute: AW_WORLD_VOIP_RIGHT
			Ascii     eminent_domain_right;                   // attribute: AW_WORLD_EMINENT_DOMAIN_RIGHT
			Ascii     special_objects_right;                  // attribute: AW_WORLD_SPECIAL_OBJECTS_RIGHT
			Ascii     public_speaker_right;                   // attribute: AW_WORLD_PUBLIC_SPEAKER_RIGHT
			Ascii     bots_right;                             // attribute: AW_WORLD_BOTS_RIGHT
			Ascii     eject_right;                            // attribute: AW_WORLD_EJECT_RIGHT
			Ascii     speak_right;                            // attribute: AW_WORLD_SPEAK_RIGHT
			Ascii     special_commands_right;                 // attribute: AW_WORLD_SPECIAL_COMMANDS_RIGHT
			Ascii     special_commands;                       // attribute: AW_WORLD_SPECIAL_COMMANDS
			Ascii     v4_objects_right;                       // attribute: AW_WORLD_V4_OBJECTS_RIGHT
			Ascii     title;                                  // attribute: AW_WORLD_TITLE
			Ascii     home_page;                              // attribute: AW_WORLD_HOME_PAGE
			int       rating;                                 // attribute: AW_WORLD_RATING
			Ascii     welcome_message;                        // attribute: AW_WORLD_WELCOME_MESSAGE
			Ascii     keywords;                               // attribute: AW_WORLD_KEYWORDS
			Ascii     ground;                                 // attribute: AW_WORLD_GROUND
			BOOL      repeating_ground;                       // attribute: AW_WORLD_REPEATING_GROUND
			BOOL      enable_terrain;                         // attribute: AW_WORLD_ENABLE_TERRAIN
			float     terrain_ambient;                        // attribute: AW_WORLD_TERRAIN_AMBIENT
			float     terrain_diffuse;                        // attribute: AW_WORLD_TERRAIN_DIFFUSE
			float     terrain_offset;                         // attribute: AW_WORLD_TERRAIN_OFFSET
			Ascii     object_path;                            // attribute: AW_WORLD_OBJECT_PATH
			int       object_refresh;                         // attribute: AW_WORLD_OBJECT_REFRESH
			Ascii     object_password;                        // attribute: AW_WORLD_OBJECT_PASSWORD
			BOOL      enable_referer;                         // attribute: AW_WORLD_ENABLE_REFERER
			int       cell_limit;                             // attribute: AW_WORLD_CELL_LIMIT
			int       restricted_radius;                      // attribute: AW_WORLD_RESTRICTED_RADIUS
			int       max_light_radius;                       // attribute: AW_WORLD_MAX_LIGHT_RADIUS
			BOOL      disable_create_url;                     // attribute: AW_WORLD_DISABLE_CREATE_URL
			BOOL      allow_object_select;                    // attribute: AW_WORLD_ALLOW_OBJECT_SELECT
			BOOL      allow_3_axis_rotation;                  // attribute: AW_WORLD_ALLOW_3_AXIS_ROTATION
			BOOL      allow_tourist_build;                    // attribute: AW_WORLD_ALLOW_TOURIST_BUILD
			int       mover_empty_reset_timeout;              // attribute: AW_WORLD_MOVER_EMPTY_RESET_TIMEOUT
			int       mover_used_reset_timeout;               // attribute: AW_WORLD_MOVER_USED_RESET_TIMEOUT
			BOOL      enable_bump_event;                      // attribute: AW_WORLD_ENABLE_BUMP_EVENT
			BOOL      enable_sync_events;                     // attribute: AW_WORLD_ENABLE_SYNC_EVENTS
			BOOL      fog_enable;                             // attribute: AW_WORLD_FOG_ENABLE
			int       fog_minimum;                            // attribute: AW_WORLD_FOG_MINIMUM
			int       fog_maximum;                            // attribute: AW_WORLD_FOG_MAXIMUM
			BOOL      fog_tinted;                             // attribute: AW_WORLD_FOG_TINTED
			Colour    fog_colour;                             // attribute: AW_WORLD_FOG_RED
			int       minimum_visibility;                     // attribute: AW_WORLD_MINIMUM_VISIBILITY
			float     light_x;                                // attribute: AW_WORLD_LIGHT_X
			float     light_y;                                // attribute: AW_WORLD_LIGHT_Y
			float     light_z;                                // attribute: AW_WORLD_LIGHT_Z
			Colour    light_colour;                           // attribute: AW_WORLD_LIGHT_RED
			Colour    ambient_light_colour;                   // attribute: AW_WORLD_AMBIENT_LIGHT_RED
			Ascii     light_texture;                          // attribute: AW_WORLD_LIGHT_TEXTURE
			Ascii     light_mask;                             // attribute: AW_WORLD_LIGHT_MASK
			int       light_draw_size;                        // attribute: AW_WORLD_LIGHT_DRAW_SIZE
			BOOL      light_draw_front;                       // attribute: AW_WORLD_LIGHT_DRAW_FRONT
			BOOL      light_draw_bright;                      // attribute: AW_WORLD_LIGHT_DRAW_BRIGHT
			BOOL      light_source_use_color;                 // attribute: AW_WORLD_LIGHT_SOURCE_USE_COLOR
			Colour    light_source_color;                     // attribute: AW_WORLD_LIGHT_SOURCE_COLOR
			BOOL      disable_shadows;                        // attribute: AW_WORLD_DISABLE_SHADOWS
			Ascii     entry_point;                            // attribute: AW_WORLD_ENTRY_POINT
			BOOL      allow_passthru;                         // attribute: AW_WORLD_ALLOW_PASSTHRU
			BOOL      allow_flying;                           // attribute: AW_WORLD_ALLOW_FLYING
			BOOL      allow_teleport;                         // attribute: AW_WORLD_ALLOW_TELEPORT
			BOOL      allow_avatar_collision;                 // attribute: AW_WORLD_ALLOW_AVATAR_COLLISION
			float     gravity;                                // attribute: AW_WORLD_GRAVITY
			float     friction;                               // attribute: AW_WORLD_FRICTION
			BOOL      slopeslide_enabled;                     // attribute: AW_WORLD_SLOPESLIDE_ENABLED
			float     slopeslide_min_angle;                   // attribute: AW_WORLD_SLOPESLIDE_MIN_ANGLE
			float     slopeslide_max_angle;                   // attribute: AW_WORLD_SLOPESLIDE_MAX_ANGLE
			BOOL      enable_camera_collision;                // attribute: AW_WORLD_ENABLE_CAMERA_COLLISION
			Ascii     backdrop;                               // attribute: AW_WORLD_BACKDROP
			Ascii     skybox;                                 // attribute: AW_WORLD_SKYBOX
			Colour    sky_north_colour;                       // attribute: AW_WORLD_SKY_NORTH_RED
			Colour    sky_south_colour;                       // attribute: AW_WORLD_SKY_SOUTH_RED
			Colour    sky_east_colour;                        // attribute: AW_WORLD_SKY_EAST_RED
			Colour    sky_west_colour;                        // attribute: AW_WORLD_SKY_WEST_RED
			Colour    sky_top_colour;                         // attribute: AW_WORLD_SKY_TOP_RED
			Colour    sky_bottom_colour;                      // attribute: AW_WORLD_SKY_BOTTOM_RED
			Ascii     clouds_layer1_texture;                  // attribute: AW_WORLD_CLOUDS_LAYER1_TEXTURE
			Ascii     clouds_layer1_mask;                     // attribute: AW_WORLD_CLOUDS_LAYER1_MASK
			float     clouds_layer1_tile;                     // attribute: AW_WORLD_CLOUDS_LAYER1_TILE
			float     clouds_layer1_speed_x;                  // attribute: AW_WORLD_CLOUDS_LAYER1_SPEED_X
			float     clouds_layer1_speed_z;                  // attribute: AW_WORLD_CLOUDS_LAYER1_SPEED_Z
			int       clouds_layer1_opacity;                  // attribute: AW_WORLD_CLOUDS_LAYER1_OPACITY
			Ascii     clouds_layer2_texture;                  // attribute: AW_WORLD_CLOUDS_LAYER2_TEXTURE
			Ascii     clouds_layer2_mask;                     // attribute: AW_WORLD_CLOUDS_LAYER2_MASK
			float     clouds_layer2_tile;                     // attribute: AW_WORLD_CLOUDS_LAYER2_TILE
			float     clouds_layer2_speed_x;                  // attribute: AW_WORLD_CLOUDS_LAYER2_SPEED_X
			float     clouds_layer2_speed_z;                  // attribute: AW_WORLD_CLOUDS_LAYER2_SPEED_Z
			int       clouds_layer2_opacity;                  // attribute: AW_WORLD_CLOUDS_LAYER2_OPACITY
			Ascii     clouds_layer3_texture;                  // attribute: AW_WORLD_CLOUDS_LAYER3_TEXTURE
			Ascii     clouds_layer3_mask;                     // attribute: AW_WORLD_CLOUDS_LAYER3_MASK
			float     clouds_layer3_tile;                     // attribute: AW_WORLD_CLOUDS_LAYER3_TILE
			float     clouds_layer3_speed_x;                  // attribute: AW_WORLD_CLOUDS_LAYER3_SPEED_X
			float     clouds_layer3_speed_z;                  // attribute: AW_WORLD_CLOUDS_LAYER3_SPEED_Z
			int       clouds_layer3_opacity;                  // attribute: AW_WORLD_CLOUDS_LAYER3_OPACITY
			BOOL      disable_chat;                           // attribute: AW_WORLD_DISABLE_CHAT
			BOOL      allow_citizen_whisper;                  // attribute: AW_WORLD_ALLOW_CITIZEN_WHISPER
			BOOL      allow_tourist_whisper;                  // attribute: AW_WORLD_ALLOW_TOURIST_WHISPER
			BOOL      always_show_names;                      // attribute: AW_WORLD_ALWAYS_SHOW_NAMES
			BOOL      disable_avatar_list;                    // attribute: AW_WORLD_DISABLE_AVATAR_LIST
			int       avatar_refresh_rate;                    // attribute: AW_WORLD_AVATAR_REFRESH_RATE
			BOOL      enable_cav;                             // attribute: AW_WORLD_ENABLE_CAV
			BOOL      enable_pav;                             // attribute: AW_WORLD_ENABLE_PAV
			BOOL      chat_disable_url_clicks;                // attribute: AW_WORLD_CHAT_DISABLE_URL_CLICKS
			BOOL      disable_multiple_media;                 // attribute: AW_WORLD_DISABLE_MULTIPLE_MEDIA
			Ascii     botmenu_url;                            // attribute: AW_WORLD_BOTMENU_URL
			BOOL      water_enabled;                          // attribute: AW_WORLD_WATER_ENABLED
			Ascii     water_texture;                          // attribute: AW_WORLD_WATER_TEXTURE
			Ascii     water_mask;                             // attribute: AW_WORLD_WATER_MASK
			Ascii     water_bottom_texture;                   // attribute: AW_WORLD_WATER_BOTTOM_TEXTURE
			Ascii     water_bottom_mask;                      // attribute: AW_WORLD_WATER_BOTTOM_MASK
			int       water_opacity;                          // attribute: AW_WORLD_WATER_OPACITY
			int       water_visibility;                       // attribute: AW_WORLD_WATER_VISIBILITY
			Colour    water_colour;                           // attribute: AW_WORLD_WATER_RED
			float     water_level;                            // attribute: AW_WORLD_WATER_LEVEL
			float     water_surface_move;                     // attribute: AW_WORLD_WATER_SURFACE_MOVE
			float     water_wave_move;                        // attribute: AW_WORLD_WATER_WAVE_MOVE
			float     water_speed;                            // attribute: AW_WORLD_WATER_SPEED
			float     buoyancy;                               // attribute: AW_WORLD_BUOYANCY
			float     water_friction;                         // attribute: AW_WORLD_WATER_FRICTION
			BOOL      water_under_terrain;                    // attribute: AW_WORLD_WATER_UNDER_TERRAIN
			Ascii     sound_footstep;                         // attribute: AW_WORLD_SOUND_FOOTSTEP
			Ascii     sound_water_enter;                      // attribute: AW_WORLD_SOUND_WATER_ENTER
			Ascii     sound_water_exit;                       // attribute: AW_WORLD_SOUND_WATER_EXIT
			Ascii     sound_ambient;                          // attribute: AW_WORLD_SOUND_AMBIENT

		
		public: // functions
			void	from_sdk   (AW::Bot& Client);
			void	to_sdk     (AW::Bot& Client);
			void    dx_sdk     (AW::Bot& Client, bool ExchangeMode);
			
			// reading and writing files
			errno_t write_file (Ascii FilePath);
			errno_t	read_file  (Ascii FilePath);
			AW::EnhancedINI    get_EnhancedINI();
			void               set_EnhancedINI(AW::EnhancedINI& INI);
			void    dx_file    (AW::EnhancedINI& INI, bool Direction);
			
		public: // operators
			bool	attributes_equal(const Attributes& attr);
			
		public: // string read and write operators
		    Ascii   get_by_name(Ascii AttributeID);
			bool	set_by_name(Ascii AttributeID, Ascii Value);
			
		public: // GroupMap
			static GroupMap& StaticGroupMap();		
		};
	}
}