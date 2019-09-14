#pragma once
#include "serializer.h"
#include "tsb.h"
#include "umbra_remote_class.h"
#include "raw_param_editor.h"
#include "PropertyScriptCtrl.h"
#include "GlobalDex.h"

namespace CIS { namespace Config { 
namespace AutoTemplates 
{
	class InterfacePropertyBuilder;

	class InterfaceData : 
		public Umbra::RemoteClass,
		public IRawParamEditor
	{
	public: // type defenition for property builder
		typedef InterfacePropertyBuilder PropertyBuilder;
	
	public: // source data
		Ascii       prefix_key;
		AW::Colour  output_colour;
		bool        output_bold;
		bool        output_italic;
		bool        always_use_consoles;
		bool        use_command_welcome;
		bool        allow_remote_config;
		bool        use_default_responses;

		
	public:
		InterfaceData()
		{
			prefix_key = "#";
			output_colour = 0xFF0000;
			output_bold = false;
			output_italic = false;
			always_use_consoles = false;
			use_command_welcome = false;
			allow_remote_config = false;
			use_default_responses = false;

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
			char* section_id = const_cast<char*>("cis_Interface");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "prefix_key", prefix_key);
			SerializerSystem.Exchange(ExchangeMode, section_id, "output_colour", output_colour);
			SerializerSystem.Exchange(ExchangeMode, section_id, "output_bold", output_bold);
			SerializerSystem.Exchange(ExchangeMode, section_id, "output_italic", output_italic);
			SerializerSystem.Exchange(ExchangeMode, section_id, "always_use_consoles", always_use_consoles);
			SerializerSystem.Exchange(ExchangeMode, section_id, "use_command_welcome", use_command_welcome);
			SerializerSystem.Exchange(ExchangeMode, section_id, "allow_remote_config", allow_remote_config);
			SerializerSystem.Exchange(ExchangeMode, section_id, "use_default_responses", use_default_responses);

		}
		
		InterfaceData& GetDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual void DataUpdated() { }
		
	public: // read only operators
		Ascii        get_PrefixKey                     () const { return prefix_key; }
		AW::Colour   get_OutputColour                  () const { return output_colour; }
		bool         get_OutputBold                    () const { return output_bold; }
		bool         get_OutputItalic                  () const { return output_italic; }
		bool         get_AlwaysUseConsoles             () const { return always_use_consoles; }
		bool         get_UseCommandWelcome             () const { return use_command_welcome; }
		bool         get_AllowRemoteConfig             () const { return allow_remote_config; }
		bool         get_UseDefaultResponses           () const { return use_default_responses; }

		virtual void set_PrefixKey                     (Ascii        Assgn) { prefix_key = Assgn; }
		virtual void set_OutputColour                  (AW::Colour   Assgn) { output_colour = Assgn; }
		virtual void set_OutputBold                    (bool         Assgn) { output_bold = Assgn; }
		virtual void set_OutputItalic                  (bool         Assgn) { output_italic = Assgn; }
		virtual void set_AlwaysUseConsoles             (bool         Assgn) { always_use_consoles = Assgn; }
		virtual void set_UseCommandWelcome             (bool         Assgn) { use_command_welcome = Assgn; }
		virtual void set_AllowRemoteConfig             (bool         Assgn) { allow_remote_config = Assgn; }
		virtual void set_UseDefaultResponses           (bool         Assgn) { use_default_responses = Assgn; }

		
	public: // string conversion
		virtual Ascii param_read(Ascii ParamName)
		{
			if (ParamName == "prefix_key") {
				return Conversion::ToString(prefix_key);
			}
			if (ParamName == "output_colour") {
				return Conversion::ToString(output_colour);
			}
			if (ParamName == "output_bold") {
				return Conversion::ToString(output_bold);
			}
			if (ParamName == "output_italic") {
				return Conversion::ToString(output_italic);
			}
			if (ParamName == "always_use_consoles") {
				return Conversion::ToString(always_use_consoles);
			}
			if (ParamName == "use_command_welcome") {
				return Conversion::ToString(use_command_welcome);
			}
			if (ParamName == "allow_remote_config") {
				return Conversion::ToString(allow_remote_config);
			}
			if (ParamName == "use_default_responses") {
				return Conversion::ToString(use_default_responses);
			}

			return "";
		}
		
