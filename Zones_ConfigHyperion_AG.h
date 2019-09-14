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
	class ConfigHyperionPropertyBuilder;

	//
	// Import Data
	//
	// a:7:{i:0;a:4:{s:4:"name";s:7:"enabled";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:11:"target_bots";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:13:"template_path";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:14:"inherit_parent";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:20:"auto_refresh_seconds";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:5;a:4:{s:4:"name";s:21:"auto_refresh_distance";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:6;a:4:{s:4:"name";s:30:"auto_refresh_vertical_distance";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class ConfigHyperionData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef ConfigHyperionPropertyBuilder PropertyBuilder;
	
	public: // source data
		bool enabled;
		bool target_bots;
		String template_path;
		bool inherit_parent;
		int         auto_refresh_seconds;
		int         auto_refresh_distance;
		int         auto_refresh_vertical_distance;

		
	public:
		ConfigHyperionData()
		{
			enabled = false;
			target_bots = false;
			template_path = "{$_app.world_path}\\world_templates\\{$template_name}.awatt;{$_app.data_path}\\shared_world_templates\\{$template_name}.awatt";
			inherit_parent = true;
			auto_refresh_seconds = 0;
			auto_refresh_distance = 0;
			auto_refresh_vertical_distance = 0;

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
			char* section_id = const_cast<char*>("zones_ConfigHyperion");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "enabled", enabled);
			SerializerSystem.Exchange(ExchangeMode, section_id, "target_bots", target_bots);
			SerializerSystem.Exchange(ExchangeMode, section_id, "template_path", template_path);
			SerializerSystem.Exchange(ExchangeMode, section_id, "inherit_parent", inherit_parent);
			SerializerSystem.Exchange(ExchangeMode, section_id, "auto_refresh_seconds", auto_refresh_seconds);
			SerializerSystem.Exchange(ExchangeMode, section_id, "auto_refresh_distance", auto_refresh_distance);
			SerializerSystem.Exchange(ExchangeMode, section_id, "auto_refresh_vertical_distance", auto_refresh_vertical_distance);

		}
		
		ConfigHyperionData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		bool  getEnabled                       () const { return enabled; }
		bool  getTargetBots                    () const { return target_bots; }
		String  getTemplatePath                  () const { return template_path; }
		bool  getInheritParent                 () const { return inherit_parent; }
		int          getAutoRefreshSeconds            () const { return auto_refresh_seconds; }
		int          getAutoRefreshDistance           () const { return auto_refresh_distance; }
		int          getAutoRefreshVerticalDistance   () const { return auto_refresh_vertical_distance; }

		virtual void setEnabled                       (bool  Assgn) { enabled = Assgn; }
		virtual void setTargetBots                    (bool  Assgn) { target_bots = Assgn; }
		virtual void setTemplatePath                  (String  Assgn) { template_path = Assgn; }
		virtual void setInheritParent                 (bool  Assgn) { inherit_parent = Assgn; }
		virtual void setAutoRefreshSeconds            (int          Assgn) { auto_refresh_seconds = Assgn; }
		virtual void setAutoRefreshDistance           (int          Assgn) { auto_refresh_distance = Assgn; }
		virtual void setAutoRefreshVerticalDistance   (int          Assgn) { auto_refresh_vertical_distance = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
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
			if (ParamName == "auto_refresh_seconds") {
				return Conversion::ToString(auto_refresh_seconds);
			}
			if (ParamName == "auto_refresh_distance") {
				return Conversion::ToString(auto_refresh_distance);
			}
			if (ParamName == "auto_refresh_vertical_distance") {
				return Conversion::ToString(auto_refresh_vertical_distance);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
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
			if (ParamName == "auto_refresh_seconds") { 
				auto_refresh_seconds = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "auto_refresh_distance") { 
				auto_refresh_distance = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "auto_refresh_vertical_distance") { 
				auto_refresh_vertical_distance = Conversion::ToInt(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(enabled, enabled);
			UMBRA_PROPERTY_RW(target_bots, target_bots);
			UMBRA_PROPERTY_RW(template_path, template_path);
			UMBRA_PROPERTY_RW(inherit_parent, inherit_parent);
			UMBRA_PROPERTY_RW(auto_refresh_seconds, auto_refresh_seconds);
			UMBRA_PROPERTY_RW(auto_refresh_distance, auto_refresh_distance);
			UMBRA_PROPERTY_RW(auto_refresh_vertical_distance, auto_refresh_vertical_distance);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class ConfigHyperionPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CButton          *pEnabled       ;
		CButton          *pTargetBots    ;
		TextBox          *pTemplatePath  ;
		CButton          *pInheritParent ;
		CStatic			 *pAutoChanges   ;
		TextBox          *pAutoRefreshSeconds;
		TextBox          *pAutoRefreshDistance;
		TextBox          *pAutoRefreshVerticalDistance;

		
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
			pAutoChanges    = PageCtrl.create_label     (GroupID, "", "Automatic Refreshing");
			pAutoRefreshSeconds = PageCtrl.create_edit      (GroupID, "", "Auto Refresh Seconds");
			pAutoRefreshDistance = PageCtrl.create_edit      (GroupID, "", "Auto Refresh Distance");
			pAutoRefreshVerticalDistance = PageCtrl.create_edit      (GroupID, "", "Auto Refresh Vertical Distance");

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
		
		bool dialog_to_attributes(ConfigHyperionData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
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
			exr.exchange(Attr.auto_refresh_seconds, (*pAutoRefreshSeconds));
			exr.exchange(Attr.auto_refresh_distance, (*pAutoRefreshDistance));
			exr.exchange(Attr.auto_refresh_vertical_distance, (*pAutoRefreshVerticalDistance));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(ConfigHyperionData& RemoteData)
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