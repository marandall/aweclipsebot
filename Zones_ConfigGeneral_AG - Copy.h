#pragma once
#include "serializer.h"
#include "tsb.h"
#include "umbra_remote_class.h"
#include "raw_param_editor.h"
#include "PropertyScriptCtrl.h"
#include "GlobalDex.h"

namespace Zones { 
namespace AutoTemplates 
{
	class ConfigGeneralPropertyBuilder;

	class ConfigGeneralData : 
		public Umbra::RemoteClass,
		public IRawParamEditor
	{
	public: // type defenition for property builder
		typedef ConfigGeneralPropertyBuilder PropertyBuilder;
	
	public: // source data
		bool        enabled;
		MessageInfo announcement_format;
		Ascii       welcome_message;
		bool        enable_protection;
		Ascii       protection_action;
		Ascii       protection_alert;

		
	public:
		ConfigGeneralData()
		{
			enabled = false;
			announcement_format = "[{$_bot.name}]:\t{text}";
			welcome_message = "";
			enable_protection = false;
			protection_action = "avteleport ${$user.session}, world_ep()";
			protection_alert = "Sorry, you are not allowed to enter this zone.";

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
			char* section_id = const_cast<char*>("zones_ConfigGeneral");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "enabled", enabled);
			SerializerSystem.Exchange(ExchangeMode, section_id, "announcement_format", announcement_format);
			SerializerSystem.Exchange(ExchangeMode, section_id, "welcome_message", welcome_message);
			SerializerSystem.Exchange(ExchangeMode, section_id, "enable_protection", enable_protection);
			SerializerSystem.Exchange(ExchangeMode, section_id, "protection_action", protection_action);
			SerializerSystem.Exchange(ExchangeMode, section_id, "protection_alert", protection_alert);

		}
		
		ConfigGeneralData& GetDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual void DataUpdated() { }
		
	public: // read only operators
		bool         get_Enabled                       () const { return enabled; }
		MessageInfo  get_AnnouncementFormat            () const { return announcement_format; }
		Ascii        get_WelcomeMessage                () const { return welcome_message; }
		bool         get_EnableProtection              () const { return enable_protection; }
		Ascii        get_ProtectionAction              () const { return protection_action; }
		Ascii        get_ProtectionAlert               () const { return protection_alert; }

		virtual void set_Enabled                       (bool         Assgn) { enabled = Assgn; }
		virtual void set_AnnouncementFormat            (MessageInfo  Assgn) { announcement_format = Assgn; }
		virtual void set_WelcomeMessage                (Ascii        Assgn) { welcome_message = Assgn; }
		virtual void set_EnableProtection              (bool         Assgn) { enable_protection = Assgn; }
		virtual void set_ProtectionAction              (Ascii        Assgn) { protection_action = Assgn; }
		virtual void set_ProtectionAlert               (Ascii        Assgn) { protection_alert = Assgn; }

		
	public: // string conversion
		virtual Ascii param_read(Ascii ParamName)
		{
			if (ParamName == "enabled") {
				return Conversion::ToString(enabled);
			}
			if (ParamName == "welcome_message") {
				return Conversion::ToString(welcome_message);
			}
			if (ParamName == "enable_protection") {
				return Conversion::ToString(enable_protection);
			}
			if (ParamName == "protection_action") {
				return Conversion::ToString(protection_action);
			}
			if (ParamName == "protection_alert") {
				return Conversion::ToString(protection_alert);
			}

			return "";
		}
		
		virtual bool param_write(Ascii ParamName, Ascii Value)
		{
			if (ParamName == "enabled") { 
				enabled = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "welcome_message") { 
				welcome_message = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "enable_protection") { 
				enable_protection = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "protection_action") { 
				protection_action = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "protection_alert") { 
				protection_alert = Conversion::ToString(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		Umbra::Variant Umbra_GetProperty(Umbra::string ID, Umbra::ES_GetProperty& ES)
		{
			if (strcmp(ID.c_str(), "enabled"                     ) == 0) return enabled;
			if (strcmp(ID.c_str(), "announcement_format"         ) == 0) return Umbra::Variant(announcement_format.Umbra_GetRemote());
			if (strcmp(ID.c_str(), "welcome_message"             ) == 0) return (const char*)welcome_message;
			if (strcmp(ID.c_str(), "enable_protection"           ) == 0) return enable_protection;
			if (strcmp(ID.c_str(), "protection_action"           ) == 0) return (const char*)protection_action;
			if (strcmp(ID.c_str(), "protection_alert"            ) == 0) return (const char*)protection_alert;

			
			// nothing found
			throw Umbra::UnsupportedPropertyException(ID);
		}	
	};

	
	class ConfigGeneralPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CButton          *pEnabled       ;
		OutputFormatCtrl *pAnnouncementFormat;
		TextBox          *pWelcomeMessage;
		CButton          *pEnableProtection;
		TextBox          *pProtectionAction;
		TextBox          *pProtectionAlert;

		
	public: // a set of the special property type
		ConfigGeneralData data;
		ConfigGeneralData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pEnabled        = PageCtrl.create_check     (GroupID, "", "Enabled");
			pAnnouncementFormat = PageCtrl.create_formmater (GroupID, "", "Announcement Format");
			pWelcomeMessage = PageCtrl.create_edit      (GroupID, "", "Welcome Message");
			pEnableProtection = PageCtrl.create_check     (GroupID, "", "Enable Protection");
			pProtectionAction = PageCtrl.create_edit      (GroupID, "", "Protection Action");
			pProtectionAlert = PageCtrl.create_edit      (GroupID, "", "Protection Alert");

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
		
		void dialog_to_attributes(ConfigGeneralData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
		}
		
		void attributes_to_dialog(ConfigGeneralData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(ConfigGeneralData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.enabled, (*pEnabled       ));
			exr.exchange(Attr.announcement_format, (*pAnnouncementFormat));
			exr.exchange(Attr.welcome_message, (*pWelcomeMessage));
			exr.exchange(Attr.enable_protection, (*pEnableProtection));
			exr.exchange(Attr.protection_action, (*pProtectionAction));
			exr.exchange(Attr.protection_alert, (*pProtectionAlert));

			
			if (Mode == DataExchanger::FROM_CONTROL)
			{
				Attr.DataUpdated();
			}
			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(ConfigGeneralData& RemoteData)
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