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

namespace InterfaceConfig { 
namespace AutoTemplates 
{
	class WorldListUpdatesPropertyBuilder;

	//
	// Import Data
	//
	// a:6:{i:0;a:4:{s:4:"name";s:15:"query_frequency";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:17:"use_smart_history";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:11:"show_online";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:12:"show_offline";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:19:"show_status_changes";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:5;a:4:{s:4:"name";s:19:"show_users_breaking";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class WorldListUpdatesData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef WorldListUpdatesPropertyBuilder PropertyBuilder;
	
	public: // source data
		int         query_frequency;
		bool use_smart_history;
		bool show_online;
		bool show_offline;
		bool show_status_changes;
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
			onUpdated();
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
		
		WorldListUpdatesData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		int          getQueryFrequency                () const { return query_frequency; }
		bool  getUseSmartHistory               () const { return use_smart_history; }
		bool  getShowOnline                    () const { return show_online; }
		bool  getShowOffline                   () const { return show_offline; }
		bool  getShowStatusChanges             () const { return show_status_changes; }
		int          getShowUsersBreaking             () const { return show_users_breaking; }

		virtual void setQueryFrequency                (int          Assgn) { query_frequency = Assgn; }
		virtual void setUseSmartHistory               (bool  Assgn) { use_smart_history = Assgn; }
		virtual void setShowOnline                    (bool  Assgn) { show_online = Assgn; }
		virtual void setShowOffline                   (bool  Assgn) { show_offline = Assgn; }
		virtual void setShowStatusChanges             (bool  Assgn) { show_status_changes = Assgn; }
		virtual void setShowUsersBreaking             (int          Assgn) { show_users_breaking = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
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
		
		virtual bool param_write(String ParamName, String Value)
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
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(query_frequency, query_frequency);
			UMBRA_PROPERTY_RW(use_smart_history, use_smart_history);
			UMBRA_PROPERTY_RW(show_online, show_online);
			UMBRA_PROPERTY_RW(show_offline, show_offline);
			UMBRA_PROPERTY_RW(show_status_changes, show_status_changes);
			UMBRA_PROPERTY_RW(show_users_breaking, show_users_breaking);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
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
			data.onUpdated();
		}
		
		void attributes_to_dialog()
		{
			data_exchange(data, DataExchanger::TO_CONTROL);
		}
		
		bool dialog_to_attributes(WorldListUpdatesData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
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


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(WorldListUpdatesData& RemoteData)
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