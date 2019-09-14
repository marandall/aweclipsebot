#include "stdafx.h"

// include files for property management
#include "property_object.h"
#include "property_support.h"
#include "awsdk_object_support.h"
#include "property_hp_parser.h"
#include "CallbackQueue.h"
#include "high_performance_encoder.h"

// object formatting
using namespace AW::ObjectFormats;
using namespace Property;

String FileIO_ArrayToHex(std::vector<char>& Array)
{
	String var;
	for (size_t i = 0; i < Array.size(); i++)
		var.AppendFormat("%02x", Array[i]);
	return var;
}

class DirectBuildCallback : public CallbackQueue::Target
{
public: // maintain a result variable
	Umbra::Variant	userDefined;

protected:
	void handle_result(EclipseBot& Client, int Result)
	{

	}
};


#define SIGNED_HEX(x)		((x < 0) ? "-" : ""), abs(x) 

String ObjectData::getModelDescriptor()
{
	if (isV3())				return getModel();
	else if (isParticle())	return "{particle}";
	else if (isZone())		return "{zone}";
	else if (isCamera())	return "{camera}";
	else if (isMover())		return String("{mover:") + getModel() + "}";
	else return "Unknown Type";
}

String shorten(const char* Data)
{
	CStringA conv = Data;
	if (conv.GetLength() > 50)
		return conv.Mid(0, 50) + "...";
	else
		return conv;
}

String range_about(::aw_type_vector_range& vr)
{
	String temp;
	temp.Format("(%.1f/%.1f, %.1f/%.1f, %.1f/%.1f)", vr.min.x, vr.min.y, vr.min.z, vr.max.x, vr.max.y, vr.max.z);
	return temp;
}

String vec_about(::aw_type_vector& vr)
{
	String temp;
	temp.Format("(%.1f, %.1f, %.1f)", vr.x, vr.y, vr.z);
	return temp;
}

