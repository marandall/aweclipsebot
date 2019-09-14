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
	class MoverBoundsPropertyBuilder;

	//
	// Import Data
	//
	// a:4:{i:0;a:4:{s:4:"name";s:14:"max_plane_size";s:4:"type";s:5:"float";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:5:"water";s:4:"type";s:5:"tsb_t";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:7:"visible";s:4:"type";s:5:"tsb_t";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:12:"blocked_chat";s:4:"type";s:5:"tsb_t";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class MoverBoundsData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef MoverBoundsPropertyBuilder PropertyBuilder;
	
	public: // source data
		float       max_plane_size;
		tsb_t       water;
		tsb_t       visible;
		tsb_t       blocked_chat;

		
	public:
		MoverBoundsData()
		{
			max_plane_size = 0.0F;
			water = B_EMPTY;
			visible = B_EMPTY;
			blocked_chat = B_EMPTY;

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
			char* section_id = const_cast<char*>("propfilterMoverBounds");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "max_plane_size", max_plane_size);
			SerializerSystem.Exchange(ExchangeMode, section_id, "water", water);
			SerializerSystem.Exchange(ExchangeMode, section_id, "visible", visible);
			SerializerSystem.Exchange(ExchangeMode, section_id, "blocked_chat", blocked_chat);

		}
		
		MoverBoundsData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		float        getMaxPlaneSize                  () const { return max_plane_size; }
		tsb_t        getWater                         () const { return water; }
		tsb_t        getVisible                       () const { return visible; }
		tsb_t        getBlockedChat                   () const { return blocked_chat; }

		virtual void setMaxPlaneSize                  (float        Assgn) { max_plane_size = Assgn; }
		virtual void setWater                         (tsb_t        Assgn) { water = Assgn; }
		virtual void setVisible                       (tsb_t        Assgn) { visible = Assgn; }
		virtual void setBlockedChat                   (tsb_t        Assgn) { blocked_chat = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "max_plane_size") {
				return Conversion::ToString(max_plane_size);
			}
			if (ParamName == "water") {
				return Conversion::ToStringTSB(water);
			}
			if (ParamName == "visible") {
				return Conversion::ToStringTSB(visible);
			}
			if (ParamName == "blocked_chat") {
				return Conversion::ToStringTSB(blocked_chat);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "max_plane_size") { 
				max_plane_size = Conversion::ToFloat(Value);
				return true;
			}
			if (ParamName == "water") { 
				water = Conversion::ToTSB(Value);
				return true;
			}
			if (ParamName == "visible") { 
				visible = Conversion::ToTSB(Value);
				return true;
			}
			if (ParamName == "blocked_chat") { 
				blocked_chat = Conversion::ToTSB(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(max_plane_size, max_plane_size);
			UMBRA_PROPERTY_RW(water, water);
			UMBRA_PROPERTY_RW(visible, visible);
			UMBRA_PROPERTY_RW(blocked_chat, blocked_chat);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class MoverBoundsPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pMaxPlaneSize  ;
		CButton          *pWater         ;
		CButton          *pVisible       ;
		CButton          *pBlockedChat   ;

		
	public: // a set of the special property type
		MoverBoundsData data;
		MoverBoundsData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pMaxPlaneSize   = PageCtrl.create_edit      (GroupID, "", "Max Plane Size");
			pWater          = PageCtrl.create_tristate  (GroupID, "", "Water");
			pVisible        = PageCtrl.create_tristate  (GroupID, "", "Visible");
			pBlockedChat    = PageCtrl.create_tristate  (GroupID, "", "Blocked Chat");

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
		
		bool dialog_to_attributes(MoverBoundsData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(MoverBoundsData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(MoverBoundsData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.max_plane_size, (*pMaxPlaneSize  ));
			exr.exchange(Attr.water, (*pWater         ));
			exr.exchange(Attr.visible, (*pVisible       ));
			exr.exchange(Attr.blocked_chat, (*pBlockedChat   ));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(MoverBoundsData& RemoteData)
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