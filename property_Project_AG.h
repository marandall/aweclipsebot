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

namespace Property { 
namespace AutoTemplates 
{
	class ProjectPropertyBuilder;

	//
	// Import Data
	//
	// a:8:{i:0;a:4:{s:4:"name";s:2:"id";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:8:"tracking";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:8:"job_type";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:17:"job_build_success";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:16:"job_build_failed";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:5;a:4:{s:4:"name";s:14:"job_last_error";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:6;a:4:{s:4:"name";s:15:"use_object_load";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:7;a:4:{s:4:"name";s:11:"limbo_count";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class ProjectData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef ProjectPropertyBuilder PropertyBuilder;
	
	public: // source data
		String id;
		int         tracking;
		int         job_type;
		int         job_build_success;
		int         job_build_failed;
		int         job_last_error;
		bool use_object_load;
		int         limbo_count;

		
	public:
		ProjectData()
		{
			id = "";
			tracking = 0;
			job_type = 0;
			job_build_success = 0;
			job_build_failed = 0;
			job_last_error = 0;
			use_object_load = false;
			limbo_count = 0;

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
			char* section_id = const_cast<char*>("prop_Project");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "id", id);
			SerializerSystem.Exchange(ExchangeMode, section_id, "tracking", tracking);
			SerializerSystem.Exchange(ExchangeMode, section_id, "job_type", job_type);
			SerializerSystem.Exchange(ExchangeMode, section_id, "job_build_success", job_build_success);
			SerializerSystem.Exchange(ExchangeMode, section_id, "job_build_failed", job_build_failed);
			SerializerSystem.Exchange(ExchangeMode, section_id, "job_last_error", job_last_error);
			SerializerSystem.Exchange(ExchangeMode, section_id, "use_object_load", use_object_load);
			SerializerSystem.Exchange(ExchangeMode, section_id, "limbo_count", limbo_count);

		}
		
		ProjectData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		String  getId                            () const { return id; }
		int          getTracking                      () const { return tracking; }
		int          getJobType                       () const { return job_type; }
		int          getJobBuildSuccess               () const { return job_build_success; }
		int          getJobBuildFailed                () const { return job_build_failed; }
		int          getJobLastError                  () const { return job_last_error; }
		bool  getUseObjectLoad                 () const { return use_object_load; }
		int          getLimboCount                    () const { return limbo_count; }

		virtual void setId                            (String  Assgn) { id = Assgn; }
		virtual void setTracking                      (int          Assgn) { tracking = Assgn; }
		virtual void setJobType                       (int          Assgn) { job_type = Assgn; }
		virtual void setJobBuildSuccess               (int          Assgn) { job_build_success = Assgn; }
		virtual void setJobBuildFailed                (int          Assgn) { job_build_failed = Assgn; }
		virtual void setJobLastError                  (int          Assgn) { job_last_error = Assgn; }
		virtual void setUseObjectLoad                 (bool  Assgn) { use_object_load = Assgn; }
		virtual void setLimboCount                    (int          Assgn) { limbo_count = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "id") {
				return Conversion::ToString(id);
			}
			if (ParamName == "tracking") {
				return Conversion::ToString(tracking);
			}
			if (ParamName == "job_type") {
				return Conversion::ToString(job_type);
			}
			if (ParamName == "job_build_success") {
				return Conversion::ToString(job_build_success);
			}
			if (ParamName == "job_build_failed") {
				return Conversion::ToString(job_build_failed);
			}
			if (ParamName == "job_last_error") {
				return Conversion::ToString(job_last_error);
			}
			if (ParamName == "use_object_load") {
				return Conversion::ToString(use_object_load);
			}
			if (ParamName == "limbo_count") {
				return Conversion::ToString(limbo_count);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "id") { 
				id = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "tracking") { 
				tracking = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "job_type") { 
				job_type = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "job_build_success") { 
				job_build_success = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "job_build_failed") { 
				job_build_failed = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "job_last_error") { 
				job_last_error = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "use_object_load") { 
				use_object_load = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "limbo_count") { 
				limbo_count = Conversion::ToInt(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(id, id);
			UMBRA_PROPERTY_RW(tracking, tracking);
			UMBRA_PROPERTY_RW(job_type, job_type);
			UMBRA_PROPERTY_RW(job_build_success, job_build_success);
			UMBRA_PROPERTY_RW(job_build_failed, job_build_failed);
			UMBRA_PROPERTY_RW(job_last_error, job_last_error);
			UMBRA_PROPERTY_RW(use_object_load, use_object_load);
			UMBRA_PROPERTY_RW(limbo_count, limbo_count);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class ProjectPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pId            ;
		TextBox          *pTracking      ;
		TextBox          *pJobType       ;
		TextBox          *pJobBuildSuccess;
		TextBox          *pJobBuildFailed;
		TextBox          *pJobLastError  ;
		CButton          *pUseObjectLoad ;
		TextBox          *pLimboCount    ;

		
	public: // a set of the special property type
		ProjectData data;
		ProjectData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pId             = PageCtrl.create_edit      (GroupID, "", "Id");
			pTracking       = PageCtrl.create_edit      (GroupID, "", "Tracking");
			pJobType        = PageCtrl.create_edit      (GroupID, "", "Job Type");
			pJobBuildSuccess = PageCtrl.create_edit      (GroupID, "", "Job Build Success");
			pJobBuildFailed = PageCtrl.create_edit      (GroupID, "", "Job Build Failed");
			pJobLastError   = PageCtrl.create_edit      (GroupID, "", "Job Last Error");
			pUseObjectLoad  = PageCtrl.create_check     (GroupID, "", "Use Object Load");
			pLimboCount     = PageCtrl.create_edit      (GroupID, "", "Limbo Count");

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
		
		bool dialog_to_attributes(ProjectData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(ProjectData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(ProjectData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.id, (*pId            ));
			exr.exchange(Attr.tracking, (*pTracking      ));
			exr.exchange(Attr.job_type, (*pJobType       ));
			exr.exchange(Attr.job_build_success, (*pJobBuildSuccess));
			exr.exchange(Attr.job_build_failed, (*pJobBuildFailed));
			exr.exchange(Attr.job_last_error, (*pJobLastError  ));
			exr.exchange(Attr.use_object_load, (*pUseObjectLoad ));
			exr.exchange(Attr.limbo_count, (*pLimboCount    ));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(ProjectData& RemoteData)
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