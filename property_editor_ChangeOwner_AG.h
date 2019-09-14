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

namespace Property { namespace Editors { 
namespace AutoTemplates 
{
	class ChangeOwnerPropertyBuilder;

	//
	// Import Data
	//
	// a:2:{i:0;a:4:{s:4:"name";s:12:"convert_from";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:10:"convert_to";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class ChangeOwnerData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef ChangeOwnerPropertyBuilder PropertyBuilder;
	
	public: // source data
		String convert_from;
		String convert_to;

		
	public:
		ChangeOwnerData()
		{
			convert_from = "";
			convert_to = "";

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
			char* section_id = const_cast<char*>("propeditorChangeOwner");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "convert_from", convert_from);
			SerializerSystem.Exchange(ExchangeMode, section_id, "convert_to", convert_to);

		}
		
		ChangeOwnerData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		String  getConvertFrom                   () const { return convert_from; }
		String  getConvertTo                     () const { return convert_to; }

		virtual void setConvertFrom                   (String  Assgn) { convert_from = Assgn; }
		virtual void setConvertTo                     (String  Assgn) { convert_to = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "convert_from") {
				return Conversion::ToString(convert_from);
			}
			if (ParamName == "convert_to") {
				return Conversion::ToString(convert_to);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "convert_from") { 
				convert_from = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "convert_to") { 
				convert_to = Conversion::ToString(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(convert_from, convert_from);
			UMBRA_PROPERTY_RW(convert_to, convert_to);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class ChangeOwnerPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pConvertFrom   ;
		TextBox          *pConvertTo     ;

		
	public: // a set of the special property type
		ChangeOwnerData data;
		ChangeOwnerData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pConvertFrom    = PageCtrl.create_edit      (GroupID, "", "Convert From");
			pConvertTo      = PageCtrl.create_edit      (GroupID, "", "Convert To");

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
		
		bool dialog_to_attributes(ChangeOwnerData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(ChangeOwnerData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(ChangeOwnerData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.convert_from, (*pConvertFrom   ));
			exr.exchange(Attr.convert_to, (*pConvertTo     ));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(ChangeOwnerData& RemoteData)
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