		virtual bool param_write(Ascii ParamName, Ascii Value)
		{
			if (ParamName == "prefix_key") { 
				prefix_key = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "output_colour") { 
				output_colour = Conversion::ToColour(Value);
				return true;
			}
			if (ParamName == "output_bold") { 
				output_bold = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "output_italic") { 
				output_italic = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "always_use_consoles") { 
				always_use_consoles = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "use_command_welcome") { 
				use_command_welcome = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "allow_remote_config") { 
				allow_remote_config = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "use_default_responses") { 
				use_default_responses = Conversion::ToBool(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		Umbra::Variant Umbra_GetProperty(Umbra::string ID, Umbra::ES_GetProperty& ES)
		{
			if (strcmp(ID.c_str(), "prefix_key"                  ) == 0) return (const char*)prefix_key;
			if (strcmp(ID.c_str(), "output_colour"               ) == 0) return output_colour.value;
			if (strcmp(ID.c_str(), "output_bold"                 ) == 0) return output_bold;
			if (strcmp(ID.c_str(), "output_italic"               ) == 0) return output_italic;
			if (strcmp(ID.c_str(), "always_use_consoles"         ) == 0) return always_use_consoles;
			if (strcmp(ID.c_str(), "use_command_welcome"         ) == 0) return use_command_welcome;
			if (strcmp(ID.c_str(), "allow_remote_config"         ) == 0) return allow_remote_config;
			if (strcmp(ID.c_str(), "use_default_responses"       ) == 0) return use_default_responses;

			
			// nothing found
			throw Umbra::UnsupportedPropertyException(ID);
		}	
	};

	
	class InterfacePropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pPrefixKey     ;
		ColourButton     *pOutputColour  ;
		CButton          *pOutputBold    ;
		CButton          *pOutputItalic  ;
		CButton          *pAlwaysUseConsoles;
		CButton          *pUseCommandWelcome;
		CButton          *pAllowRemoteConfig;
		CButton          *pUseDefaultResponses;

		
	public: // a set of the special property type
		InterfaceData data;
		InterfaceData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pPrefixKey      = PageCtrl.create_edit      (GroupID, "", "Prefix Key");
			pOutputColour   = PageCtrl.create_colour    (GroupID, "", "Output Colour");
			pOutputBold     = PageCtrl.create_check     (GroupID, "", "Output Bold");
			pOutputItalic   = PageCtrl.create_check     (GroupID, "", "Output Italic");
			pAlwaysUseConsoles = PageCtrl.create_check     (GroupID, "", "Always Use Consoles");
			pUseCommandWelcome = PageCtrl.create_check     (GroupID, "", "Use Command Welcome");
			pAllowRemoteConfig = PageCtrl.create_check     (GroupID, "", "Allow Remote Config");
			pUseDefaultResponses = PageCtrl.create_check     (GroupID, "", "Use Default Responses");

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
		
		void dialog_to_attributes(InterfaceData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
		}
		
		void attributes_to_dialog(InterfaceData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(InterfaceData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.prefix_key, (*pPrefixKey     ));
			exr.exchange(Attr.output_colour, (*pOutputColour  ));
			exr.exchange(Attr.output_bold, (*pOutputBold    ));
			exr.exchange(Attr.output_italic, (*pOutputItalic  ));
			exr.exchange(Attr.always_use_consoles, (*pAlwaysUseConsoles));
			exr.exchange(Attr.use_command_welcome, (*pUseCommandWelcome));
			exr.exchange(Attr.allow_remote_config, (*pAllowRemoteConfig));
			exr.exchange(Attr.use_default_responses, (*pUseDefaultResponses));

			
			if (Mode == DataExchanger::FROM_CONTROL)
			{
				Attr.DataUpdated();
			}
			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(InterfaceData& RemoteData)
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
} }