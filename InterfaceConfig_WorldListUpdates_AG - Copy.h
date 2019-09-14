#pragma once
#include "serializer.h"
#include "tsb.h"
#include "umbra_remote_class.h"
#include "raw_param_editor.h"
#include "PropertyScriptCtrl.h"
#include "GlobalDex.h"

namespace InterfaceConfig { 
namespace AutoTemplates 
{
	class WorldListUpdatesPropertyBuilder;

	class WorldListUpdatesData : 
		public Umbra::RemoteClass,
		public IRawParamEditor
	{
	public: // type defenition for property builder
		typedef WorldListUpdatesPropertyBuilder PropertyBuilder;
	
	public: // source data
		int         query_frequency;
		bool        use_smart_history;
		bool        show_online;
		bool        show_offline;
		bool        show_status_changes;
		int         show_users_breaking;

		
	public:
		WorldListUpdatesData()
		{
			query_frequency = 30;
			use_smart_history = false;
			show_online = false;
			show_offline = false;
			show_status_changes = false;
			show_users_breaking = 40;

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
			char* section_id = const_cast<char*>("ifg_WorldListUpdates");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "query_frequency", query_frequency);
			SerializerSystem.Exchange(ExchangeMode, section_id, "use_smart_history", use_smart_history);
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_online", show_online);
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_offline", show_offline);
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_status_changes", show_status_changes);
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_users_breaking", show_users_breaking);

		}
		
		WorldListUpdatesData& GetDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual void DataUpdated() { }
		
	public: // read only operators
		int          get_QueryFrequency                () const { return query_frequency; }
		bool         get_UseSmartHistory               () const { return use_smart_history; }
		bool         get_ShowOnline                    () const { return show_online; }
		bool         get_ShowOffline                   () const { return show_offline; }
		bool         get_ShowStatusChanges             () const { return show_status_changes; }
		int          get_ShowUsersBreaking             () const { return show_users_breaking; }

		virtual void set_QueryFrequency                (int          Assgn) { query_frequency = Assgn; }
		virtual void set_UseSmartHistory               (bool         Assgn) { use_smart_history = Assgn; }
		virtual void set_ShowOnline                    (bool         Assgn) { show_online = Assgn; }
		virtual void set_ShowOffline                   (bool         Assgn) { show_offline = Assgn; }
		virtual void set_ShowStatusChanges             (bool         Assgn) { show_status_changes = Assgn; }
		virtual void set_ShowUsersBreaking             (int          Assgn) { show_users_breaking = Assgn; }

		
	public: // string conversion
		virtual Ascii param_read(Ascii ParamName)
		{
			if (ParamName == "query_frequency") {
				return Conversion::ToString(query_frequency);
			}
			if (ParamName == "use_smart_history") {
				return Conversion::ToString(use_smart_history);
			}
			if (ParamName == "show_online") {
				return Conversion::ToString(show_online);
			}
			if (ParamName == "show_offline") {
				return Conversion::ToString(show_offline);
			}
			if (ParamName == "show_status_changes") {
				return Conversion::ToString(show_status_changes);
			}
			if (ParamName == "show_users_breaking") {
				return Conversion::ToString(show_users_breaking);
			}

			return "";
		}
		
		virtual bool param_write(Ascii ParamName, Ascii Value)
		{
			if (ParamName == "query_frequency") { 
				query_frequency = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "use_smart_history") { 
				use_smart_history = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "show_online") { 
				show_online = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "show_offline") { 
				show_offline = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "show_status_changes") { 
				show_status_changes = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "show_users_breaking") { 
				show_users_breaking = Conversion::ToInt(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		Umbra::Variant Umbra_GetProperty(Umbra::string ID, Umbra::ES_GetProperty& ES)
		{
			if (strcmp(ID.c_str(), "query_frequency"             ) == 0) return query_frequency;
			if (strcmp(ID.c_str(), "use_smart_history"           ) == 0) return use_smart_history;
			if (strcmp(ID.c_str(), "show_online"                 ) == 0) return show_online;
			if (strcmp(ID.c_str(), "show_offline"                ) == 0) return show_offline;
			if (strcmp(ID.c_str(), "show_status_changes"         ) == 0) return show_status_changes;
			if (strcmp(ID.c_str(), "show_users_breaking"         ) == 0) return show_users_breaking;

			
			// nothing found
			throw Umbra::UnsupportedPropertyException(ID);
		}	
	};

	
	class WorldListUpdatesPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pQueryFrequency;
		CButton          *pUseSmartHistory;
		CButton          *pShowOnline    ;
		CButton          *pShowOffline   ;
		CButton          *pShowStatusChanges;
		TextBox          *pShowUsersBreaking;

		
	public: // a set of the special property type
		WorldListUpdatesData data;
		WorldListUpdatesData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pQueryFrequency = PageCtrl.create_edit      (GroupID, "", "Query Frequency");
			pUseSmartHistory = PageCtrl.create_check     (GroupID, "", "Use Smart History");
			pShowOnline     = PageCtrl.create_check     (GroupID, "", "Show Online");
			pShowOffline    = PageCtrl.create_check     (GroupID, "", "Show Offline");
			pShowStatusChanges = PageCtrl.create_check     (GroupID, "", "Show Status Changes");
			pShowUsersBreaking = PageCtrl.create_edit      (GroupID, "", "Show Users Breaking");

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
		
		void dialog_to_attributes(WorldListUpdatesData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
		}
		
		void attributes_to_dialog(WorldListUpdatesData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(WorldListUpdatesData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.query_frequency, (*pQueryFrequency));
			exr.exchange(Attr.use_smart_history, (*pUseSmartHistory));
			exr.exchange(Attr.show_online, (*pShowOnline    ));
			exr.exchange(Attr.show_offline, (*pShowOffline   ));
			exr.exchange(Attr.show_status_changes, (*pShowStatusChanges));
			exr.exchange(Attr.show_users_breaking, (*pShowUsersBreaking));

			
			if (Mode == DataExchanger::FROM_CONTROL)
			{
				Attr.DataUpdated();
			}
			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(WorldListUpdatesData& RemoteData)
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