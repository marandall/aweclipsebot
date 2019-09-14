#pragma once
#include "umbra_classlib.h"
#include "umbra_cst_httprequest.h"

namespace Umbra { namespace ClassLib { namespace Usl {

	class UrlParseInfoImpl : public RemoteClass
	{
	public: // file information
		Umbra::Cst::UrlParseInfo upi;

	public: // umbra function handling
		virtual std::string getUmbraClass() 
		{ 
			return "Usl.UrlParseInfo"; 
		}

		bool doUmbraProperty(Variant& V, PropertyRequest& R)
		{
			UMBRA_PROPERTY_R(source,	upi.source);
			UMBRA_PROPERTY_R(protocol,upi.protocol);
			UMBRA_PROPERTY_R(host,		upi.host);
			UMBRA_PROPERTY_R(path,		upi.path);
			UMBRA_PROPERTY_R(port,		upi.port);

			// nothing found
			return PropertyRequest::NOT_HANDLED;
		}
	};

} } }