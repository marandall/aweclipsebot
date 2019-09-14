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
	class ParticleBoundsPropertyBuilder;

	//
	// Import Data
	//
	// a:4:{i:0;a:4:{s:4:"name";s:14:"max_plane_size";s:4:"type";s:5:"float";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:16:"max_plane_volume";s:4:"type";s:5:"float";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:22:"max_plane_acceleration";s:4:"type";s:5:"float";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:18:"max_plane_distance";s:4:"type";s:5:"float";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class ParticleBoundsData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef ParticleBoundsPropertyBuilder PropertyBuilder;
	
	public: // source data
		float       max_plane_size;
		float       max_plane_volume;
		float       max_plane_acceleration;
		float       max_plane_distance;

		
	public:
		ParticleBoundsData()
		{
			max_plane_size = 0.0F;
			max_plane_volume = 0.0F;
			max_plane_acceleration = 0.0F;
			max_plane_distance = 0.0F;

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
			char* section_id = const_cast<char*>("propfilterParticleBounds");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "max_plane_size", max_plane_size);
			SerializerSystem.Exchange(ExchangeMode, section_id, "max_plane_volume", max_plane_volume);
			SerializerSystem.Exchange(ExchangeMode, section_id, "max_plane_acceleration", max_plane_acceleration);
			SerializerSystem.Exchange(ExchangeMode, section_id, "max_plane_distance", max_plane_distance);

		}
		
		ParticleBoundsData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		float        getMaxPlaneSize                  () const { return max_plane_size; }
		float        getMaxPlaneVolume                () const { return max_plane_volume; }
		float        getMaxPlaneAcceleration          () const { return max_plane_acceleration; }
		float        getMaxPlaneDistance              () const { return max_plane_distance; }

		virtual void setMaxPlaneSize                  (float        Assgn) { max_plane_size = Assgn; }
		virtual void setMaxPlaneVolume                (float        Assgn) { max_plane_volume = Assgn; }
		virtual void setMaxPlaneAcceleration          (float        Assgn) { max_plane_acceleration = Assgn; }
		virtual void setMaxPlaneDistance              (float        Assgn) { max_plane_distance = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "max_plane_size") {
				return Conversion::ToString(max_plane_size);
			}
			if (ParamName == "max_plane_volume") {
				return Conversion::ToString(max_plane_volume);
			}
			if (ParamName == "max_plane_acceleration") {
				return Conversion::ToString(max_plane_acceleration);
			}
			if (ParamName == "max_plane_distance") {
				return Conversion::ToString(max_plane_distance);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "max_plane_size") { 
				max_plane_size = Conversion::ToFloat(Value);
				return true;
			}
			if (ParamName == "max_plane_volume") { 
				max_plane_volume = Conversion::ToFloat(Value);
				return true;
			}
			if (ParamName == "max_plane_acceleration") { 
				max_plane_acceleration = Conversion::ToFloat(Value);
				return true;
			}
			if (ParamName == "max_plane_distance") { 
				max_plane_distance = Conversion::ToFloat(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(max_plane_size, max_plane_size);
			UMBRA_PROPERTY_RW(max_plane_volume, max_plane_volume);
			UMBRA_PROPERTY_RW(max_plane_acceleration, max_plane_acceleration);
			UMBRA_PROPERTY_RW(max_plane_distance, max_plane_distance);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class ParticleBoundsPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pMaxPlaneSize  ;
		TextBox          *pMaxPlaneVolume;
		TextBox          *pMaxPlaneAcceleration;
		TextBox          *pMaxPlaneDistance;

		
	public: // a set of the special property type
		ParticleBoundsData data;
		ParticleBoundsData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pMaxPlaneSize   = PageCtrl.create_edit      (GroupID, "", "Max Plane Size");
			pMaxPlaneVolume = PageCtrl.create_edit      (GroupID, "", "Max Plane Volume");
			pMaxPlaneAcceleration = PageCtrl.create_edit      (GroupID, "", "Max Plane Acceleration");
			pMaxPlaneDistance = PageCtrl.create_edit      (GroupID, "", "Max Plane Distance");

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
		
		bool dialog_to_attributes(ParticleBoundsData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(ParticleBoundsData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(ParticleBoundsData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.max_plane_size, (*pMaxPlaneSize  ));
			exr.exchange(Attr.max_plane_volume, (*pMaxPlaneVolume));
			exr.exchange(Attr.max_plane_acceleration, (*pMaxPlaneAcceleration));
			exr.exchange(Attr.max_plane_distance, (*pMaxPlaneDistance));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(ParticleBoundsData& RemoteData)
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