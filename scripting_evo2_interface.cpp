#include "stdafx.h"
#include "scripting_evo2.h"
#include "eclipseevolutiondialog.h"
#include <Mmsystem.h>
#include "InputLineDialog.h"

namespace ScriptingHost_ClassLib
{
	namespace App
	{
		bool D_Interface::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 
		{ 
			// balloon styles
			UMBRA_PROPERTY_C(BALLOON_ICON_NONE,			NIIF_NONE);
			UMBRA_PROPERTY_C(BALLOON_ICON_INFO,			NIIF_INFO);
			UMBRA_PROPERTY_C(BALLOON_ICON_WARNING,	NIIF_WARNING)
			UMBRA_PROPERTY_C(BALLOON_ICON_ERROR,		NIIF_ERROR);

			// unsupported element
			return Umbra::PropertyRequest::NOT_HANDLED;  
		}

		bool D_Interface::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R) 
		{ 
			//
			// Alert Beeps
			//

			UMBRA_FUNCTION_BEGIN(beep) {
				UMBRA_FUNCTION_CS() {
					BOOL e = PlaySound(MAKEINTRESOURCE(IDR_BEEP1), GetModuleHandle(0), SND_RESOURCE | SND_ASYNC);
					int rc = GetLastError();
					int j = 0;
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			UMBRA_FUNCTION_BEGIN(beep2) {
				UMBRA_FUNCTION_CS() {
					PlaySound(MAKEINTRESOURCE(IDR_BEEP2), GetModuleHandle(0), SND_RESOURCE | SND_ASYNC);
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			UMBRA_FUNCTION_BEGIN(beep3) {
				UMBRA_FUNCTION_CS() {
					PlaySound(MAKEINTRESOURCE(IDR_BEEP3), GetModuleHandle(0), SND_RESOURCE | SND_ASYNC);
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// custom sounds
			//

			UMBRA_FUNCTION_BEGIN(playSound) {
				UMBRA_FUNCTION_DA(0, Path, Umbra::VT_STRING);
				// code information break
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( PlaySoundA(vaPath.toString().c_str(), NULL, SND_FILENAME | SND_ASYNC) );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			UMBRA_FUNCTION_BEGIN(playWindowsSound) {
				UMBRA_FUNCTION_DA(0, Type, Umbra::VT_STRING);
				// code information break
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( PlaySoundA(vaType.toString().c_str(), NULL, SND_ALIAS_ID | SND_ASYNC) );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// Taskbar
			//

			UMBRA_FUNCTION_BEGIN(popupTrayBalloon) {
				UMBRA_FUNCTION_DA(0, Text,  Umbra::VT_STRING);
				UMBRA_FUNCTION_OA(1, Title, Umbra::VT_STRING, "");

				// code information break
				UMBRA_FUNCTION_CS() {
					String title = vaTitle.toString().c_str();
					if (title.GetLength() == 0)
					{
						title.Format("[%s] in %s", bot.getBotName(), bot._string(AW_WORLD_NAME));
					}

				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();


			//
			// Request Information
			//

			UMBRA_FUNCTION_BEGIN(prompt) {
				UMBRA_FUNCTION_DA(0, Text, Umbra::VT_STRING);
				UMBRA_FUNCTION_OA(1, Title, Umbra::VT_STRING, "Prompt");
				UMBRA_FUNCTION_OA(2, Default, Umbra::VT_STRING, "");
				UMBRA_FUNCTION_CS() {
					InputLineDialog d;
					d.v_Text	= vaDefault.toString().c_str();
					d.title 	= vaTitle.toString().c_str();
					d.v_about	= vaText.toString().c_str();

					// only return if IDOK given
					if (d.DoModal() == IDOK)
					{
						UMBRA_FUNCTION_RET( (LPCSTR)d.v_Text );
					}
					else
					{
						UMBRA_FUNCTION_RET( Umbra::Variant() );
					}
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();


			UMBRA_FUNCTION_BEGIN(confirm) {
				UMBRA_FUNCTION_DA(0, Text, Umbra::VT_STRING);
				UMBRA_FUNCTION_OA(1, Title, Umbra::VT_STRING, "Script Confirm");
				UMBRA_FUNCTION_OA(2, Icon, Umbra::VT_INTEGER, MB_ICONQUESTION);
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( rt_diag->MessageBox(vaText.toString().c_str(), vaTitle.toString().c_str(), vaIcon.toInt32() | MB_YESNO) == IDYES );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			// unsupported element
			return Umbra::FunctionRequest::NOT_HANDLED;  
		}
	}
}