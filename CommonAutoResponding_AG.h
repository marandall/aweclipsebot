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

namespace CommonFeatures { 
namespace AutoTemplates 
{
	class AutoRespondingPropertyBuilder;

	//
	// Import Data
	//
	// a:6:{i:0;a:4:{s:4:"name";s:15:"auto_greet_bots";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:15:"greet_bots_with";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:14:"identify_owner";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:20:"ignore_hail_on_eject";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:25:"ignore_teleports_on_eject";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:5;a:4:{s:4:"name";s:30:"allow_teleports_between_worlds";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class AutoRespondingData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef AutoRespondingPropertyBuilder PropertyBuilder;
	
	public: // source data
		bool auto_greet_bots;
		String greet_bots_with;
		bool identify_owner;
		bool ignore_hail_on_eject;
		bool ignore_teleports_on_eject;
		bool allow_teleports_between_worlds;

		
	public:
		AutoRespondingData()
		{
			auto_greet_bots = false;
			greet_bots_with = "";
			identify_owner = false;
			ignore_hail_on_eject = false;
			ignore_teleports_on_eject = false;
			allow_teleports_between_worlds = false;

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
			char* section_id = const_cast<char*>("common_AutoResponding");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "auto_greet_bots", auto_greet_bots);
			SerializerSystem.Exchange(ExchangeMode, section_id, "greet_bots_with", greet_bots_with);
			SerializerSystem.Exchange(ExchangeMode, section_id, "identify_owner", identify_owner);
			SerializerSystem.Exchange(ExchangeMode, section_id, "ignore_hail_on_eject", ignore_hail_on_eject);
			SerializerSystem.Exchange(ExchangeMode, section_id, "ignore_teleports_on_eject", ignore_teleports_on_eject);
			SerializerSystem.Exchange(ExchangeMode, section_id, "allow_teleports_between_worlds", allow_teleports_between_worlds);

		}
		
		AutoRespondingData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		bool  getAutoGreetBots                 () const { return auto_greet_bots; }
		String  getGreetBotsWith                 () const { return greet_bots_with; }
		bool  getIdentifyOwner                 () const { return identify_owner; }
		bool  getIgnoreHailOnEject             () const { return ignore_hail_on_eject; }
		bool  getIgnoreTeleportsOnEject        () const { return ignore_teleports_on_eject; }
		bool  getAllowTeleportsBetweenWorlds   () const { return allow_teleports_between_worlds; }

		virtual void setAutoGreetBots                 (bool  Assgn) { auto_greet_bots = Assgn; }
		virtual void setGreetBotsWith                 (String  Assgn) { greet_bots_with = Assgn; }
		virtual void setIdentifyOwner                 (bool  Assgn) { identify_owner = Assgn; }
		virtual void setIgnoreHailOnEject             (bool  Assgn) { ignore_hail_on_eject = Assgn; }
		virtual void setIgnoreTeleportsOnEject        (bool  Assgn) { ignore_teleports_on_eject = Assgn; }
		virtual void setAllowTeleportsBetweenWorlds   (bool  Assgn) { allow_teleports_between_worlds = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "auto_greet_bots") {
				return Conversion::ToString(auto_greet_bots);
			}
			if (ParamName == "greet_bots_with") {
				return Conversion::ToString(greet_bots_with);
			}
			if (ParamName == "identify_owner") {
				return Conversion::ToString(identify_owner);
			}
			if (ParamName == "ignore_hail_on_eject") {
				return Conversion::ToString(ignore_hail_on_eject);
			}
			if (ParamName == "ignore_teleports_on_eject") {
				return Conversion::ToString(ignore_teleports_on_eject);
			}
			if (ParamName == "allow_teleports_between_worlds") {
				return Conversion::ToString(allow_teleports_between_worlds);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "auto_greet_bots") { 
				auto_greet_bots = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "greet_bots_with") { 
				greet_bots_with = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "identify_owner") { 
				identify_owner = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "ignore_hail_on_eject") { 
				ignore_hail_on_eject = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "ignore_teleports_on_eject") { 
				ignore_teleports_on_eject = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "allow_teleports_between_worlds") { 
				allow_teleports_between_worlds = Conversion::ToBool(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(auto_greet_bots, auto_greet_bots);
			UMBRA_PROPERTY_RW(greet_bots_with, greet_bots_with);
			UMBRA_PROPERTY_RW(identify_owner, identify_owner);
			UMBRA_PROPERTY_RW(ignore_hail_on_eject, ignore_hail_on_eject);
			UMBRA_PROPERTY_RW(ignore_teleports_on_eject, ignore_teleports_on_eject);
			UMBRA_PROPERTY_RW(allow_teleports_between_worlds, allow_teleports_between_worlds);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class AutoRespondingPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CButton          *pAutoGreetBots ;
		TextBox          *pGreetBotsWith ;
		CButton          *pIdentifyOwner ;
		CButton          *pIgnoreHailOnEject;
		CButton          *pIgnoreTeleportsOnEject;
		CButton          *pAllowTeleportsBetweenWorlds;

		
	public: // a set of the special property type
		AutoRespondingData data;
		AutoRespondingData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pAutoGreetBots  = PageCtrl.create_check     (GroupID, "", "Greet Bots");
			pGreetBotsWith  = PageCtrl.create_edit      (GroupID, "", "Greet Bots With");
			pIdentifyOwner  = PageCtrl.create_check     (GroupID, "", "Identify Owner");
			pIgnoreHailOnEject = PageCtrl.create_check     (GroupID, "", "Ignore Hails (ejo)");
			pIgnoreTeleportsOnEject = PageCtrl.create_check     (GroupID, "", "Ignore Teleports (ejo)");
			pAllowTeleportsBetweenWorlds = PageCtrl.create_check     (GroupID, "", "Allow Inter-World TP");

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
		
		bool dialog_to_attributes(AutoRespondingData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(AutoRespondingData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(AutoRespondingData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.auto_greet_bots, (*pAutoGreetBots ));
			exr.exchange(Attr.greet_bots_with, (*pGreetBotsWith ));
			exr.exchange(Attr.identify_owner, (*pIdentifyOwner ));
			exr.exchange(Attr.ignore_hail_on_eject, (*pIgnoreHailOnEject));
			exr.exchange(Attr.ignore_teleports_on_eject, (*pIgnoreTeleportsOnEject));
			exr.exchange(Attr.allow_teleports_between_worlds, (*pAllowTeleportsBetweenWorlds));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(AutoRespondingData& RemoteData)
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