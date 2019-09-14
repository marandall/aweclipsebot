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

namespace ChatControllers { 
namespace AutoTemplates 
{
	class ColorTableEntryPropertyBuilder;

	//
	// Import Data
	//
	// a:4:{i:0;a:4:{s:4:"name";s:2:"id";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:4:"bold";s:4:"type";s:5:"tsb_t";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:6:"italic";s:4:"type";s:5:"tsb_t";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:5:"color";s:4:"type";s:10:"aw::colour";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class ColorTableEntryData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef ColorTableEntryPropertyBuilder PropertyBuilder;
	
	public: // source data
		String id;
		tsb_t       bold;
		tsb_t       italic;
		AW::Colour  color;

		
	public:
		ColorTableEntryData()
		{
			id = "";
			bold = B_EMPTY;
			italic = B_EMPTY;
			color = 0xFFFFFF;

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
			char* section_id = const_cast<char*>("relay_ColorTableEntry");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "id", id);
			SerializerSystem.Exchange(ExchangeMode, section_id, "bold", bold);
			SerializerSystem.Exchange(ExchangeMode, section_id, "italic", italic);
			SerializerSystem.Exchange(ExchangeMode, section_id, "color", color);

		}
		
		ColorTableEntryData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		String  getId                            () const { return id; }
		tsb_t        getBold                          () const { return bold; }
		tsb_t        getItalic                        () const { return italic; }
		AW::Colour   getColor                         () const { return color; }

		virtual void setId                            (String  Assgn) { id = Assgn; }
		virtual void setBold                          (tsb_t        Assgn) { bold = Assgn; }
		virtual void setItalic                        (tsb_t        Assgn) { italic = Assgn; }
		virtual void setColor                         (AW::Colour   Assgn) { color = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "id") {
				return Conversion::ToString(id);
			}
			if (ParamName == "bold") {
				return Conversion::ToStringTSB(bold);
			}
			if (ParamName == "italic") {
				return Conversion::ToStringTSB(italic);
			}
			if (ParamName == "color") {
				return Conversion::ToString(color);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "id") { 
				id = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "bold") { 
				bold = Conversion::ToTSB(Value);
				return true;
			}
			if (ParamName == "italic") { 
				italic = Conversion::ToTSB(Value);
				return true;
			}
			if (ParamName == "color") { 
				color = Conversion::ToColour(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(id, id);
			UMBRA_PROPERTY_RW(bold, bold);
			UMBRA_PROPERTY_RW(italic, italic);
			UMBRA_PROPERTY_RW(color, color);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class ColorTableEntryPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pId            ;
		CButton          *pBold          ;
		CButton          *pItalic        ;
		ColourButton     *pColor         ;

		
	public: // a set of the special property type
		ColorTableEntryData data;
		ColorTableEntryData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pId             = PageCtrl.create_edit      (GroupID, "", "Id");
			pBold           = PageCtrl.create_tristate  (GroupID, "", "Bold");
			pItalic         = PageCtrl.create_tristate  (GroupID, "", "Italic");
			pColor          = PageCtrl.create_colour    (GroupID, "", "Color");

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
		
		bool dialog_to_attributes(ColorTableEntryData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(ColorTableEntryData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(ColorTableEntryData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.id, (*pId            ));
			exr.exchange(Attr.bold, (*pBold          ));
			exr.exchange(Attr.italic, (*pItalic        ));
			exr.exchange(Attr.color, (*pColor         ));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(ColorTableEntryData& RemoteData)
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