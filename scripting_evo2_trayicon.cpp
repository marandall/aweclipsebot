#include "stdafx.h"
#include "scripting_evo2.h"
#include "headsup_userplugin.h"

namespace ScriptingHost_ClassLib
{
	namespace App
	{
		Umbra::RemoteClass* D_HeadsUpTrayIcon::doUmbraNewInstance(Umbra::Host* HostPtr)
		{
			return new HeadsUp::Element();
		}

		bool D_HeadsUpTrayIcon::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
			using namespace HeadsUp;
			UMBRA_PROPERTY_R(CLICK_METHOD_NONE,			Element::CLICK_METHOD_NONE);
			UMBRA_PROPERTY_R(CLICK_METHOD_CIS,			Element::CLICK_METHOD_CIS);
			UMBRA_PROPERTY_R(CLICK_METHOD_SCRIPT,		Element::CLICK_METHOD_SCRIPT);
			UMBRA_PROPERTY_R(CLICK_METHOD_URL,			Element::CLICK_METHOD_URL);
			UMBRA_PROPERTY_R(CLICK_METHOD_EXPAND,		Element::CLICK_METHOD_EXPAND);
			UMBRA_PROPERTY_R(CLICK_METHOD_COLLAPSE,		Element::CLICK_METHOD_COLLAPSE);
			UMBRA_PROPERTY_R(CLICK_METHOD_CHANGEROW,	Element::CLICK_METHOD_CHANGEROW);
			UMBRA_PROPERTY_R(CLICK_METHOD_MAX,			Element::CLICK_METHOD_MAX);

			// nada
			return R.NOT_HANDLED;
		}
		
