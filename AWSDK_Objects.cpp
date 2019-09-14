#include "stdafx.h"
#include "awsdk_objects_v3.h"
#include "awsdk_bot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace AW
{
	class PartialExtractor
	{
	private:
		String	buffer;
		int		cur_pos;

	public: 
		PartialExtractor(String Data) 
		{ 
			buffer	= Data; 
			cur_pos = 0; 
		}

		String getNext(int Length)
		{
			String temp = buffer.Mid(cur_pos, Length);
			cur_pos+= Length;
			return temp;
		}
	};

	void CopyHexToBinary(std::vector<char> Array, String Data)
	{
		Array.reserve(Data.GetLength() / 2);

		// copy the structure backwards
		char byte_dual[3] = { 0, 0, 0 };
		char* pEnd;
		const char* pString = (LPCSTR)Data;
		for (size_t i = 0; i < (size_t)Data.GetLength(); i+=2)
		{
			byte_dual[0] = *(pString + i);
			byte_dual[1] = *(pString + i + 1);
			
			// allocate byte to range buffer
			Array[i / 2] = (char)(unsigned char)strtol(byte_dual, &pEnd, 16);
		}
	}

	String ArrayToHex(std::vector<char>& Array)
	{
		String var;
		for (size_t i = 0; i < Array.size(); i++)
			var.AppendFormat("%02x", Array[i]);
		return var;
	}

	// appends a string to the array
	// the array must already have been resized (raw)
	void AppendStringToPointer_Raw(char* Begin, String Data)
	{
		for (size_t i = 0; i < (size_t)Data.GetLength(); i++)
			*(Begin + i) = *(((LPCSTR)Data) + i);
	}


	void ObjectData::__reconstruct()
	{
		type			= AW_OBJECT_TYPE_V3;		// the type of object
		owner			= 0;		// citizen number of builder
		id				= 0;		// the id number of the object
		timestamp	= 0;		// the 32 bit timestamp
		number		= 0;		// the 32 bit object number
	}

	String ObjectData::EstablishName()
	{
		/* skip if an empty action */
		if (action.GetLength() == 0)
			return "";

		if (action.Find("name tag1_sn") != -1)
		{
			int j = 0;
		}

		/* check the actions */
		SplitString triggers(action, ";", -1);
		for (size_t t = 0; t < triggers.size(); t++)
		{
			/* make the trigger lower cased */
			String trigger = triggers[t]; 
			trigger.Trim(); trigger.MakeLower();

			/* only accept create tags */
			if (trigger.Left(7) != "create ")
				continue;

			/* find the trigger */
			trigger = trigger.Mid(7);
			SplitString funcs(trigger, ",", -1);
			for (size_t f = 0; f < funcs.size(); f++)
			{
				/* check the function for a name */
				String func = funcs[f];
				func.Trim();
				if (func.Left(5) != "name ")
					continue;

				/* return the result */
				return func.Mid(5);
			}
		}

		/* nothing */
		return "";
	}

	aw_object_data_zone* ObjectData::ToZoneData()
	{
		return static_cast<aw_object_data_zone*>((void*)&binary_array.front());
	}
	
	aw_object_data_particles* ObjectData::ToParticlesData()
	{
		return static_cast<aw_object_data_particles*>((void*)&binary_array.front());
	}
	
	aw_object_data_camera* ObjectData::ToCameraData()
	{
		return static_cast<aw_object_data_camera*>((void*)&binary_array.front());
	}
	
	aw_object_data_mover* ObjectData::ToMoverData()
	{
		return static_cast<aw_object_data_mover*>((void*)&binary_array.front());
	}

	void ObjectData::PrepChange(Bot& Client)
	{
		Client._int_set(AW_OBJECT_ID,			id);
		Client._int_set(AW_OBJECT_OLD_NUMBER,	0);
		Client._int_set(AW_OBJECT_OLD_X,		m_x);
		Client._int_set(AW_OBJECT_OLD_Z,		m_z);
	}

#include "property_support.h"

	void ObjectData::from_sdk(Bot& Client)
	{
		int type_id = Client._int(AW_OBJECT_TYPE);

		// the standard stuff
		model	= Client._string(AW_OBJECT_MODEL);
		description	= Client._string(AW_OBJECT_DESCRIPTION);
		action	= Client._string(AW_OBJECT_ACTION);

		bool dir = AW::EXCHANGE_UPDATE_DATA;
		Client._exchange(dir, AW_OBJECT_MODEL,	model);
		Client._exchange(dir, AW_OBJECT_DESCRIPTION, description);
		Client._exchange(dir, AW_OBJECT_ACTION, action);

		Client._exchange(dir, AW_OBJECT_X,		m_x);
		Client._exchange(dir, AW_OBJECT_Y,		m_y);
		Client._exchange(dir, AW_OBJECT_Z,		m_z);
		Client._exchange(dir, AW_OBJECT_YAW,	m_yaw);
		Client._exchange(dir, AW_OBJECT_TILT,	m_pitch);
		Client._exchange(dir, AW_OBJECT_ROLL,	m_roll);

		Client._exchange(dir, AW_OBJECT_TYPE,	type);
		Client._exchange(dir, AW_OBJECT_ID,		id);
		Client._exchange(dir, AW_OBJECT_NUMBER,	number);
		Client._exchange(dir, AW_OBJECT_BUILD_TIMESTAMP, timestamp);
		Client._exchange(dir, AW_OBJECT_OWNER,	owner);

		// if the type is that of v3 
		UINT	data_len = 0;
		char*	pData = Client._data(AW_OBJECT_DATA, &data_len);
		binary_array.resize(data_len, 0);
		if (pData == 0)
		{

		}
		else
		{
			for (size_t i = 0; i < data_len; i++)
				binary_array[i] = *(pData + i);
		}

		/* check the name */
		UpdateName();
	}

	void ObjectData::to_sdk(Bot& Client)
	{
		Client._string_set(AW_OBJECT_MODEL,	model);
		Client._string_set(AW_OBJECT_DESCRIPTION, description);
		Client._string_set(AW_OBJECT_ACTION, action);

		Client._int_set(AW_OBJECT_X,		m_x);
		Client._int_set(AW_OBJECT_Y,		m_y);
		Client._int_set(AW_OBJECT_Z,		m_z);
		Client._int_set(AW_OBJECT_YAW,		m_yaw);
		Client._int_set(AW_OBJECT_TILT,		m_pitch);
		Client._int_set(AW_OBJECT_ROLL,		m_roll);

		Client._int_set(AW_OBJECT_TYPE,		type);
		Client._int_set(AW_OBJECT_NUMBER,	number);
		Client._int_set(AW_OBJECT_BUILD_TIMESTAMP, timestamp);
		Client._int_set(AW_OBJECT_OWNER,	owner);
		Client._int_set(AW_OBJECT_ID,		0);
		
		// if the type is that of v3 
		if (binary_array.size())
		{
			Client._data_set(AW_OBJECT_DATA, &binary_array[0], (unsigned int)binary_array.size());
		}
	}

	bool ObjectData::PropConvert_ReadVersion4_0(SplitString& Data, int Offset)
	{
		// take reference of our arguments for no particular reason
		SplitString&	data		= Data;
		int				start_at	= Offset;

		// tokenise etc
		owner		= atoi(data[0 + start_at]);
		timestamp	= atoi(data[1 + start_at]);
		m_x			= atoi(data[2 + start_at]);
		m_y			= atoi(data[3 + start_at]);
		m_z			= atoi(data[4 + start_at]);
		m_yaw		= atoi(data[5 + start_at]);
		m_pitch		= atoi(data[6 + start_at]);
		m_roll		= atoi(data[7 + start_at]);
		type		= atoi(data[8 + start_at]);

		// get the standard string lengths
		int model_len	= atoi(data[9 + start_at]);
		int desc_len	= atoi(data[10 + start_at]);
		int action_len	= atoi(data[11 + start_at]);
		int bin_len		= atoi(data[12 + start_at]);

		// extract strings using partial extractor
		String		temp_string = data[13 + start_at];
		temp_string.Replace("\n", "€");
		PartialExtractor pe(data[13 + start_at]);
		model		= pe.getNext(model_len);
		description	= pe.getNext(desc_len);
		action		= pe.getNext(action_len);
		String bin	= pe.getNext(bin_len);
		UpdateName();

		return true;
	}

	bool ObjectData::PropConvert_ReadVersion4(String Line)
	{
		SplitString data(Line, " ", 14);
		
		// anything registering more or less than 14 is invalid
		if (data.size() != 14)
			return false;

		return PropConvert_ReadVersion4_0(data, 0);
	}
	
	String ObjectData::PropConvert_WriteVersion4_0()
	{
		// number owner x y z yaw tilt roll type modlen desclen actlen dlen data
		String data;
		String form_data = model + description + action;
		form_data.Replace("\n", "€");
		form_data+= ArrayToHex(binary_array);

		data.Format("%d %d %d %d %d %d %d %d %d %d %d %d %d %s",
				owner,		timestamp,
				m_x,		m_y,
				m_z,		m_yaw, 
				m_pitch,	m_roll,
				type,		
				model.GetLength(),
				description.GetLength(),
				action.GetLength(),
				(int)binary_array.size(),
				form_data
			);
		return data;
	}

	String ObjectData::PropConvert_WriteVersion4()
	{
		return PropConvert_WriteVersion4_0();
	}

	// gets the structure reference for the array
	aw_object_data_particles& Object_Particle::getStruct()
	{
		return *((aw_object_data_particles*)&_store[0]);
	}

	void Object_Particle::UpdateStructure()
	{
		// dump everything in the vector and re-allocate
		_store.resize(sizeof(aw_object_data_particles) + assetlist.GetLength() + name.GetLength(), 0);
	
		// setup using local structure
		aw_object_data_particles& p_data = getStruct();

		// append the strings to the end
		AppendStringToPointer_Raw(p_data.str_data, assetlist + name);
		p_data.asset_list_len	= assetlist.GetLength();
		p_data.name_len				= name.GetLength();
	}

	void Object_Particle::UpdateClass()
	{
		// setup string extraction
		aw_object_data_particles& p_data = getStruct();
		PartialExtractor pe(p_data.str_data);
		
		WriteMsg(p_data.str_data, RGB_RED, OT_MAIN);

		// copy
		assetlist	= pe.getNext(p_data.asset_list_len);		// assets first
		name		= pe.getNext(p_data.name_len);				// 
	}

	


	// gets the structure reference for the array
	aw_object_data_zone& Object_Zone::getStruct()
	{
		return *((aw_object_data_zone*)&_store[0]);
	}


	// update the structure length
	void Object_Zone::UpdateStructure()
	{
		// dump everything in the vector and re-allocate
		String str_list = footstep + ambient + camera + targetcur + voip_rights + name;
		_store.resize(sizeof(aw_object_data_zone) + str_list.GetLength(), 0);
	
		// setup using local structure
		aw_object_data_zone& z_data = getStruct();

		// append the strings to the end
		AppendStringToPointer_Raw(z_data.str_data, str_list);
		z_data.footstep_len			= footstep.GetLength();		// length of 1st string in str_data
		z_data.ambient_len			= ambient.GetLength();		// length of 2nd string in str_data
		z_data.camera_len				= camera.GetLength();			// length of 3rd string in str_data
		z_data.target_cur_len		= targetcur.GetLength();	// length of 4th string in str_data
		z_data.name_len					= name.GetLength();				// length of 5th piece 
		z_data.voip_rights_len	= voip_rights.GetLength();// 
	}

	void Object_Zone::UpdateClass()
	{
		// setup string extraction
		aw_object_data_zone& z_data = getStruct();
		PartialExtractor pe(z_data.str_data);

		// copy strings
		footstep		= pe.getNext(z_data.footstep_len);
		ambient			= pe.getNext(z_data.ambient_len);
		camera			= pe.getNext(z_data.camera_len);
		targetcur		= pe.getNext(z_data.target_cur_len);
		voip_rights = pe.getNext(z_data.voip_rights_len);
		name				= pe.getNext(z_data.name_len);
	}

	// gets the structure reference for the array
	aw_object_data_mover& Object_Mover::getStruct()
	{
		return *((aw_object_data_mover*)&_store[0]);
	}

	// update the structure length
	void Object_Mover::UpdateStructure()
	{
		// dump everything in the vector and re-allocate
		String str_list = name + seq + script + sound + waypoints + bump_name;
		_store.resize(sizeof(aw_object_data_mover) + str_list.GetLength(), 0);
	
		// setup using local structure
		aw_object_data_mover& m_data = getStruct();

		// append the strings to the end
		AppendStringToPointer_Raw(m_data.str_data, str_list);
		m_data.name_len			= name		.GetLength();
		m_data.seq_len			= seq		.GetLength();
		m_data.script_len		= script	.GetLength();
		m_data.sound_len		= sound		.GetLength();
		m_data.waypoints_len	= waypoints	.GetLength();
		m_data.bump_name_len	= bump_name	.GetLength();
	}

	void Object_Mover::UpdateClass()
	{
		// setup string extraction
		aw_object_data_mover& m_data = getStruct();
		PartialExtractor pe(m_data.str_data);
		
		// copy
		name		= pe.getNext(m_data.name_len);
		seq			= pe.getNext(m_data.seq_len);
		script		= pe.getNext(m_data.script_len);
		sound		= pe.getNext(m_data.sound_len);
		waypoints	= pe.getNext(m_data.waypoints_len);
		bump_name	= pe.getNext(m_data.bump_name_len);
	}


	// gets the structure reference for the array
	aw_object_data_camera& Object_Camera::getStruct()
	{
		return *((aw_object_data_camera*)&_store[0]);
	}

	// update the structure length
	void  Object_Camera::UpdateStructure()
	{
		// dump everything in the vector and re-allocate
		String str_list = name;
		_store.resize(sizeof(aw_object_data_camera) + str_list.GetLength(), 0);
	
		// setup using local structure
		aw_object_data_camera& c_data = getStruct();

		// append the strings to the end
		AppendStringToPointer_Raw(c_data.str_data, str_list);
		c_data.name_len			= name		.GetLength();
	}

	void Object_Camera::UpdateClass()
	{
		// setup string extraction
		aw_object_data_camera& c_data = getStruct();
		PartialExtractor pe(c_data.str_data);
		
		// copy
		name		= pe.getNext(c_data.name_len);
	}
}