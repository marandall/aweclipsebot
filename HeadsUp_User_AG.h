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
	class UserPropertyBuilder;

	//
	// Import Data
	//
	// a:3:{i:0;a:4:{s:4:"name";s:7:"visible";s:4:"type";s:5:"tsb_t";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:8:"expanded";s:4:"type";s:5:"tsb_t";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:3:"row";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class UserData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef UserPropertyBuilder PropertyBuilder;
	
	public: // source data
		tsb_t       visible;
		tsb_t       expanded;
		String row;

		
	public:
		UserData()
		{
			visible = B_EMPTY;
			expanded = B_EMPTY;
			row = "";

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
			char* section_id = const_cast<char*>("hud_User");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "visible", visible);
			SerializerSystem.Exchange(ExchangeMode, section_id, "expanded", expanded);
			SerializerSystem.Exchange(ExchangeMode, section_id, "row", row);

		}
		
		UserData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		tsb_t        getVisible                       () const { return visible; }
		tsb_t        getExpanded                      () const { return expanded; }
		String  getRow                           () const { return row; }

		virtual void setVisible                       (tsb_t        Assgn) { visible = Assgn; }
		virtual void setExpanded                      (tsb_t        Assgn) { expanded = Assgn; }
		virtual void setRow                           (String  Assgn) { row = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "visible") {
				return Conversion::ToStringTSB(visible);
			}
			if (ParamName == "expanded") {
				return Conversion::ToStringTSB(expanded);
			}
			if (ParamName == "row") {
				return Conversion::ToString(row);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "visible") { 
				visible = Conversion::ToTSB(Value);
				return true;
			}
			if (ParamName == "expanded") { 
				expanded = Conversion::ToTSB(Value);
				return true;
			}
			if (ParamName == "row") { 
				row = Conversion::ToString(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(visible, visible);
			UMBRA_PROPERTY_RW(expanded, expanded);
			UMBRA_PROPERTY_RW(row, row);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class UserPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CButton          *pVisible       ;
		CButton          *pExpanded      ;
		TextBox          *pRow           ;

		
	public: // a set of the special property type
		UserData data;
		UserData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pVisible        = PageCtrl.create_tristate  (GroupID, "", "Visible");
			pExpanded       = PageCtrl.create_tristate  (GroupID, "", "Expanded");
			pRow            = PageCtrl.create_edit      (GroupID, "", "Row");

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
		
		bool dialog_to_attributes(UserData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(UserData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(UserData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.visible, (*pVisible       ));
			exr.exchange(Attr.expanded, (*pExpanded      ));
			exr.exchange(Attr.row, (*pRow           ));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(UserData& RemoteData)
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