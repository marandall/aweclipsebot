#include "stdafx.h"
#include "Movers.h"
#include "movers_autodriver.h"
#include "awsdk_entity.h"
#include "autonumber.h"
#include "scripting_iterator.h"

namespace Movers
{
	Tracker			g_Movers;

	Tracker::Tracker()
	{
	}

	Tracker::~Tracker()
	{
		clear();
	}

	void Tracker::clear()
	{
		m_list.clear();
	}

	void Tracker::push_back(Mover SourceEntity)
	{
		// push to back of list
		m_list.push_back(SourceEntity);
	}

	void Tracker::remove_by_id(int Mover_Id)
	{
		// loop through every mover known
		for (Iterator ptr = begin(); ptr != end(); ptr++)
		{
			if (ptr->getId() != Mover_Id)
				continue;

			// erase
			m_list.erase(ptr);
			return;
		}
	}
	
	/*void Tracker::executeUmbraFunction(Umbra::Variant& Returned, Umbra::string ID, Umbra::VariantMap& VMap, Umbra::ES_CallFunction& ES)
	{
		// getByName
		//  gets a mover by its id name
		UMBRA_RMTCLS_FUNCTION_BEGIN(getByName)
			UMBRA_RMTCLS_REQUIRE_ARG(Name);

			// cast to location
			Mover* pMover = getByName(vargName.toString().c_str());
			UMBRA_RMTCLS_RETURN(pMover ? *pMover : Umbra::Variant());
		UMBRA_RMTCLS_FUNCTION_END();

		// getById
		//  gets a mover by its id code
		UMBRA_RMTCLS_FUNCTION_BEGIN(getById)
			UMBRA_RMTCLS_REQUIRE_ARG(Id);

			// cast to location
			Mover* pMover = getById(vargId.toInt32());
			UMBRA_RMTCLS_RETURN(pMover ? *pMover : Umbra::Variant());
		UMBRA_RMTCLS_FUNCTION_END();

		// get an iterator
		UMBRA_RMTCLS_FUNCTION_BEGIN(getIterator);
		typedef Umbra::RuntimeIterator<MoverList_T, MoverList_T::iterator> PtrIterator;
			PtrIterator* itr = new PtrIterator(m_list, Umbra::Variant(*this));
			UMBRA_RMTCLS_RETURN( itr->setUmbraGarbageCollected() );
		UMBRA_RMTCLS_FUNCTION_END();
	} */
	

	AutoDriverKernel& getInstance() 
	{ 
		static AutoDriverKernel k; 
		return k; 
	}


	Mover* Tracker::getById(int Mover_Id)
	{
		// loop through every mover known
		for (Iterator ptr = begin(); ptr != end(); ptr++)
			if (ptr->getId() == Mover_Id)
				return &(*ptr);

		// nothing in here
		return NULL;
	}

	Mover* Tracker::getByModelId(int ModelId)
	{
		// loop through every mover known
		for (Iterator ptr = begin(); ptr != end(); ptr++)
			if (ptr->getModelNum() == ModelId)
				return &(*ptr);

		// nothing in here
		return NULL;
	}

	Mover* Tracker::getByName(String Mover_Key)
	{
		// loop through every mover known
		for (Iterator ptr = begin(); ptr != end(); ptr++)
			if (ptr->getName().CompareNoCase(Mover_Key) == 0)
				return &(*ptr);

		// nothing in here
		return NULL;
	}

	
	bool Mover::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		UMBRA_PROPERTY_REDIRECT(__super);

		// collection functions for attached iteration
		UMBRA_PROPERTY_R(connected_count, (int)connected_avatars.size());