		bool D_HeadsUpTrayIcon::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R) 
		{ 
			//
			// creates a heads up icon using 80x80
			//

			UMBRA_FUNCTION_BEGIN(CreateBasicJump) {
				// required arguments
				UMBRA_FUNCTION_DA(0, Text,  Umbra::VT_STRING);
				UMBRA_FUNCTION_DA(1, Image, Umbra::VT_STRING);
				UMBRA_FUNCTION_DA(2, Key,   Umbra::VT_STRING);

				// code information break
				UMBRA_FUNCTION_CS() {
					HeadsUp::Element em;
					
					// configure basics
					em.click_function = String("jump ") + vaText.toString().c_str();
					em.click_method = HeadsUp::Element::CLICK_METHOD_CIS;

					em.text		= vaImage.toString().c_str();
					em.type		= ::AW_HUD_TYPE_IMAGE;
					em.key		= vaKey.toString().c_str();

					// standard dimensions
					em.size_x = 64;
					em.size_y = 64;
					em.size_z = 64;
					em.visible = true;

					// jump to given location
					UMBRA_FUNCTION_RET( (new HeadsUp::Element(em))->setUmbraGarbageCollected() );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// creates a heads up icon using 80x80
			//

			UMBRA_FUNCTION_BEGIN(CreateLabel) {
				// required arguments
				UMBRA_FUNCTION_DA(0, Text,  Umbra::VT_STRING);
				UMBRA_FUNCTION_DA(1, Key,   Umbra::VT_STRING);

				// code information break
				UMBRA_FUNCTION_CS() {
					HeadsUp::Element em;
					em.click_method = HeadsUp::Element::CLICK_METHOD_NONE;

					em.text		= vaText.toString().c_str();
					em.type		= ::AW_HUD_TYPE_TEXT;
					em.key		= vaKey.toString().c_str();

					// standard dimensions
					em.size_x = 64;
					em.size_y = 64;
					em.size_z = 64;
					em.visible = true;

					// jump to given location
					UMBRA_FUNCTION_RET( (new HeadsUp::Element(em))->setUmbraGarbageCollected() );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// creates a heads up icon using 80x80
			//

			UMBRA_FUNCTION_BEGIN(CreateCis) {
				// required arguments
				UMBRA_FUNCTION_DA(0, Text,  Umbra::VT_STRING);
				UMBRA_FUNCTION_DA(1, Image, Umbra::VT_STRING);
				UMBRA_FUNCTION_DA(2, Key,   Umbra::VT_STRING);
				UMBRA_FUNCTION_DA(3, Index,		Umbra::VT_INTEGER);

				// code information break
				UMBRA_FUNCTION_CS() {
					HeadsUp::Element em;
					
					// configure basics
					em.click_function = vaText.toString().c_str();
					em.click_method = HeadsUp::Element::CLICK_METHOD_CIS;

					em.text		= vaImage.toString().c_str();
					em.type		= ::AW_HUD_TYPE_IMAGE;
					em.key		= vaKey.toString().c_str();
					em.index	= vaIndex.toInt32();

					// standard dimensions
					em.size_x = 64;
					em.size_y = 64;
					em.size_z = 64;
					em.visible = true;

					// jump to given location
					UMBRA_FUNCTION_RET( (new HeadsUp::Element(em))->setUmbraGarbageCollected() );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// creates a heads up icon using 80x80
			//

			UMBRA_FUNCTION_BEGIN(CreateScriptable) {
				// required arguments
				UMBRA_FUNCTION_DA(0, Command,	Umbra::VT_STRING);
				UMBRA_FUNCTION_DA(1, Image,		Umbra::VT_STRING);
				UMBRA_FUNCTION_DA(2, Key,		Umbra::VT_STRING);
				UMBRA_FUNCTION_DA(3, Index,		Umbra::VT_INTEGER);

				// code information break
				UMBRA_FUNCTION_CS() {
					HeadsUp::Element em;
					
					// configure basics
					em.click_function = vaCommand.toString().c_str();
					em.click_method = HeadsUp::Element::CLICK_METHOD_SCRIPT;

					em.text		= vaImage.toString().c_str();
					em.type		= ::AW_HUD_TYPE_IMAGE;
					em.key		= vaKey.toString().c_str();
					em.index	= vaIndex.toInt32();
					
					// standard dimensions
					em.size_x = 64;
					em.size_y = 64;
					em.size_z = 64;
					em.visible = true;

					// jump to given location
					UMBRA_FUNCTION_RET( (new HeadsUp::Element(em))->setUmbraGarbageCollected() );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// creates a heads up icon using 80x80 that opens a links
			//

			UMBRA_FUNCTION_BEGIN(CreateWebLink) {
				// required arguments
				UMBRA_FUNCTION_DA(0, Url,		Umbra::VT_STRING);
				UMBRA_FUNCTION_DA(1, Image,		Umbra::VT_STRING);
				UMBRA_FUNCTION_DA(2, Key,		Umbra::VT_STRING);
				UMBRA_FUNCTION_DA(3, Index,		Umbra::VT_INTEGER);

				// code information break
				UMBRA_FUNCTION_CS() {
					HeadsUp::Element em;
					
					// configure basics
					em.click_function = vaUrl.toString().c_str();
					em.click_method = HeadsUp::Element::CLICK_METHOD_URL;

					em.text		= vaImage.toString().c_str();
					em.type		= ::AW_HUD_TYPE_IMAGE;
					em.key		= vaKey.toString().c_str();
					em.index	= vaIndex.toInt32();
					
					// standard dimensions
					em.size_x = 64;
					em.size_y = 64;
					em.size_z = 64;
					em.visible = true;

					// jump to given location
					UMBRA_FUNCTION_RET( (new HeadsUp::Element(em))->setUmbraGarbageCollected() );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			UMBRA_FUNCTION_BEGIN(CreateSideLink) {
				// required arguments
				UMBRA_FUNCTION_DA(0, Url,		Umbra::VT_STRING);
				UMBRA_FUNCTION_DA(1, Image,	Umbra::VT_STRING);
				UMBRA_FUNCTION_DA(2, Key,		Umbra::VT_STRING);
				UMBRA_FUNCTION_DA(3, Index,	Umbra::VT_INTEGER);

				// code information break
				UMBRA_FUNCTION_CS() {
					HeadsUp::Element em;
					
					// configure basics
					em.click_function = vaUrl.toString().c_str();
					em.click_method = HeadsUp::Element::CLICK_METHOD_SIDE_URL;

					em.text		= vaImage.toString().c_str();
					em.type		= ::AW_HUD_TYPE_IMAGE;
					em.key		= vaKey.toString().c_str();
					em.index	= vaIndex.toInt32();
					
					// standard dimensions
					em.size_x = 64;
					em.size_y = 64;
					em.size_z = 64;
					em.visible = true;

					// jump to given location
					UMBRA_FUNCTION_RET( (new HeadsUp::Element(em))->setUmbraGarbageCollected() );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// creates a heads up icon using 80x80 that changes rows
			//

			UMBRA_FUNCTION_BEGIN(CreateRowChange) {
				// required arguments
				UMBRA_FUNCTION_DA(0, Row,		Umbra::VT_STRING);
				UMBRA_FUNCTION_DA(1, Image,		Umbra::VT_STRING);
				UMBRA_FUNCTION_DA(2, Key,		Umbra::VT_STRING);
				UMBRA_FUNCTION_DA(3, Index,		Umbra::VT_INTEGER);

				// code information break
				UMBRA_FUNCTION_CS() {
					HeadsUp::Element em;
					
					// configure basics
					em.click_function = vaRow.toString().c_str();
					em.click_method = HeadsUp::Element::CLICK_METHOD_CHANGEROW;

					em.text		= vaImage.toString().c_str();
					em.type		= ::AW_HUD_TYPE_IMAGE;
					em.key		= vaKey.toString().c_str();
					em.index	= vaIndex.toInt32();

					// standard dimensions
					em.size_x = 64;
					em.size_y = 64;
					em.size_z = 64;
					em.visible = true;

					// jump to given location
					UMBRA_FUNCTION_RET( (new HeadsUp::Element(em))->setUmbraGarbageCollected() );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();


			// nothing
			return R.NOT_HANDLED;
		}
	}
}