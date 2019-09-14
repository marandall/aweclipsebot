#include "stdafx.h"
#include "EclipseEvolution.h"
#include "WorldOptionsDialog.h"

void WorldOptionsDialog::select_combo_by_index(CComboBox& CB, int Index)
{
	for (int i = 0; i < CB.GetCount(); i++)
		if (CB.GetItemData(i) == Index)
			CB.SetCurSel(i);
}

void WorldOptionsDialog::create_objects()
{
	PropertyScriptCtrl& pcs = *c_pProperties;
	controls.c_pEnterRight            = pcs.create_edit  (WAG_RIGHT           , "", "Enter Right");
	controls.c_pBuildRight            = pcs.create_edit  (WAG_RIGHT           , "", "Build Right");
	controls.c_pVoipRight             = pcs.create_edit  (WAG_RIGHT           , "", "Voip Right");
	controls.c_pEminentDomainRight    = pcs.create_edit  (WAG_RIGHT           , "", "Eminent Domain Right");
	controls.c_pSpecialObjectsRight   = pcs.create_edit  (WAG_RIGHT           , "", "Special Objects Right");
	controls.c_pPublicSpeakerRight    = pcs.create_edit  (WAG_RIGHT           , "", "Public Speaker Right");
	controls.c_pBotsRight             = pcs.create_edit  (WAG_RIGHT           , "", "Bots Right");
	controls.c_pEjectRight            = pcs.create_edit  (WAG_RIGHT           , "", "Eject Right");
	controls.c_pSpeakRight            = pcs.create_edit  (WAG_RIGHT           , "", "Speak Right");
	controls.c_pSpecialCommandsRight  = pcs.create_edit  (WAG_RIGHT           , "", "Special Commands Right");
	controls.c_pSpecialCommands       = pcs.create_edit  (WAG_RIGHT           , "", "Special Commands");
	controls.c_pV4ObjectsRight        = pcs.create_edit  (WAG_RIGHT           , "", "V4 Objects Right");
	controls.c_pTerrainRight          = pcs.create_edit  (WAG_RIGHT           , "", "Terrain Right");
	controls.c_pTitle                 = pcs.create_edit  (WAG_GENERAL         , "", "Title");
	controls.c_pHomePage              = pcs.create_edit  (WAG_GENERAL         , "", "Home Page");
	controls.c_pRating                = pcs.create_combo (WAG_GENERAL         , "", "Rating");
	controls.c_pWelcomeMessage        = pcs.create_edit  (WAG_GENERAL         , "", "Welcome Message");
	controls.c_pKeywords              = pcs.create_edit  (WAG_GENERAL         , "", "Keywords");
	controls.c_pGround                = pcs.create_edit  (WAG_GROUND          , "", "Ground");
	controls.c_pRepeatingGround       = pcs.create_check (WAG_GROUND          , "", "Repeating Ground");
	controls.c_pEnableTerrain         = pcs.create_check (WAG_GROUND          , "", "Enable Terrain");
	controls.c_pTerrainAmbient        = pcs.create_edit  (WAG_GROUND          , "", "Terrain Ambient");
	controls.c_pTerrainDiffuse        = pcs.create_edit  (WAG_GROUND          , "", "Terrain Diffuse");
	controls.c_pTerrainOffset         = pcs.create_edit  (WAG_GROUND          , "", "Terrain Offset");
	controls.c_pObjectPath            = pcs.create_edit  (WAG_OBJECT_PATH     , "", "Object Path");
	controls.c_pObjectRefresh         = pcs.create_edit  (WAG_OBJECT_PATH     , "", "Object Refresh");
	controls.c_pObjectPassword        = pcs.create_edit  (WAG_OBJECT_PATH     , "", "Object Password");
	controls.c_pEnableReferer         = pcs.create_check (WAG_OBJECT_PATH     , "", "Enable Referer");
	controls.c_pCavObjectPath         = pcs.create_edit  (WAG_OBJECT_PATH     , "", "Cav Object Path");
	controls.c_pCavObjectPassword     = pcs.create_edit  (WAG_OBJECT_PATH     , "", "Cav Object Password");
	controls.c_pCavObjectRefresh      = pcs.create_edit  (WAG_OBJECT_PATH     , "", "Cav Object Refresh");
	controls.c_pCellLimit             = pcs.create_combo (WAG_BUILDING        , "", "Cell Limit");
	controls.c_pRestrictedRadius      = pcs.create_edit  (WAG_BUILDING        , "", "Restricted Radius");
	controls.c_pMaxLightRadius        = pcs.create_edit  (WAG_BUILDING        , "", "Max Light Radius");
	controls.c_pDisableCreateUrl      = pcs.create_check (WAG_BUILDING        , "", "Disable Create Url");
	controls.c_pAllowObjectSelect     = pcs.create_check (WAG_BUILDING        , "", "Allow Object Select");
	controls.c_pAllow3AxisRotation    = pcs.create_check (WAG_BUILDING        , "", "Allow 3 Axis Rotation");
	controls.c_pAllowTouristBuild     = pcs.create_check (WAG_BUILDING        , "", "Allow Tourist Build");
	controls.c_pMoverEmptyResetTimeout= pcs.create_edit  (WAG_BUILDING        , "", "Mover Empty Reset Timeout");
	controls.c_pMoverUsedResetTimeout = pcs.create_edit  (WAG_BUILDING        , "", "Mover Used Reset Timeout");
	controls.c_pEnableBumpEvent       = pcs.create_check (WAG_BUILDING        , "", "Enable Bump Event");
	controls.c_pEnableSyncEvents      = pcs.create_check (WAG_BUILDING        , "", "Enable Sync Events");
	controls.c_pFogEnable             = pcs.create_check (WAG_FOG             , "", "Fog Enable");
	controls.c_pFogMinimum            = pcs.create_edit  (WAG_FOG             , "", "Fog Minimum");
	controls.c_pFogMaximum            = pcs.create_edit  (WAG_FOG             , "", "Fog Maximum");
	controls.c_pFogTinted             = pcs.create_check (WAG_FOG             , "", "Fog Tinted");
	controls.c_pFog                   = pcs.create_colour(WAG_FOG             , "", "Fog");
	controls.c_pMinimumVisibility     = pcs.create_combo (WAG_LIGHTING        , "", "Minimum Visibility");
	controls.c_pLightX                = pcs.create_edit  (WAG_LIGHTING        , "", "Light X");
	controls.c_pLightY                = pcs.create_edit  (WAG_LIGHTING        , "", "Light Y");
	controls.c_pLightZ                = pcs.create_edit  (WAG_LIGHTING        , "", "Light Z");
	controls.c_pLight                 = pcs.create_colour(WAG_LIGHTING        , "", "Light");
	controls.c_pAmbientLight          = pcs.create_colour(WAG_LIGHTING        , "", "Ambient Light");
	controls.c_pLightTexture          = pcs.create_edit  (WAG_LIGHTING        , "", "Light Texture");
	controls.c_pLightMask             = pcs.create_edit  (WAG_LIGHTING        , "", "Light Mask");
	controls.c_pLightDrawSize         = pcs.create_edit  (WAG_LIGHTING        , "", "Light Draw Size");
	controls.c_pLightDrawFront        = pcs.create_check (WAG_LIGHTING        , "", "Light Draw Front");
	controls.c_pLightDrawBright       = pcs.create_check (WAG_LIGHTING        , "", "Light Draw Bright");
	controls.c_pLightSourceUseColor   = pcs.create_check (WAG_LIGHTING        , "", "Light Source Use Color");
	controls.c_pLightSourceColor      = pcs.create_colour(WAG_LIGHTING        , "", "Light Source Color");
	controls.c_pDisableShadows        = pcs.create_check (WAG_LIGHTING        , "", "Disable Shadows");
	controls.c_pEntryPoint            = pcs.create_edit  (WAG_MOVEMENT        , "", "Entry Point");
	controls.c_pAllowPassthru         = pcs.create_check (WAG_MOVEMENT        , "", "Allow Passthru");
	controls.c_pAllowFlying           = pcs.create_check (WAG_MOVEMENT        , "", "Allow Flying");
	controls.c_pAllowTeleport         = pcs.create_check (WAG_MOVEMENT        , "", "Allow Teleport");
	controls.c_pAllowAvatarCollision  = pcs.create_check (WAG_MOVEMENT        , "", "Allow Avatar Collision");
	controls.c_pGravity               = pcs.create_edit  (WAG_MOVEMENT        , "", "Gravity");
	controls.c_pFriction              = pcs.create_edit  (WAG_MOVEMENT        , "", "Friction");
	controls.c_pSlopeslideEnabled     = pcs.create_check (WAG_MOVEMENT        , "", "Slopeslide Enabled");
	controls.c_pSlopeslideMinAngle    = pcs.create_edit  (WAG_MOVEMENT        , "", "Slopeslide Min Angle");
	controls.c_pSlopeslideMaxAngle    = pcs.create_edit  (WAG_MOVEMENT        , "", "Slopeslide Max Angle");
	controls.c_pEnableCameraCollision = pcs.create_check (WAG_MOVEMENT        , "", "Enable Camera Collision");
	controls.c_pCameraZoom            = pcs.create_edit  (WAG_MOVEMENT        , "", "Camera Zoom");
	controls.c_pBackdrop              = pcs.create_edit  (WAG_SKY             , "", "Backdrop");
	controls.c_pSkybox                = pcs.create_edit  (WAG_SKY             , "", "Skybox");
	controls.c_pSkyNorth              = pcs.create_colour(WAG_SKY             , "", "Sky North");
	controls.c_pSkySouth              = pcs.create_colour(WAG_SKY             , "", "Sky South");
	controls.c_pSkyEast               = pcs.create_colour(WAG_SKY             , "", "Sky East");
	controls.c_pSkyWest               = pcs.create_colour(WAG_SKY             , "", "Sky West");
	controls.c_pSkyTop                = pcs.create_colour(WAG_SKY             , "", "Sky Top");
	controls.c_pSkyBottom             = pcs.create_colour(WAG_SKY             , "", "Sky Bottom");
	controls.c_pCloudsLayer1Texture   = pcs.create_edit  (WAG_CLOUDS_1        , "", "Layer1 Texture");
	controls.c_pCloudsLayer1Mask      = pcs.create_edit  (WAG_CLOUDS_1        , "", "Layer1 Mask");
	controls.c_pCloudsLayer1Tile      = pcs.create_edit  (WAG_CLOUDS_1        , "", "Layer1 Tile");
	controls.c_pCloudsLayer1SpeedX    = pcs.create_edit  (WAG_CLOUDS_1        , "", "Layer1 Speed X");
	controls.c_pCloudsLayer1SpeedZ    = pcs.create_edit  (WAG_CLOUDS_1        , "", "Layer1 Speed Z");
	controls.c_pCloudsLayer1Opacity   = pcs.create_edit  (WAG_CLOUDS_1        , "", "Layer1 Opacity");
	controls.c_pCloudsLayer2Texture   = pcs.create_edit  (WAG_CLOUDS_2        , "", "Layer2 Texture");
	controls.c_pCloudsLayer2Mask      = pcs.create_edit  (WAG_CLOUDS_2        , "", "Layer2 Mask");
	controls.c_pCloudsLayer2Tile      = pcs.create_edit  (WAG_CLOUDS_2        , "", "Layer2 Tile");
	controls.c_pCloudsLayer2SpeedX    = pcs.create_edit  (WAG_CLOUDS_2        , "", "Layer2 Speed X");
	controls.c_pCloudsLayer2SpeedZ    = pcs.create_edit  (WAG_CLOUDS_2        , "", "Layer2 Speed Z");
	controls.c_pCloudsLayer2Opacity   = pcs.create_edit  (WAG_CLOUDS_2        , "", "Layer2 Opacity");
	controls.c_pCloudsLayer3Texture   = pcs.create_edit  (WAG_CLOUDS_3        , "", "Layer3 Texture");
	controls.c_pCloudsLayer3Mask      = pcs.create_edit  (WAG_CLOUDS_3        , "", "Layer3 Mask");
	controls.c_pCloudsLayer3Tile      = pcs.create_edit  (WAG_CLOUDS_3        , "", "Layer3 Tile");
	controls.c_pCloudsLayer3SpeedX    = pcs.create_edit  (WAG_CLOUDS_3        , "", "Layer3 Speed X");
	controls.c_pCloudsLayer3SpeedZ    = pcs.create_edit  (WAG_CLOUDS_3        , "", "Layer3 Speed Z");
	controls.c_pCloudsLayer3Opacity   = pcs.create_edit  (WAG_CLOUDS_3        , "", "Layer3 Opacity");
	controls.c_pDisableChat           = pcs.create_check (WAG_COMMUNICATIONS  , "", "Disable Chat");
	controls.c_pAllowCitizenWhisper   = pcs.create_check (WAG_COMMUNICATIONS  , "", "Allow Citizen Whisper");
	controls.c_pAllowTouristWhisper   = pcs.create_check (WAG_COMMUNICATIONS  , "", "Allow Tourist Whisper");
	controls.c_pAlwaysShowNames       = pcs.create_check (WAG_COMMUNICATIONS  , "", "Always Show Names");
	controls.c_pDisableAvatarList     = pcs.create_check (WAG_AVATARS         , "", "Disable Avatar List");
	controls.c_pAvatarRefreshRate     = pcs.create_edit  (WAG_AVATARS         , "", "Avatar Refresh Rate");
	controls.c_p26347 = pcs.create_label(WAG_AVATARS, "", "Custom Avatar Options");
	controls.c_pEnableCavAwEnableCavCitizen = pcs.create_check(WAG_AVATARS, "", "-  CAV Citizens");
	controls.c_pEnableCavAwEnableCavBot = pcs.create_check(WAG_AVATARS, "", "-  CAV Bot");
	controls.c_pEnableCavAwEnableCavTourist = pcs.create_check(WAG_AVATARS, "", "-  CAV Tourist");
	controls.c_pEnableCavAwEnableCavWorldCitizen = pcs.create_check(WAG_AVATARS, "", "-  CAV World Citizen");
	controls.c_pEnableCavAwEnableCavWorldBot = pcs.create_check(WAG_AVATARS, "", "-  CAV World Bot");
	controls.c_pEnableCavAwEnableCavWorldTourist = pcs.create_check(WAG_AVATARS, "", "-  CAV World Tourist");
	controls.c_pEnablePav             = pcs.create_check (WAG_AVATARS         , "", "Enable Pav");
	controls.c_pChatDisableUrlClicks  = pcs.create_check (WAG_COMMUNICATIONS  , "", "Chat Disable Url Clicks");
	controls.c_pDisableMultipleMedia  = pcs.create_check (WAG_COMMUNICATIONS  , "", "Disable Multiple Media");
	controls.c_pBotmenuUrl            = pcs.create_edit  (WAG_COMMUNICATIONS  , "", "Botmenu Url");
	controls.c_pVoipConferenceGlobal  = pcs.create_check (WAG_COMMUNICATIONS  , "", "Voip Conference Global");
	controls.c_pVoipModerateGlobal    = pcs.create_check (WAG_COMMUNICATIONS  , "", "Voip Moderate Global");
	controls.c_pWaterEnabled          = pcs.create_check (WAG_WATER           , "", "Enabled");
	controls.c_pWaterTexture          = pcs.create_edit  (WAG_WATER           , "", "Texture");
	controls.c_pWaterMask             = pcs.create_edit  (WAG_WATER           , "", "Mask");
	controls.c_pWaterBottomTexture    = pcs.create_edit  (WAG_WATER           , "", "Bottom Texture");
	controls.c_pWaterBottomMask       = pcs.create_edit  (WAG_WATER           , "", "Bottom Mask");
	controls.c_pWaterOpacity          = pcs.create_edit  (WAG_WATER           , "", "Opacity");
	controls.c_pWaterVisibility       = pcs.create_edit  (WAG_WATER           , "", "Visibility");
	controls.c_pWater                 = pcs.create_colour(WAG_WATER           , "", "");
	controls.c_pWaterLevel            = pcs.create_edit  (WAG_WATER           , "", "Level");
	controls.c_pWaterSurfaceMove      = pcs.create_edit  (WAG_WATER           , "", "Surface Move");
	controls.c_pWaterWaveMove         = pcs.create_edit  (WAG_WATER           , "", "Wave Move");
	controls.c_pWaterSpeed            = pcs.create_edit  (WAG_WATER           , "", "Speed");
	controls.c_pBuoyancy              = pcs.create_edit  (WAG_WATER           , "", "Buoyancy");
	controls.c_pWaterFriction         = pcs.create_edit  (WAG_WATER           , "", "Friction");
	controls.c_pWaterUnderTerrain     = pcs.create_check (WAG_WATER           , "", "Under Terrain");
	controls.c_pSoundFootstep         = pcs.create_edit  (WAG_SOUND           , "", "Sound Footstep");
	controls.c_pSoundWaterEnter       = pcs.create_edit  (WAG_SOUND           , "", "Sound  Enter");
	controls.c_pSoundWaterExit        = pcs.create_edit  (WAG_SOUND           , "", "Sound  Exit");
	controls.c_pSoundAmbient          = pcs.create_edit  (WAG_SOUND           , "", "Sound Ambient");

}

