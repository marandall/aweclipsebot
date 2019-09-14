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
	class ConfigHyperionPropertyBuilder;

	class ConfigHyperionData : 
		public Umbra::RemoteClass,
		public IRawParamEditor
	{
	public: // type defenition for property builder
		typedef ConfigHyperionPropertyBuilder PropertyBuilder;
	
	public: // source data
		bool        enabled;
		bool        target_bots;
		Ascii       template_path;
		bool        inherit_parent;

		
	public:
		ConfigHyperionData()
		{
			enabled = false;
			target_bots = false;
			template_path = "{$_app.world_path}\\world_templates\\{$template_name}.awatt;{$_app.data_path}\\shared_world_templates\\{$template_name}.awatt";
			inherit_parent = true;

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
			char* section_id = const_cast<char*>("zones_ConfigHyperion");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "enabled", enabled);
			SerializerSystem.Exchange(ExchangeMode, section_id, "target_bots", target_bots);
			SerializerSystem.Exchange(ExchangeMode, section_id, "template_path", template_path);
			SerializerSystem.Exchange(ExchangeMode, section_id, "inherit_parent", inherit_parent);

		}
		
		ConfigHyperionData& GetDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual void DataUpdated() { }
		
	public: // read only operators
		bool         get_Enabled                       () const { return enabled; }
		bool         get_TargetBots                    () const { return target_bots; }
		Ascii        get_TemplatePath                  () const { return template_path; }
		bool         get_InheritParent                 () const { return inherit_parent; }

		virtual void set_Enabled                       (bool         Assgn) { enabled = Assgn; }
		virtual void set_TargetBots                    (bool         Assgn) { target_bots = Assgn; }
		virtual void set_TemplatePath                  (Ascii        Assgn) { template_path = Assgn; }
		virtual void set_InheritParent                 (bool         Assgn) { inherit_parent = Assgn; }

		
	public: // string conversion
		virtual Ascii param_read(Ascii ParamName)
		{
			if (ParamName == "enabled") {
				return Conversion::ToString(enabled);
			}
			if (ParamName == "target_bots") {
				return Conversion::ToString(target_bots);
			}
			if (ParamName == "template_path") {
				return Conversion::ToString(template_path);
			}
			if (ParamName == "inherit_parent") {
				return Conversion::ToString(inherit_parent);
			}

			return "";
		}
		
		virtual bool param_write(Ascii ParamName, Ascii Value)
		{
			if (ParamName == "enabled") { 
				enabled = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "target_bots") { 
				target_bots = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "template_path") { 
				template_path = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "inherit_parent") { 
				inherit_parent = Conversion::ToBool(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		Umbra::Variant Umbra_GetProperty(Umbra::string ID, Umbra::ES_GetProperty& ES)
		{
			if (strcmp(ID.c_str(), "enabled"                     ) == 0) return enabled;
			if (strcmp(ID.c_str(), "target_bots"                 ) == 0) return target_bots;
			if (strcmp(ID.c_str(), "template_path"               ) == 0) return (const char*)template_path;
			if (strcmp(ID.c_str(), "inherit_parent"              ) == 0) return inherit_parent;

			
			// nothing found
			throw Umbra::UnsupportedPropertyException(ID);
		}	
	};

	
	class ConfigHyperionPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CButton          *pEnabled       ;
		CButton          *pTargetBots    ;
		TextBox          *pTemplatePath  ;
		CButton          *pInheritParent ;

		
	public: // a set of the special property type
		ConfigHyperionData data;
		ConfigHyperionData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pEnabled        = PageCtrl.create_check     (GroupID, "", "Enabled");
			pTargetBots     = PageCtrl.create_check     (GroupID, "", "Target Bots");
			pTemplatePath   = PageCtrl.create_edit      (GroupID, "", "Template Path");
			pInheritParent  = PageCtrl.create_check     (GroupID, "", "Inherit Parent");

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
		
		void dialog_to_attributes(ConfigHyperionData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
		}
		
		void attributes_to_dialog(ConfigHyperionData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(ConfigHyperionData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.enabled, (*pEnabled       ));
			exr.exchange(Attr.target_bots, (*pTargetBots    ));
			exr.exchange(Attr.template_path, (*pTemplatePath  ));
			exr.exchange(Attr.inherit_parent, (*pInheritParent ));

			
			if (Mode == DataExchanger::FROM_CONTROL)
			{
				Attr.DataUpdated();
			}
			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(ConfigHyperionData& RemoteData)
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