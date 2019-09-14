#include "stdafx.h"
#include "avatar.h"
#include "GlobalUmbra.h"
#include "RelayGlobalChatAex.h"
#include "zones.h"
#include "movers.h"
#include "HeadsUp_UserPlugin.h"
#include "dynamicrights.h"
#include "cis_dispatch.h"
#include "relayprivatechannels.h"
#include "cis_configuration.h"
#include "relayrelay.h"

// memory allocation checking
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Umbra;

bool Avatar::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
{
	// reflection
	UMBRA_PROPERTY_REDIRECT(AvatarData);

	// zone stuff
	UMBRA_PROPERTY_R(zone, *getZone());

	// connected zone
	Movers::Mover* pMover = Movers::g_Movers.getById(connected_mover_id);
	UMBRA_PROPERTY_R(connected_mover,		pMover ? *pMover : Umbra::Variant());
	UMBRA_PROPERTY_R(connected_mover_id,	connected_mover_id);

	// channel operations
	UMBRA_PROPERTY_R(channel_count,			(int)getChannelBindings().size());
	UMBRA_PROPERTY_RW(default_channel,		getChannelBindings()._default_channel);

	// hud operations
	UMBRA_PROPERTY_RW(hud_row, getHud().row);				

	// user types
	UMBRA_PROPERTY_R(is_citizen,				IsCitizen());
	UMBRA_PROPERTY_R(is_tourist,				IsTourist());
	UMBRA_PROPERTY_R(is_bot,					IsBot());
	UMBRA_PROPERTY_R(best_lock,					GetBestLock());
	
	// update times
	UMBRA_PROPERTY_R(last_event_time,			last_event_time);
	UMBRA_PROPERTY_R(last_chat_time,			last_chat_time);
	UMBRA_PROPERTY_R(last_object_time,			last_object_time);
	UMBRA_PROPERTY_R(last_movement_time,		last_movement_time);

	// encountered status
	UMBRA_PROPERTY_R(first_encounter_time,		first_encounter_time);
	UMBRA_PROPERTY_R(first_encounter_world,		first_encounter_world);
	UMBRA_PROPERTY_R(first_encounter_position,	first_encounter_position);	
	UMBRA_PROPERTY_R(last_encounter_time,		last_encounter_time);		
	UMBRA_PROPERTY_R(last_encounter_world,		last_encounter_world);		
	UMBRA_PROPERTY_R(last_encounter_position,	last_encounter_position);	
	UMBRA_PROPERTY_R(previous_encounter_count,	previous_encounter_count);

	//
	// script wrapper referencing
	//

	UMBRA_PROPERTY_RW(__subclass,			script_wrapper);

	RemoteClass* pScriptWrapper = script_wrapper.toRemoteClass();
	if (pScriptWrapper) {
		if (pScriptWrapper->doUmbraProperty(V, R) == PropertyRequest::HAS_HANDLED) {
			return PropertyRequest::HAS_HANDLED;
		}
	}

	// nothing found
	return Umbra::PropertyRequest::NOT_HANDLED;
}