void WorldOptionsDialog::attributes_to_screen(WorldAttributes& Attr)
{
	controls.c_pEnterRight            ->OverloadedSet(Attr.enter_right);
	controls.c_pBuildRight            ->OverloadedSet(Attr.build_right);
	controls.c_pVoipRight             ->OverloadedSet(Attr.voip_right);
	controls.c_pEminentDomainRight    ->OverloadedSet(Attr.eminent_domain_right);
	controls.c_pSpecialObjectsRight   ->OverloadedSet(Attr.special_objects_right);
	controls.c_pPublicSpeakerRight    ->OverloadedSet(Attr.public_speaker_right);
	controls.c_pBotsRight             ->OverloadedSet(Attr.bots_right);
	controls.c_pEjectRight            ->OverloadedSet(Attr.eject_right);
	controls.c_pSpeakRight            ->OverloadedSet(Attr.speak_right);
	controls.c_pSpecialCommandsRight  ->OverloadedSet(Attr.special_commands_right);
	controls.c_pSpecialCommands       ->OverloadedSet(Attr.special_commands);
	controls.c_pV4ObjectsRight        ->OverloadedSet(Attr.v4_objects_right);
	controls.c_pTerrainRight          ->OverloadedSet(Attr.terrain_right);
	controls.c_pTitle                 ->OverloadedSet(Attr.title);
	controls.c_pHomePage              ->OverloadedSet(Attr.home_page);
	select_combo_by_index(*controls.c_pRating, Attr.rating);
	controls.c_pWelcomeMessage        ->OverloadedSet(Attr.welcome_message);
	controls.c_pKeywords              ->OverloadedSet(Attr.keywords);
	controls.c_pGround                ->OverloadedSet(Attr.ground);
	controls.c_pRepeatingGround       ->SetCheck(Attr.repeating_ground);
	controls.c_pEnableTerrain         ->SetCheck(Attr.enable_terrain);
	controls.c_pTerrainAmbient        ->OverloadedSet(Attr.terrain_ambient);
	controls.c_pTerrainDiffuse        ->OverloadedSet(Attr.terrain_diffuse);
	controls.c_pTerrainOffset         ->OverloadedSet(Attr.terrain_offset);
	controls.c_pObjectPath            ->OverloadedSet(Attr.object_path);
	controls.c_pObjectRefresh         ->OverloadedSet(Attr.object_refresh);
	controls.c_pObjectPassword        ->OverloadedSet(Attr.object_password);
	controls.c_pEnableReferer         ->SetCheck(Attr.enable_referer);
	controls.c_pCavObjectPath         ->OverloadedSet(Attr.cav_object_path);
	controls.c_pCavObjectPassword     ->OverloadedSet(Attr.cav_object_password);
	controls.c_pCavObjectRefresh      ->OverloadedSet(Attr.cav_object_refresh);
	select_combo_by_index(*controls.c_pCellLimit, Attr.cell_limit);
	controls.c_pRestrictedRadius      ->OverloadedSet(Attr.restricted_radius);
	controls.c_pMaxLightRadius        ->OverloadedSet(Attr.max_light_radius);
	controls.c_pDisableCreateUrl      ->SetCheck(Attr.disable_create_url);
	controls.c_pAllowObjectSelect     ->SetCheck(Attr.allow_object_select);
	controls.c_pAllow3AxisRotation    ->SetCheck(Attr.allow_3_axis_rotation);
	controls.c_pAllowTouristBuild     ->SetCheck(Attr.allow_tourist_build);
	controls.c_pMoverEmptyResetTimeout->OverloadedSet(Attr.mover_empty_reset_timeout);
	controls.c_pMoverUsedResetTimeout ->OverloadedSet(Attr.mover_used_reset_timeout);
	controls.c_pEnableBumpEvent       ->SetCheck(Attr.enable_bump_event);
	controls.c_pEnableSyncEvents      ->SetCheck(Attr.enable_sync_events);
	controls.c_pFogEnable             ->SetCheck(Attr.fog_enable);
	controls.c_pFogMinimum            ->OverloadedSet(Attr.fog_minimum);
	controls.c_pFogMaximum            ->OverloadedSet(Attr.fog_maximum);
	controls.c_pFogTinted             ->SetCheck(Attr.fog_tinted);
	(*controls.c_pFog)       =Attr.fog.value;
	select_combo_by_index(*controls.c_pMinimumVisibility, Attr.minimum_visibility);
	controls.c_pLightX                ->OverloadedSet(Attr.light_x);
	controls.c_pLightY                ->OverloadedSet(Attr.light_y);
	controls.c_pLightZ                ->OverloadedSet(Attr.light_z);
	(*controls.c_pLight)     =Attr.light.value;
	(*controls.c_pAmbientLight)=Attr.ambient_light.value;
	controls.c_pLightTexture          ->OverloadedSet(Attr.light_texture);
	controls.c_pLightMask             ->OverloadedSet(Attr.light_mask);
	controls.c_pLightDrawSize         ->OverloadedSet(Attr.light_draw_size);
	controls.c_pLightDrawFront        ->SetCheck(Attr.light_draw_front);
	controls.c_pLightDrawBright       ->SetCheck(Attr.light_draw_bright);
	controls.c_pLightSourceUseColor   ->SetCheck(Attr.light_source_use_color);
	(*controls.c_pLightSourceColor)=Attr.light_source_color.value;
	controls.c_pDisableShadows        ->SetCheck(Attr.disable_shadows);
	controls.c_pEntryPoint            ->OverloadedSet(Attr.entry_point);
	controls.c_pAllowPassthru         ->SetCheck(Attr.allow_passthru);
	controls.c_pAllowFlying           ->SetCheck(Attr.allow_flying);
	controls.c_pAllowTeleport         ->SetCheck(Attr.allow_teleport);
	controls.c_pAllowAvatarCollision  ->SetCheck(Attr.allow_avatar_collision);
	controls.c_pGravity               ->OverloadedSet(Attr.gravity);
	controls.c_pFriction              ->OverloadedSet(Attr.friction);
	controls.c_pSlopeslideEnabled     ->SetCheck(Attr.slopeslide_enabled);
	controls.c_pSlopeslideMinAngle    ->OverloadedSet(Attr.slopeslide_min_angle);
	controls.c_pSlopeslideMaxAngle    ->OverloadedSet(Attr.slopeslide_max_angle);
	controls.c_pEnableCameraCollision ->SetCheck(Attr.enable_camera_collision);
	controls.c_pCameraZoom            ->OverloadedSet(Attr.camera_zoom);
	controls.c_pBackdrop              ->OverloadedSet(Attr.backdrop);
	controls.c_pSkybox                ->OverloadedSet(Attr.skybox);
	(*controls.c_pSkyNorth)  =Attr.sky_north.value;
	(*controls.c_pSkySouth)  =Attr.sky_south.value;
	(*controls.c_pSkyEast)   =Attr.sky_east.value;
	(*controls.c_pSkyWest)   =Attr.sky_west.value;
	(*controls.c_pSkyTop)    =Attr.sky_top.value;
	(*controls.c_pSkyBottom) =Attr.sky_bottom.value;
	controls.c_pCloudsLayer1Texture   ->OverloadedSet(Attr.clouds_layer1_texture);
	controls.c_pCloudsLayer1Mask      ->OverloadedSet(Attr.clouds_layer1_mask);
	controls.c_pCloudsLayer1Tile      ->OverloadedSet(Attr.clouds_layer1_tile);
	controls.c_pCloudsLayer1SpeedX    ->OverloadedSet(Attr.clouds_layer1_speed_x);
	controls.c_pCloudsLayer1SpeedZ    ->OverloadedSet(Attr.clouds_layer1_speed_z);
	controls.c_pCloudsLayer1Opacity   ->OverloadedSet(Attr.clouds_layer1_opacity);
	controls.c_pCloudsLayer2Texture   ->OverloadedSet(Attr.clouds_layer2_texture);
	controls.c_pCloudsLayer2Mask      ->OverloadedSet(Attr.clouds_layer2_mask);
	controls.c_pCloudsLayer2Tile      ->OverloadedSet(Attr.clouds_layer2_tile);
	controls.c_pCloudsLayer2SpeedX    ->OverloadedSet(Attr.clouds_layer2_speed_x);
	controls.c_pCloudsLayer2SpeedZ    ->OverloadedSet(Attr.clouds_layer2_speed_z);
	controls.c_pCloudsLayer2Opacity   ->OverloadedSet(Attr.clouds_layer2_opacity);
	controls.c_pCloudsLayer3Texture   ->OverloadedSet(Attr.clouds_layer3_texture);
	controls.c_pCloudsLayer3Mask      ->OverloadedSet(Attr.clouds_layer3_mask);
	controls.c_pCloudsLayer3Tile      ->OverloadedSet(Attr.clouds_layer3_tile);
	controls.c_pCloudsLayer3SpeedX    ->OverloadedSet(Attr.clouds_layer3_speed_x);
	controls.c_pCloudsLayer3SpeedZ    ->OverloadedSet(Attr.clouds_layer3_speed_z);
	controls.c_pCloudsLayer3Opacity   ->OverloadedSet(Attr.clouds_layer3_opacity);
	controls.c_pDisableChat           ->SetCheck(Attr.disable_chat);
	controls.c_pAllowCitizenWhisper   ->SetCheck(Attr.allow_citizen_whisper);
	controls.c_pAllowTouristWhisper   ->SetCheck(Attr.allow_tourist_whisper);
	controls.c_pAlwaysShowNames       ->SetCheck(Attr.always_show_names);
	controls.c_pDisableAvatarList     ->SetCheck(Attr.disable_avatar_list);
	controls.c_pAvatarRefreshRate     ->OverloadedSet(Attr.avatar_refresh_rate);
	controls.c_pEnableCavAwEnableCavCitizen->SetCheck((Attr.enable_cav & AW_ENABLE_CAV_CITIZEN) ? TRUE : FALSE);
	controls.c_pEnableCavAwEnableCavBot->SetCheck((Attr.enable_cav & AW_ENABLE_CAV_BOT) ? TRUE : FALSE);
	controls.c_pEnableCavAwEnableCavTourist->SetCheck((Attr.enable_cav & AW_ENABLE_CAV_TOURIST) ? TRUE : FALSE);
	controls.c_pEnableCavAwEnableCavWorldCitizen->SetCheck((Attr.enable_cav & AW_ENABLE_CAV_WORLD_CITIZEN) ? TRUE : FALSE);
	controls.c_pEnableCavAwEnableCavWorldBot->SetCheck((Attr.enable_cav & AW_ENABLE_CAV_WORLD_BOT) ? TRUE : FALSE);
	controls.c_pEnableCavAwEnableCavWorldTourist->SetCheck((Attr.enable_cav & AW_ENABLE_CAV_WORLD_TOURIST) ? TRUE : FALSE);
	controls.c_pEnablePav             ->SetCheck(Attr.enable_pav);
	controls.c_pChatDisableUrlClicks  ->SetCheck(Attr.chat_disable_url_clicks);
	controls.c_pDisableMultipleMedia  ->SetCheck(Attr.disable_multiple_media);
	controls.c_pBotmenuUrl            ->OverloadedSet(Attr.botmenu_url);
	controls.c_pVoipConferenceGlobal  ->SetCheck(Attr.voip_conference_global);
	controls.c_pVoipModerateGlobal    ->SetCheck(Attr.voip_moderate_global);
	controls.c_pWaterEnabled          ->SetCheck(Attr.water_enabled);
	controls.c_pWaterTexture          ->OverloadedSet(Attr.water_texture);
	controls.c_pWaterMask             ->OverloadedSet(Attr.water_mask);
	controls.c_pWaterBottomTexture    ->OverloadedSet(Attr.water_bottom_texture);
	controls.c_pWaterBottomMask       ->OverloadedSet(Attr.water_bottom_mask);
	controls.c_pWaterOpacity          ->OverloadedSet(Attr.water_opacity);
	controls.c_pWaterVisibility       ->OverloadedSet(Attr.water_visibility);
	(*controls.c_pWater)     =Attr.water.value;
	controls.c_pWaterLevel            ->OverloadedSet(Attr.water_level);
	controls.c_pWaterSurfaceMove      ->OverloadedSet(Attr.water_surface_move);
	controls.c_pWaterWaveMove         ->OverloadedSet(Attr.water_wave_move);
	controls.c_pWaterSpeed            ->OverloadedSet(Attr.water_speed);
	controls.c_pBuoyancy              ->OverloadedSet(Attr.buoyancy);
	controls.c_pWaterFriction         ->OverloadedSet(Attr.water_friction);
	controls.c_pWaterUnderTerrain     ->SetCheck(Attr.water_under_terrain);
	controls.c_pSoundFootstep         ->OverloadedSet(Attr.sound_footstep);
	controls.c_pSoundWaterEnter       ->OverloadedSet(Attr.sound_water_enter);
	controls.c_pSoundWaterExit        ->OverloadedSet(Attr.sound_water_exit);
	controls.c_pSoundAmbient          ->OverloadedSet(Attr.sound_ambient);

}

