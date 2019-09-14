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

namespace Property { namespace Filters { 
namespace AutoTemplates 
{
	class SelectionRectPropertyBuilder;

	//
	// Import Data
	//
	// a:2:{i:0;a:4:{s:4:"name";s:3:"top";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:6:"bottom";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class SelectionRectData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef SelectionRectPropertyBuilder PropertyBuilder;
	
	public: // source data
		String top;
		String bottom;

		
	public:
		SelectionRectData()
		{
			top = "";
			bottom = "";

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
			char* section_id = const_cast<char*>("propfilterSelectionRect");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "top", top);
			SerializerSystem.Exchange(ExchangeMode, section_id, "bottom", bottom);

		}
		
		SelectionRectData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		String  getTop                           () const { return top; }
		String  getBottom                        () const { return bottom; }

		virtual void setTop                           (String  Assgn) { top = Assgn; }
		virtual void setBottom                        (String  Assgn) { bottom = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "top") {
				return Conversion::ToString(top);
			}
			if (ParamName == "bottom") {
				return Conversion::ToString(bottom);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "top") { 
				top = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "bottom") { 
				bottom = Conversion::ToString(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(top, top);
			UMBRA_PROPERTY_RW(bottom, bottom);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class SelectionRectPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pTop           ;
		TextBox          *pBottom        ;

		
	public: // a set of the special property type
		SelectionRectData data;
		SelectionRectData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pTop            = PageCtrl.create_edit      (GroupID, "", "Top");
			pBottom         = PageCtrl.create_edit      (GroupID, "", "Bottom");

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
		
		bool dialog_to_attributes(SelectionRectData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(SelectionRectData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(SelectionRectData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.top, (*pTop           ));
			exr.exchange(Attr.bottom, (*pBottom        ));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(SelectionRectData& RemoteData)
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
} } 