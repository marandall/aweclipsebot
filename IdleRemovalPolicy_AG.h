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

namespace Policy { 
namespace AutoTemplates 
{
	class IdleRemovalPolicyPropertyBuilder;

	//
	// Import Data
	//
	// a:7:{i:0;a:4:{s:4:"name";s:7:"enabled";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:20:"activate_above_users";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:15:"initial_message";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:20:"require_initial_chat";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:17:"remove_after_idle";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:5;a:4:{s:4:"name";s:11:"teleport_to";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:6;a:4:{s:4:"name";s:15:"removal_message";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class IdleRemovalPolicyData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef IdleRemovalPolicyPropertyBuilder PropertyBuilder;
	
	public: // source data
		bool enabled;
		int         activate_above_users;
		String initial_message;
		int         require_initial_chat;
		int         remove_after_idle;
		String teleport_to;
		String removal_message;

		
	public:
		IdleRemovalPolicyData()
		{
			enabled = false;
			activate_above_users = 0;
			initial_message = "";
			require_initial_chat = 0;
			remove_after_idle = 0;
			teleport_to = "";
			removal_message = "";

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
			char* section_id = const_cast<char*>("policy_IdleRemovalPolicy");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "enabled", enabled);
			SerializerSystem.Exchange(ExchangeMode, section_id, "activate_above_users", activate_above_users);
			SerializerSystem.Exchange(ExchangeMode, section_id, "initial_message", initial_message);
			SerializerSystem.Exchange(ExchangeMode, section_id, "require_initial_chat", require_initial_chat);
			SerializerSystem.Exchange(ExchangeMode, section_id, "remove_after_idle", remove_after_idle);
			SerializerSystem.Exchange(ExchangeMode, section_id, "teleport_to", teleport_to);
			SerializerSystem.Exchange(ExchangeMode, section_id, "removal_message", removal_message);

		}
		
		IdleRemovalPolicyData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		bool  getEnabled                       () const { return enabled; }
		int          getActivateAboveUsers            () const { return activate_above_users; }
		String  getInitialMessage                () const { return initial_message; }
		int          getRequireInitialChat            () const { return require_initial_chat; }
		int          getRemoveAfterIdle               () const { return remove_after_idle; }
		String  getTeleportTo                    () const { return teleport_to; }
		String  getRemovalMessage                () const { return removal_message; }

		virtual void setEnabled                       (bool  Assgn) { enabled = Assgn; }
		virtual void setActivateAboveUsers            (int          Assgn) { activate_above_users = Assgn; }
		virtual void setInitialMessage                (String  Assgn) { initial_message = Assgn; }
		virtual void setRequireInitialChat            (int          Assgn) { require_initial_chat = Assgn; }
		virtual void setRemoveAfterIdle               (int          Assgn) { remove_after_idle = Assgn; }
		virtual void setTeleportTo                    (String  Assgn) { teleport_to = Assgn; }
		virtual void setRemovalMessage                (String  Assgn) { removal_message = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "enabled") {
				return Conversion::ToString(enabled);
			}
			if (ParamName == "activate_above_users") {
				return Conversion::ToString(activate_above_users);
			}
			if (ParamName == "initial_message") {
				return Conversion::ToString(initial_message);
			}
			if (ParamName == "require_initial_chat") {
				return Conversion::ToString(require_initial_chat);
			}
			if (ParamName == "remove_after_idle") {
				return Conversion::ToString(remove_after_idle);
			}
			if (ParamName == "teleport_to") {
				return Conversion::ToString(teleport_to);
			}
			if (ParamName == "removal_message") {
				return Conversion::ToString(removal_message);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "enabled") { 
				enabled = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "activate_above_users") { 
				activate_above_users = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "initial_message") { 
				initial_message = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "require_initial_chat") { 
				require_initial_chat = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "remove_after_idle") { 
				remove_after_idle = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "teleport_to") { 
				teleport_to = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "removal_message") { 
				removal_message = Conversion::ToString(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(enabled, enabled);
			UMBRA_PROPERTY_RW(activate_above_users, activate_above_users);
			UMBRA_PROPERTY_RW(initial_message, initial_message);
			UMBRA_PROPERTY_RW(require_initial_chat, require_initial_chat);
			UMBRA_PROPERTY_RW(remove_after_idle, remove_after_idle);
			UMBRA_PROPERTY_RW(teleport_to, teleport_to);
			UMBRA_PROPERTY_RW(removal_message, removal_message);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class IdleRemovalPolicyPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CButton          *pEnabled       ;
		TextBox          *pActivateAboveUsers;
		TextBox          *pInitialMessage;
		TextBox          *pRequireInitialChat;
		TextBox          *pRemoveAfterIdle;
		TextBox          *pTeleportTo    ;
		TextBox          *pRemovalMessage;

		
	public: // a set of the special property type
		IdleRemovalPolicyData data;
		IdleRemovalPolicyData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pEnabled        = PageCtrl.create_check     (GroupID, "", "Enabled");
			pActivateAboveUsers = PageCtrl.create_edit      (GroupID, "", "Activate Above Users");
			pInitialMessage = PageCtrl.create_edit      (GroupID, "", "Initial Message");
			pRequireInitialChat = PageCtrl.create_edit      (GroupID, "", "Require Initial Chat");
			pRemoveAfterIdle = PageCtrl.create_edit      (GroupID, "", "Remove After Idle");
			pTeleportTo     = PageCtrl.create_edit      (GroupID, "", "Teleport To");
			pRemovalMessage = PageCtrl.create_edit      (GroupID, "", "Removal Message");

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
		
		bool dialog_to_attributes(IdleRemovalPolicyData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(IdleRemovalPolicyData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(IdleRemovalPolicyData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.enabled, (*pEnabled       ));
			exr.exchange(Attr.activate_above_users, (*pActivateAboveUsers));
			exr.exchange(Attr.initial_message, (*pInitialMessage));
			exr.exchange(Attr.require_initial_chat, (*pRequireInitialChat));
			exr.exchange(Attr.remove_after_idle, (*pRemoveAfterIdle));
			exr.exchange(Attr.teleport_to, (*pTeleportTo    ));
			exr.exchange(Attr.removal_message, (*pRemovalMessage));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(IdleRemovalPolicyData& RemoteData)
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