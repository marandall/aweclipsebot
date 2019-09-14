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
	class StringFinderPropertyBuilder;

	//
	// Import Data
	//
	// a:5:{i:0;a:4:{s:4:"name";s:6:"string";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:8:"in_model";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:9:"in_action";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:14:"in_description";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:14:"case_sensitive";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class StringFinderData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef StringFinderPropertyBuilder PropertyBuilder;
	
	public: // source data
		String string;
		bool in_model;
		bool in_action;
		bool in_description;
		bool case_sensitive;

		
	public:
		StringFinderData()
		{
			string = "";
			in_model = false;
			in_action = false;
			in_description = false;
			case_sensitive = false;

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
			char* section_id = const_cast<char*>("propfilterStringFinder");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "string", string);
			SerializerSystem.Exchange(ExchangeMode, section_id, "in_model", in_model);
			SerializerSystem.Exchange(ExchangeMode, section_id, "in_action", in_action);
			SerializerSystem.Exchange(ExchangeMode, section_id, "in_description", in_description);
			SerializerSystem.Exchange(ExchangeMode, section_id, "case_sensitive", case_sensitive);

		}
		
		StringFinderData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		String  getString                        () const { return string; }
		bool  getInModel                       () const { return in_model; }
		bool  getInAction                      () const { return in_action; }
		bool  getInDescription                 () const { return in_description; }
		bool  getCaseSensitive                 () const { return case_sensitive; }

		virtual void setString                        (String  Assgn) { string = Assgn; }
		virtual void setInModel                       (bool  Assgn) { in_model = Assgn; }
		virtual void setInAction                      (bool  Assgn) { in_action = Assgn; }
		virtual void setInDescription                 (bool  Assgn) { in_description = Assgn; }
		virtual void setCaseSensitive                 (bool  Assgn) { case_sensitive = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "string") {
				return Conversion::ToString(string);
			}
			if (ParamName == "in_model") {
				return Conversion::ToString(in_model);
			}
			if (ParamName == "in_action") {
				return Conversion::ToString(in_action);
			}
			if (ParamName == "in_description") {
				return Conversion::ToString(in_description);
			}
			if (ParamName == "case_sensitive") {
				return Conversion::ToString(case_sensitive);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "string") { 
				string = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "in_model") { 
				in_model = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "in_action") { 
				in_action = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "in_description") { 
				in_description = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "case_sensitive") { 
				case_sensitive = Conversion::ToBool(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(string, string);
			UMBRA_PROPERTY_RW(in_model, in_model);
			UMBRA_PROPERTY_RW(in_action, in_action);
			UMBRA_PROPERTY_RW(in_description, in_description);
			UMBRA_PROPERTY_RW(case_sensitive, case_sensitive);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class StringFinderPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pString        ;
		CButton          *pInModel       ;
		CButton          *pInAction      ;
		CButton          *pInDescription ;
		CButton          *pCaseSensitive ;

		
	public: // a set of the special property type
		StringFinderData data;
		StringFinderData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pString         = PageCtrl.create_edit      (GroupID, "", "String");
			pInModel        = PageCtrl.create_check     (GroupID, "", "In Model");
			pInAction       = PageCtrl.create_check     (GroupID, "", "In Action");
			pInDescription  = PageCtrl.create_check     (GroupID, "", "In Description");
			pCaseSensitive  = PageCtrl.create_check     (GroupID, "", "Case Sensitive");

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
		
		bool dialog_to_attributes(StringFinderData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(StringFinderData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(StringFinderData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.string, (*pString        ));
			exr.exchange(Attr.in_model, (*pInModel       ));
			exr.exchange(Attr.in_action, (*pInAction      ));
			exr.exchange(Attr.in_description, (*pInDescription ));
			exr.exchange(Attr.case_sensitive, (*pCaseSensitive ));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(StringFinderData& RemoteData)
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