#pragma once
#include "serializer.h"
#include "tsb.h"
#include "umbra_remote_class.h"
#include "raw_param_editor.h"
#include "PropertyScriptCtrl.h"
#include "GlobalDex.h"

namespace UserProfile { 
namespace AutoTemplates 
{
	class FreeJoinPropertyBuilder;

	class FreeJoinData : 
		public Umbra::RemoteClass,
		public IRawParamEditor
	{
	public: // type defenition for property builder
		typedef FreeJoinPropertyBuilder PropertyBuilder;
	
	public: // source data
		tsb_t       enabled;
		tsb_t       silent;

		
	public:
		FreeJoinData()
		{
			enabled = B_EMPTY;
			silent = B_EMPTY;

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
			char* section_id = const_cast<char*>("avp_FreeJoin");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "enabled", enabled);
			SerializerSystem.Exchange(ExchangeMode, section_id, "silent", silent);

		}
		
		FreeJoinData& GetDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual void DataUpdated() { }
		
	public: // read only operators
		tsb_t        get_Enabled                       () const { return enabled; }
		tsb_t        get_Silent                        () const { return silent; }

		virtual void set_Enabled                       (tsb_t        Assgn) { enabled = Assgn; }
		virtual void set_Silent                        (tsb_t        Assgn) { silent = Assgn; }

		
	public: // string conversion
		virtual Ascii param_read(Ascii ParamName)
		{
			if (ParamName == "enabled") {
				return Conversion::ToStringTSB(enabled);
			}
			if (ParamName == "silent") {
				return Conversion::ToStringTSB(silent);
			}

			return "";
		}
		
		virtual bool param_write(Ascii ParamName, Ascii Value)
		{
			if (ParamName == "enabled") { 
				enabled = Conversion::ToTSB(Value);
				return true;
			}
			if (ParamName == "silent") { 
				silent = Conversion::ToTSB(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		Umbra::Variant Umbra_GetProperty(Umbra::string ID, Umbra::ES_GetProperty& ES)
		{
			if (strcmp(ID.c_str(), "enabled"                     ) == 0) return enabled;
			if (strcmp(ID.c_str(), "silent"                      ) == 0) return silent;

			
			// nothing found
			throw Umbra::UnsupportedPropertyException(ID);
		}	
	};

	
	class FreeJoinPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CButton          *pEnabled       ;
		CButton          *pSilent        ;

		
	public: // a set of the special property type
		FreeJoinData data;
		FreeJoinData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pEnabled        = PageCtrl.create_tristate  (GroupID, "", "Enabled");
			pSilent         = PageCtrl.create_tristate  (GroupID, "", "Silent");

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
		
		void dialog_to_attributes(FreeJoinData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
		}
		
		void attributes_to_dialog(FreeJoinData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(FreeJoinData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.enabled, (*pEnabled       ));
			exr.exchange(Attr.silent, (*pSilent        ));

			
			if (Mode == DataExchanger::FROM_CONTROL)
			{
				Attr.DataUpdated();
			}
			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(FreeJoinData& RemoteData)
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