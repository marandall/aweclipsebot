#include "stdafx.h"
#include "awsdk_entity.h"
#include "awsdk_bot.h"
#include <vector>

namespace AW
{
	void Entity::on_update(Bot& Client)
	{
		// location
		m_type	= Client._int(AW_ENTITY_TYPE);
		m_id	= Client._int(AW_ENTITY_ID);
		m_state	= Client._int(AW_ENTITY_STATE);
		m_flags	= Client._int(AW_ENTITY_FLAGS);

		// position
		m_x		= Client._int(AW_ENTITY_X);
		m_y		= Client._int(AW_ENTITY_Y);
		m_z		= Client._int(AW_ENTITY_Z);
		m_yaw	= Client._int(AW_ENTITY_YAW);
		m_pitch = Client._int(AW_ENTITY_PITCH);
		m_roll	= Client._int(AW_ENTITY_ROLL);
	}

	void Entity::on_entity_add(Bot& Client)
	{
		on_update(Client);

		// additional data
		m_owner_session	= Client._int(AW_ENTITY_OWNER_SESSION);
		m_model_num		= Client._int(AW_ENTITY_MODEL_NUM);
		m_owner_citizen	= Client._int(AW_ENTITY_OWNER_CITIZEN);

		// position of start
		m_origin.setTilt(Client._int(AW_OBJECT_TILT));
		m_origin.setTilt(Client._int(AW_OBJECT_ROLL));
		m_origin.set(
				Client._int(AW_OBJECT_X),
				Client._int(AW_OBJECT_Y),
				Client._int(AW_OBJECT_Z),
				Client._int(AW_OBJECT_YAW)
			);

		// extract the object name
		UINT len = 0;
		void* pData = Client._data(AW_OBJECT_DATA, &len);
		
		// create temp object
		std::vector<char> obj_data;
		obj_data.resize(len, 0);
		memcpy(&obj_data[0], pData, len);

		// alloc object
		aw_object_data_mover* pZone = (aw_object_data_mover*)&obj_data[0];
		String temp = pZone->str_data;
		m_name = temp.Mid(0, pZone->name_len);
	}
	
	void Entity::on_entity_change(Bot& Client)
	{
		on_update(Client);
	}

	void Entity::on_entity_delete(Bot& Client)
	{

	}




	int Entity::setStateIdle()
	{ 
		int next_state = AW_MOVER_STATE_IDLE;
		int rc = m_pBot->_mover_set_state(m_id, next_state, m_model_num);
		m_state = next_state;
		return rc;
	}

	int Entity::setStateStart()
	{
		int next_state = AW_MOVER_STATE_START;
		int rc = m_pBot->_mover_set_state(m_id, next_state, m_model_num);
		m_state = next_state;
		return rc;
	}

	int Entity::setStateMove() 
	{
		int next_state = AW_MOVER_STATE_MOVE;
		int rc = m_pBot->_mover_set_state(m_id, next_state, m_model_num);
		m_state = next_state;
		return rc;
	}

	int Entity::setStatePause() 
	{
		int next_state = AW_MOVER_STATE_PAUSE;
		int rc = m_pBot->_mover_set_state(m_id, next_state, m_model_num);
		m_state = next_state;
		return rc;
	}

	int Entity::setStateContinue() 
	{
		int next_state = AW_MOVER_STATE_CONTINUE;
		int rc = m_pBot->_mover_set_state(m_id, next_state, m_model_num);
		m_state = next_state;
		return rc;
	}

	int Entity::setStateStop() 
	{
		int next_state = AW_MOVER_STATE_STOP;
		int rc = m_pBot->_mover_set_state(m_id, next_state, m_model_num);
		m_state = next_state;
		return rc;
	}

	int Entity::setStateReset() 
	{
		int next_state = AW_MOVER_STATE_RESET;
		int rc = m_pBot->_mover_set_state(m_id, next_state, m_model_num);
		m_state = next_state;
		return rc;
	}
}