		return Umbra::PropertyRequest::NOT_HANDLED;
	}

	bool Mover::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		UMBRA_FUNCTION_BEGIN(attach) {
			UMBRA_FUNCTION_DA(0, Av, Umbra::VT_CLASS);
			UMBRA_FUNCTION_DA(1, Dist, Umbra::VT_INT);
			UMBRA_FUNCTION_DA(2, Angle, Umbra::VT_INT);
			UMBRA_FUNCTION_DA(3, YDelta, Umbra::VT_INT);
			UMBRA_FUNCTION_DA(4, YawDelta, Umbra::VT_INT);
			UMBRA_FUNCTION_DA(5, PitchDelta, Umbra::VT_INT);
			UMBRA_FUNCTION_CC(Av, Avatar);

			// code information break
			UMBRA_FUNCTION_CS() {		
				setStateStart();
				UMBRA_FUNCTION_RET( aw_mover_rider_add(m_id, pAv->getSession(), vaDist.toInt32(), vaAngle.toInt32(), vaYDelta.toInt32(), vaYawDelta.toInt32(), vaPitchDelta.toInt32() );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(attached) {
			UMBRA_FUNCTION_DA(0, Index, Umbra::VT_INT);
			// code information break
			UMBRA_FUNCTION_CS() {	
				size_t index = vaIndex.toInt32();
				if (index > 0 && index < connected_avatars.size())
				{
					Avatar* pAv = avatars.getSession(connected_avatars[index]);
					if (pAv)
					{
						UMBRA_FUNCTION_RET( *pAv) );
					}
				}
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		return Umbra::FunctionRequest::NOT_HANDLED;
	}

	bool Tracker::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		//
		// get an iterator
		//

		UMBRA_FUNCTION_BEGIN(getIterator) {
			// code information break
			UMBRA_FUNCTION_CS() {
				// create iterator type defenition
				typedef Umbra::CodeIterator<MoverList_T::iterator> CodeItr;
				CodeItr* pItr = new CodeItr(*this, _umbra_isa, m_list.begin(), m_list.end());
				pItr->setUmbraGarbageCollected();

				UMBRA_FUNCTION_RET( *pItr );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		return Umbra::FunctionRequest::NOT_HANDLED;
	}

	//
	//
	//
	//
	//

	AutoDriver* AutoDriverKernel::createNew(String Id)
	{
		// cannot add another by the same name
		if (getEntryByName(Id) != 0)
			return 0;

		AutoDriver pEntry = AutoDriver();
		pEntry.id			= Id;
		pEntry.auto_id		= AutoNumber::getNext();
		list.push_back(pEntry);
		return &list.back();
	}

	bool AutoDriverKernel::Delete(String Id)
	{
		// loop through every mover known
		for (iterator ptr = begin(); ptr != end(); ptr++)
		{
			if (ptr->id.CompareNoCase(Id) == 0)
			{
				list.erase(ptr);
				return true;
			}
		}

		// nothing in here
		return false;
	}

	AutoDriverKernel::AutoDriverKernel()
	{
	}

	AutoDriverKernel::~AutoDriverKernel()
	{
		clear();
	}

	AutoDriverKernel& AutoDriverKernel::getInstance()
	{
		static AutoDriverKernel k;
		return k;
	}

	void AutoDriverKernel::clear()
	{
		list.clear();
	}

	bool AutoDriverKernel::process(float Resolution, CommonBot& Client)
	{
		for (iterator ptr = begin(); ptr != end(); ptr++)
			ptr->HandleMove(Resolution);
		return true;
	}

	int AutoDriver::HandleMove(float Resolution)
	{
		// handle this particular mover
		Mover* pMover = g_Movers.getByName(this->id);;
		if (pMover == NULL)
			return 0;
		
		// work out new position
		AW::LocationEx next_move;
		int action = ctrl.getNextChange(*pMover, next_move, Resolution);
		if (action != Movement::IMovement::DO_MOVE)
			return 0;

		// if the state is not start
		if (!pMover->isStateContinue())
		{
			pMover->setStateContinue();
		}

		// calculate the radians
		int x		= next_move.getX();
		int y		= next_move.getY();
		int z		= next_move.getZ();
		int yaw		= next_move.getYAW();
		int tilt	= next_move.getPitch();
		int roll	= next_move.getRoll();

		Location v;

		// set the movers position
		bot._mover_set_position(pMover->getId(), x, y, z, yaw, tilt, roll);
		pMover->setPositionAt(next_move);
		return 1;
	}

	AutoDriver* AutoDriverKernel::getEntryByName(String Id)
	{
		for (iterator ptr = begin(); ptr != end(); ptr++)
			if (ptr->id.CompareNoCase(Id) == 0)
				return &*ptr;
		return NULL;
	}

	AutoDriver* AutoDriverKernel::getEntryByAuto(int Id)
	{
		for (iterator ptr = begin(); ptr != end(); ptr++)
			if (ptr->auto_id == Id)
				return &*ptr;
		return NULL;
	}
}