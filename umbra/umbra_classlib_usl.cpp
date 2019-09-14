#include "stdafx.h"
#include "umbra_classlib_usl.h"
#include "umbra_classlib_usl_urlparseinfo.h"
#include "umbra_tools.h"
#include "umbra_cst_httprequest.h"

namespace Umbra { namespace ClassLib { namespace Usl { 

	bool D_Usl::doUmbraFunction(Variant& V, FunctionRequest& R)
	{
		//
		// encode special characters
		//

		UMBRA_FUNCTION_BEGIN(urlencode)
			UMBRA_FUNCTION_DA(0, Text, Umbra::VT_STRING);
			UMBRA_FUNCTION_CS()
				UMBRA_FUNCTION_RET( Umbra::Tools::urlencode(vaText.toString()) );
			UMBRA_FUNCTION_CE()
		UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(urldecode)
			UMBRA_FUNCTION_DA(0, Text, Umbra::VT_STRING);
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( Umbra::Tools::urldecode(vaText.toString()) );
			} UMBRA_FUNCTION_CE()
		UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(htmlspecialchars)
			UMBRA_FUNCTION_DA(0, Text, Umbra::VT_STRING);
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( Umbra::Tools::htmlspecialchars(vaText.toString()) );
			} UMBRA_FUNCTION_CE()
		UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(htmlspecialchars_decode)
			UMBRA_FUNCTION_DA(0, Text, Umbra::VT_STRING);
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( Umbra::Tools::htmlspecialchars_decode(vaText.toString()) );
			} UMBRA_FUNCTION_CE()
		UMBRA_FUNCTION_END();

		//
		// Logical Handling
		//

