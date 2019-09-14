#pragma once
#include "serializer.h"
#include "tsb.h"
#include "umbra_remote_class.h"
#include "raw_param_editor.h"
#include "PropertyScriptCtrl.h"
#include "GlobalDex.h"

namespace ChatControllers { 
namespace AutoTemplates 
{
	class ChatExtenderAexPropertyBuilder;

	class ChatExtenderAexData : 
		public Umbra::RemoteClass,
		public IRawParamEditor
	{
	public: // type defenition for property builder
		typedef ChatExtenderAexPropertyBuilder PropertyBuilder;
	
	public: // source data
		bool        connected;
		bool        passive;

		
	public:
		ChatExtenderAexData()
		{
			connected = false;
			passive = false;

		}
		
	public: // file operations
		virtual void Load(Serializer& SerializerSystem, const char* IniKey = NULL)
		{
			ini_exchange(SerializerSystem, AW::READ_FROM_INI, IniKey);
			DataUpdated();
		}
		
		virtual void Save(Serializer& SerializerSystem, const char* IniKey = NULL)
		{
			ini_exchange(SerializerSystem, AW::WRITE_TO_INI, IniKey);
		}
		
		void ini_exchange(Serializer& SerializerSystem, bool ExchangeMode, const char* IniKey)
		{
			char* section_id = const_cast<char*>("relay_ChatExtenderAex");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "connected", connected);
			SerializerSystem.Exchange(ExchangeMode, section_id, "passive", passive);

		}
		
		ChatExtenderAexData& GetDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual void DataUpdated() { }
		
	public: // read only operators
		bool         get_Connected                     () const { return connected; }
		bool         get_Passive                       () const { return passive; }

		virtual void set_Connected                     (bool         Assgn) { connected = Assgn; }
		virtual void set_Passive                       (bool         Assgn) { passive = Assgn; }

		
	public: // string conversion
		virtual Ascii param_read(Ascii ParamName)
		{
			if (ParamName == "connected") {
				return Conversion::ToString(connected);
			}
			if (ParamName == "passive") {
				return Conversion::ToString(passive);
			}

			return "";
		}
		
		virtual bool param_write(Ascii ParamName, Ascii Value)
		{
			if (ParamName == "connected") { 
				connected = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "passive") { 
				passive = Conversion::ToBool(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		Umbra::Variant Umbra_GetProperty(Umbra::string ID, Umbra::ES_GetProperty& ES)
		{
			if (strcmp(ID.c_str(), "connected"                   ) == 0) return connected;
			if (strcmp(ID.c_str(), "passive"                     ) == 0) return passive;

			
			// nothing found
			throw Umbra::UnsupportedPropertyException(ID);
		}	
	};

	
	class ChatExtenderAexPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CButton          *pConnected     ;
		CButton          *pPassive       ;

		
	public: // a set of the special property type
		ChatExtenderAexData data;
		ChatExtenderAexData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pConnected      = PageCtrl.create_check     (GroupID, "", "Connected");
			pPassive        = PageCtrl.create_check     (GroupID, "", "Passive");

			return;
		}
		
		void dialog_to_attributes()
		{
			data_exchange(data, DataExchanger::FROM_CONTROL);
		}
		
		void attributes_to_dialog()
		{
			data_exchange(data, DataExchanger::TO_CONTROL);
		}
		
		void dialog_to_attributes(ChatExtenderAexData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
		}
		
		void attributes_to_dialog(ChatExtenderAexData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(ChatExtenderAexData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.connected, (*pConnected     ));
			exr.exchange(Attr.passive, (*pPassive       ));

			
			if (Mode == DataExchanger::FROM_CONTROL)
			{
				Attr.DataUpdated();
			}
			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(ChatExtenderAexData& RemoteData)
		{
			pRemoteData = &RemoteData;
			return *dynamic_cast<PropertyBuilderBase*>(this);
		}
		
		PropertyBuilderBase& get_Base()
		{
			return *dynamic_cast<PropertyBuilderBase*>(this);
		}
		
		void dialog_to_remote_attributes()
		{
			data_exchange(*pRemoteData, DataExchanger::FROM_CONTROL);
		}
		
		void remote_attributes_to_dialog()
		{
			data_exchange(*pRemoteData, DataExchanger::TO_CONTROL);
		}
	};
	

}
} 