bool ObjectData::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
{
	// reflection
	UMBRA_FUNCTION_REDIRECT(__super);

	//
	// clone this object
	//

	UMBRA_FUNCTION_BEGIN(clone) {		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( (new ObjectData(*this))->setUmbraGarbageCollected() );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// Changing Parameters
	//

	UMBRA_FUNCTION_BEGIN(changeTo) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Other, Umbra::VT_CLASS);
		UMBRA_FUNCTION_CC(Other, ObjectData);

		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( bot.object_change2(*this, *pOther, new DirectBuildCallback()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(changeDescription) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Text, Umbra::VT_STRING);
		// code information break
		UMBRA_FUNCTION_CS() {
			ObjectData c2 = *this;
			c2.description = vaText.toString().c_str();
			// change immediately
			UMBRA_FUNCTION_RET( bot.object_change2(*this, c2, new DirectBuildCallback()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(changeAction) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Text, Umbra::VT_STRING);
		// code information break
		UMBRA_FUNCTION_CS() {
			ObjectData c2 = *this;
			c2.action = vaText.toString().c_str();
			// change immediately
			UMBRA_FUNCTION_RET( bot.object_change2(*this, c2, new DirectBuildCallback()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(changeModel) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Text, Umbra::VT_STRING);
		// code information break
		UMBRA_FUNCTION_CS() {
			ObjectData c2 = *this;
			c2.model = vaText.toString().c_str();
			// change immediately
			UMBRA_FUNCTION_RET( bot.object_change2(*this, c2, new DirectBuildCallback()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(add) {
		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( bot.object_add(*this, new DirectBuildCallback()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(delete) {
		// code information break
		UMBRA_FUNCTION_CS() {
			UMBRA_FUNCTION_RET( bot.object_delete(*this, new DirectBuildCallback()) );
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	//
	// Assigning particle effects
	//

	UMBRA_FUNCTION_BEGIN(applyCamera) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Info, Umbra::VT_CLASS);
		UMBRA_FUNCTION_CC(Info, AW::Object_Camera);

		// code information break
		UMBRA_FUNCTION_CS() {
			type = AW_OBJECT_TYPE_CAMERA;
			pInfo->UpdateStructure();
			binary_array = pInfo->_store;
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(applyMover) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Info, Umbra::VT_CLASS);
		UMBRA_FUNCTION_CC(Info, AW::Object_Mover);

		// code information break
		UMBRA_FUNCTION_CS() {
			type = AW_OBJECT_TYPE_MOVER;
			pInfo->UpdateStructure();
			binary_array = pInfo->_store;
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(applyParticles) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Info, Umbra::VT_CLASS);
		UMBRA_FUNCTION_CC(Info, AW::Object_Particle);

		// code information break
		UMBRA_FUNCTION_CS() {
			type = AW_OBJECT_TYPE_PARTICLES;
			pInfo->UpdateStructure();
			binary_array = pInfo->_store;
			::aw_object_data_particles p = pInfo->getStruct();
			int j = 0;
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(applyZone) {
		// required arguments
		UMBRA_FUNCTION_DA(0, Info, Umbra::VT_CLASS);
		UMBRA_FUNCTION_CC(Info, AW::Object_Zone);

		// code information break
		UMBRA_FUNCTION_CS() {
			type = AW_OBJECT_TYPE_ZONE;
			pInfo->UpdateStructure();
			binary_array = pInfo->_store;
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	UMBRA_FUNCTION_BEGIN(applyV3) {
		// code information break
		UMBRA_FUNCTION_CS() {
			type = AW_OBJECT_TYPE_V3;
			binary_array.clear();
		} UMBRA_FUNCTION_CE();
	} UMBRA_FUNCTION_END();

	// nothing found
	return R.NOT_HANDLED;
}

bool ObjectData::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
{
	// reflection
	UMBRA_PROPERTY_REDIRECT(__super);

	// propdump type information
	UMBRA_PROPERTY_R(propdump_v2,		FileIO_getPropdump_v2());
	UMBRA_PROPERTY_R(propdump_v3,		FileIO_getPropdump_v3());
	UMBRA_PROPERTY_R(propdump_v4,		FileIO_getPropdump_v4());

	// nothing found
	return R.NOT_HANDLED;
}

String ObjectData::getDetails()
{
	String details;

	// v3 type shows description and action
	if (isV3() == true)
	{
		details.Format("description='%s' action='%s'", shorten(getDescription()), shorten(getAction()));
	}
	// particle includes assets and size
	else if (isParticle() == true)
	{
		// extract particle information type
		AW::Object_Particle pe(&binary_array);
		pe.UpdateClass();

		// detail
		details.Format("assets='%s' name='%s' size%s volume%s", shorten(pe.assetlist), shorten(pe.name), 
				range_about(pe.getStruct().size), range_about(pe.getStruct().volume));
	}
	// zone includes size
	else if (isZone() == true)
	{
		details.Format("size%s", vec_about(ToZoneData()->size));
	}
	else if (isMover() == true)
	{
		// extract mover information type
		AW::Object_Mover pe(&binary_array);
		details.Format("name='%s' type=%d", shorten(pe.name), pe.getStruct().type);
	}
	else if (isCamera() == true)
	{
		// extract particle information type
		AW::Object_Camera pe(&binary_array);

		// detail
		details.Format("name='%s'", shorten(pe.name));
	}
	else
	{
		details.Format("No data available for unknown type");
	}

	// return details
	return details;
}


using namespace Property::FileFormat;

String ObjectData::FileIO_getPropertyProjectLine_v1()
{
	// builds a property project version 1
	// data line, signed hex encoding
	String form_data = Property::FileIO_ArrayToHex(binary_array);

	// add each piece of data
	const int BSIZE = 1024;
	static SpaceSeperator data;
	data.reset();

	using namespace Property::FileFormat;
	sprintf_s(data[PP1_ID],			BSIZE, "%d", id); 
	sprintf_s(data[PP1_NUMBER],		BSIZE, "%d", number); 
	sprintf_s(data[PP1_ORIGIN],		BSIZE, "%d", 0); 
	sprintf_s(data[PP1_OWNER],		BSIZE, "%d", owner); 
	sprintf_s(data[PP1_TIMESTAMP],	BSIZE, "%d", timestamp); 
	sprintf_s(data[PP1_X],			BSIZE, "%d", m_x); 
	sprintf_s(data[PP1_Y],			BSIZE, "%d", m_y); 
	sprintf_s(data[PP1_Z],			BSIZE, "%d", m_z); 
	sprintf_s(data[PP1_YAW],		BSIZE, "%d", m_yaw); 
	sprintf_s(data[PP1_TILT],		BSIZE, "%d", m_pitch); 
	sprintf_s(data[PP1_ROLL],		BSIZE, "%d", m_roll); 
	sprintf_s(data[PP1_TYPE],		BSIZE, "%d", type); 
	sprintf_s(data[PP1_SELECTED],	BSIZE, "%d", selected ? 1 : 0); 
	sprintf_s(data[PP1_LAYER],		BSIZE, "%d", 0); 

	// copy the strings
	StringEncoder::encode_slashes(data[PP1_MODEL],		 (LPCSTR)model, true);
	StringEncoder::encode_slashes(data[PP1_ACTION],		 (LPCSTR)action, true);
	StringEncoder::encode_slashes(data[PP1_DESCRIPTION], (LPCSTR)description, true);
	StringEncoder::encode_slashes(data[PP1_BINARY],		 (LPCSTR)form_data, true);

	// return the completed line
	char output[1024 * 10];
	data.build(output, PP1_MAX_ALIGNMENT);
	return output;
}


bool ObjectData::FileIO_UsePropertyProjectLine_v1(String& Line, Property::PropertyProject1_FormatData*)
{
	Line.Trim("\r\n \t");

	// slit the sting
	SpaceSeperator data;
	data.reset();
	data.parse(Line);

	CStringA bin_data;

	// starting position used to keep other code clean
	id			= (int)atoi(data[PP1_ID]);
	number		= (int)atoi(data[PP1_NUMBER]);
	owner		= (int)atoi(data[PP1_OWNER]);
	timestamp	= (int)atoi(data[PP1_TIMESTAMP]);
	m_x			= (int)atoi(data[PP1_X]);
	m_y			= (int)atoi(data[PP1_Y]);
	m_z			= (int)atoi(data[PP1_Z]);
	m_yaw		= (int)atoi(data[PP1_YAW]);
	m_pitch		= (int)atoi(data[PP1_TILT]);
	m_roll		= (int)atoi(data[PP1_ROLL]);
	type		= (int)atoi(data[PP1_TYPE]);

	// the string data
	model		= data[PP1_MODEL];
	description	= data[PP1_DESCRIPTION];
	action		= data[PP1_ACTION];
	bin_data	= data[PP1_BINARY];

	// old information
	selected	= strcmp(data[PP1_SELECTED], "1") == 0;

	// copy binary back
	Property::FileIO_CopyHexToBinary(binary_array, bin_data);

	if (type == AW_OBJECT_TYPE_PARTICLES && owner == 348037)
	{
		aw_object_data_particles* p = ToParticlesData();
		int k = 0;
	}
	UpdateName();
	return true;
}

bool ObjectData::FileIO_UsePropdump_v2(String Line)
{
	// take reference of our arguments for no particular reason
	SplitString		data(Line, " ", V2_MAX_ALIGNMENT);
	String			temp_string, bin;
	int				model_len, desc_len, action_len;

	// if our alignment count is wrong abort
	if (data.size() != V2_MAX_ALIGNMENT)
		return false;

	// tokenise	etc
	owner			= atoi(data[V2_OWNER]);
	timestamp		= atoi(data[V2_TIMESTAMP]);
	m_x				= atoi(data[V2_X]);
	m_y				= atoi(data[V2_Y]);
	m_z				= atoi(data[V2_Z]);
	m_yaw			= atoi(data[V2_YAW]);
	m_pitch			= 0;
	m_roll			= 0;

	// get the standard string lengths
	model_len		= atoi(data[V2_MODEL_LEN]);
	desc_len		= atoi(data[V2_DESCRIPTION_LEN]);
	action_len		= atoi(data[V2_ACTION_LEN]);

	// extract strings using partial extractor
	temp_string		= data[V3_STRING_DATA];
	
	// we replace the euro symbol with the newline
	temp_string.Replace("€", "\n");
	PartialExtractor pe(temp_string);

	// extract the model types and binary
	model			= pe.getNext(model_len);
	description		= pe.getNext(desc_len);
	action			= pe.getNext(action_len);
	UpdateName();
	return true;
}


bool ObjectData::FileIO_UsePropdump_v3(String Line)
{
	// take reference of our arguments for no particular reason
	SplitString		data(Line, " ", V3_MAX_ALIGNMENT);
	String			temp_string, bin;
	int				model_len, desc_len, action_len;

	// if our alignment count is wrong abort
	if (data.size() != V3_MAX_ALIGNMENT)
		return false;

	// tokenise	etc
	owner			= atoi(data[V3_OWNER]);
	timestamp		= atoi(data[V3_TIMESTAMP]);
	m_x				= atoi(data[V3_X]);
	m_y				= atoi(data[V3_Y]);
	m_z				= atoi(data[V3_Z]);
	m_yaw			= atoi(data[V3_YAW]);
	m_pitch			= atoi(data[V3_TILT]);
	m_roll			= atoi(data[V3_ROLL]);

	// get the standard string lengths
	model_len		= atoi(data[V3_MODEL_LEN]);
	desc_len		= atoi(data[V3_DESCRIPTION_LEN]);
	action_len		= atoi(data[V3_ACTION_LEN]);
	temp_string		= data[V3_STRING_DATA];
	
	// we replace the euro symbol with the newline
	temp_string.Replace("€", "\n");
	PartialExtractor pe(temp_string);

	// extract the model types and binary
	model			= pe.getNext(model_len);
	description		= pe.getNext(desc_len);
	action			= pe.getNext(action_len);
	UpdateName();
	return true;
}

bool ObjectData::FileIO_UsePropdump_v4(String Line)
{
	// take reference of our arguments for no particular reason
	SplitString		data(Line, " ", V4_MAX_ALIGNMENT);
	String			temp_string, bin;
	int				model_len, desc_len, action_len,  bin_len;

	// if our alignment count is wrong abort
	if (data.size() != V4_MAX_ALIGNMENT)
		return false;

	// tokenise	etc
	owner			= atoi(data[V4_OWNER]);
	timestamp		= atoi(data[V4_TIMESTAMP]);
	m_x				= atoi(data[V4_X]);
	m_y				= atoi(data[V4_Y]);
	m_z				= atoi(data[V4_Z]);
	m_yaw			= atoi(data[V4_YAW]);
	m_pitch			= atoi(data[V4_TILT]);
	m_roll			= atoi(data[V4_ROLL]);
	type			= atoi(data[V4_TYPE]);

	// get the standard string lengths
	model_len		= atoi(data[V4_MODEL_LEN]);
	desc_len		= atoi(data[V4_DESCRIPTION_LEN]);
	action_len		= atoi(data[V4_ACTION_LEN]);
	bin_len			= atoi(data[V4_BINARY_LEN]);
	temp_string		= data[V4_STRING_DATA];
	
	// we replace the euro symbol with the newline
	temp_string.Replace("€", "\n");
	PartialExtractor pe(temp_string);

	// extract the model types and binary
	model			= pe.getNext(model_len);
	description		= pe.getNext(desc_len);
	action			= pe.getNext(action_len);
	bin				= pe.getLast();

	// copy binary to string
	Property::FileIO_CopyHexToBinary(binary_array, bin);

	UpdateName();
	return true;
}

String ObjectData::FileIO_getPropdump(int Version)
{
	switch (Version)
	{
	case 2:
		return FileIO_getPropdump_v2();
	case 3:
		return FileIO_getPropdump_v3();
	case 4:
		return FileIO_getPropdump_v4();
	default:
		return "";
	};
}

String ObjectData::FileIO_getPropdump_v2(void)
{
	// number owner x y z yaw tilt roll type modlen desclen actlen dlen data
	String data;
	
	// merge the strings and replace newline with euro
	String form_data = model + description + action;
	form_data.Replace("\n", "€");

	// format data
	data.Format("%d %d %d %d %d %d %d %d %d %s",
			owner,		timestamp,
			m_x,		m_y,
			m_z,		m_yaw, 
			model.GetLength(),
			description.GetLength(),
			action.GetLength(),
			form_data
		);
	return data;
}

String ObjectData::FileIO_getPropdump_v3(void)
{
	// number owner x y z yaw tilt roll type modlen desclen actlen dlen data
	String data;
	
	// merge the strings and replace newline with euro
	String form_data = model + description + action;
	form_data.Replace("\n", "€");

	// format data
	data.Format("%d %d %d %d %d %d %d %d %d %d %d %s",
			owner,		timestamp,
			m_x,		m_y,
			m_z,		m_yaw, 
			m_pitch,	m_roll,
			model.GetLength(),
			description.GetLength(),
			action.GetLength(),
			form_data
		);
	return data;
}

String ObjectData::FileIO_getPropdump_v4(void)
{
	// number owner x y z yaw tilt roll type modlen desclen actlen dlen data
	String data;
	
	// merge the strings and replace newline with euro
	String form_data = model + description + action;
	form_data.Replace("\n", "€");
	
	// add the hex encoded binary data
	form_data+= Property::FileIO_ArrayToHex(binary_array);

	// format data
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

	if (type == AW_OBJECT_TYPE_PARTICLES)
	{
	}

	return data;
}

String ObjectData::getVariantPropdump(int Version)
{
	if (Version == FileFormat::FORMAT_PROJECT)
		return FileIO_getPropertyProjectLine_v1();
	else if (Version == FileFormat::FORMAT_V2)
		return FileIO_getPropdump_v2();
	else if (Version == FileFormat::FORMAT_V3)
		return FileIO_getPropdump_v3();
	else if (Version == FileFormat::FORMAT_V4)
		return FileIO_getPropdump_v4();
	return "";
}