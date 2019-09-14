#include "stdafx.h"
#include "scripting_evo2.h"
#include "crypto.h"

namespace ScriptingHost_ClassLib
{
	namespace App
	{
		bool D_Crypto::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 
		{ 
			// unsupported element
			return Umbra::PropertyRequest::NOT_HANDLED;  
		}

		bool D_Crypto::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R) 
		{ 
			UMBRA_FUNCTION_BEGIN(md5) {
				UMBRA_FUNCTION_DA(0, Text, Umbra::VT_STRING);
				// code information break
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( Crypto::md5(vaText.toString()) );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			UMBRA_FUNCTION_BEGIN(crc32) {
				UMBRA_FUNCTION_DA(0, Text, Umbra::VT_STRING);
				// code information break
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( Crypto::crc32(vaText.toString()) );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			// unsupported element
			return Umbra::FunctionRequest::NOT_HANDLED;  
		}
	}
}