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

namespace CIS { namespace Config { 
namespace AutoTemplates 
{
	class JumpPointsPropertyBuilder;

	//
	// Import Data
	//
	// a:3:{i:0;a:4:{s:4:"name";s:12:"max_per_user";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:20:"seperate_by_at_least";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:19:"search_result_limit";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class JumpPointsData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef JumpPointsPropertyBuilder PropertyBuilder;
	
	public: // source data
		int         max_per_user;
		int         seperate_by_at_least;
		int         search_result_limit;

		
	public:
		JumpPointsData()
		{
			max_per_user = 50;
			seperate_by_at_least = 0;
			search_result_limit = 5;

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
			char* section_id = const_cast<char*>("cis_JumpPoints");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "max_per_user", max_per_user);
			SerializerSystem.Exchange(ExchangeMode, section_id, "seperate_by_at_least", seperate_by_at_least);
			SerializerSystem.Exchange(ExchangeMode, section_id, "search_result_limit", search_result_limit);

		}
		
		JumpPointsData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		int          getMaxPerUser                    () const { return max_per_user; }
		int          getSeperateByAtLeast             () const { return seperate_by_at_least; }
		int          getSearchResultLimit             () const { return search_result_limit; }

		virtual void setMaxPerUser                    (int          Assgn) { max_per_user = Assgn; }
		virtual void setSeperateByAtLeast             (int          Assgn) { seperate_by_at_least = Assgn; }
		virtual void setSearchResultLimit             (int          Assgn) { search_result_limit = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "max_per_user") {
				return Conversion::ToString(max_per_user);
			}
			if (ParamName == "seperate_by_at_least") {
				return Conversion::ToString(seperate_by_at_least);
			}
			if (ParamName == "search_result_limit") {
				return Conversion::ToString(search_result_limit);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "max_per_user") { 
				max_per_user = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "seperate_by_at_least") { 
				seperate_by_at_least = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "search_result_limit") { 
				search_result_limit = Conversion::ToInt(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(max_per_user, max_per_user);
			UMBRA_PROPERTY_RW(seperate_by_at_least, seperate_by_at_least);
			UMBRA_PROPERTY_RW(search_result_limit, search_result_limit);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class JumpPointsPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pMaxPerUser    ;
		TextBox          *pSeperateByAtLeast;
		TextBox          *pSearchResultLimit;

		
	public: // a set of the special property type
		JumpPointsData data;
		JumpPointsData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pMaxPerUser     = PageCtrl.create_edit      (GroupID, "", "Max Per User");
			pSeperateByAtLeast = PageCtrl.create_edit      (GroupID, "", "Seperate By At Least");
			pSearchResultLimit = PageCtrl.create_edit      (GroupID, "", "Search Result Limit");

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
		
		bool dialog_to_attributes(JumpPointsData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(JumpPointsData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(JumpPointsData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.max_per_user, (*pMaxPerUser    ));
			exr.exchange(Attr.seperate_by_at_least, (*pSeperateByAtLeast));
			exr.exchange(Attr.search_result_limit, (*pSearchResultLimit));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(JumpPointsData& RemoteData)
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