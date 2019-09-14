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
	class PropertyUpdatesPropertyBuilder;

	//
	// Import Data
	//
	// a:8:{i:0;a:4:{s:4:"name";s:15:"show_add_target";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:13:"on_add_format";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:18:"show_delete_target";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:16:"on_delete_format";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:17:"show_click_target";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:5;a:4:{s:4:"name";s:15:"on_click_format";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:6;a:4:{s:4:"name";s:18:"show_select_target";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:7;a:4:{s:4:"name";s:16:"on_select_format";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class PropertyUpdatesData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef PropertyUpdatesPropertyBuilder PropertyBuilder;
	
	public: // source data
		int         show_add_target;
		String on_add_format;
		int         show_delete_target;
		String on_delete_format;
		int         show_click_target;
		String on_click_format;
		int         show_select_target;
		String on_select_format;

		
	public:
		PropertyUpdatesData()
		{
			show_add_target = 0;
			on_add_format = "EVENT_OBJECT_ADD {$object.model} added by {$user.name} at {$model.coords}";
			show_delete_target = 0;
			on_delete_format = "EVENT_OBJECT_DELETE {$object.model} deleted by {$user.name} at {$model.coords}";
			show_click_target = 0;
			on_click_format = "EVENT_OBJECT_CLICK {$user.name} clicked {$object.model} at {$object.coords}";
			show_select_target = 0;
			on_select_format = "EVENT_OBJECT_SELECT {$user.name} selected {$object.model} at {$object.coords}";

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
			char* section_id = const_cast<char*>("ifg_PropertyUpdates");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_add_target", show_add_target);
			SerializerSystem.Exchange(ExchangeMode, section_id, "on_add_format", on_add_format);
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_delete_target", show_delete_target);
			SerializerSystem.Exchange(ExchangeMode, section_id, "on_delete_format", on_delete_format);
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_click_target", show_click_target);
			SerializerSystem.Exchange(ExchangeMode, section_id, "on_click_format", on_click_format);
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_select_target", show_select_target);
			SerializerSystem.Exchange(ExchangeMode, section_id, "on_select_format", on_select_format);

		}
		
		PropertyUpdatesData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		int          getShowAddTarget                 () const { return show_add_target; }
		String  getOnAddFormat                   () const { return on_add_format; }
		int          getShowDeleteTarget              () const { return show_delete_target; }
		String  getOnDeleteFormat                () const { return on_delete_format; }
		int          getShowClickTarget               () const { return show_click_target; }
		String  getOnClickFormat                 () const { return on_click_format; }
		int          getShowSelectTarget              () const { return show_select_target; }
		String  getOnSelectFormat                () const { return on_select_format; }

		virtual void setShowAddTarget                 (int          Assgn) { show_add_target = Assgn; }
		virtual void setOnAddFormat                   (String  Assgn) { on_add_format = Assgn; }
		virtual void setShowDeleteTarget              (int          Assgn) { show_delete_target = Assgn; }
		virtual void setOnDeleteFormat                (String  Assgn) { on_delete_format = Assgn; }
		virtual void setShowClickTarget               (int          Assgn) { show_click_target = Assgn; }
		virtual void setOnClickFormat                 (String  Assgn) { on_click_format = Assgn; }
		virtual void setShowSelectTarget              (int          Assgn) { show_select_target = Assgn; }
		virtual void setOnSelectFormat                (String  Assgn) { on_select_format = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "show_add_target") {
				return Conversion::ToString(show_add_target);
			}
			if (ParamName == "on_add_format") {
				return Conversion::ToString(on_add_format);
			}
			if (ParamName == "show_delete_target") {
				return Conversion::ToString(show_delete_target);
			}
			if (ParamName == "on_delete_format") {
				return Conversion::ToString(on_delete_format);
			}
			if (ParamName == "show_click_target") {
				return Conversion::ToString(show_click_target);
			}
			if (ParamName == "on_click_format") {
				return Conversion::ToString(on_click_format);
			}
			if (ParamName == "show_select_target") {
				return Conversion::ToString(show_select_target);
			}
			if (ParamName == "on_select_format") {
				return Conversion::ToString(on_select_format);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "show_add_target") { 
				show_add_target = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "on_add_format") { 
				on_add_format = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "show_delete_target") { 
				show_delete_target = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "on_delete_format") { 
				on_delete_format = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "show_click_target") { 
				show_click_target = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "on_click_format") { 
				on_click_format = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "show_select_target") { 
				show_select_target = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "on_select_format") { 
				on_select_format = Conversion::ToString(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(show_add_target, show_add_target);
			UMBRA_PROPERTY_RW(on_add_format, on_add_format);
			UMBRA_PROPERTY_RW(show_delete_target, show_delete_target);
			UMBRA_PROPERTY_RW(on_delete_format, on_delete_format);
			UMBRA_PROPERTY_RW(show_click_target, show_click_target);
			UMBRA_PROPERTY_RW(on_click_format, on_click_format);
			UMBRA_PROPERTY_RW(show_select_target, show_select_target);
			UMBRA_PROPERTY_RW(on_select_format, on_select_format);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class PropertyUpdatesPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		ComboBox         *pShowAddTarget ;
		TextBox          *pOnAddFormat   ;
		ComboBox         *pShowDeleteTarget;
		TextBox          *pOnDeleteFormat;
		ComboBox         *pShowClickTarget;
		TextBox          *pOnClickFormat ;
		ComboBox         *pShowSelectTarget;
		TextBox          *pOnSelectFormat;

		
	public: // a set of the special property type
		PropertyUpdatesData data;
		PropertyUpdatesData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pShowAddTarget  = PageCtrl.create_combo     (GroupID, "", "Show Add Target");
			pOnAddFormat    = PageCtrl.create_edit      (GroupID, "", "On Add Format");
			pShowDeleteTarget = PageCtrl.create_combo     (GroupID, "", "Show Delete Target");
			pOnDeleteFormat = PageCtrl.create_edit      (GroupID, "", "On Delete Format");
			pShowClickTarget = PageCtrl.create_combo     (GroupID, "", "Show Click Target");
			pOnClickFormat  = PageCtrl.create_edit      (GroupID, "", "On Click Format");
			pShowSelectTarget = PageCtrl.create_combo     (GroupID, "", "Show Select Target");
			pOnSelectFormat = PageCtrl.create_edit      (GroupID, "", "On Select Format");

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
		
		bool dialog_to_attributes(PropertyUpdatesData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(PropertyUpdatesData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(PropertyUpdatesData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.show_add_target, (*pShowAddTarget ));
			exr.exchange(Attr.on_add_format, (*pOnAddFormat   ));
			exr.exchange(Attr.show_delete_target, (*pShowDeleteTarget));
			exr.exchange(Attr.on_delete_format, (*pOnDeleteFormat));
			exr.exchange(Attr.show_click_target, (*pShowClickTarget));
			exr.exchange(Attr.on_click_format, (*pOnClickFormat ));
			exr.exchange(Attr.show_select_target, (*pShowSelectTarget));
			exr.exchange(Attr.on_select_format, (*pOnSelectFormat));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(PropertyUpdatesData& RemoteData)
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