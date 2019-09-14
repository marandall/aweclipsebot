#include "stdafx.h"
#include "scripting_evo2.h"
#include "KnownList.h"
#include "CommonFeatures.h"
#include "RelayRelay.h"
#include "versions.h"
#include "worldlist.h"
#include "property_objects.h"
#include "cis_dispatch.h"
#include "versions.h"
#include "tsb.h"
#include "zones.h"
#include "umbra/umbra_classlib.h"
#include "movers.h"
#include "movers_autodriver.h"
#include "beacons.h"
#include "headsup_userplugin.h"
#include "RelayRelay.h"
#include "dynamicrights.h"
#include "BotgramProtocolV2.h"
#include "beacons.h"


namespace ScriptingHost_ClassLib
{
	namespace App
	{
		bool Namespace::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 
		{ 
			// class static list
			static D_HeadsUpTrayIcon _heads_up_tray_icon;

			// application constants
			UMBRA_PROPERTY_R(APP_BUILD,				EVOLUTION_VERSION);
			UMBRA_PROPERTY_R(APP_ID,					EVOLUTION_IDENTIFY);

			// constant types
			UMBRA_PROPERTY_R(B_TRUE,					B_TRUE);
			UMBRA_PROPERTY_R(B_FALSE,					B_FALSE);
			UMBRA_PROPERTY_R(B_DEFAULT,				B_EMPTY);

			// primary access to kernels
			UMBRA_PROPERTY_R(bot,							bot);
			UMBRA_PROPERTY_R(avatars,					avatars);
			UMBRA_PROPERTY_R(worlds,					bot.getWorldList());
			UMBRA_PROPERTY_R(movers,					Movers::g_Movers);
			UMBRA_PROPERTY_R(zones,						Zones::g_ZonesMgr);
			UMBRA_PROPERTY_R(objects,					bot.getLiveCache());
			UMBRA_PROPERTY_R(tr_kernel,				ChatControllers::g_Relay.total_relay);
			UMBRA_PROPERTY_R(channels,				ChatControllers::g_Relay.channels);
			UMBRA_PROPERTY_R(jump_system,			Beacons::Kernel::getInst());		

			// component access kernels
			UMBRA_PROPERTY_R(relay_kernel,		ChatControllers::g_Relay);
			UMBRA_PROPERTY_R(root_zone,				*Zones::g_ZonesMgr.GetRoot());
			UMBRA_PROPERTY_R(permissions,			DynamicRights::g_Provider);

			// class information
			UMBRA_PROPERTY_R(KnownList,				_known_list);
			UMBRA_PROPERTY_R(Crypto,					_crypto);
			UMBRA_PROPERTY_R(TrayIcon,				_heads_up_tray_icon);
			UMBRA_PROPERTY_R(LogStyle,				_log_style);
			UMBRA_PROPERTY_R(Property,				_property);
			UMBRA_PROPERTY_R(Interface,				_interface);

			// commmon features
			UMBRA_PROPERTY_R(common_greetings,			CommonFeatures::g_Greetings);
			UMBRA_PROPERTY_R(common_auto_responder, CommonFeatures::g_AutoResponding);

			// disk paths
			UMBRA_PROPERTY_R(APP_PATH,				(const char*)GetAppPath());
			UMBRA_PROPERTY_R(PROFILE_PATH,		(const char*)getProfilePath());

			// unsupported element
			return Umbra::PropertyRequest::NOT_HANDLED;  
		}

