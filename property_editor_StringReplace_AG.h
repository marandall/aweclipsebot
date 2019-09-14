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
	class StringReplacePropertyBuilder;

	//
	// Import Data
	//
	// a:5:{i:0;a:4:{s:4:"name";s:4:"find";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:11:"replacement";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:12:"search_model";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:18:"search_description";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:13:"search_action";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class StringReplaceData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef StringReplacePropertyBuilder PropertyBuilder;
	
	public: // source data
		String find;
		String replacement;
		bool search_model;
		bool search_description;
		bool search_action;

		
	public:
		StringReplaceData()
		{
			find = "";
			replacement = "";
			search_model = false;
			search_description = false;
			search_action = false;

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
			char* section_id = const_cast<char*>("propeditorStringReplace");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "find", find);
			SerializerSystem.Exchange(ExchangeMode, section_id, "replacement", replacement);
			SerializerSystem.Exchange(ExchangeMode, section_id, "search_model", search_model);
			SerializerSystem.Exchange(ExchangeMode, section_id, "search_description", search_description);
			SerializerSystem.Exchange(ExchangeMode, section_id, "search_action", search_action);

		}
		
		StringReplaceData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		String  getFind                          () const { return find; }
		String  getReplacement                   () const { return replacement; }
		bool  getSearchModel                   () const { return search_model; }
		bool  getSearchDescription             () const { return search_description; }
		bool  getSearchAction                  () const { return search_action; }

		virtual void setFind                          (String  Assgn) { find = Assgn; }
		virtual void setReplacement                   (String  Assgn) { replacement = Assgn; }
		virtual void setSearchModel                   (bool  Assgn) { search_model = Assgn; }
		virtual void setSearchDescription             (bool  Assgn) { search_description = Assgn; }
		virtual void setSearchAction                  (bool  Assgn) { search_action = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "find") {
				return Conversion::ToString(find);
			}
			if (ParamName == "replacement") {
				return Conversion::ToString(replacement);
			}
			if (ParamName == "search_model") {
				return Conversion::ToString(search_model);
			}
			if (ParamName == "search_description") {
				return Conversion::ToString(search_description);
			}
			if (ParamName == "search_action") {
				return Conversion::ToString(search_action);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "find") { 
				find = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "replacement") { 
				replacement = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "search_model") { 
				search_model = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "search_description") { 
				search_description = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "search_action") { 
				search_action = Conversion::ToBool(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(find, find);
			UMBRA_PROPERTY_RW(replacement, replacement);
			UMBRA_PROPERTY_RW(search_model, search_model);
			UMBRA_PROPERTY_RW(search_description, search_description);
			UMBRA_PROPERTY_RW(search_action, search_action);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class StringReplacePropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pFind          ;
		TextBox          *pReplacement   ;
		CButton          *pSearchModel   ;
		CButton          *pSearchDescription;
		CButton          *pSearchAction  ;

		
	public: // a set of the special property type
		StringReplaceData data;
		StringReplaceData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pFind           = PageCtrl.create_edit      (GroupID, "", "Find");
			pReplacement    = PageCtrl.create_edit      (GroupID, "", "Replacement");
			pSearchModel    = PageCtrl.create_check     (GroupID, "", "Search Model");
			pSearchDescription = PageCtrl.create_check     (GroupID, "", "Search Description");
			pSearchAction   = PageCtrl.create_check     (GroupID, "", "Search Action");

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
		
		bool dialog_to_attributes(StringReplaceData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(StringReplaceData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(StringReplaceData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.find, (*pFind          ));
			exr.exchange(Attr.replacement, (*pReplacement   ));
			exr.exchange(Attr.search_model, (*pSearchModel   ));
			exr.exchange(Attr.search_description, (*pSearchDescription));
			exr.exchange(Attr.search_action, (*pSearchAction  ));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(StringReplaceData& RemoteData)
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