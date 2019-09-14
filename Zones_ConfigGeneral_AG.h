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

namespace Zones { 
namespace AutoTemplates 
{
	class ConfigGeneralPropertyBuilder;

	//
	// Import Data
	//
	// a:6:{i:0;a:4:{s:4:"name";s:7:"enabled";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:19:"announcement_format";s:4:"type";s:11:"messageinfo";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:15:"welcome_message";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:17:"enable_protection";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:17:"protection_action";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:5;a:4:{s:4:"name";s:16:"protection_alert";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class ConfigGeneralData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef ConfigGeneralPropertyBuilder PropertyBuilder;
	
	public: // source data
		bool enabled;
		MessageInfo announcement_format;
		String welcome_message;
		bool enable_protection;
		String protection_action;
		String protection_alert;

		
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
			onUpdated();
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
		
		ConfigGeneralData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		bool  getEnabled                       () const { return enabled; }
		MessageInfo  getAnnouncementFormat            () const { return announcement_format; }
		String  getWelcomeMessage                () const { return welcome_message; }
		bool  getEnableProtection              () const { return enable_protection; }
		String  getProtectionAction              () const { return protection_action; }
		String  getProtectionAlert               () const { return protection_alert; }

		virtual void setEnabled                       (bool  Assgn) { enabled = Assgn; }
		virtual void setAnnouncementFormat            (MessageInfo  Assgn) { announcement_format = Assgn; }
		virtual void setWelcomeMessage                (String  Assgn) { welcome_message = Assgn; }
		virtual void setEnableProtection              (bool  Assgn) { enable_protection = Assgn; }
		virtual void setProtectionAction              (String  Assgn) { protection_action = Assgn; }
		virtual void setProtectionAlert               (String  Assgn) { protection_alert = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
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
		
		virtual bool param_write(String ParamName, String Value)
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
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(enabled, enabled);
			UMBRA_PROPERTY_RW(announcement_format, announcement_format);
			UMBRA_PROPERTY_RW(welcome_message, welcome_message);
			UMBRA_PROPERTY_RW(enable_protection, enable_protection);
			UMBRA_PROPERTY_RW(protection_action, protection_action);
			UMBRA_PROPERTY_RW(protection_alert, protection_alert);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
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
			data.onUpdated();
		}
		
		void attributes_to_dialog()
		{
			data_exchange(data, DataExchanger::TO_CONTROL);
		}
		
		bool dialog_to_attributes(ConfigGeneralData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
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


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(ConfigGeneralData& RemoteData)
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