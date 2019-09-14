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
	class TranslatePropertyBuilder;

	//
	// Import Data
	//
	// a:2:{i:0;a:4:{s:4:"name";s:6:"origin";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:11:"destination";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class TranslateData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef TranslatePropertyBuilder PropertyBuilder;
	
	public: // source data
		String origin;
		String destination;

		
	public:
		TranslateData()
		{
			origin = "";
			destination = "";

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
			char* section_id = const_cast<char*>("propeditorTranslate");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "origin", origin);
			SerializerSystem.Exchange(ExchangeMode, section_id, "destination", destination);

		}
		
		TranslateData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		String  getOrigin                        () const { return origin; }
		String  getDestination                   () const { return destination; }

		virtual void setOrigin                        (String  Assgn) { origin = Assgn; }
		virtual void setDestination                   (String  Assgn) { destination = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "origin") {
				return Conversion::ToString(origin);
			}
			if (ParamName == "destination") {
				return Conversion::ToString(destination);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "origin") { 
				origin = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "destination") { 
				destination = Conversion::ToString(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(origin, origin);
			UMBRA_PROPERTY_RW(destination, destination);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class TranslatePropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pOrigin        ;
		TextBox          *pDestination   ;

		
	public: // a set of the special property type
		TranslateData data;
		TranslateData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pOrigin         = PageCtrl.create_edit      (GroupID, "", "Origin");
			pDestination    = PageCtrl.create_edit      (GroupID, "", "Destination");

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
		
		bool dialog_to_attributes(TranslateData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(TranslateData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(TranslateData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.origin, (*pOrigin        ));
			exr.exchange(Attr.destination, (*pDestination   ));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(TranslateData& RemoteData)
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