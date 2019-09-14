#pragma once
#include "serializer.h"
#include "tsb.h"
#include "umbra_remote_class.h"
#include "raw_param_editor.h"
#include "PropertyScriptCtrl.h"
#include "GlobalDex.h"

namespace CIS { namespace Config { 
namespace AutoTemplates 
{
	class JumpPointsPropertyBuilder;

	class JumpPointsData : 
		public Umbra::RemoteClass,
		public IRawParamEditor
	{
	public: // type defenition for property builder
		typedef JumpPointsPropertyBuilder PropertyBuilder;
	
	public: // source data
		int         max_per_user;
		int         seperate_by_at_least;
		int         near_shows_closest;
		bool        allow_delete_all;

		
	public:
		JumpPointsData()
		{
			max_per_user = 50;
			seperate_by_at_least = 0;
			near_shows_closest = 5;
			allow_delete_all = false;

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
			char* section_id = const_cast<char*>("cis_JumpPoints");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "max_per_user", max_per_user);
			SerializerSystem.Exchange(ExchangeMode, section_id, "seperate_by_at_least", seperate_by_at_least);
			SerializerSystem.Exchange(ExchangeMode, section_id, "near_shows_closest", near_shows_closest);
			SerializerSystem.Exchange(ExchangeMode, section_id, "allow_delete_all", allow_delete_all);

		}
		
		JumpPointsData& GetDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual void DataUpdated() { }
		
	public: // read only operators
		int          get_MaxPerUser                    () const { return max_per_user; }
		int          get_SeperateByAtLeast             () const { return seperate_by_at_least; }
		int          get_NearShowsClosest              () const { return near_shows_closest; }
		bool         get_AllowDeleteAll                () const { return allow_delete_all; }

		virtual void set_MaxPerUser                    (int          Assgn) { max_per_user = Assgn; }
		virtual void set_SeperateByAtLeast             (int          Assgn) { seperate_by_at_least = Assgn; }
		virtual void set_NearShowsClosest              (int          Assgn) { near_shows_closest = Assgn; }
		virtual void set_AllowDeleteAll                (bool         Assgn) { allow_delete_all = Assgn; }

		
	public: // string conversion
		virtual Ascii param_read(Ascii ParamName)
		{
			if (ParamName == "max_per_user") {
				return Conversion::ToString(max_per_user);
			}
			if (ParamName == "seperate_by_at_least") {
				return Conversion::ToString(seperate_by_at_least);
			}
			if (ParamName == "near_shows_closest") {
				return Conversion::ToString(near_shows_closest);
			}
			if (ParamName == "allow_delete_all") {
				return Conversion::ToString(allow_delete_all);
			}

			return "";
		}
		
		virtual bool param_write(Ascii ParamName, Ascii Value)
		{
			if (ParamName == "max_per_user") { 
				max_per_user = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "seperate_by_at_least") { 
				seperate_by_at_least = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "near_shows_closest") { 
				near_shows_closest = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "allow_delete_all") { 
				allow_delete_all = Conversion::ToBool(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		Umbra::Variant Umbra_GetProperty(Umbra::string ID, Umbra::ES_GetProperty& ES)
		{
			if (strcmp(ID.c_str(), "max_per_user"                ) == 0) return max_per_user;
			if (strcmp(ID.c_str(), "seperate_by_at_least"        ) == 0) return seperate_by_at_least;
			if (strcmp(ID.c_str(), "near_shows_closest"          ) == 0) return near_shows_closest;
			if (strcmp(ID.c_str(), "allow_delete_all"            ) == 0) return allow_delete_all;

			
			// nothing found
			throw Umbra::UnsupportedPropertyException(ID);
		}	
	};

	
	class JumpPointsPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pMaxPerUser    ;
		TextBox          *pSeperateByAtLeast;
		TextBox          *pNearShowsClosest;
		CButton          *pAllowDeleteAll;

		
	public: // a set of the special property type
		JumpPointsData data;
		JumpPointsData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pMaxPerUser     = PageCtrl.create_edit      (GroupID, "", "Max Per User");
			pSeperateByAtLeast = PageCtrl.create_edit      (GroupID, "", "Seperate By At Least");
			pNearShowsClosest = PageCtrl.create_edit      (GroupID, "", "Near Shows Closest");
			pAllowDeleteAll = PageCtrl.create_check     (GroupID, "", "Allow Delete All");

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
		
		void dialog_to_attributes(JumpPointsData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
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
			exr.exchange(Attr.near_shows_closest, (*pNearShowsClosest));
			exr.exchange(Attr.allow_delete_all, (*pAllowDeleteAll));

			
			if (Mode == DataExchanger::FROM_CONTROL)
			{
				Attr.DataUpdated();
			}
			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(JumpPointsData& RemoteData)
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
} }