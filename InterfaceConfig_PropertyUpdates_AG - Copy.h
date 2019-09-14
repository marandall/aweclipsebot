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
	class PropertyUpdatesPropertyBuilder;

	class PropertyUpdatesData : 
		public Umbra::RemoteClass,
		public IRawParamEditor
	{
	public: // type defenition for property builder
		typedef PropertyUpdatesPropertyBuilder PropertyBuilder;
	
	public: // source data
		bool        show_add;
		Ascii       on_add_format;
		bool        show_delete;
		Ascii       on_delete_format;
		bool        show_click;
		Ascii       on_click_format;
		bool        show_select;
		Ascii       on_select_format;

		
	public:
		PropertyUpdatesData()
		{
			show_add = false;
			on_add_format = "EVENT_OBJECT_ADD {$object.model} added by {$user.name} at {$model.coords}";
			show_delete = false;
			on_delete_format = "EVENT_OBJECT_DELETE {$object.model} deleted by {$user.name} at {$model.coords}";
			show_click = false;
			on_click_format = "EVENT_OBJECT_CLICK {$user.name} clicked {$object.model} at {$object.coords}";
			show_select = false;
			on_select_format = "EVENT_OBJECT_SELECT {$user.name} selected {$object.model} at {$object.coords}";

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
			char* section_id = const_cast<char*>("ifg_PropertyUpdates");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_add", show_add);
			SerializerSystem.Exchange(ExchangeMode, section_id, "on_add_format", on_add_format);
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_delete", show_delete);
			SerializerSystem.Exchange(ExchangeMode, section_id, "on_delete_format", on_delete_format);
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_click", show_click);
			SerializerSystem.Exchange(ExchangeMode, section_id, "on_click_format", on_click_format);
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_select", show_select);
			SerializerSystem.Exchange(ExchangeMode, section_id, "on_select_format", on_select_format);

		}
		
		PropertyUpdatesData& GetDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual void DataUpdated() { }
		
	public: // read only operators
		bool         get_ShowAdd                       () const { return show_add; }
		Ascii        get_OnAddFormat                   () const { return on_add_format; }
		bool         get_ShowDelete                    () const { return show_delete; }
		Ascii        get_OnDeleteFormat                () const { return on_delete_format; }
		bool         get_ShowClick                     () const { return show_click; }
		Ascii        get_OnClickFormat                 () const { return on_click_format; }
		bool         get_ShowSelect                    () const { return show_select; }
		Ascii        get_OnSelectFormat                () const { return on_select_format; }

		virtual void set_ShowAdd                       (bool         Assgn) { show_add = Assgn; }
		virtual void set_OnAddFormat                   (Ascii        Assgn) { on_add_format = Assgn; }
		virtual void set_ShowDelete                    (bool         Assgn) { show_delete = Assgn; }
		virtual void set_OnDeleteFormat                (Ascii        Assgn) { on_delete_format = Assgn; }
		virtual void set_ShowClick                     (bool         Assgn) { show_click = Assgn; }
		virtual void set_OnClickFormat                 (Ascii        Assgn) { on_click_format = Assgn; }
		virtual void set_ShowSelect                    (bool         Assgn) { show_select = Assgn; }
		virtual void set_OnSelectFormat                (Ascii        Assgn) { on_select_format = Assgn; }

		
	public: // string conversion
		virtual Ascii param_read(Ascii ParamName)
		{
			if (ParamName == "show_add") {
				return Conversion::ToString(show_add);
			}
			if (ParamName == "on_add_format") {
				return Conversion::ToString(on_add_format);
			}
			if (ParamName == "show_delete") {
				return Conversion::ToString(show_delete);
			}
			if (ParamName == "on_delete_format") {
				return Conversion::ToString(on_delete_format);
			}
			if (ParamName == "show_click") {
				return Conversion::ToString(show_click);
			}
			if (ParamName == "on_click_format") {
				return Conversion::ToString(on_click_format);
			}
			if (ParamName == "show_select") {
				return Conversion::ToString(show_select);
			}
			if (ParamName == "on_select_format") {
				return Conversion::ToString(on_select_format);
			}

			return "";
		}
		
		virtual bool param_write(Ascii ParamName, Ascii Value)
		{
			if (ParamName == "show_add") { 
				show_add = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "on_add_format") { 
				on_add_format = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "show_delete") { 
				show_delete = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "on_delete_format") { 
				on_delete_format = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "show_click") { 
				show_click = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "on_click_format") { 
				on_click_format = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "show_select") { 
				show_select = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "on_select_format") { 
				on_select_format = Conversion::ToString(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		Umbra::Variant Umbra_GetProperty(Umbra::string ID, Umbra::ES_GetProperty& ES)
		{
			if (strcmp(ID.c_str(), "show_add"                    ) == 0) return show_add;
			if (strcmp(ID.c_str(), "on_add_format"               ) == 0) return (const char*)on_add_format;
			if (strcmp(ID.c_str(), "show_delete"                 ) == 0) return show_delete;
			if (strcmp(ID.c_str(), "on_delete_format"            ) == 0) return (const char*)on_delete_format;
			if (strcmp(ID.c_str(), "show_click"                  ) == 0) return show_click;
			if (strcmp(ID.c_str(), "on_click_format"             ) == 0) return (const char*)on_click_format;
			if (strcmp(ID.c_str(), "show_select"                 ) == 0) return show_select;
			if (strcmp(ID.c_str(), "on_select_format"            ) == 0) return (const char*)on_select_format;

			
			// nothing found
			throw Umbra::UnsupportedPropertyException(ID);
		}	
	};

	
	class PropertyUpdatesPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CButton          *pShowAdd       ;
		TextBox          *pOnAddFormat   ;
		CButton          *pShowDelete    ;
		TextBox          *pOnDeleteFormat;
		CButton          *pShowClick     ;
		TextBox          *pOnClickFormat ;
		CButton          *pShowSelect    ;
		TextBox          *pOnSelectFormat;

		
	public: // a set of the special property type
		PropertyUpdatesData data;
		PropertyUpdatesData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pShowAdd        = PageCtrl.create_check     (GroupID, "", "Show Add");
			pOnAddFormat    = PageCtrl.create_edit      (GroupID, "", "On Add Format");
			pShowDelete     = PageCtrl.create_check     (GroupID, "", "Show Delete");
			pOnDeleteFormat = PageCtrl.create_edit      (GroupID, "", "On Delete Format");
			pShowClick      = PageCtrl.create_check     (GroupID, "", "Show Click");
			pOnClickFormat  = PageCtrl.create_edit      (GroupID, "", "On Click Format");
			pShowSelect     = PageCtrl.create_check     (GroupID, "", "Show Select");
			pOnSelectFormat = PageCtrl.create_edit      (GroupID, "", "On Select Format");

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
		
		void dialog_to_attributes(PropertyUpdatesData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
		}
		
		void attributes_to_dialog(PropertyUpdatesData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(PropertyUpdatesData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.show_add, (*pShowAdd       ));
			exr.exchange(Attr.on_add_format, (*pOnAddFormat   ));
			exr.exchange(Attr.show_delete, (*pShowDelete    ));
			exr.exchange(Attr.on_delete_format, (*pOnDeleteFormat));
			exr.exchange(Attr.show_click, (*pShowClick     ));
			exr.exchange(Attr.on_click_format, (*pOnClickFormat ));
			exr.exchange(Attr.show_select, (*pShowSelect    ));
			exr.exchange(Attr.on_select_format, (*pOnSelectFormat));

			
			if (Mode == DataExchanger::FROM_CONTROL)
			{
				Attr.DataUpdated();
			}
			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(PropertyUpdatesData& RemoteData)
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