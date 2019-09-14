#include "stdafx.h"
#include "EclipseBot.h"
#include "GlobalUmbra.h"
#include "worldattributes.h"
#include "property_query_world.h"

using namespace Umbra;

Umbra::VariantMap EclipseBot::CreateVariantMap(Avatar* pUser)
{
	// create the map
	Umbra::VariantMap vmap;

	// the bots
	vmap["bot"]				= *this;
	vmap["avatars"]			= avatars;
	
	// add the user if the map exists
	if (pUser) vmap["user"]	= *pUser;
	return vmap;
}

bool EclipseBot::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
{
	// use the common bot interface
	UMBRA_FUNCTION_REDIRECT(CommonBot);

	//
	// read only attribute access
	//
	
	UMBRA_FUNCTION_BEGIN(aw_int) {
		UMBRA_FUNCTION_DA(0, AttrId, Umbra::VT_INTEGER);
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( _int((AW_ATTRIBUTE)vaAttrId.toInt32()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(aw_string) {
		UMBRA_FUNCTION_DA(0, AttrId, Umbra::VT_INTEGER);
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( _string((AW_ATTRIBUTE)vaAttrId.toInt32()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(aw_bool) {
		UMBRA_FUNCTION_DA(0, AttrId, Umbra::VT_INTEGER);
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( _bool((AW_ATTRIBUTE)vaAttrId.toInt32()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(aw_float) {
		UMBRA_FUNCTION_DA(0, AttrId, Umbra::VT_INTEGER);
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( _float((AW_ATTRIBUTE)vaAttrId.toInt32()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// change world attributes
	//
	
	UMBRA_FUNCTION_BEGIN(changeWorldAttributes) {
		// required arguments
		UMBRA_FUNCTION_DA(0, World,    Umbra::VT_CLASS);
		UMBRA_FUNCTION_CC(World, WorldAttributes);

		// code information break
		UMBRA_FUNCTION_CS() {
			pWorld->to_sdk(*this);
			UMBRA_FUNCTION_RET(_world_attributes_change());
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();


	//
	// change world attributes
	//
	
	UMBRA_FUNCTION_BEGIN(getWorldAttributes) {
		// code information break
		UMBRA_FUNCTION_CS() {
			WorldAttributes* attr = new WorldAttributes();
			attr->setUmbraGarbageCollected();
			attr->from_sdk(*this);

			UMBRA_FUNCTION_RET(*attr);
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// change world attributes
	//
	
	UMBRA_FUNCTION_BEGIN(say) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Text, Umbra::VT_STRING);

		// code information break
		UMBRA_FUNCTION_CS() {
			std::string out = vaText.toString();
			UMBRA_FUNCTION_RET( say(out.c_str()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// querying property
	//
	
	UMBRA_FUNCTION_BEGIN(queryWorld) {
		// required arguments
		UMBRA_FUNCTION_OA(0, Type, Umbra::VT_INTEGER, 0);
		UMBRA_FUNCTION_OA(1, Project, Umbra::VT_STRING, "__live");
		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( queryBackup(vaType.toInt32(), "@__live") );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(query5x5) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Place, Umbra::VT_CLASS);
		UMBRA_FUNCTION_CC(Place, AW::Location);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( Query5x5(pPlace->getCellX(), pPlace->getCellZ()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	// could not be handled
	return R.NOT_HANDLED;
}