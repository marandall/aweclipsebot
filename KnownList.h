#pragma once
#include "awsdk_support.h"
#include "umbra/umbra_remoteclass.h"
#include "umbra/umbra_variantmap.h"

class KnownList : 
	public AW::KnownList,
	public Umbra::RemoteClass
{
public: // umbra implemenation
	// pure reflection methods
	String getAsAnd() { return toString("and");	}
	String getAsOr()	{ return toString("or");	}

	bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		// reflect get functions
		UMBRA_PROPERTY_R(as_and,	getAsAnd());
		UMBRA_PROPERTY_R(and_or,	getAsOr());

		// items
		UMBRA_PROPERTY_R(count,	(int)size());
		UMBRA_PROPERTY_R(size,	(int)size());

		// nothing found
		return Umbra::PropertyRequest::NOT_HANDLED;
	}
};