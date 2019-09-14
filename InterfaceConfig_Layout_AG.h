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
	class LayoutPropertyBuilder;

	//
	// Import Data
	//
	// a:5:{i:0;a:4:{s:4:"name";s:12:"avatars_list";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:11:"worlds_list";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:10:"users_list";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:14:"show_mini_logs";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:15:"side_pane_width";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class LayoutData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef LayoutPropertyBuilder PropertyBuilder;
	
	public: // source data
		int         avatars_list;
		int         worlds_list;
		int         users_list;
		bool show_mini_logs;
		int         side_pane_width;

		
	public:
		LayoutData()
		{
			avatars_list = 1;
			worlds_list = 1;
			users_list = 0;
			show_mini_logs = false;
			side_pane_width = 180;

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
			char* section_id = const_cast<char*>("ifg_Layout");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "avatars_list", avatars_list);
			SerializerSystem.Exchange(ExchangeMode, section_id, "worlds_list", worlds_list);
			SerializerSystem.Exchange(ExchangeMode, section_id, "users_list", users_list);
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_mini_logs", show_mini_logs);
			SerializerSystem.Exchange(ExchangeMode, section_id, "side_pane_width", side_pane_width);

		}
		
		LayoutData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		int          getAvatarsList                   () const { return avatars_list; }
		int          getWorldsList                    () const { return worlds_list; }
		int          getUsersList                     () const { return users_list; }
		bool  getShowMiniLogs                  () const { return show_mini_logs; }
		int          getSidePaneWidth                 () const { return side_pane_width; }

		virtual void setAvatarsList                   (int          Assgn) { avatars_list = Assgn; }
		virtual void setWorldsList                    (int          Assgn) { worlds_list = Assgn; }
		virtual void setUsersList                     (int          Assgn) { users_list = Assgn; }
		virtual void setShowMiniLogs                  (bool  Assgn) { show_mini_logs = Assgn; }
		virtual void setSidePaneWidth                 (int          Assgn) { side_pane_width = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "avatars_list") {
				return Conversion::ToString(avatars_list);
			}
			if (ParamName == "worlds_list") {
				return Conversion::ToString(worlds_list);
			}
			if (ParamName == "users_list") {
				return Conversion::ToString(users_list);
			}
			if (ParamName == "show_mini_logs") {
				return Conversion::ToString(show_mini_logs);
			}
			if (ParamName == "side_pane_width") {
				return Conversion::ToString(side_pane_width);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "avatars_list") { 
				avatars_list = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "worlds_list") { 
				worlds_list = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "users_list") { 
				users_list = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "show_mini_logs") { 
				show_mini_logs = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "side_pane_width") { 
				side_pane_width = Conversion::ToInt(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(avatars_list, avatars_list);
			UMBRA_PROPERTY_RW(worlds_list, worlds_list);
			UMBRA_PROPERTY_RW(users_list, users_list);
			UMBRA_PROPERTY_RW(show_mini_logs, show_mini_logs);
			UMBRA_PROPERTY_RW(side_pane_width, side_pane_width);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class LayoutPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		ComboBox         *pAvatarsList   ;
		ComboBox         *pWorldsList    ;
		ComboBox         *pUsersList     ;
		CButton          *pShowMiniLogs  ;
		TextBox          *pSidePaneWidth ;

		
	public: // a set of the special property type
		LayoutData data;
		LayoutData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pAvatarsList    = PageCtrl.create_combo     (GroupID, "", "Avatars List");
			pWorldsList     = PageCtrl.create_combo     (GroupID, "", "Worlds List");
			pUsersList      = PageCtrl.create_combo     (GroupID, "", "Users List");
			pShowMiniLogs   = PageCtrl.create_check     (GroupID, "", "Show Mini Logs");
			pSidePaneWidth  = PageCtrl.create_edit      (GroupID, "", "Side Pane Width");

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
		
		bool dialog_to_attributes(LayoutData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(LayoutData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(LayoutData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.avatars_list, (*pAvatarsList   ));
			exr.exchange(Attr.worlds_list, (*pWorldsList    ));
			exr.exchange(Attr.users_list, (*pUsersList     ));
			exr.exchange(Attr.show_mini_logs, (*pShowMiniLogs  ));
			exr.exchange(Attr.side_pane_width, (*pSidePaneWidth ));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(LayoutData& RemoteData)
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