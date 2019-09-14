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

namespace ChatControllers { 
namespace AutoTemplates 
{
	class ChannelPropertyBuilder;

	//
	// Import Data
	//
	// a:5:{i:0;a:4:{s:4:"name";s:4:"name";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:10:"persistant";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:13:"owner_citizen";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:9:"is_public";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:8:"password";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class ChannelData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef ChannelPropertyBuilder PropertyBuilder;
	
	public: // source data
		String name;
		bool persistant;
		int         owner_citizen;
		bool is_public;
		String password;

		
	public:
		ChannelData()
		{
			name = "";
			persistant = true;
			owner_citizen = 0;
			is_public = false;
			password = "";

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
			char* section_id = const_cast<char*>("relay_Channel");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "name", name);
			SerializerSystem.Exchange(ExchangeMode, section_id, "persistant", persistant);
			SerializerSystem.Exchange(ExchangeMode, section_id, "owner_citizen", owner_citizen);
			SerializerSystem.Exchange(ExchangeMode, section_id, "is_public", is_public);
			SerializerSystem.Exchange(ExchangeMode, section_id, "password", password);

		}
		
		ChannelData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		String  getName                          () const { return name; }
		bool  getPersistant                    () const { return persistant; }
		int          getOwnerCitizen                  () const { return owner_citizen; }
		bool  getIsPublic                      () const { return is_public; }
		String  getPassword                      () const { return password; }

		virtual void setName                          (String  Assgn) { name = Assgn; }
		virtual void setPersistant                    (bool  Assgn) { persistant = Assgn; }
		virtual void setOwnerCitizen                  (int          Assgn) { owner_citizen = Assgn; }
		virtual void setIsPublic                      (bool  Assgn) { is_public = Assgn; }
		virtual void setPassword                      (String  Assgn) { password = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "name") {
				return Conversion::ToString(name);
			}
			if (ParamName == "persistant") {
				return Conversion::ToString(persistant);
			}
			if (ParamName == "owner_citizen") {
				return Conversion::ToString(owner_citizen);
			}
			if (ParamName == "is_public") {
				return Conversion::ToString(is_public);
			}
			if (ParamName == "password") {
				return Conversion::ToString(password);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "name") { 
				name = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "persistant") { 
				persistant = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "owner_citizen") { 
				owner_citizen = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "is_public") { 
				is_public = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "password") { 
				password = Conversion::ToString(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(name, name);
			UMBRA_PROPERTY_RW(persistant, persistant);
			UMBRA_PROPERTY_RW(owner_citizen, owner_citizen);
			UMBRA_PROPERTY_RW(is_public, is_public);
			UMBRA_PROPERTY_RW(password, password);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class ChannelPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pName          ;
		CButton          *pPersistant    ;
		TextBox          *pOwnerCitizen  ;
		CButton          *pIsPublic      ;
		TextBox          *pPassword      ;

		
	public: // a set of the special property type
		ChannelData data;
		ChannelData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pName           = PageCtrl.create_edit      (GroupID, "", "Name");
			pPersistant     = PageCtrl.create_check     (GroupID, "", "Persistant");
			pOwnerCitizen   = PageCtrl.create_edit      (GroupID, "", "Owner Citizen");
			pIsPublic       = PageCtrl.create_check     (GroupID, "", "Is Public");
			pPassword       = PageCtrl.create_edit      (GroupID, "", "Password");

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
		
		bool dialog_to_attributes(ChannelData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(ChannelData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(ChannelData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.name, (*pName          ));
			exr.exchange(Attr.persistant, (*pPersistant    ));
			exr.exchange(Attr.owner_citizen, (*pOwnerCitizen  ));
			exr.exchange(Attr.is_public, (*pIsPublic      ));
			exr.exchange(Attr.password, (*pPassword      ));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(ChannelData& RemoteData)
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