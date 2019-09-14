#pragma once
#include "serializer.h"
#include "tsb.h"
#include "autonumber.h"

// umbra includes
#include "umbra/umbra_remoteclass.h"
#include "umbra/umbra_variant.h"

#include "raw_param_editor.h"
#include "PropertyScriptCtrl.h"
#include "GlobalDex.h"

namespace WeatherControl { 
namespace AutoTemplates 
{
	class TweenerPropertyBuilder;

	//
	// Import Data
	//
	// a:3:{i:0;a:4:{s:4:"name";s:10:"day_length";s:4:"type";s:5:"float";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:4:"rate";s:4:"type";s:5:"float";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:6:"offset";s:4:"type";s:5:"float";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class TweenerData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier
	{
	public: // type defenition for property builder
		typedef TweenerPropertyBuilder PropertyBuilder;
	
	public: // source data
		float       day_length;
		float       rate;
		float       offset;

		
	public:
		TweenerData()
		{
			day_length = 0.0F;
			rate = 1;
			offset = 0.0F;

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
			char* section_id = const_cast<char*>("weather_Tweener");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "day_length", day_length);
			SerializerSystem.Exchange(ExchangeMode, section_id, "rate", rate);
			SerializerSystem.Exchange(ExchangeMode, section_id, "offset", offset);

		}
		
		TweenerData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		float        getDayLength                     () const { return day_length; }
		float        getRate                          () const { return rate; }
		float        getOffset                        () const { return offset; }

		virtual void setDayLength                     (float        Assgn) { day_length = Assgn; }
		virtual void setRate                          (float        Assgn) { rate = Assgn; }
		virtual void setOffset                        (float        Assgn) { offset = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "day_length") {
				return Conversion::ToString(day_length);
			}
			if (ParamName == "rate") {
				return Conversion::ToString(rate);
			}
			if (ParamName == "offset") {
				return Conversion::ToString(offset);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "day_length") { 
				day_length = Conversion::ToFloat(Value);
				return true;
			}
			if (ParamName == "rate") { 
				rate = Conversion::ToFloat(Value);
				return true;
			}
			if (ParamName == "offset") { 
				offset = Conversion::ToFloat(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(day_length, day_length);
			UMBRA_PROPERTY_RW(rate, rate);
			UMBRA_PROPERTY_RW(offset, offset);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class TweenerPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pDayLength     ;
		TextBox          *pRate          ;
		TextBox          *pOffset        ;

		
	public: // a set of the special property type
		TweenerData data;
		TweenerData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pDayLength      = PageCtrl.create_edit      (GroupID, "", "Day Length");
			pRate           = PageCtrl.create_edit      (GroupID, "", "Rate");
			pOffset         = PageCtrl.create_edit      (GroupID, "", "Offset");

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
		
		bool dialog_to_attributes(TweenerData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(TweenerData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(TweenerData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.day_length, (*pDayLength     ));
			exr.exchange(Attr.rate, (*pRate          ));
			exr.exchange(Attr.offset, (*pOffset        ));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(TweenerData& RemoteData)
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