void WorldOptionsDialog::screen_to_attributes(WorldAttributes& Attr)
{
	controls.c_pEnterRight->OverloadedGet(Attr.enter_right);
	controls.c_pBuildRight->OverloadedGet(Attr.build_right);
	controls.c_pVoipRight->OverloadedGet(Attr.voip_right);
	controls.c_pEminentDomainRight->OverloadedGet(Attr.eminent_domain_right);
	controls.c_pSpecialObjectsRight->OverloadedGet(Attr.special_objects_right);
	controls.c_pPublicSpeakerRight->OverloadedGet(Attr.public_speaker_right);
	controls.c_pBotsRight->OverloadedGet(Attr.bots_right);
	controls.c_pEjectRight->OverloadedGet(Attr.eject_right);
	controls.c_pSpeakRight->OverloadedGet(Attr.speak_right);
	controls.c_pSpecialCommandsRight->OverloadedGet(Attr.special_commands_right);
	controls.c_pSpecialCommands->OverloadedGet(Attr.special_commands);
	controls.c_pV4ObjectsRight->OverloadedGet(Attr.v4_objects_right);
	controls.c_pTerrainRight->OverloadedGet(Attr.terrain_right);
	controls.c_pTitle->OverloadedGet(Attr.title);
	controls.c_pHomePage->OverloadedGet(Attr.home_page);
	Attr.rating = controls.c_pRating->GetItemData(controls.c_pRating->GetCurSel());
	controls.c_pWelcomeMessage->OverloadedGet(Attr.welcome_message);
	controls.c_pKeywords->OverloadedGet(Attr.keywords);
	controls.c_pGround->OverloadedGet(Attr.ground);
	Attr.repeating_ground = controls.c_pRepeatingGround->GetCheck();
	Attr.enable_terrain = controls.c_pEnableTerrain->GetCheck();
	controls.c_pTerrainAmbient->OverloadedGet(Attr.terrain_ambient);
	controls.c_pTerrainDiffuse->OverloadedGet(Attr.terrain_diffuse);
	controls.c_pTerrainOffset->OverloadedGet(Attr.terrain_offset);
	controls.c_pObjectPath->OverloadedGet(Attr.object_path);
	controls.c_pObjectRefresh->OverloadedGet(Attr.object_refresh);
	controls.c_pObjectPassword->OverloadedGet(Attr.object_password);
	Attr.enable_referer = controls.c_pEnableReferer->GetCheck();
	controls.c_pCavObjectPath->OverloadedGet(Attr.cav_object_path);
	controls.c_pCavObjectPassword->OverloadedGet(Attr.cav_object_password);
	controls.c_pCavObjectRefresh->OverloadedGet(Attr.cav_object_refresh);
	Attr.cell_limit = controls.c_pCellLimit->GetItemData(controls.c_pCellLimit->GetCurSel());
	controls.c_pRestrictedRadius->OverloadedGet(Attr.restricted_radius);
	controls.c_pMaxLightRadius->OverloadedGet(Attr.max_light_radius);
	Attr.disable_create_url = controls.c_pDisableCreateUrl->GetCheck();
	Attr.allow_object_select = controls.c_pAllowObjectSelect->GetCheck();
	Attr.allow_3_axis_rotation = controls.c_pAllow3AxisRotation->GetCheck();
	Attr.allow_tourist_build = controls.c_pAllowTouristBuild->GetCheck();
	controls.c_pMoverEmptyResetTimeout->OverloadedGet(Attr.mover_empty_reset_timeout);
	controls.c_pMoverUsedResetTimeout->OverloadedGet(Attr.mover_used_reset_timeout);
	Attr.enable_bump_event = controls.c_pEnableBumpEvent->GetCheck();
	Attr.enable_sync_events = controls.c_pEnableSyncEvents->GetCheck();
	Attr.fog_enable = controls.c_pFogEnable->GetCheck();
	controls.c_pFogMinimum->OverloadedGet(Attr.fog_minimum);
	controls.c_pFogMaximum->OverloadedGet(Attr.fog_maximum);
	Attr.fog_tinted = controls.c_pFogTinted->GetCheck();
	Attr.fog.value = controls.c_pFog->getColour();
	Attr.minimum_visibility = controls.c_pMinimumVisibility->GetItemData(controls.c_pMinimumVisibility->GetCurSel());
	controls.c_pLightX->OverloadedGet(Attr.light_x);
	controls.c_pLightY->OverloadedGet(Attr.light_y);
	controls.c_pLightZ->OverloadedGet(Attr.light_z);
	Attr.light.value = controls.c_pLight->getColour();
	Attr.ambient_light.value = controls.c_pAmbientLight->getColour();
	controls.c_pLightTexture->OverloadedGet(Attr.light_texture);
	controls.c_pLightMask->OverloadedGet(Attr.light_mask);
	controls.c_pLightDrawSize->OverloadedGet(Attr.light_draw_size);
	Attr.light_draw_front = controls.c_pLightDrawFront->GetCheck();
	Attr.light_draw_bright = controls.c_pLightDrawBright->GetCheck();
	Attr.light_source_use_color = controls.c_pLightSourceUseColor->GetCheck();
	Attr.light_source_color.value = controls.c_pLightSourceColor->getColour();
	Attr.disable_shadows = controls.c_pDisableShadows->GetCheck();
	controls.c_pEntryPoint->OverloadedGet(Attr.entry_point);
	Attr.allow_passthru = controls.c_pAllowPassthru->GetCheck();
	Attr.allow_flying = controls.c_pAllowFlying->GetCheck();
	Attr.allow_teleport = controls.c_pAllowTeleport->GetCheck();
	Attr.allow_avatar_collision = controls.c_pAllowAvatarCollision->GetCheck();
	controls.c_pGravity->OverloadedGet(Attr.gravity);
	controls.c_pFriction->OverloadedGet(Attr.friction);
	Attr.slopeslide_enabled = controls.c_pSlopeslideEnabled->GetCheck();
	controls.c_pSlopeslideMinAngle->OverloadedGet(Attr.slopeslide_min_angle);
	controls.c_pSlopeslideMaxAngle->OverloadedGet(Attr.slopeslide_max_angle);
	Attr.enable_camera_collision = controls.c_pEnableCameraCollision->GetCheck();
	controls.c_pCameraZoom->OverloadedGet(Attr.camera_zoom);
	controls.c_pBackdrop->OverloadedGet(Attr.backdrop);
	controls.c_pSkybox->OverloadedGet(Attr.skybox);
	Attr.sky_north.value = controls.c_pSkyNorth->getColour();
	Attr.sky_south.value = controls.c_pSkySouth->getColour();
	Attr.sky_east.value = controls.c_pSkyEast->getColour();
	Attr.sky_west.value = controls.c_pSkyWest->getColour();
	Attr.sky_top.value = controls.c_pSkyTop->getColour();
	Attr.sky_bottom.value = controls.c_pSkyBottom->getColour();
	controls.c_pCloudsLayer1Texture->OverloadedGet(Attr.clouds_layer1_texture);
	controls.c_pCloudsLayer1Mask->OverloadedGet(Attr.clouds_layer1_mask);
	controls.c_pCloudsLayer1Tile->OverloadedGet(Attr.clouds_layer1_tile);
	controls.c_pCloudsLayer1SpeedX->OverloadedGet(Attr.clouds_layer1_speed_x);
	controls.c_pCloudsLayer1SpeedZ->OverloadedGet(Attr.clouds_layer1_speed_z);
	controls.c_pCloudsLayer1Opacity->OverloadedGet(Attr.clouds_layer1_opacity);
	controls.c_pCloudsLayer2Texture->OverloadedGet(Attr.clouds_layer2_texture);
	controls.c_pCloudsLayer2Mask->OverloadedGet(Attr.clouds_layer2_mask);
	controls.c_pCloudsLayer2Tile->OverloadedGet(Attr.clouds_layer2_tile);
	controls.c_pCloudsLayer2SpeedX->OverloadedGet(Attr.clouds_layer2_speed_x);
	controls.c_pCloudsLayer2SpeedZ->OverloadedGet(Attr.clouds_layer2_speed_z);
	controls.c_pCloudsLayer2Opacity->OverloadedGet(Attr.clouds_layer2_opacity);
	controls.c_pCloudsLayer3Texture->OverloadedGet(Attr.clouds_layer3_texture);
	controls.c_pCloudsLayer3Mask->OverloadedGet(Attr.clouds_layer3_mask);
	controls.c_pCloudsLayer3Tile->OverloadedGet(Attr.clouds_layer3_tile);
	controls.c_pCloudsLayer3SpeedX->OverloadedGet(Attr.clouds_layer3_speed_x);
	controls.c_pCloudsLayer3SpeedZ->OverloadedGet(Attr.clouds_layer3_speed_z);
	controls.c_pCloudsLayer3Opacity->OverloadedGet(Attr.clouds_layer3_opacity);
	Attr.disable_chat = controls.c_pDisableChat->GetCheck();
	Attr.allow_citizen_whisper = controls.c_pAllowCitizenWhisper->GetCheck();
	Attr.allow_tourist_whisper = controls.c_pAllowTouristWhisper->GetCheck();
	Attr.always_show_names = controls.c_pAlwaysShowNames->GetCheck();
	Attr.disable_avatar_list = controls.c_pDisableAvatarList->GetCheck();
	controls.c_pAvatarRefreshRate->OverloadedGet(Attr.avatar_refresh_rate);
	Attr.enable_cav = 0;
	Attr.enable_cav |= controls.c_pEnableCavAwEnableCavCitizen->GetCheck() ? AW_ENABLE_CAV_CITIZEN : 0;
	Attr.enable_cav |= controls.c_pEnableCavAwEnableCavBot->GetCheck() ? AW_ENABLE_CAV_BOT : 0;
	Attr.enable_cav |= controls.c_pEnableCavAwEnableCavTourist->GetCheck() ? AW_ENABLE_CAV_TOURIST : 0;
	Attr.enable_cav |= controls.c_pEnableCavAwEnableCavWorldCitizen->GetCheck() ? AW_ENABLE_CAV_WORLD_CITIZEN : 0;
	Attr.enable_cav |= controls.c_pEnableCavAwEnableCavWorldBot->GetCheck() ? AW_ENABLE_CAV_WORLD_BOT : 0;
	Attr.enable_cav |= controls.c_pEnableCavAwEnableCavWorldTourist->GetCheck() ? AW_ENABLE_CAV_WORLD_TOURIST : 0;
	Attr.enable_pav = controls.c_pEnablePav->GetCheck();
	Attr.chat_disable_url_clicks = controls.c_pChatDisableUrlClicks->GetCheck();
	Attr.disable_multiple_media = controls.c_pDisableMultipleMedia->GetCheck();
	controls.c_pBotmenuUrl->OverloadedGet(Attr.botmenu_url);
	Attr.voip_conference_global = controls.c_pVoipConferenceGlobal->GetCheck();
	Attr.voip_moderate_global = controls.c_pVoipModerateGlobal->GetCheck();
	Attr.water_enabled = controls.c_pWaterEnabled->GetCheck();
	controls.c_pWaterTexture->OverloadedGet(Attr.water_texture);
	controls.c_pWaterMask->OverloadedGet(Attr.water_mask);
	controls.c_pWaterBottomTexture->OverloadedGet(Attr.water_bottom_texture);
	controls.c_pWaterBottomMask->OverloadedGet(Attr.water_bottom_mask);
	controls.c_pWaterOpacity->OverloadedGet(Attr.water_opacity);
	controls.c_pWaterVisibility->OverloadedGet(Attr.water_visibility);
	Attr.water.value = controls.c_pWater->getColour();
	controls.c_pWaterLevel->OverloadedGet(Attr.water_level);
	controls.c_pWaterSurfaceMove->OverloadedGet(Attr.water_surface_move);
	controls.c_pWaterWaveMove->OverloadedGet(Attr.water_wave_move);
	controls.c_pWaterSpeed->OverloadedGet(Attr.water_speed);
	controls.c_pBuoyancy->OverloadedGet(Attr.buoyancy);
	controls.c_pWaterFriction->OverloadedGet(Attr.water_friction);
	Attr.water_under_terrain = controls.c_pWaterUnderTerrain->GetCheck();
	controls.c_pSoundFootstep->OverloadedGet(Attr.sound_footstep);
	controls.c_pSoundWaterEnter->OverloadedGet(Attr.sound_water_enter);
	controls.c_pSoundWaterExit->OverloadedGet(Attr.sound_water_exit);
	controls.c_pSoundAmbient->OverloadedGet(Attr.sound_ambient);

}