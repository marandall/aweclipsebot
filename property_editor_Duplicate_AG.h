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

namespace Property { namespace Editors { 
namespace AutoTemplates 
{
	class DuplicatePropertyBuilder;

	//
	// Import Data
	//
	// a:5:{i:0;a:4:{s:4:"name";s:11:"transform_a";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:8:"repeat_a";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:11:"transform_b";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:8:"repeat_b";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:16:"delete_origional";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class DuplicateData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef DuplicatePropertyBuilder PropertyBuilder;
	
	public: // source data
		String transform_a;
		int         repeat_a;
		String transform_b;
		int         repeat_b;
		bool delete_origional;

		
	public:
		DuplicateData()
		{
			transform_a = "";
			repeat_a = 1;
			transform_b = "";
			repeat_b = 1;
			delete_origional = true;

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
			char* section_id = const_cast<char*>("propeditorDuplicate");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "transform_a", transform_a);
			SerializerSystem.Exchange(ExchangeMode, section_id, "repeat_a", repeat_a);
			SerializerSystem.Exchange(ExchangeMode, section_id, "transform_b", transform_b);
			SerializerSystem.Exchange(ExchangeMode, section_id, "repeat_b", repeat_b);
			SerializerSystem.Exchange(ExchangeMode, section_id, "delete_origional", delete_origional);

		}
		
		DuplicateData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		String  getTransformA                    () const { return transform_a; }
		int          getRepeatA                       () const { return repeat_a; }
		String  getTransformB                    () const { return transform_b; }
		int          getRepeatB                       () const { return repeat_b; }
		bool  getDeleteOrigional               () const { return delete_origional; }

		virtual void setTransformA                    (String  Assgn) { transform_a = Assgn; }
		virtual void setRepeatA                       (int          Assgn) { repeat_a = Assgn; }
		virtual void setTransformB                    (String  Assgn) { transform_b = Assgn; }
		virtual void setRepeatB                       (int          Assgn) { repeat_b = Assgn; }
		virtual void setDeleteOrigional               (bool  Assgn) { delete_origional = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "transform_a") {
				return Conversion::ToString(transform_a);
			}
			if (ParamName == "repeat_a") {
				return Conversion::ToString(repeat_a);
			}
			if (ParamName == "transform_b") {
				return Conversion::ToString(transform_b);
			}
			if (ParamName == "repeat_b") {
				return Conversion::ToString(repeat_b);
			}
			if (ParamName == "delete_origional") {
				return Conversion::ToString(delete_origional);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "transform_a") { 
				transform_a = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "repeat_a") { 
				repeat_a = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "transform_b") { 
				transform_b = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "repeat_b") { 
				repeat_b = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "delete_origional") { 
				delete_origional = Conversion::ToBool(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(transform_a, transform_a);
			UMBRA_PROPERTY_RW(repeat_a, repeat_a);
			UMBRA_PROPERTY_RW(transform_b, transform_b);
			UMBRA_PROPERTY_RW(repeat_b, repeat_b);
			UMBRA_PROPERTY_RW(delete_origional, delete_origional);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class DuplicatePropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pTransformA    ;
		TextBox          *pRepeatA       ;
		TextBox          *pTransformB    ;
		TextBox          *pRepeatB       ;
		CButton          *pDeleteOrigional;

		
	public: // a set of the special property type
		DuplicateData data;
		DuplicateData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pTransformA     = PageCtrl.create_edit      (GroupID, "", "Transform A");
			pRepeatA        = PageCtrl.create_edit      (GroupID, "", "Repeat A");
			pTransformB     = PageCtrl.create_edit      (GroupID, "", "Transform B");
			pRepeatB        = PageCtrl.create_edit      (GroupID, "", "Repeat B");
			pDeleteOrigional = PageCtrl.create_check     (GroupID, "", "Delete Origional");

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
		
		bool dialog_to_attributes(DuplicateData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(DuplicateData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(DuplicateData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.transform_a, (*pTransformA    ));
			exr.exchange(Attr.repeat_a, (*pRepeatA       ));
			exr.exchange(Attr.transform_b, (*pTransformB    ));
			exr.exchange(Attr.repeat_b, (*pRepeatB       ));
			exr.exchange(Attr.delete_origional, (*pDeleteOrigional));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(DuplicateData& RemoteData)
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