		UMBRA_FUNCTION_BEGIN(iif)
			UMBRA_FUNCTION_DA(0, Q1, Umbra::VT_ANY);
			UMBRA_FUNCTION_DA(0, V1, Umbra::VT_ANY);
			UMBRA_FUNCTION_DA(0, V2, Umbra::VT_ANY);
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( vaQ1.toBool() ? vaV1 : vaV2 );
			} UMBRA_FUNCTION_CE()
		UMBRA_FUNCTION_END();

		// 
		// conversion
		//

		UMBRA_FUNCTION_BEGIN(bin2hex)
			UMBRA_FUNCTION_DA(0, Text, Umbra::VT_STRING);
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( Umbra::Tools::bin2hex(vaText.toString()) );
			} UMBRA_FUNCTION_CE()
		UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(hex2bin)
			UMBRA_FUNCTION_DA(0, Text, Umbra::VT_STRING);
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( Umbra::Tools::hex2bin(vaText.toString()) );
			} UMBRA_FUNCTION_CE()
		UMBRA_FUNCTION_END();

		//
		// String Capitalisation
		//

		UMBRA_FUNCTION_BEGIN(ucfirst)
			UMBRA_FUNCTION_DA(0, Text, Umbra::VT_STRING);
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( Umbra::Tools::ucfirst(vaText.toString()) );
			} UMBRA_FUNCTION_CE()
		UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(ucwords)
			UMBRA_FUNCTION_DA(0, Text, Umbra::VT_STRING);
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( Umbra::Tools::ucwords(vaText.toString()) );
			} UMBRA_FUNCTION_CE()
		UMBRA_FUNCTION_END();

		//
		// String Implosion
		//

		UMBRA_FUNCTION_BEGIN(implodeArgs)
			UMBRA_FUNCTION_DA(0, Glue, Umbra::VT_STRING);
			UMBRA_FUNCTION_CS() {
				// generate a list of each argument
				std::vector<std::string> pieces;
				std::string result;
				for (size_t i = 1; i < R.args.size(); i++)
				{
					result.append(R.args[i].toString());					
					if ((i+1) != R.args.size())
						result.append(vaGlue.toString());
				}
				// return the result of the implosion
				UMBRA_FUNCTION_RET( result );
			} UMBRA_FUNCTION_CE()
		UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(implode)
			UMBRA_FUNCTION_DA(0, Array, Umbra::VT_CLASS);
			UMBRA_FUNCTION_DA(1, Glue, Umbra::VT_STRING);
			UMBRA_FUNCTION_CC(Array, VectorImpl);
			UMBRA_FUNCTION_CS() {
				// generate a list of each argument
				std::vector<std::string> pieces;
				std::string result;
				for (size_t i = 0; i < pArray->arr.size(); i++)
				{
					result.append(pArray->arr[i].toString());					
					if ((i+1) != pArray->arr.size())
						result.append(vaGlue.toString());
				}
				// return the result of the implosion
				UMBRA_FUNCTION_RET( result );
			} UMBRA_FUNCTION_CE()
		UMBRA_FUNCTION_END();

		//
		// String Repetition
		//

		UMBRA_FUNCTION_BEGIN(str_repeat)
			UMBRA_FUNCTION_DA(0, Text, Umbra::VT_STRING);
			UMBRA_FUNCTION_DA(1, Count, Umbra::VT_INTEGER);
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( Umbra::Tools::str_repeat(vaText.toString(), vaCount.toInt32()) );
			} UMBRA_FUNCTION_CE()
		UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(str_pad)
			UMBRA_FUNCTION_DA(0, Count, Umbra::VT_INTEGER);
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( Umbra::Tools::str_pad(vaCount.toInt32()) );
			} UMBRA_FUNCTION_CE()
		UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(chr)
			UMBRA_FUNCTION_DA(0, Symbol, Umbra::VT_INTEGER);
			UMBRA_FUNCTION_CS() {
				char out[8];
				sprintf_s(out, sizeof(out), "%c", (char)vaSymbol.toInt32());
				UMBRA_FUNCTION_RET( string(out) );
			} UMBRA_FUNCTION_CE()
		UMBRA_FUNCTION_END();

		//
		// Type Parsing
		//

		UMBRA_FUNCTION_BEGIN(parseInt)
			UMBRA_FUNCTION_DA(0, Text, Umbra::VT_STRING);
			UMBRA_FUNCTION_DA(1, Radix, Umbra::VT_INTEGER);
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( Umbra::Tools::parseint(vaText.toString(), vaRadix.toInt32()) );
			} UMBRA_FUNCTION_CE()
		UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(parseDouble)
			UMBRA_FUNCTION_DA(0, Text, Umbra::VT_STRING);
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( Umbra::Tools::parsedouble(vaText.toString()) );
			} UMBRA_FUNCTION_CE()
		UMBRA_FUNCTION_END();


		//
		// Networking
		//

		UMBRA_FUNCTION_BEGIN(parse_url)
			UMBRA_FUNCTION_DA(0, Path, Umbra::VT_STRING);
			UMBRA_FUNCTION_CS() {
				Umbra::Cst::UrlParseInfo upi = Umbra::Cst::HttpRequest::ParseUrl(vaPath.toString());

				// create the new class
				Umbra::ClassLib::Usl::UrlParseInfoImpl* pImp = new Umbra::ClassLib::Usl::UrlParseInfoImpl();
				pImp->setUmbraGarbageCollected();
				pImp->upi = upi;
				UMBRA_FUNCTION_RET( *pImp );
			} UMBRA_FUNCTION_CE()
		UMBRA_FUNCTION_END();

		//
		// file actions
		//

		UMBRA_FUNCTION_BEGIN(file_get_contents)
			UMBRA_FUNCTION_DA(0, Path, Umbra::VT_STRING);
			UMBRA_FUNCTION_CS() {
				std::string fc = Umbra::Tools::file_get_contents(vaPath.toString());
				UMBRA_FUNCTION_RET( fc );
			} UMBRA_FUNCTION_CE()
		UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(url_get_contents)
		{
			UMBRA_FUNCTION_DA(0, Path, Umbra::VT_STRING);
			UMBRA_FUNCTION_CS() 
			{
				Umbra::Cst::HttpRequest req;
				if (req.get(vaPath.toString()) == 0)
				{
					UMBRA_FUNCTION_RET(req.body);
				}
				else
				{
					UMBRA_FUNCTION_RET( Umbra::Variant() );
				}
			} 
			UMBRA_FUNCTION_CE()
		}
		UMBRA_FUNCTION_END();

		// nothing
		return R.NOT_HANDLED;
	}

} } }