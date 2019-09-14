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

namespace HeadsUp { 
namespace AutoTemplates 
{
	class ConfigurationPropertyBuilder;

	//
	// Import Data
	//
	// a:8:{i:0;a:4:{s:4:"name";s:7:"enabled";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:8:"position";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:14:"x_edge_padding";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:14:"y_edge_padding";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:7:"spacing";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:5;a:4:{s:4:"name";s:7:"opacity";s:4:"type";s:5:"float";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:6;a:4:{s:4:"name";s:18:"default_visibility";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:7;a:4:{s:4:"name";s:17:"default_expansion";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class ConfigurationData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef ConfigurationPropertyBuilder PropertyBuilder;
	
	public: // source data
		bool enabled;
		int         position;
		int         x_edge_padding;
		int         y_edge_padding;
		int         spacing;
		float       opacity;
		bool default_visibility;
		bool default_expansion;

		
	public:
		ConfigurationData()
		{
			enabled = false;
			position = 0;
			x_edge_padding = 5;
			y_edge_padding = 5;
			spacing = 5;
			opacity = 0.75;
			default_visibility = false;
			default_expansion = true;

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
			char* section_id = const_cast<char*>("hud_Configuration");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "enabled", enabled);
			SerializerSystem.Exchange(ExchangeMode, section_id, "position", position);
			SerializerSystem.Exchange(ExchangeMode, section_id, "x_edge_padding", x_edge_padding);
			SerializerSystem.Exchange(ExchangeMode, section_id, "y_edge_padding", y_edge_padding);
			SerializerSystem.Exchange(ExchangeMode, section_id, "spacing", spacing);
			SerializerSystem.Exchange(ExchangeMode, section_id, "opacity", opacity);
			SerializerSystem.Exchange(ExchangeMode, section_id, "default_visibility", default_visibility);
			SerializerSystem.Exchange(ExchangeMode, section_id, "default_expansion", default_expansion);

		}
		
		ConfigurationData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		bool  getEnabled                       () const { return enabled; }
		int          getPosition                      () const { return position; }
		int          getXEdgePadding                  () const { return x_edge_padding; }
		int          getYEdgePadding                  () const { return y_edge_padding; }
		int          getSpacing                       () const { return spacing; }
		float        getOpacity                       () const { return opacity; }
		bool  getDefaultVisibility             () const { return default_visibility; }
		bool  getDefaultExpansion              () const { return default_expansion; }

		virtual void setEnabled                       (bool  Assgn) { enabled = Assgn; }
		virtual void setPosition                      (int          Assgn) { position = Assgn; }
		virtual void setXEdgePadding                  (int          Assgn) { x_edge_padding = Assgn; }
		virtual void setYEdgePadding                  (int          Assgn) { y_edge_padding = Assgn; }
		virtual void setSpacing                       (int          Assgn) { spacing = Assgn; }
		virtual void setOpacity                       (float        Assgn) { opacity = Assgn; }
		virtual void setDefaultVisibility             (bool  Assgn) { default_visibility = Assgn; }
		virtual void setDefaultExpansion              (bool  Assgn) { default_expansion = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "enabled") {
				return Conversion::ToString(enabled);
			}
			if (ParamName == "position") {
				return Conversion::ToString(position);
			}
			if (ParamName == "x_edge_padding") {
				return Conversion::ToString(x_edge_padding);
			}
			if (ParamName == "y_edge_padding") {
				return Conversion::ToString(y_edge_padding);
			}
			if (ParamName == "spacing") {
				return Conversion::ToString(spacing);
			}
			if (ParamName == "opacity") {
				return Conversion::ToString(opacity);
			}
			if (ParamName == "default_visibility") {
				return Conversion::ToString(default_visibility);
			}
			if (ParamName == "default_expansion") {
				return Conversion::ToString(default_expansion);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "enabled") { 
				enabled = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "position") { 
				position = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "x_edge_padding") { 
				x_edge_padding = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "y_edge_padding") { 
				y_edge_padding = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "spacing") { 
				spacing = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "opacity") { 
				opacity = Conversion::ToFloat(Value);
				return true;
			}
			if (ParamName == "default_visibility") { 
				default_visibility = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "default_expansion") { 
				default_expansion = Conversion::ToBool(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(enabled, enabled);
			UMBRA_PROPERTY_RW(position, position);
			UMBRA_PROPERTY_RW(x_edge_padding, x_edge_padding);
			UMBRA_PROPERTY_RW(y_edge_padding, y_edge_padding);
			UMBRA_PROPERTY_RW(spacing, spacing);
			UMBRA_PROPERTY_RW(opacity, opacity);
			UMBRA_PROPERTY_RW(default_visibility, default_visibility);
			UMBRA_PROPERTY_RW(default_expansion, default_expansion);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class ConfigurationPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CButton          *pEnabled       ;
		ComboBox         *pPosition      ;
		TextBox          *pXEdgePadding  ;
		TextBox          *pYEdgePadding  ;
		TextBox          *pSpacing       ;
		TextBox          *pOpacity       ;
		CButton          *pDefaultVisibility;
		CButton          *pDefaultExpansion;

		
	public: // a set of the special property type
		ConfigurationData data;
		ConfigurationData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pEnabled        = PageCtrl.create_check     (GroupID, "", "Enabled");
			pPosition       = PageCtrl.create_combo     (GroupID, "", "Position");
			pXEdgePadding   = PageCtrl.create_edit      (GroupID, "", "X Edge Padding");
			pYEdgePadding   = PageCtrl.create_edit      (GroupID, "", "Y Edge Padding");
			pSpacing        = PageCtrl.create_edit      (GroupID, "", "Spacing");
			pOpacity        = PageCtrl.create_edit      (GroupID, "", "Opacity");
			pDefaultVisibility = PageCtrl.create_check     (GroupID, "", "Default Visibility");
			pDefaultExpansion = PageCtrl.create_check     (GroupID, "", "Default Expansion");

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
		
		bool dialog_to_attributes(ConfigurationData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(ConfigurationData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(ConfigurationData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.enabled, (*pEnabled       ));
			exr.exchange(Attr.position, (*pPosition      ));
			exr.exchange(Attr.x_edge_padding, (*pXEdgePadding  ));
			exr.exchange(Attr.y_edge_padding, (*pYEdgePadding  ));
			exr.exchange(Attr.spacing, (*pSpacing       ));
			exr.exchange(Attr.opacity, (*pOpacity       ));
			exr.exchange(Attr.default_visibility, (*pDefaultVisibility));
			exr.exchange(Attr.default_expansion, (*pDefaultExpansion));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(ConfigurationData& RemoteData)
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