		bool Namespace::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R) 
		{ 
			//
			// Store
			//

			UMBRA_FUNCTION_BEGIN(store) {
				// required arguments
				UMBRA_FUNCTION_DA(0, Key,  Umbra::VT_STRING);

				// code information break
				UMBRA_FUNCTION_CS() {
					String k = vaKey.toString().c_str();
					UMBRA_FUNCTION_RET( &store[k].getVM() );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();
				
			//
			// run a cis commmand
			//

			UMBRA_FUNCTION_BEGIN(consoleCommand) {
				// required arguments
				UMBRA_FUNCTION_DA(0, Text,  Umbra::VT_STRING);
				UMBRA_FUNCTION_DA(1, Supr,  Umbra::VT_BOOL);

				// code information break
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( CIS::CommandInterpreter::beginConsole( vaText.toString().c_str(), vaSupr.toBool()) );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// log an item to window
			//

			UMBRA_FUNCTION_BEGIN(writeLog) {
				// required arguments
				UMBRA_FUNCTION_DA(0, Text,  Umbra::VT_STRING);
				UMBRA_FUNCTION_OA(1, Color, Umbra::VT_INTEGER, 0x00);
				UMBRA_FUNCTION_OA(2, Mask,  Umbra::VT_INTEGER, OT_MAIN);

				// code information break
				UMBRA_FUNCTION_CS() {
					WriteMsg( vaText.toString().c_str(), vaColor.toInt32(), vaMask.toInt32() );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// search for an avatar
			//

			UMBRA_FUNCTION_BEGIN(getAvatar) {
				// required arguments
				UMBRA_FUNCTION_DA(0, AvatarKey, Umbra::VT_STRING);

				// code information break
				UMBRA_FUNCTION_CS() {
					Avatar* pAvatar = avatars.GetByKey(vaAvatarKey.toString().c_str());
					UMBRA_FUNCTION_RET( pAvatar ? *pAvatar : Umbra::Variant() );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// search for a mover
			//

			UMBRA_FUNCTION_BEGIN(getMover) {
				// required arguments
				UMBRA_FUNCTION_DA(0, MoverId, Umbra::VT_STRING);

				// code information break
				UMBRA_FUNCTION_CS() {
					Movers::Mover* pMover = Movers::g_Movers.getByName(vaMoverId.toString().c_str());
					UMBRA_FUNCTION_RET( pMover ? *pMover : Umbra::Variant() );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// search for a mover driver
			//

			UMBRA_FUNCTION_BEGIN(getMoverDriver) {
				// required arguments
				UMBRA_FUNCTION_DA(0, MoverId, Umbra::VT_STRING);

				// code information break
				UMBRA_FUNCTION_CS() {
					Movers::AutoDriver* pAuto = Movers::AutoDriverKernel::getInstance().getEntryByName( vaMoverId.toString().c_str() );
					UMBRA_FUNCTION_RET( pAuto ? *pAuto : Umbra::Variant() );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// add a new movement driver
			//

			UMBRA_FUNCTION_BEGIN(addMoverDriver) {
				// required arguments
				UMBRA_FUNCTION_DA(0, MoverId, Umbra::VT_STRING);

				// code information break
				UMBRA_FUNCTION_CS() {
					Movers::AutoDriver* pAuto = Movers::AutoDriverKernel::getInstance().createNew( vaMoverId.toString().c_str() );
					UMBRA_FUNCTION_RET( pAuto ? *pAuto : Umbra::Variant() );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// delete a movement driver
			//

			UMBRA_FUNCTION_BEGIN(deleteMoverDriver) {
				// required arguments
				UMBRA_FUNCTION_DA(0, MoverId, Umbra::VT_STRING);

				// code information break
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( Movers::AutoDriverKernel::getInstance().Delete( vaMoverId.toString().c_str()) );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// find a jump point
			//

			UMBRA_FUNCTION_BEGIN(getBeacon) {
				// required arguments
				UMBRA_FUNCTION_DA(0, BeaconId, Umbra::VT_STRING);

				// code information break
				UMBRA_FUNCTION_CS() {
					Beacons::Beacon* pAuto = Beacons::Kernel::getInst().Search( vaBeaconId.toString().c_str() );
					UMBRA_FUNCTION_RET( pAuto ? *pAuto : Umbra::Variant() );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// find a jump point by nearest
			//

			UMBRA_FUNCTION_BEGIN(getBeaconByProximity) {
				// required arguments
				UMBRA_FUNCTION_DA(0, Position, Umbra::VT_CLASS);
				UMBRA_FUNCTION_OA(1, Prefix,   Umbra::VT_STRING, "");

				vaPosition;
				RemoteClass* pRemote = vaPosition.toRemoteClass();
				UMBRA_FUNCTION_CC(Position, ::AW::Location);

				// code information break
				UMBRA_FUNCTION_CS() {
					Beacons::Beacon* pAuto = Beacons::Kernel::getInst().getBeaconByProximity( *pPosition, vaPrefix.toString().c_str() );
					UMBRA_FUNCTION_RET( pAuto ? *pAuto : Umbra::Variant() );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// Channel Info
			//

			UMBRA_FUNCTION_BEGIN(getChannel) {
				// required arguments
				UMBRA_FUNCTION_DA(0, Id, Umbra::VT_STRING);

				// code information break
				UMBRA_FUNCTION_CS() {
					using namespace ChatControllers;
					Channels::Channel* pAuto = g_Relay.channels.search( vaId.toString().c_str() );
					UMBRA_FUNCTION_RET( pAuto ? *pAuto : Umbra::Variant() );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// create a new broadcast thread
			//

			UMBRA_FUNCTION_BEGIN(createBotgramThread) {
				// required arguments
				UMBRA_FUNCTION_DA(0, ContentType, Umbra::VT_STRING);
				UMBRA_FUNCTION_OA(1, Body, Umbra::VT_STRING, "");

				// code information break
				UMBRA_FUNCTION_CS() {
					BotgramTransferProtocol::BroadcastThread* pThread = new BotgramTransferProtocol::BroadcastThread();
					pThread->setUmbraGarbageCollected();
					pThread->content_type = vaContentType.toString().c_str();
					pThread->body					= vaBody.toString().c_str();
					UMBRA_FUNCTION_RET( *pThread );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			// unsupported element
			return Umbra::FunctionRequest::NOT_HANDLED;  
		}
	}
}