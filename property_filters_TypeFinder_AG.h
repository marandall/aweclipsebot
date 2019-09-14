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
	class TypeFinderPropertyBuilder;

	//
	// Import Data
	//
	// a:5:{i:0;a:4:{s:4:"name";s:2:"v3";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:5:"zones";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:9:"particles";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:6:"movers";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:7:"cameras";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class TypeFinderData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef TypeFinderPropertyBuilder PropertyBuilder;
	
	public: // source data
		bool v3;
		bool zones;
		bool particles;
		bool movers;
		bool cameras;

		
	public:
		TypeFinderData()
		{
			v3 = false;
			zones = false;
			particles = false;
			movers = false;
			cameras = false;

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
			char* section_id = const_cast<char*>("propfilterTypeFinder");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "v3", v3);
			SerializerSystem.Exchange(ExchangeMode, section_id, "zones", zones);
			SerializerSystem.Exchange(ExchangeMode, section_id, "particles", particles);
			SerializerSystem.Exchange(ExchangeMode, section_id, "movers", movers);
			SerializerSystem.Exchange(ExchangeMode, section_id, "cameras", cameras);

		}
		
		TypeFinderData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		bool  getV3                            () const { return v3; }
		bool  getZones                         () const { return zones; }
		bool  getParticles                     () const { return particles; }
		bool  getMovers                        () const { return movers; }
		bool  getCameras                       () const { return cameras; }

		virtual void setV3                            (bool  Assgn) { v3 = Assgn; }
		virtual void setZones                         (bool  Assgn) { zones = Assgn; }
		virtual void setParticles                     (bool  Assgn) { particles = Assgn; }
		virtual void setMovers                        (bool  Assgn) { movers = Assgn; }
		virtual void setCameras                       (bool  Assgn) { cameras = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "v3") {
				return Conversion::ToString(v3);
			}
			if (ParamName == "zones") {
				return Conversion::ToString(zones);
			}
			if (ParamName == "particles") {
				return Conversion::ToString(particles);
			}
			if (ParamName == "movers") {
				return Conversion::ToString(movers);
			}
			if (ParamName == "cameras") {
				return Conversion::ToString(cameras);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "v3") { 
				v3 = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "zones") { 
				zones = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "particles") { 
				particles = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "movers") { 
				movers = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "cameras") { 
				cameras = Conversion::ToBool(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(v3, v3);
			UMBRA_PROPERTY_RW(zones, zones);
			UMBRA_PROPERTY_RW(particles, particles);
			UMBRA_PROPERTY_RW(movers, movers);
			UMBRA_PROPERTY_RW(cameras, cameras);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class TypeFinderPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CButton          *pV3            ;
		CButton          *pZones         ;
		CButton          *pParticles     ;
		CButton          *pMovers        ;
		CButton          *pCameras       ;

		
	public: // a set of the special property type
		TypeFinderData data;
		TypeFinderData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pV3             = PageCtrl.create_check     (GroupID, "", "V3");
			pZones          = PageCtrl.create_check     (GroupID, "", "Zones");
			pParticles      = PageCtrl.create_check     (GroupID, "", "Particles");
			pMovers         = PageCtrl.create_check     (GroupID, "", "Movers");
			pCameras        = PageCtrl.create_check     (GroupID, "", "Cameras");

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
		
		bool dialog_to_attributes(TypeFinderData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(TypeFinderData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(TypeFinderData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.v3, (*pV3            ));
			exr.exchange(Attr.zones, (*pZones         ));
			exr.exchange(Attr.particles, (*pParticles     ));
			exr.exchange(Attr.movers, (*pMovers        ));
			exr.exchange(Attr.cameras, (*pCameras       ));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(TypeFinderData& RemoteData)
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