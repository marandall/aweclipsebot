#pragma once
#include "serializer.h"
#include "tsb.h"
#include "umbra/umbra_remoteclass.h"
#include "raw_param_editor.h"
#include "PropertyScriptCtrl.h"
#include "GlobalDex.h"

namespace ChatControllers { namespace Channels { 
namespace AutoTemplates 
{
	class BanPropertyBuilder;

	class BanData : 
		public Umbra::RemoteClass,
		public IRawParamEditor
	{
	public: // type defenition for property builder
		typedef BanPropertyBuilder PropertyBuilder;
	
	public: // source data
		key;

		
	public:
		BanData()
		{
			
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
			char* section_id = const_cast<char*>("relay_Ban");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "key", key);

		}
		
		BanData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		 getKey                           () const { return key; }

		virtual void setKey                           ( Assgn) { key = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			
			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			
			return false;
		}	
		
	public: // umbra interfaces
		void doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(key, key);

			
			// nothing found
			throw Umbra::UnsupportedPropertyException(R);
		}
	};

	
	class BanPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		*pKey           ;

		
	public: // a set of the special property type
		BanData data;
		BanData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pKey            = PageCtrl.(GroupID, "", "Key");

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
		
		bool dialog_to_attributes(BanData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(BanData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(BanData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.key, (*pKey           ));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(BanData& RemoteData)
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