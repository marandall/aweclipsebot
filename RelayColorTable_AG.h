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
	class ColorTablePropertyBuilder;

	class ColorTableData : 
		public Umbra::RemoteClass,
		public IRawParamEditor
	{
	public: // type defenition for property builder
		typedef ColorTablePropertyBuilder PropertyBuilder;
	
	public: // source data
		bool        connected;
		bool        passive;

		
	public:
		ColorTableData()
		{
			connected = false;
			passive = false;

		}
		
	public: // file operations
		virtual void Load(Serializer& SerializerSystem, const char* IniKey = NULL)
		{
			ini_exchange(SerializerSystem, AW::READ_FROM_INI, IniKey);
			onUpdated();
		}
		
		virtual void Save(Serializer& SerializerSystem, const char* IniKey = NULL)
		{
			ini_exchange(SerializerSystem, AW::WRITE_TO_INI, IniKey);
		}
		
		void ini_exchange(Serializer& SerializerSystem, bool ExchangeMode, const char* IniKey)
		{
			char* section_id = const_cast<char*>("relay_ColorTable");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "connected", connected);
			SerializerSystem.Exchange(ExchangeMode, section_id, "passive", passive);

		}
		
		ColorTableData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		bool         getConnected                     () const { return connected; }
		bool         getPassive                       () const { return passive; }

		virtual void setConnected                     (bool         Assgn) { connected = Assgn; }
		virtual void setPassive                       (bool         Assgn) { passive = Assgn; }

		
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
		void grabUmbraProperty(Umbra::Variant& Returned, Umbra::string ID, Umbra::ES_GetProperty& ES)
		{
			// nothing found
			throw Umbra::UnsupportedPropertyException(ID);
		}
		
		Umbra::string getUmbraString(Umbra::ES_GetString& ES)
		{
			return "[instance(namespace ChatControllers { .ColorTable.} )]";
		}
		
		void executeUmbraFunction(Umbra::Variant& Returned, Umbra::string ID, Umbra::VariantMap& VMap, Umbra::ES_CallFunction& ES)
		{
			UMBRA_RMTCLS_A0_REFLECT(getConnected,                 int);
			UMBRA_RMTCLS_A0_REFLECT(getPassive,                   int);

			
			// nothing found
			throw Umbra::UnsupportedFunctionException(ID);
		}
		
		Umbra::string getUmbraClassName()
		{
			return "";
		}
	};

	
	class ColorTablePropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CButton          *pConnected     ;
		CButton          *pPassive       ;

		
	public: // a set of the special property type
		ColorTableData data;
		ColorTableData* pRemoteData;
		
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
			data.onUpdated();
		}
		
		void attributes_to_dialog()
		{
			data_exchange(data, DataExchanger::TO_CONTROL);
		}
		
		bool dialog_to_attributes(ColorTableData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(ColorTableData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(ColorTableData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.connected, (*pConnected     ));
			exr.exchange(Attr.passive, (*pPassive       ));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(ColorTableData& RemoteData)
		{
			pRemoteData = &RemoteData;
			return *dynamic_cast<PropertyBuilderBase*>(this);
		}
		
		PropertyBuilderBase& getBase()
		{
			return *dynamic_cast<PropertyBuilderBase*>(this);
		}
		
		
		
		bool dialog_to_remote_attributes()
		{
			data_exchange(*pRemoteData, DataExchanger::FROM_CONTROL);
			return pRemoteData->onUpdated();
		}
		
		void remote_attributes_to_dialog()
		{
			data_exchange(*pRemoteData, DataExchanger::TO_CONTROL);
		}
	};
	

}
} 