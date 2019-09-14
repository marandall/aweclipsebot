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
	class BuildStatePropertyBuilder;

	//
	// Import Data
	//
	// a:5:{i:0;a:4:{s:4:"name";s:4:"idle";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:6:"queued";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:7:"pending";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:9:"succeeded";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:6:"failed";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class BuildStateData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef BuildStatePropertyBuilder PropertyBuilder;
	
	public: // source data
		bool idle;
		bool queued;
		bool pending;
		bool succeeded;
		bool failed;

		
	public:
		BuildStateData()
		{
			idle = false;
			queued = false;
			pending = false;
			succeeded = false;
			failed = false;

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
			char* section_id = const_cast<char*>("propfilterBuildState");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "idle", idle);
			SerializerSystem.Exchange(ExchangeMode, section_id, "queued", queued);
			SerializerSystem.Exchange(ExchangeMode, section_id, "pending", pending);
			SerializerSystem.Exchange(ExchangeMode, section_id, "succeeded", succeeded);
			SerializerSystem.Exchange(ExchangeMode, section_id, "failed", failed);

		}
		
		BuildStateData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		bool  getIdle                          () const { return idle; }
		bool  getQueued                        () const { return queued; }
		bool  getPending                       () const { return pending; }
		bool  getSucceeded                     () const { return succeeded; }
		bool  getFailed                        () const { return failed; }

		virtual void setIdle                          (bool  Assgn) { idle = Assgn; }
		virtual void setQueued                        (bool  Assgn) { queued = Assgn; }
		virtual void setPending                       (bool  Assgn) { pending = Assgn; }
		virtual void setSucceeded                     (bool  Assgn) { succeeded = Assgn; }
		virtual void setFailed                        (bool  Assgn) { failed = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "idle") {
				return Conversion::ToString(idle);
			}
			if (ParamName == "queued") {
				return Conversion::ToString(queued);
			}
			if (ParamName == "pending") {
				return Conversion::ToString(pending);
			}
			if (ParamName == "succeeded") {
				return Conversion::ToString(succeeded);
			}
			if (ParamName == "failed") {
				return Conversion::ToString(failed);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "idle") { 
				idle = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "queued") { 
				queued = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "pending") { 
				pending = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "succeeded") { 
				succeeded = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "failed") { 
				failed = Conversion::ToBool(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(idle, idle);
			UMBRA_PROPERTY_RW(queued, queued);
			UMBRA_PROPERTY_RW(pending, pending);
			UMBRA_PROPERTY_RW(succeeded, succeeded);
			UMBRA_PROPERTY_RW(failed, failed);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class BuildStatePropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CButton          *pIdle          ;
		CButton          *pQueued        ;
		CButton          *pPending       ;
		CButton          *pSucceeded     ;
		CButton          *pFailed        ;

		
	public: // a set of the special property type
		BuildStateData data;
		BuildStateData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pIdle           = PageCtrl.create_check     (GroupID, "", "Idle");
			pQueued         = PageCtrl.create_check     (GroupID, "", "Queued");
			pPending        = PageCtrl.create_check     (GroupID, "", "Pending");
			pSucceeded      = PageCtrl.create_check     (GroupID, "", "Succeeded");
			pFailed         = PageCtrl.create_check     (GroupID, "", "Failed");

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
		
		bool dialog_to_attributes(BuildStateData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(BuildStateData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(BuildStateData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.idle, (*pIdle          ));
			exr.exchange(Attr.queued, (*pQueued        ));
			exr.exchange(Attr.pending, (*pPending       ));
			exr.exchange(Attr.succeeded, (*pSucceeded     ));
			exr.exchange(Attr.failed, (*pFailed        ));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(BuildStateData& RemoteData)
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