#pragma once
#include "umbra_classlib.h"
#include "umbra_classlib_usl_crypto_crc32.h"

namespace Umbra { namespace ClassLib { namespace Usl { namespace Crypto {

	class CryptoNamespace : public Umbra::RemoteClass
	{
	public: // simple function access
		bool doUmbraFunction(Variant& V, FunctionRequest& R)
		{
			// carry out crc32 function on a piece of text
			//

			UMBRA_FUNCTION_BEGIN(crc32) {
				UMBRA_FUNCTION_DA(0, Text, VT_STRING);
				UMBRA_FUNCTION_CS() {
					Crc32Tool crc;
					UMBRA_FUNCTION_RET( crc.Get_CRC(vaText.toString()) );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			// unable to handle this request
			return R.NOT_HANDLED;
		}
	};

// end of namespace
} } } }