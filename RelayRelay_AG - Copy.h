#pragma once
#include "serializer.h"
#include "tsb.h"
#include "umbra_remote_class.h"
#include "raw_param_editor.h"
#include "PropertyScriptCtrl.h"
#include "GlobalDex.h"

namespace ChatControllers { 
namespace AutoTemplates 
{
	class RelayPropertyBuilder;

	class RelayData : 
		public Umbra::RemoteClass,
		public IRawParamEditor
	{
	public: // type defenition for property builder
		typedef RelayPropertyBuilder PropertyBuilder;
	
	public: // source data
		int         relay_mode;
		bool        filter_bad_words;
		Ascii       display_name;
		AW::Colour  display_colour;
		bool        display_bold;
		bool        display_italic;

		
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
			DataUpdated();
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
		
		RelayData& GetDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual void DataUpdated() { }
		
	public: // read only operators
		int          get_RelayMode                     () const { return relay_mode; }
		bool         get_FilterBadWords                () const { return filter_bad_words; }
		Ascii        get_DisplayName                   () const { return display_name; }
		AW::Colour   get_DisplayColour                 () const { return display_colour; }
		bool         get_DisplayBold                   () const { return display_bold; }
		bool         get_DisplayItalic                 () const { return display_italic; }

		virtual void set_RelayMode                     (int          Assgn) { relay_mode = Assgn; }
		virtual void set_FilterBadWords                (bool         Assgn) { filter_bad_words = Assgn; }
		virtual void set_DisplayName                   (Ascii        Assgn) { display_name = Assgn; }
		virtual void set_DisplayColour                 (AW::Colour   Assgn) { display_colour = Assgn; }
		virtual void set_DisplayBold                   (bool         Assgn) { display_bold = Assgn; }
		virtual void set_DisplayItalic                 (bool         Assgn) { display_italic = Assgn; }

		
	public: // string conversion
		virtual Ascii param_read(Ascii ParamName)
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
		
		virtual bool param_write(Ascii ParamName, Ascii Value)
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
		Umbra::Variant Umbra_GetProperty(Umbra::string ID, Umbra::ES_GetProperty& ES)
		{
			if (strcmp(ID.c_str(), "relay_mode"                  ) == 0) return relay_mode;
			if (strcmp(ID.c_str(), "filter_bad_words"            ) == 0) return filter_bad_words;
			if (strcmp(ID.c_str(), "display_name"                ) == 0) return (const char*)display_name;
			if (strcmp(ID.c_str(), "display_colour"              ) == 0) return display_colour.value;
			if (strcmp(ID.c_str(), "display_bold"                ) == 0) return display_bold;
			if (strcmp(ID.c_str(), "display_italic"              ) == 0) return display_italic;

			
			// nothing found
			throw Umbra::UnsupportedPropertyException(ID);
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
		}
		
		void attributes_to_dialog()
		{
			data_exchange(data, DataExchanger::TO_CONTROL);
		}
		
		void dialog_to_attributes(RelayData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
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

			
			if (Mode == DataExchanger::FROM_CONTROL)
			{
				Attr.DataUpdated();
			}
			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(RelayData& RemoteData)
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
} 