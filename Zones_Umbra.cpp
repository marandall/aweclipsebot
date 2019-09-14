#include "stdafx.h"
#include "zones.h"

using namespace Umbra;
using namespace Zones;

//
//
//

bool Controller::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
{
	UMBRA_PROPERTY_R(root, m_root);

	// nothing found
	return R.NOT_HANDLED;
}

//
//
//

bool Controller::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
{
	UMBRA_FUNCTION_BEGIN(getZoneByPosition) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Position, Umbra::VT_CLASS);
		UMBRA_FUNCTION_CC(Position, AW::Location);	
		// code block
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( *getZone(*pPosition, 0) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(getById) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Id, Umbra::VT_STRING);
		// code block
		UMBRA_FUNCTION_CS() {
			Zone* pFound = m_root.search(vaId.toString().c_str());
			UMBRA_FUNCTION_RET( pFound ? *pFound : Umbra::Variant() );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// File IO
	//

	UMBRA_FUNCTION_BEGIN(useRoot) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Root, Umbra::VT_CLASS);
		UMBRA_FUNCTION_CC(Root, Zone);	
		// code information break
		UMBRA_FUNCTION_CS() {
			m_root = *pRoot;
			m_root.update_after_changes();
			for (int i = 0; i < static_cast<int>(avatars.size()); i++)
			{
				avatars[i].ZoneReset();
				avatars[i].ZoneModifyPos(avatars[i]);
			}
			UMBRA_FUNCTION_RET( m_root );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();
		
	UMBRA_FUNCTION_BEGIN(load) {
		// required arguments
		UMBRA_FUNCTION_DA(0, File, Umbra::VT_STRING);
		// code block
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( m_root.load(vaFile.toString().c_str()) ); 
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(save) {
		// required arguments
		UMBRA_FUNCTION_DA(0, File, Umbra::VT_STRING);
		// code block
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( m_root.save(vaFile.toString().c_str()) ); 
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	// nothing found
	return Umbra::FunctionRequest::NOT_HANDLED;
}

bool Zone::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
{
	// check base classes
	UMBRA_PROPERTY_REDIRECT(Zones::AutoTemplates::ZoneData);

	// zone information
	UMBRA_PROPERTY_R(auto_id,							getAutoNumber());
	UMBRA_PROPERTY_R(name,								getName());
	UMBRA_PROPERTY_R(full_descriptor,			getFullDescriptor());

	// overrides
	UMBRA_PROPERTY_R(override_fly,				getOverrideFly());
	UMBRA_PROPERTY_R(override_shift,			getOverrideShift());
	UMBRA_PROPERTY_R(override_teleport,		getOverrideTeleport());
	UMBRA_PROPERTY_R(override_template,		getOverrideTemplate());
	UMBRA_PROPERTY_R(override_final,			getOverrideFinal());

	// logical overrides
	UMBRA_PROPERTY_R(logical_fly,					getLogicalFly());
	UMBRA_PROPERTY_R(logical_shift,				getLogicalShift());
	UMBRA_PROPERTY_R(logical_teleport,		getLogicalTeleport());
	UMBRA_PROPERTY_R(logical_template,		getLogicalTemplate());
	UMBRA_PROPERTY_R(logical_final,				getLogicalFinal());

	// parents
	UMBRA_PROPERTY_R(parent,							parent_zone ? Umbra::Variant() : *parent_zone);
	UMBRA_PROPERTY_R(child_count,					getChildCount());

	// chat channels
	UMBRA_PROPERTY_R(logical_comms_channel, getLogicalCommsChannel());

	// nothing found
	return Umbra::PropertyRequest::NOT_HANDLED;
}

bool Zone::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
{
	// search by an id key
	UMBRA_FUNCTION_BEGIN(search) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Text,  Umbra::VT_STRING);

		// code information break
		UMBRA_FUNCTION_CS() {
			Zone* pZone = search(vaText.toString().c_str());
			if (pZone) { UMBRA_FUNCTION_RET( *pZone ) }
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(getChildByPosition) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Position, Umbra::VT_CLASS);
		UMBRA_FUNCTION_OA(1, Self, Umbra::VT_BOOLEAN, true);
		UMBRA_FUNCTION_CC(Position, AW::Location);	
		// code block
		UMBRA_FUNCTION_CS() {
			Zone* pZ = FindZone(*pPosition, 0);
			if (pZ)
			{
				UMBRA_FUNCTION_RET( *pZ );
			}
			else if (vaSelf.toBool() == true && id == ".")
			{
				UMBRA_FUNCTION_RET( *this );
			}
			else
			{
				UMBRA_FUNCTION_RET( Umbra::Variant() );
			}
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(getChildById) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Id, Umbra::VT_STRING);
		// code block
		UMBRA_FUNCTION_CS() {
			Zone* pFound = search(vaId.toString().c_str());
			UMBRA_FUNCTION_RET( pFound ? *pFound : Umbra::Variant() );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(clone) {
		UMBRA_FUNCTION_CS() {
			Zone* pCopy = new Zone();
			*pCopy = *this;
			pCopy->update_after_changes();
			UMBRA_FUNCTION_RET( pCopy->setUmbraGarbageCollected() );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// child control system
	//
	
	UMBRA_FUNCTION_BEGIN(createChild) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Id, Umbra::VT_STRING);

		// code information break
		UMBRA_FUNCTION_CS() {
			Zone z;
			z.id = vaId.toString().c_str();
			z.parent_zone = this;
			children.push_back(z);
			update_after_changes();

			// return the last item
			UMBRA_FUNCTION_RET( children.back() )
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(deleteChildren) {
		// code information break
		UMBRA_FUNCTION_CS() {
			children.clear();
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(deleteChild) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Id, Umbra::VT_INTEGER);
		// code information break
		UMBRA_FUNCTION_CS() {
			for (ZoneList::iterator zptr = children.begin(); zptr != children.end(); zptr++)
			{
				if (zptr->getAutoNumber() == vaId.toInt32())
				{
					children.erase(zptr);
					UMBRA_FUNCTION_RET( true );
				}
			}
			
			// could not find it
			UMBRA_FUNCTION_RET( false );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(deleteImmediateChildById) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Id, Umbra::VT_STRING);
		// code information break
		UMBRA_FUNCTION_CS() {
			String name = vaId.toString().c_str();
			for (ZoneList::iterator zptr = children.begin(); zptr != children.end(); zptr++)
			{
				if (zptr->id == name)
				{
					children.erase(zptr);
					UMBRA_FUNCTION_RET( true );
				}
			}
			
			// could not find it
			UMBRA_FUNCTION_RET( false );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// Region Management
	//

	UMBRA_FUNCTION_BEGIN(createRectangularRegion) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Top, Umbra::VT_CLASS);
		UMBRA_FUNCTION_DA(1, Bottom, Umbra::VT_CLASS);
		UMBRA_FUNCTION_OA(2, Id, Umbra::VT_STRING, String("rect") + ToString(regions.rectangles.size()) );
		UMBRA_FUNCTION_CC(Top, AW::Location);	
		UMBRA_FUNCTION_CC(Bottom, AW::Location);	
		// code information break
		UMBRA_FUNCTION_CS() {
			Zones::Regions::Rectangle r;
			r.name		= vaId.toString().c_str();
			r.top			= pTop->toHighPoint(*pBottom);
			r.bottom	= pTop->toLowPoint(*pBottom);
			regions.rectangles.push_back(r);

			// return the last item
			UMBRA_FUNCTION_RET( regions.rectangles.back() )
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(createCylindricalRegion) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Base, Umbra::VT_CLASS);
		UMBRA_FUNCTION_DA(1, Radius, Umbra::VT_INTEGER);
		UMBRA_FUNCTION_DA(2, Height, Umbra::VT_INTEGER);
		UMBRA_FUNCTION_OA(3, Id, Umbra::VT_STRING, String("cyl") + ToString(regions.cylinders.size()) );
		UMBRA_FUNCTION_CC(Base, AW::Location);	
		// code information break
		UMBRA_FUNCTION_CS() {
			Zones::Regions::Cylinder r;
			r.name		= vaId.toString().c_str();
			r.centre	= *pBase;
			r.height	= vaHeight.toInt32();
			r.radius	= vaRadius.toInt32();
			regions.cylinders.push_back(r);

			// return the last item
			UMBRA_FUNCTION_RET( regions.cylinders.back() )
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	// nothing found
	return Umbra::FunctionRequest::NOT_HANDLED;
}