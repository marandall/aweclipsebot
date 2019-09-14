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
	class RelayPropertyBuilder;

	//
	// Import Data
	//
	// a:6:{i:0;a:4:{s:4:"name";s:10:"relay_mode";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:16:"filter_bad_words";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:12:"display_name";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:14:"display_colour";s:4:"type";s:10:"aw::colour";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:12:"display_bold";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:5;a:4:{s:4:"name";s:14:"display_italic";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class RelayData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef RelayPropertyBuilder PropertyBuilder;
	
	public: // source data
		int         relay_mode;
		bool filter_bad_words;
		String display_name;
		AW::Colour  display_colour;
		bool display_bold;
		bool display_italic;

		
	public:
		RelayData()
		{
			relay_mode = 0;
			filter_bad_words = true;
			display_name = "{$bot.name}";
			display_colour = 0xFFFFFF;
			display_bold = false;
			display_italic = false;

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
			char* section_id = const_cast<char*>("relay_Relay");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "relay_mode", relay_mode);
			SerializerSystem.Exchange(ExchangeMode, section_id, "filter_bad_words", filter_bad_words);
			SerializerSystem.Exchange(ExchangeMode, section_id, "display_name", display_name);
			SerializerSystem.Exchange(ExchangeMode, section_id, "display_colour", display_colour);
			SerializerSystem.Exchange(ExchangeMode, section_id, "display_bold", display_bold);
			SerializerSystem.Exchange(ExchangeMode, section_id, "display_italic", display_italic);

		}
		
		RelayData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		int          getRelayMode                     () const { return relay_mode; }
		bool  getFilterBadWords                () const { return filter_bad_words; }
		String  getDisplayName                   () const { return display_name; }
		AW::Colour   getDisplayColour                 () const { return display_colour; }
		bool  getDisplayBold                   () const { return display_bold; }
		bool  getDisplayItalic                 () const { return display_italic; }

		virtual void setRelayMode                     (int          Assgn) { relay_mode = Assgn; }
		virtual void setFilterBadWords                (bool  Assgn) { filter_bad_words = Assgn; }
		virtual void setDisplayName                   (String  Assgn) { display_name = Assgn; }
		virtual void setDisplayColour                 (AW::Colour   Assgn) { display_colour = Assgn; }
		virtual void setDisplayBold                   (bool  Assgn) { display_bold = Assgn; }
		virtual void setDisplayItalic                 (bool  Assgn) { display_italic = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "relay_mode") {
				return Conversion::ToString(relay_mode);
			}
			if (ParamName == "filter_bad_words") {
				return Conversion::ToString(filter_bad_words);
			}
			if (ParamName == "display_name") {
				return Conversion::ToString(display_name);
			}
			if (ParamName == "display_colour") {
				return Conversion::ToString(display_colour);
			}
			if (ParamName == "display_bold") {
				return Conversion::ToString(display_bold);
			}
			if (ParamName == "display_italic") {
				return Conversion::ToString(display_italic);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "relay_mode") { 
				relay_mode = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "filter_bad_words") { 
				filter_bad_words = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "display_name") { 
				display_name = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "display_colour") { 
				display_colour = Conversion::ToColour(Value);
				return true;
			}
			if (ParamName == "display_bold") { 
				display_bold = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "display_italic") { 
				display_italic = Conversion::ToBool(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(relay_mode, relay_mode);
			UMBRA_PROPERTY_RW(filter_bad_words, filter_bad_words);
			UMBRA_PROPERTY_RW(display_name, display_name);
			UMBRA_PROPERTY_RW(display_colour, display_colour);
			UMBRA_PROPERTY_RW(display_bold, display_bold);
			UMBRA_PROPERTY_RW(display_italic, display_italic);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class RelayPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		ComboBox         *pRelayMode     ;
		CButton          *pFilterBadWords;
		TextBox          *pDisplayName   ;
		ColourButton     *pDisplayColour ;
		CButton          *pDisplayBold   ;
		CButton          *pDisplayItalic ;

		
	public: // a set of the special property type
		RelayData data;
		RelayData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pRelayMode      = PageCtrl.create_combo     (GroupID, "", "Relay Mode");
			pFilterBadWords = PageCtrl.create_check     (GroupID, "", "Filter Bad Words");
			pDisplayName    = PageCtrl.create_edit      (GroupID, "", "Display Name");
			pDisplayColour  = PageCtrl.create_colour    (GroupID, "", "Display Colour");
			pDisplayBold    = PageCtrl.create_check     (GroupID, "", "Display Bold");
			pDisplayItalic  = PageCtrl.create_check     (GroupID, "", "Display Italic");

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
		
		bool dialog_to_attributes(RelayData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(RelayData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(RelayData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.relay_mode, (*pRelayMode     ));
			exr.exchange(Attr.filter_bad_words, (*pFilterBadWords));
			exr.exchange(Attr.display_name, (*pDisplayName   ));
			exr.exchange(Attr.display_colour, (*pDisplayColour ));
			exr.exchange(Attr.display_bold, (*pDisplayBold   ));
			exr.exchange(Attr.display_italic, (*pDisplayItalic ));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(RelayData& RemoteData)
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