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
	class InequalityPropertyBuilder;

	//
	// Import Data
	//
	// a:6:{i:0;a:4:{s:4:"name";s:1:"x";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:9:"greater_x";s:4:"type";s:5:"tsb_t";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:1:"y";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:9:"greater_y";s:4:"type";s:5:"tsb_t";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:1:"z";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:5;a:4:{s:4:"name";s:9:"greater_z";s:4:"type";s:5:"tsb_t";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class InequalityData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef InequalityPropertyBuilder PropertyBuilder;
	
	public: // source data
		String x;
		tsb_t       greater_x;
		String y;
		tsb_t       greater_y;
		String z;
		tsb_t       greater_z;

		
	public:
		InequalityData()
		{
			x = "";
			greater_x = B_EMPTY;
			y = "";
			greater_y = B_EMPTY;
			z = "";
			greater_z = B_EMPTY;

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
			char* section_id = const_cast<char*>("propfilterInequality");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "x", x);
			SerializerSystem.Exchange(ExchangeMode, section_id, "greater_x", greater_x);
			SerializerSystem.Exchange(ExchangeMode, section_id, "y", y);
			SerializerSystem.Exchange(ExchangeMode, section_id, "greater_y", greater_y);
			SerializerSystem.Exchange(ExchangeMode, section_id, "z", z);
			SerializerSystem.Exchange(ExchangeMode, section_id, "greater_z", greater_z);

		}
		
		InequalityData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		String  getX                             () const { return x; }
		tsb_t        getGreaterX                      () const { return greater_x; }
		String  getY                             () const { return y; }
		tsb_t        getGreaterY                      () const { return greater_y; }
		String  getZ                             () const { return z; }
		tsb_t        getGreaterZ                      () const { return greater_z; }

		virtual void setX                             (String  Assgn) { x = Assgn; }
		virtual void setGreaterX                      (tsb_t        Assgn) { greater_x = Assgn; }
		virtual void setY                             (String  Assgn) { y = Assgn; }
		virtual void setGreaterY                      (tsb_t        Assgn) { greater_y = Assgn; }
		virtual void setZ                             (String  Assgn) { z = Assgn; }
		virtual void setGreaterZ                      (tsb_t        Assgn) { greater_z = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "x") {
				return Conversion::ToString(x);
			}
			if (ParamName == "greater_x") {
				return Conversion::ToStringTSB(greater_x);
			}
			if (ParamName == "y") {
				return Conversion::ToString(y);
			}
			if (ParamName == "greater_y") {
				return Conversion::ToStringTSB(greater_y);
			}
			if (ParamName == "z") {
				return Conversion::ToString(z);
			}
			if (ParamName == "greater_z") {
				return Conversion::ToStringTSB(greater_z);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "x") { 
				x = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "greater_x") { 
				greater_x = Conversion::ToTSB(Value);
				return true;
			}
			if (ParamName == "y") { 
				y = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "greater_y") { 
				greater_y = Conversion::ToTSB(Value);
				return true;
			}
			if (ParamName == "z") { 
				z = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "greater_z") { 
				greater_z = Conversion::ToTSB(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(x, x);
			UMBRA_PROPERTY_RW(greater_x, greater_x);
			UMBRA_PROPERTY_RW(y, y);
			UMBRA_PROPERTY_RW(greater_y, greater_y);
			UMBRA_PROPERTY_RW(z, z);
			UMBRA_PROPERTY_RW(greater_z, greater_z);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class InequalityPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pX             ;
		CButton          *pGreaterX      ;
		TextBox          *pY             ;
		CButton          *pGreaterY      ;
		TextBox          *pZ             ;
		CButton          *pGreaterZ      ;

		
	public: // a set of the special property type
		InequalityData data;
		InequalityData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pX              = PageCtrl.create_edit      (GroupID, "", "X");
			pGreaterX       = PageCtrl.create_tristate  (GroupID, "", "Greater X");
			pY              = PageCtrl.create_edit      (GroupID, "", "Y");
			pGreaterY       = PageCtrl.create_tristate  (GroupID, "", "Greater Y");
			pZ              = PageCtrl.create_edit      (GroupID, "", "Z");
			pGreaterZ       = PageCtrl.create_tristate  (GroupID, "", "Greater Z");

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
		
		bool dialog_to_attributes(InequalityData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(InequalityData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(InequalityData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.x, (*pX             ));
			exr.exchange(Attr.greater_x, (*pGreaterX      ));
			exr.exchange(Attr.y, (*pY             ));
			exr.exchange(Attr.greater_y, (*pGreaterY      ));
			exr.exchange(Attr.z, (*pZ             ));
			exr.exchange(Attr.greater_z, (*pGreaterZ      ));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(InequalityData& RemoteData)
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