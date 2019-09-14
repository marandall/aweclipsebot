#pragma once
#include "serializer.h"
#include "tsb.h"
#include "autonumber.h"

// umbra includes
#include "umbra/umbra_remoteclass.h"
#include "umbra/umbra_variant.h"

#include "raw_param_editor.h"
#include "PropertyScriptCtrl.h"
#include "GlobalDex.h"

namespace CIS { namespace Config { 
namespace AutoTemplates 
{
	class AccessPropertyBuilder;

	//
	// Import Data
	//
	// a:4:{i:0;a:4:{s:4:"name";s:21:"allow_remote_commands";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:22:"allow_botgram_commands";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:17:"disabled_commands";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:19:"always_use_consoles";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class AccessData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier
	{
	public: // type defenition for property builder
		typedef AccessPropertyBuilder PropertyBuilder;
	
	public: // source data
		bool allow_remote_commands;
		bool allow_botgram_commands;
		String disabled_commands;
		bool always_use_consoles;

		
	public:
		AccessData()
		{
			allow_remote_commands = true;
			allow_botgram_commands = false;
			disabled_commands = ;
			always_use_consoles = false;

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
			char* section_id = const_cast<char*>("cis_Access");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "allow_remote_commands", allow_remote_commands);
			SerializerSystem.Exchange(ExchangeMode, section_id, "allow_botgram_commands", allow_botgram_commands);
			SerializerSystem.Exchange(ExchangeMode, section_id, "disabled_commands", disabled_commands);
			SerializerSystem.Exchange(ExchangeMode, section_id, "always_use_consoles", always_use_consoles);

		}
		
		AccessData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		bool  getAllowRemoteCommands           () const { return allow_remote_commands; }
		bool  getAllowBotgramCommands          () const { return allow_botgram_commands; }
		String  getDisabledCommands              () const { return disabled_commands; }
		bool  getAlwaysUseConsoles             () const { return always_use_consoles; }

		virtual void setAllowRemoteCommands           (bool  Assgn) { allow_remote_commands = Assgn; }
		virtual void setAllowBotgramCommands          (bool  Assgn) { allow_botgram_commands = Assgn; }
		virtual void setDisabledCommands              (String  Assgn) { disabled_commands = Assgn; }
		virtual void setAlwaysUseConsoles             (bool  Assgn) { always_use_consoles = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "allow_remote_commands") {
				return Conversion::ToString(allow_remote_commands);
			}
			if (ParamName == "allow_botgram_commands") {
				return Conversion::ToString(allow_botgram_commands);
			}
			if (ParamName == "disabled_commands") {
				return Conversion::ToString(disabled_commands);
			}
			if (ParamName == "always_use_consoles") {
				return Conversion::ToString(always_use_consoles);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "allow_remote_commands") { 
				allow_remote_commands = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "allow_botgram_commands") { 
				allow_botgram_commands = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "disabled_commands") { 
				disabled_commands = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "always_use_consoles") { 
				always_use_consoles = Conversion::ToBool(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(allow_remote_commands, allow_remote_commands);
			UMBRA_PROPERTY_RW(allow_botgram_commands, allow_botgram_commands);
			UMBRA_PROPERTY_RW(disabled_commands, disabled_commands);
			UMBRA_PROPERTY_RW(always_use_consoles, always_use_consoles);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class AccessPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CButton          *pAllowRemoteCommands;
		CButton          *pAllowBotgramCommands;
		TextBox          *pDisabledCommands;
		CButton          *pAlwaysUseConsoles;

		
	public: // a set of the special property type
		AccessData data;
		AccessData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pAllowRemoteCommands = PageCtrl.create_check     (GroupID, "", "Allow Remote Commands");
			pAllowBotgramCommands = PageCtrl.create_check     (GroupID, "", "Allow Botgram Commands");
			pDisabledCommands = PageCtrl.create_edit      (GroupID, "", "Disabled Commands");
			pAlwaysUseConsoles = PageCtrl.create_check     (GroupID, "", "Always Use Consoles");

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
		
		bool dialog_to_attributes(AccessData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(AccessData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(AccessData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.allow_remote_commands, (*pAllowRemoteCommands));
			exr.exchange(Attr.allow_botgram_commands, (*pAllowBotgramCommands));
			exr.exchange(Attr.disabled_commands, (*pDisabledCommands));
			exr.exchange(Attr.always_use_consoles, (*pAlwaysUseConsoles));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(AccessData& RemoteData)
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