bool Avatar::doUmbraFunctionForCommunication(Umbra::Variant& V, Umbra::FunctionRequest& R)
{
	UMBRA_FUNCTION_BEGIN(whisperTo) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Text,  Umbra::VT_STRING);
		UMBRA_FUNCTION_OA(1, Hide,  Umbra::VT_BOOLEAN, false);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( WhisperEx(vaText.toString().c_str(), vaHide.toBool()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();
	
	UMBRA_FUNCTION_BEGIN(consoleMessage) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Text,    Umbra::VT_STRING);
		UMBRA_FUNCTION_DA(1, Colour,  Umbra::VT_INTEGER);
		UMBRA_FUNCTION_DA(2, Style,   Umbra::VT_INTEGER);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( ConsoleMessageEx(vaText.toString().c_str(), vaColour.toInt32(), vaStyle.toInt32()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(consoleMessageWithPrefix) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Prefix,  Umbra::VT_STRING);
		UMBRA_FUNCTION_DA(1, Text,    Umbra::VT_STRING);
		UMBRA_FUNCTION_DA(2, Colour,  Umbra::VT_INTEGER);
		UMBRA_FUNCTION_DA(3, Style,   Umbra::VT_INTEGER);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( ConsoleMessagePrefixed(vaPrefix.toString().c_str(), vaText.toString().c_str(), vaColour.toInt32(), vaStyle.toInt32()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();
	
	UMBRA_FUNCTION_BEGIN(consoleWhisper) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Text,    Umbra::VT_STRING);
		UMBRA_FUNCTION_DA(1, Colour,  Umbra::VT_INTEGER);
		UMBRA_FUNCTION_DA(2, Style,   Umbra::VT_INTEGER);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( ConsoleWhisper(vaText.toString().c_str(), vaColour.toInt32(), vaStyle.toInt32()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(mute) {
		// code information break
		UMBRA_FUNCTION_CS() {
			Mute();
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(unmute) {
		// code information break
		UMBRA_FUNCTION_CS() {
			Unmute();
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	// nothing found
	return Umbra::FunctionRequest::NOT_HANDLED;
}

bool Avatar::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
{
	// sub sections of avatar manipulation
	UMBRA_FUNCTION_INCLUDE(doUmbraFunctionForWorldRights);
	UMBRA_FUNCTION_INCLUDE(doUmbraFunctionForPermissions);
	UMBRA_FUNCTION_INCLUDE(doUmbraFunctionForHeadsUpTray);
	UMBRA_FUNCTION_INCLUDE(doUmbraFunctionForCommunication);

	//
	// get a script var
	//

	UMBRA_FUNCTION_BEGIN(getVar) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Key,  Umbra::VT_STRING);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( script_vars[vaKey.toString()] );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// set a script var
	//

	UMBRA_FUNCTION_BEGIN(setVar) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Key,  Umbra::VT_STRING);
		UMBRA_FUNCTION_DA(1, Value,  Umbra::VT_ANY);

		// code information break
		UMBRA_FUNCTION_CS() {
			script_vars[vaKey.toString()] = vaValue;
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(isPattern) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Key,  Umbra::VT_STRING);
		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( IsPattern(vaKey.toString().c_str()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// eject a user
	//
	
	UMBRA_FUNCTION_BEGIN(eject) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Duration,Umbra::VT_INTEGER);
		UMBRA_FUNCTION_DA(1, Log,	  Umbra::VT_STRING);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( Eject(vaDuration.toInt32(), vaLog.toString().c_str(), "") );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// Send a url
	//
	
	UMBRA_FUNCTION_BEGIN(sendUrl) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Url,				Umbra::VT_STRING);
		UMBRA_FUNCTION_OA(1, Target,		Umbra::VT_STRING, "");
		UMBRA_FUNCTION_OA(2, Target3D,	Umbra::VT_BOOLEAN, false);
		UMBRA_FUNCTION_OA(3, PostData,	Umbra::VT_STRING, "");

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( SendURL(vaUrl.toString().c_str(), vaTarget.toString().c_str(), vaTarget3D.toBool(), vaPostData.toString().c_str()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();


	//
	// teleport a user
	//
	
	UMBRA_FUNCTION_BEGIN(teleport) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Location,Umbra::VT_CLASS);
		// optional arguments
		UMBRA_FUNCTION_OA(1, World,  Umbra::VT_STRING,  "");
		UMBRA_FUNCTION_OA(2, Warp,   Umbra::VT_BOOLEAN, false);

		// required class
		UMBRA_FUNCTION_CC(Location, AW::Location);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( Teleport(*pLocation, vaWorld.toString().c_str(), vaWarp.toBool()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// click upon the user
	//
	
	UMBRA_FUNCTION_BEGIN(click) {
		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( Click() );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// send them a noise (v4 only)
	//
	
	UMBRA_FUNCTION_BEGIN(sendNoise) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Url, Umbra::VT_STRING);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( Noise(vaUrl.toString().c_str()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// Set the users avatar
	//

	UMBRA_FUNCTION_BEGIN(changeAvatar) {
		// required arguments
		UMBRA_FUNCTION_OA(0, Avatar,  Umbra::VT_INTEGER, -1);
		UMBRA_FUNCTION_OA(1, Gesture, Umbra::VT_INTEGER, -1);
		UMBRA_FUNCTION_OA(2, State,   Umbra::VT_INTEGER, -1);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( Avatarset(vaAvatar.toInt32(), vaGesture.toInt32(), vaState.toInt32()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();


	//
	// trigger a hyperion refresh
	//
	
	UMBRA_FUNCTION_BEGIN(reloadHyperion) {
		// code information break
		UMBRA_FUNCTION_CS() {
			activateHyperion();
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// command executions
	//

	UMBRA_FUNCTION_BEGIN(doCommand) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Command, Umbra::VT_STRING);

		// code information break
		UMBRA_FUNCTION_CS() {
			int rc = CIS::CommandInterpreter::beginAvatar(*this, CIS::Config::g_Interface.getPrefixKey() + vaCommand.toString().c_str(), false, AW_CHAT_SAID);
			UMBRA_FUNCTION_RET( rc );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();



	//
	// channel extensions
	//

	UMBRA_FUNCTION_BEGIN(isConnectedToChannel) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Channel, Umbra::VT_STRING);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( getChannelBindings().isIncluding(vaChannel.toString().c_str()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(joinChannel) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Channel, Umbra::VT_STRING);
		UMBRA_FUNCTION_OA(1, Announce, Umbra::VT_BOOLEAN, false);

		// code information break
		UMBRA_FUNCTION_CS() {
			// lookup channel name
			using namespace ChatControllers;
			Channels::Channel* pChan = g_Relay.channels.search(vaChannel.toString().c_str());
			if (pChan == NULL)
			{
				UMBRA_FUNCTION_RET( false );
			}
			else
			{
				UMBRA_FUNCTION_RET( pChan->join(this, vaAnnounce.toBool()) ? true : false );
			}
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(quitChannel) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Channel, Umbra::VT_STRING);
		UMBRA_FUNCTION_OA(1, Announce, Umbra::VT_BOOLEAN, false);

		// code information break
		UMBRA_FUNCTION_CS() {
			// lookup channel name
			using namespace ChatControllers;
			Channels::Channel* pChan = g_Relay.channels.search(vaChannel.toString().c_str());
			if (pChan == NULL)
			{
				UMBRA_FUNCTION_RET( false );
			}
			else
			{
				pChan->leave(this, vaAnnounce.toBool());
				UMBRA_FUNCTION_RET( true );
			}
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();




	//
	// teleport a user
	//
	
	UMBRA_FUNCTION_BEGIN(scatter) {
		// required arguments
		UMBRA_FUNCTION_DA(0, From, Umbra::VT_CLASS);
		UMBRA_FUNCTION_DA(1, FromRadius, Umbra::VT_STRING);
		UMBRA_FUNCTION_DA(2, To, Umbra::VT_CLASS);
		UMBRA_FUNCTION_DA(3, ToRadius, Umbra::VT_STRING);

		// required class
		UMBRA_FUNCTION_CC(From, AW::Location);
		UMBRA_FUNCTION_CC(To, AW::Location);

		// code information break
		UMBRA_FUNCTION_CS() {
			if (distanceToGD(*pFrom) < vaFromRadius.toDouble())
			{
			}

			UMBRA_FUNCTION_RET( false );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();


	//
	// script wrapper referencing
	//

	RemoteClass* pScriptWrapper = script_wrapper.toRemoteClass();
	if (pScriptWrapper) {
		if (pScriptWrapper->doUmbraFunction(V, R) == FunctionRequest::HAS_HANDLED) {
			return FunctionRequest::HAS_HANDLED;
		}
	}

	//
	// nothing found
	//


	UMBRA_FUNCTION_REDIRECT(AvatarData);

	return Umbra::FunctionRequest::NOT_HANDLED;
}


bool Avatar::doUmbraFunctionForWorldRights(Umbra::Variant& V, Umbra::FunctionRequest& R)
{
	UMBRA_FUNCTION_BEGIN(isEjectEmpowered) {
		// optional arguments
		UMBRA_FUNCTION_OA(0, Single,  Umbra::VT_BOOLEAN, false);
		UMBRA_FUNCTION_OA(1, Citizen, Umbra::VT_BOOLEAN, false);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( HasEject(vaSingle.toBool(), vaCitizen.toBool()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(isPublicSpeakerEmpowered) {
		// optional arguments
		UMBRA_FUNCTION_OA(0, Single,  Umbra::VT_BOOLEAN, false);
		UMBRA_FUNCTION_OA(1, Citizen, Umbra::VT_BOOLEAN, false);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( HasPS(vaSingle.toBool(), vaCitizen.toBool()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(isRootEmpowered) {
		// optional arguments
		UMBRA_FUNCTION_OA(0, Single,  Umbra::VT_BOOLEAN, false);
		UMBRA_FUNCTION_OA(1, Citizen, Umbra::VT_BOOLEAN, false);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( HasRoot(vaSingle.toBool(), vaCitizen.toBool()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(isBotsEmpowered) {
		// optional arguments
		UMBRA_FUNCTION_OA(0, Single,  Umbra::VT_BOOLEAN, false);
		UMBRA_FUNCTION_OA(1, Citizen, Umbra::VT_BOOLEAN, false);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( HasBots(vaSingle.toBool(), vaCitizen.toBool()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(isEminentDomainEmpowered) {
		// optional arguments
		UMBRA_FUNCTION_OA(0, Single,  Umbra::VT_BOOLEAN, false);
		UMBRA_FUNCTION_OA(1, Citizen, Umbra::VT_BOOLEAN, false);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( HasEminent(vaSingle.toBool(), vaCitizen.toBool()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(isBuildEmpowered) {
		// optional arguments
		UMBRA_FUNCTION_OA(0, Single,  Umbra::VT_BOOLEAN, false);
		UMBRA_FUNCTION_OA(1, Citizen, Umbra::VT_BOOLEAN, false);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( HasBuild(vaSingle.toBool(), vaCitizen.toBool()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(isSpecialObjectsEmpowered) {
		// optional arguments
		UMBRA_FUNCTION_OA(0, Single,  Umbra::VT_BOOLEAN, false);
		UMBRA_FUNCTION_OA(1, Citizen, Umbra::VT_BOOLEAN, false);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( HasSpecialObjects(vaSingle.toBool(), vaCitizen.toBool()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(isSpecialCommandsEmpowered) {
		// optional arguments
		UMBRA_FUNCTION_OA(0, Single,  Umbra::VT_BOOLEAN, false);
		UMBRA_FUNCTION_OA(1, Citizen, Umbra::VT_BOOLEAN, false);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( HasSpecialCommands(vaSingle.toBool(), vaCitizen.toBool()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(isSpeakEmpowered) {
		// optional arguments
		UMBRA_FUNCTION_OA(0, Single,  Umbra::VT_BOOLEAN, false);
		UMBRA_FUNCTION_OA(1, Citizen, Umbra::VT_BOOLEAN, false);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( HasSpeak(vaSingle.toBool(), vaCitizen.toBool()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(isEnterEmpowered) {
		// optional arguments
		UMBRA_FUNCTION_OA(0, Single,  Umbra::VT_BOOLEAN, false);
		UMBRA_FUNCTION_OA(1, Citizen, Umbra::VT_BOOLEAN, false);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( HasEnter(vaSingle.toBool(), vaCitizen.toBool()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	// nothing found
	return Umbra::FunctionRequest::NOT_HANDLED;
}

bool Avatar::doUmbraFunctionForPermissions(Umbra::Variant& V, Umbra::FunctionRequest& R)
{
	// check their rights
	UMBRA_FUNCTION_BEGIN(getPermission) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Right, Umbra::VT_STRING);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( GetRight(vaRight.toString().c_str()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	// grant a drs permission
	UMBRA_FUNCTION_BEGIN(grantPermission) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Right, Umbra::VT_STRING);

		// code information break
		UMBRA_FUNCTION_CS() {
			_permissions->Permit(vaRight.toString().c_str(), "[Scripting]");
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	// revoke a drs permission
	UMBRA_FUNCTION_BEGIN(revokePermission) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Right, Umbra::VT_STRING);

		// code information break
		UMBRA_FUNCTION_CS() {
			_permissions->Deny(vaRight.toString().c_str());
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	// get their citizen account
	UMBRA_FUNCTION_BEGIN(allocDynamicRights) {
		UMBRA_FUNCTION_CS() {
			DynamicRights::User* pU = allocDynamicRights();
			UMBRA_FUNCTION_RET( pU ? *pU : Umbra::Variant() );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();


	// nothing found
	return Umbra::FunctionRequest::NOT_HANDLED;
}

bool Avatar::doUmbraFunctionForHeadsUpTray(Umbra::Variant& V, Umbra::FunctionRequest& R)
{	
	UMBRA_FUNCTION_BEGIN(addTrayIcon) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Em, Umbra::VT_CLASS);
		UMBRA_FUNCTION_CC(Em, HeadsUp::Element);

		// code information break
		UMBRA_FUNCTION_CS() {
			HeadsUp::Element* pBack = getHud().push_back(*pEm);
			UMBRA_FUNCTION_RET( *pBack );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(deleteTrayIcon) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Em, Umbra::VT_INTEGER);

		// code information break
		UMBRA_FUNCTION_CS() {
			getHud().deleteItem(vaEm.toInt32());
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();
	
	UMBRA_FUNCTION_BEGIN(deleteTrayIconByKey) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Em, Umbra::VT_STRING);

		// code information break
		UMBRA_FUNCTION_CS() {
			HeadsUp::Element* em = getHud().search(vaEm.toString().c_str());
			if (em)
			{
				getHud().deleteItem(em->id);
			}
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();


	UMBRA_FUNCTION_BEGIN(getTrayIconByKey) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Em, Umbra::VT_STRING);

		// code information break
		UMBRA_FUNCTION_CS() {
			HeadsUp::Element* em = getHud().search(vaEm.toString().c_str());
			UMBRA_FUNCTION_RET( em ? *em : Umbra::Variant() );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(collapseTray) {
		// code information break
		UMBRA_FUNCTION_CS() {
			getHud().collapse();
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(expandTray) {
		// code information break
		UMBRA_FUNCTION_CS() {
			getHud().expand();
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(updateTray) {
		// code information break
		UMBRA_FUNCTION_CS() {
			getHud().update_all();
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();


	//
	// FreeTray Elements
	//

	HeadsUp::FreeTray& freeTray = getFreeTray();

	UMBRA_FUNCTION_BEGIN(addHudIcon) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Icon, Umbra::VT_CLASS);
		UMBRA_FUNCTION_CC(Icon, HeadsUp::Element);

		// code information break
		UMBRA_FUNCTION_CS() {
			freeTray.elements.push_back(*pIcon);
			freeTray.elements.back().owner_session = *this;

			UMBRA_FUNCTION_RET( freeTray.elements.back() );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(getHudIcon) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Key, Umbra::VT_STRING);
		// code information break
		UMBRA_FUNCTION_CS() {
			String key = vaKey.toString().c_str();
			for (HeadsUp::FreeTray::iterator em = freeTray.elements.begin(); em != freeTray.elements.end(); em++)
			{
				if (em->key == key)
				{
					UMBRA_FUNCTION_RET( *em )
				}
			}
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(deleteHudIcon) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Key, Umbra::VT_STRING);
		// code information break
		UMBRA_FUNCTION_CS() {
			String key = vaKey.toString().c_str();
			for (HeadsUp::FreeTray::iterator em = freeTray.elements.begin(); em != freeTray.elements.end(); em++)
			{
				if (em->key == key)
				{
					DestroyHUD(em->id);
					freeTray.elements.erase(em);
					UMBRA_FUNCTION_RET( true );
				}
			}
			UMBRA_FUNCTION_RET( false );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();


	// nothing found
	return Umbra::FunctionRequest::NOT_HANDLED;
}
