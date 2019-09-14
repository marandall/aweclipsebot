#pragma once
#include "serializer.h"
#include "IConfigLoader.h"
#include "tsb.h"
#include "autonumber.h"

// umbra includes
#include "umbra/umbra_remoteclass.h"
#include "umbra/umbra_variant.h"

#include "raw_param_editor.h"
#include "PropertyScriptCtrl.h"
#include "GlobalDex.h"

namespace Property { namespace Filters { 
namespace AutoTemplates 
{
	class CitizensPropertyBuilder;

	//
	// Import Data
	//
	// a:1:{i:0;a:4:{s:4:"name";s:8:"citizens";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class CitizensData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef CitizensPropertyBuilder PropertyBuilder;
	
	public: // source data
		String citizens;

		
	public:
		CitizensData()
		{
			citizens = "";

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
			char* section_id = const_cast<char*>("propfilterCitizens");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "citizens", citizens);

		}
		
		CitizensData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		String  getCitizens                      () const { return citizens; }

		virtual void setCitizens                      (String  Assgn) { citizens = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "citizens") {
				return Conversion::ToString(citizens);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "citizens") { 
				citizens = Conversion::ToString(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(citizens, citizens);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class CitizensPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pCitizens      ;

		
	public: // a set of the special property type
		CitizensData data;
		CitizensData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pCitizens       = PageCtrl.create_edit      (GroupID, "", "Citizens");

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
		
		bool dialog_to_attributes(CitizensData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(CitizensData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(CitizensData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.citizens, (*pCitizens      ));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(CitizensData& RemoteData)
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
} } 