#include "stdafx.h"
#include "avatars.h"

// memory allocation checking
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Umbra;

bool Avatars::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
{
	// the avatar index
	UMBRA_PROPERTY_R(count,		(int)size());
	UMBRA_PROPERTY_R(length,	(int)size());
	
	// nothing found
	return Umbra::PropertyRequest::NOT_HANDLED;
}

bool Avatars::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
{
	//
	// get an iterator
	//

	UMBRA_FUNCTION_BEGIN(getIterator) {
		// code information break
		UMBRA_FUNCTION_CS() {
			// create iterator type defenition
			typedef Umbra::CodeIterator<iterator> CodeItr;
			CodeItr* pItr = new CodeItr(*this, _umbra_isa, avCol.begin(), avCol.end());
			pItr->setUmbraGarbageCollected();

			UMBRA_FUNCTION_RET( *pItr );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// the avatar index
	//
	
	UMBRA_FUNCTION_BEGIN(get) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Index,  Umbra::VT_INTEGER);

		// code information break
		UMBRA_FUNCTION_CS() {
			size_t index = vaIndex.toInt32();
			UMBRA_FUNCTION_RET( (index >= 0 && index < size()) ? *avCol[index] : Variant() );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// search for an avatar by their avatar key
	//

	UMBRA_FUNCTION_BEGIN(getName) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Key,  Umbra::VT_STRING);

		// code information break
		UMBRA_FUNCTION_CS() {
			// lookup the user, nulify if not found
			Avatar* pUser = getName(vaKey.toString().c_str());
			if (pUser != 0)
			{
				UMBRA_FUNCTION_RET( *pUser );
			}

			UMBRA_FUNCTION_RET( *pUser );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// search for an avatar by their avatar name
	//

	UMBRA_FUNCTION_BEGIN(search) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Key,  Umbra::VT_STRING);

		// code information break
		UMBRA_FUNCTION_CS() {
			// lookup the user, nulify if not found
			Avatar* pUser = getName(vaKey.toString().c_str());
			if (pUser != 0)
			{
				UMBRA_FUNCTION_RET( *pUser );
			}
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();


	//
	// search for an avatar by their avatar name
	//

	UMBRA_FUNCTION_BEGIN(search) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Key,  Umbra::VT_STRING);

		// code information break
		UMBRA_FUNCTION_CS() {
			// lookup the user, nulify if not found
			Avatar* pUser = getName(vaKey.toString().c_str());
			if (pUser != 0)
			{
				UMBRA_FUNCTION_RET( *pUser );
			}
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();


	// nothing found
	return Umbra::FunctionRequest::NOT_HANDLED;
}