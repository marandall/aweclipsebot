#pragma once
#include "serializer.h"
#include "tsb.h"
#include "umbra_remote_class.h"
#include "raw_param_editor.h"
#include "PropertyScriptCtrl.h"
#include "GlobalDex.h"

namespace Policy { 
namespace AutoTemplates 
{
	class IdleRemovalPolicyPropertyBuilder;

	class IdleRemovalPolicyData : 
		public Umbra::RemoteClass,
		public IRawParamEditor
	{
	public: // type defenition for property builder
		typedef IdleRemovalPolicyPropertyBuilder PropertyBuilder;
	
	public: // source data
		tsb_t       enabled;
		tsb_t       activate_above_users;
		Ascii       initial_message;
		int         require_initial_chat;
		int         remove_after_idle;
		Ascii       teleport_to;
		Ascii       removal_message;

		
	public:
		IdleRemovalPolicyData()
		{
			enabled = B_EMPTY;
			activate_above_users = B_EMPTY;
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
			char* section_id = const_cast<char*>("drone_IdleRemovalPolicy");
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
		tsb_t        getEnabled                       () const { return enabled; }
		tsb_t        getActivateAboveUsers            () const { return activate_above_users; }
		Ascii        getInitialMessage                () const { return initial_message; }
		int          getRequireInitialChat            () const { return require_initial_chat; }
		int          getRemoveAfterIdle               () const { return remove_after_idle; }
		Ascii        getTeleportTo                    () const { return teleport_to; }
		Ascii        getRemovalMessage                () const { return removal_message; }

		virtual void setEnabled                       (tsb_t        Assgn) { enabled = Assgn; }
		virtual void setActivateAboveUsers            (tsb_t        Assgn) { activate_above_users = Assgn; }
		virtual void setInitialMessage                (Ascii        Assgn) { initial_message = Assgn; }
		virtual void setRequireInitialChat            (int          Assgn) { require_initial_chat = Assgn; }
		virtual void setRemoveAfterIdle               (int          Assgn) { remove_after_idle = Assgn; }
		virtual void setTeleportTo                    (Ascii        Assgn) { teleport_to = Assgn; }
		virtual void setRemovalMessage                (Ascii        Assgn) { removal_message = Assgn; }

		
	public: // string conversion
		virtual Ascii param_read(Ascii ParamName)
		{
			if (ParamName == "enabled") {
				return Conversion::ToStringTSB(enabled);
			}
			if (ParamName == "activate_above_users") {
				return Conversion::ToStringTSB(activate_above_users);
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
		
		virtual bool param_write(Ascii ParamName, Ascii Value)
		{
			if (ParamName == "enabled") { 
				enabled = Conversion::ToTSB(Value);
				return true;
			}
			if (ParamName == "activate_above_users") { 
				activate_above_users = Conversion::ToTSB(Value);
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
		void grabUmbraProperty(Umbra::Variant& Returned, Umbra::string ID, Umbra::ES_GetProperty& ES)
		{
			// nothing found
			throw Umbra::UnsupportedPropertyException(ID);
		}
		
		Umbra::string getUmbraString(Umbra::ES_GetString& ES)
		{
			return "[instance(namespace Policy { .IdleRemovalPolicy.} )]";
		}
		
		void executeUmbraFunction(Umbra::Variant& Returned, Umbra::string ID, Umbra::VariantMap& VMap, Umbra::ES_CallFunction& ES)
		{
			UMBRA_RMTCLS_A0_REFLECT(getEnabled,                   int);
			UMBRA_RMTCLS_A0_REFLECT(getActivateAboveUsers,        int);
			UMBRA_RMTCLS_A0_REFLECT(getInitialMessage,            const char*);
			UMBRA_RMTCLS_A0_REFLECT(getRequireInitialChat,        int);
			UMBRA_RMTCLS_A0_REFLECT(getRemoveAfterIdle,           int);
			UMBRA_RMTCLS_A0_REFLECT(getTeleportTo,                const char*);
			UMBRA_RMTCLS_A0_REFLECT(getRemovalMessage,            const char*);

			
			// nothing found
			throw Umbra::UnsupportedFunctionException(ID);
		}
		
		Umbra::string getUmbraClassName()
		{
			return "";
		}
	};

	
	class IdleRemovalPolicyPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CButton          *pEnabled       ;
		CButton          *pActivateAboveUsers;
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
			pEnabled        = PageCtrl.create_tristate  (GroupID, "", "Enabled");
			pActivateAboveUsers = PageCtrl.create_tristate  (GroupID, "", "Activate Above Users");
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