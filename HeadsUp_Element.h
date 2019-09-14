#pragma once
#include "awsdk_hudelement.h"
#include "autonumber.h"

namespace HeadsUp
{
	class Element : public AW::HUD_Element
	{
	public:
		int		line_index;			// the index to insert it at
		bool	visible;			// is the item visible or not
		bool	is_visible;			// is the item shown or not
		
	public: // identificaion
		String	key;				// the identifier
		int			index;				// the position in the tray
		String	row;				// which row is this a part of

	public: // click style
		int			click_method;	
		String	click_function;			// the event for clicking

	public: // target session
		Umbra::Variant owner_session;

	public: // click mehtod styles
		static const int CLICK_METHOD_NONE			= 0;
		static const int CLICK_METHOD_CIS				= 1;
		static const int CLICK_METHOD_SCRIPT		= 2;
		static const int CLICK_METHOD_URL				= 3;
		static const int CLICK_METHOD_EXPAND		= 4;
		static const int CLICK_METHOD_COLLAPSE	= 5;
		static const int CLICK_METHOD_CHANGEROW	= 6;
		static const int CLICK_METHOD_SIDE_URL	= 7;
		static const int CLICK_METHOD_TELEPORT	= 8;
		static const int CLICK_METHOD_MAX				= 9;

	public: // conver the items
		static String MethodToString(int ID)
		{
			switch (ID) 
			{
			case CLICK_METHOD_NONE:				return "None";
			case CLICK_METHOD_CIS:				return "Command";
			case CLICK_METHOD_SCRIPT:			return "Script";
			case CLICK_METHOD_URL:				return "Url";
			case CLICK_METHOD_EXPAND:			return "Expand";
			case CLICK_METHOD_COLLAPSE:		return "Collapse";
			case CLICK_METHOD_CHANGEROW:	return "Change Row";
			case CLICK_METHOD_SIDE_URL:		return "Sidebar URL";
			case CLICK_METHOD_TELEPORT:		return "Teleport";
			default:											return "Unknown";
			}
		}

	public: // additional instances
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
			UMBRA_PROPERTY_REDIRECT(AW::HUD_Element);

			// the additional properties
			UMBRA_PROPERTY_RW(click_function,	click_function);
			UMBRA_PROPERTY_RW(visible,			visible);
			UMBRA_PROPERTY_RW(key,				key);
			UMBRA_PROPERTY_RW(index,			index);
			UMBRA_PROPERTY_RW(row,				row);

			// nothing found
			return R.NOT_HANDLED;
		}

		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
		{
			UMBRA_FUNCTION_REDIRECT(AW::HUD_Element);

			UMBRA_FUNCTION_BEGIN(setRow) {
				UMBRA_FUNCTION_OA(0, RowKey, Umbra::VT_STRING, "");

				// code information break
				UMBRA_FUNCTION_CS() {
					row = vaRowKey.toString().c_str();
					UMBRA_FUNCTION_RET( *this );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// size functions
			//

			UMBRA_FUNCTION_BEGIN(resize) {
				UMBRA_FUNCTION_OA(0, X, Umbra::VT_INTEGER, 0);
				UMBRA_FUNCTION_OA(1, Y, Umbra::VT_INTEGER, 0);
				UMBRA_FUNCTION_OA(2, Z, Umbra::VT_INTEGER, 0);

				// code information break
				UMBRA_FUNCTION_CS() {
					size_x = vaX.toInt32();
					size_y = vaY.toInt32();
					size_z = vaZ.toInt32();
					UMBRA_FUNCTION_RET( *this );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// Alignment Functions
			// 

			UMBRA_FUNCTION_BEGIN(attachTopLeft) {
				UMBRA_FUNCTION_OA(0, X, Umbra::VT_INTEGER, 0);
				UMBRA_FUNCTION_OA(1, Y, Umbra::VT_INTEGER, 0);

				// code information break
				UMBRA_FUNCTION_CS() {
					origin = AW_HUD_ORIGIN_TOP_LEFT;
					x = vaX.toInt32();
					y = vaY.toInt32();
					UMBRA_FUNCTION_RET( *this );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			UMBRA_FUNCTION_BEGIN(attachBottomLeft) {
				UMBRA_FUNCTION_OA(0, X, Umbra::VT_INTEGER, 0);
				UMBRA_FUNCTION_OA(1, Y, Umbra::VT_INTEGER, 0);

				// code information break
				UMBRA_FUNCTION_CS() {
					origin = AW_HUD_ORIGIN_BOTTOM_LEFT;
					x = vaX.toInt32();
					y = -vaY.toInt32() - size_y;
					UMBRA_FUNCTION_RET( *this );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			UMBRA_FUNCTION_BEGIN(attachTopRight) {
				UMBRA_FUNCTION_OA(0, X, Umbra::VT_INTEGER, 0);
				UMBRA_FUNCTION_OA(1, Y, Umbra::VT_INTEGER, 0);

				// code information break
				UMBRA_FUNCTION_CS() {
					origin = AW_HUD_ORIGIN_TOP_RIGHT;
					x = -vaX.toInt32() - size_x;
					y = vaY.toInt32();
					UMBRA_FUNCTION_RET( *this );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();


			// if we are not null
			Avatar* pOwner = dynamic_cast<Avatar*>(owner_session.toRemoteClass());
			if (pOwner != 0)
			{	
				UMBRA_FUNCTION_BEGIN(apply) {
					UMBRA_FUNCTION_CS() {
						pOwner->CreateHUD(*this);

						UMBRA_FUNCTION_RET( true );
					} UMBRA_FUNCTION_CE();
				} UMBRA_FUNCTION_END();

				UMBRA_FUNCTION_BEGIN(hide) {
					UMBRA_FUNCTION_CS() {
						pOwner->DestroyHUD(id);
					} UMBRA_FUNCTION_CE();
				} UMBRA_FUNCTION_END();
			}

			// nothing found
			return R.NOT_HANDLED;
		}

	public:
		Element() 
		{ 
			line_index	= 0; 
			visible		= false; 
			is_visible	= false;
			index		= 0;

			// click mehtods
			click_method = CLICK_METHOD_NONE;

			// assignment
			id			= AutoNumber::getNext();
		}
	};
}