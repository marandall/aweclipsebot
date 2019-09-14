#pragma once
#include "serializer.h"
#include "tsb.h"
#include "umbra_remote_class.h"
#include "raw_param_editor.h"
#include "PropertyScriptCtrl.h"
#include "GlobalDex.h"

namespace CIS { namespace Config { 
namespace AutoTemplates 
{
	class AccessPropertyBuilder;

	class AccessData : 
		public Umbra::RemoteClass,
		public IRawParamEditor
	{
	public: // type defenition for property builder
		typedef AccessPropertyBuilder PropertyBuilder;
	
	public: // source data
		bool        allow_remote_commands;
		bool        allow_botgram_commands;
		Ascii       disabled_commands;
		bool        always_use_consoles;

		
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
			DataUpdated();
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
		
		AccessData& GetDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual void DataUpdated() { }
		
	public: // read only operators
		bool         get_AllowRemoteCommands           () const { return allow_remote_commands; }
		bool         get_AllowBotgramCommands          () const { return allow_botgram_commands; }
		Ascii        get_DisabledCommands              () const { return disabled_commands; }
		bool         get_AlwaysUseConsoles             () const { return always_use_consoles; }

		virtual void set_AllowRemoteCommands           (bool         Assgn) { allow_remote_commands = Assgn; }
		virtual void set_AllowBotgramCommands          (bool         Assgn) { allow_botgram_commands = Assgn; }
		virtual void set_DisabledCommands              (Ascii        Assgn) { disabled_commands = Assgn; }
		virtual void set_AlwaysUseConsoles             (bool         Assgn) { always_use_consoles = Assgn; }

		
	public: // string conversion
		virtual Ascii param_read(Ascii ParamName)
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
		
		virtual bool param_write(Ascii ParamName, Ascii Value)
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
		Umbra::Variant Umbra_GetProperty(Umbra::string ID, Umbra::ES_GetProperty& ES)
		{
			if (strcmp(ID.c_str(), "allow_remote_commands"       ) == 0) return allow_remote_commands;
			if (strcmp(ID.c_str(), "allow_botgram_commands"      ) == 0) return allow_botgram_commands;
			if (strcmp(ID.c_str(), "disabled_commands"           ) == 0) return (const char*)disabled_commands;
			if (strcmp(ID.c_str(), "always_use_consoles"         ) == 0) return always_use_consoles;

			
			// nothing found
			throw Umbra::UnsupportedPropertyException(ID);
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
		}
		
		void attributes_to_dialog()
		{
			data_exchange(data, DataExchanger::TO_CONTROL);
		}
		
		void dialog_to_attributes(AccessData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
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

			
			if (Mode == DataExchanger::FROM_CONTROL)
			{
				Attr.DataUpdated();
			}
			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(AccessData& RemoteData)
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
} }