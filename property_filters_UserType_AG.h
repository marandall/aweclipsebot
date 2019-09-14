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
	class UserTypePropertyBuilder;

	//
	// Import Data
	//
	// a:3:{i:0;a:4:{s:4:"name";s:8:"tourists";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:8:"citizens";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:4:"bots";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class UserTypeData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef UserTypePropertyBuilder PropertyBuilder;
	
	public: // source data
		bool tourists;
		bool citizens;
		bool bots;

		
	public:
		UserTypeData()
		{
			tourists = false;
			citizens = false;
			bots = false;

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
			char* section_id = const_cast<char*>("propfilterUserType");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "tourists", tourists);
			SerializerSystem.Exchange(ExchangeMode, section_id, "citizens", citizens);
			SerializerSystem.Exchange(ExchangeMode, section_id, "bots", bots);

		}
		
		UserTypeData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		bool  getTourists                      () const { return tourists; }
		bool  getCitizens                      () const { return citizens; }
		bool  getBots                          () const { return bots; }

		virtual void setTourists                      (bool  Assgn) { tourists = Assgn; }
		virtual void setCitizens                      (bool  Assgn) { citizens = Assgn; }
		virtual void setBots                          (bool  Assgn) { bots = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "tourists") {
				return Conversion::ToString(tourists);
			}
			if (ParamName == "citizens") {
				return Conversion::ToString(citizens);
			}
			if (ParamName == "bots") {
				return Conversion::ToString(bots);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "tourists") { 
				tourists = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "citizens") { 
				citizens = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "bots") { 
				bots = Conversion::ToBool(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(tourists, tourists);
			UMBRA_PROPERTY_RW(citizens, citizens);
			UMBRA_PROPERTY_RW(bots, bots);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class UserTypePropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CButton          *pTourists      ;
		CButton          *pCitizens      ;
		CButton          *pBots          ;

		
	public: // a set of the special property type
		UserTypeData data;
		UserTypeData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pTourists       = PageCtrl.create_check     (GroupID, "", "Tourists");
			pCitizens       = PageCtrl.create_check     (GroupID, "", "Citizens");
			pBots           = PageCtrl.create_check     (GroupID, "", "Bots");

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
		
		bool dialog_to_attributes(UserTypeData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(UserTypeData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(UserTypeData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.tourists, (*pTourists      ));
			exr.exchange(Attr.citizens, (*pCitizens      ));
			exr.exchange(Attr.bots, (*pBots          ));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(UserTypeData& RemoteData)
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