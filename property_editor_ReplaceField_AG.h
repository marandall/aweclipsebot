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
	class ReplaceFieldPropertyBuilder;

	//
	// Import Data
	//
	// a:7:{i:0;a:4:{s:4:"name";s:4:"text";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:9:"new_model";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:15:"new_description";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:10:"new_action";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:8:"do_model";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:5;a:4:{s:4:"name";s:14:"do_description";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:6;a:4:{s:4:"name";s:9:"do_action";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class ReplaceFieldData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef ReplaceFieldPropertyBuilder PropertyBuilder;
	
	public: // source data
		String text;
		String new_model;
		String new_description;
		String new_action;
		bool do_model;
		bool do_description;
		bool do_action;

		
	public:
		ReplaceFieldData()
		{
			text = "";
			new_model = "";
			new_description = "";
			new_action = "";
			do_model = false;
			do_description = false;
			do_action = false;

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
			char* section_id = const_cast<char*>("propeditorReplaceField");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "text", text);
			SerializerSystem.Exchange(ExchangeMode, section_id, "new_model", new_model);
			SerializerSystem.Exchange(ExchangeMode, section_id, "new_description", new_description);
			SerializerSystem.Exchange(ExchangeMode, section_id, "new_action", new_action);
			SerializerSystem.Exchange(ExchangeMode, section_id, "do_model", do_model);
			SerializerSystem.Exchange(ExchangeMode, section_id, "do_description", do_description);
			SerializerSystem.Exchange(ExchangeMode, section_id, "do_action", do_action);

		}
		
		ReplaceFieldData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		String  getText                          () const { return text; }
		String  getNewModel                      () const { return new_model; }
		String  getNewDescription                () const { return new_description; }
		String  getNewAction                     () const { return new_action; }
		bool  getDoModel                       () const { return do_model; }
		bool  getDoDescription                 () const { return do_description; }
		bool  getDoAction                      () const { return do_action; }

		virtual void setText                          (String  Assgn) { text = Assgn; }
		virtual void setNewModel                      (String  Assgn) { new_model = Assgn; }
		virtual void setNewDescription                (String  Assgn) { new_description = Assgn; }
		virtual void setNewAction                     (String  Assgn) { new_action = Assgn; }
		virtual void setDoModel                       (bool  Assgn) { do_model = Assgn; }
		virtual void setDoDescription                 (bool  Assgn) { do_description = Assgn; }
		virtual void setDoAction                      (bool  Assgn) { do_action = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "text") {
				return Conversion::ToString(text);
			}
			if (ParamName == "new_model") {
				return Conversion::ToString(new_model);
			}
			if (ParamName == "new_description") {
				return Conversion::ToString(new_description);
			}
			if (ParamName == "new_action") {
				return Conversion::ToString(new_action);
			}
			if (ParamName == "do_model") {
				return Conversion::ToString(do_model);
			}
			if (ParamName == "do_description") {
				return Conversion::ToString(do_description);
			}
			if (ParamName == "do_action") {
				return Conversion::ToString(do_action);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "text") { 
				text = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "new_model") { 
				new_model = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "new_description") { 
				new_description = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "new_action") { 
				new_action = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "do_model") { 
				do_model = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "do_description") { 
				do_description = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "do_action") { 
				do_action = Conversion::ToBool(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(text, text);
			UMBRA_PROPERTY_RW(new_model, new_model);
			UMBRA_PROPERTY_RW(new_description, new_description);
			UMBRA_PROPERTY_RW(new_action, new_action);
			UMBRA_PROPERTY_RW(do_model, do_model);
			UMBRA_PROPERTY_RW(do_description, do_description);
			UMBRA_PROPERTY_RW(do_action, do_action);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class ReplaceFieldPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pText          ;
		TextBox          *pNewModel      ;
		TextBox          *pNewDescription;
		TextBox          *pNewAction     ;
		CButton          *pDoModel       ;
		CButton          *pDoDescription ;
		CButton          *pDoAction      ;

		
	public: // a set of the special property type
		ReplaceFieldData data;
		ReplaceFieldData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pText           = PageCtrl.create_edit      (GroupID, "", "Text");
			pNewModel       = PageCtrl.create_edit      (GroupID, "", "New Model");
			pNewDescription = PageCtrl.create_edit      (GroupID, "", "New Description");
			pNewAction      = PageCtrl.create_edit      (GroupID, "", "New Action");
			pDoModel        = PageCtrl.create_check     (GroupID, "", "Do Model");
			pDoDescription  = PageCtrl.create_check     (GroupID, "", "Do Description");
			pDoAction       = PageCtrl.create_check     (GroupID, "", "Do Action");

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
		
		bool dialog_to_attributes(ReplaceFieldData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(ReplaceFieldData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(ReplaceFieldData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.text, (*pText          ));
			exr.exchange(Attr.new_model, (*pNewModel      ));
			exr.exchange(Attr.new_description, (*pNewDescription));
			exr.exchange(Attr.new_action, (*pNewAction     ));
			exr.exchange(Attr.do_model, (*pDoModel       ));
			exr.exchange(Attr.do_description, (*pDoDescription ));
			exr.exchange(Attr.do_action, (*pDoAction      ));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(ReplaceFieldData& RemoteData)
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