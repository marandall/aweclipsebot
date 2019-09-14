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

namespace CIS { namespace Config { 
namespace AutoTemplates 
{
	class InterfacePropertyBuilder;

	//
	// Import Data
	//
	// a:7:{i:0;a:4:{s:4:"name";s:10:"prefix_key";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:13:"output_colour";s:4:"type";s:10:"aw::colour";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:11:"output_bold";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:13:"output_italic";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:19:"always_use_consoles";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:5;a:4:{s:4:"name";s:19:"use_command_welcome";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:6;a:4:{s:4:"name";s:21:"use_default_responses";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class InterfaceData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef InterfacePropertyBuilder PropertyBuilder;
	
	public: // source data
		String prefix_key;
		AW::Colour  output_colour;
		bool output_bold;
		bool output_italic;
		bool always_use_consoles;
		bool use_command_welcome;
		bool use_default_responses;

		
	public:
		InterfaceData()
		{
			prefix_key = "#";
			output_colour = 0xFF0000;
			output_bold = false;
			output_italic = false;
			always_use_consoles = false;
			use_command_welcome = false;
			use_default_responses = false;

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
			char* section_id = const_cast<char*>("cis_Interface");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "prefix_key", prefix_key);
			SerializerSystem.Exchange(ExchangeMode, section_id, "output_colour", output_colour);
			SerializerSystem.Exchange(ExchangeMode, section_id, "output_bold", output_bold);
			SerializerSystem.Exchange(ExchangeMode, section_id, "output_italic", output_italic);
			SerializerSystem.Exchange(ExchangeMode, section_id, "always_use_consoles", always_use_consoles);
			SerializerSystem.Exchange(ExchangeMode, section_id, "use_command_welcome", use_command_welcome);
			SerializerSystem.Exchange(ExchangeMode, section_id, "use_default_responses", use_default_responses);

		}
		
		InterfaceData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		String  getPrefixKey                     () const { return prefix_key; }
		AW::Colour   getOutputColour                  () const { return output_colour; }
		bool  getOutputBold                    () const { return output_bold; }
		bool  getOutputItalic                  () const { return output_italic; }
		bool  getAlwaysUseConsoles             () const { return always_use_consoles; }
		bool  getUseCommandWelcome             () const { return use_command_welcome; }
		bool  getUseDefaultResponses           () const { return use_default_responses; }

		virtual void setPrefixKey                     (String  Assgn) { prefix_key = Assgn; }
		virtual void setOutputColour                  (AW::Colour   Assgn) { output_colour = Assgn; }
		virtual void setOutputBold                    (bool  Assgn) { output_bold = Assgn; }
		virtual void setOutputItalic                  (bool  Assgn) { output_italic = Assgn; }
		virtual void setAlwaysUseConsoles             (bool  Assgn) { always_use_consoles = Assgn; }
		virtual void setUseCommandWelcome             (bool  Assgn) { use_command_welcome = Assgn; }
		virtual void setUseDefaultResponses           (bool  Assgn) { use_default_responses = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
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
			if (ParamName == "use_default_responses") {
				return Conversion::ToString(use_default_responses);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
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
			if (ParamName == "use_default_responses") { 
				use_default_responses = Conversion::ToBool(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(prefix_key, prefix_key);
			UMBRA_PROPERTY_RW(output_colour, output_colour);
			UMBRA_PROPERTY_RW(output_bold, output_bold);
			UMBRA_PROPERTY_RW(output_italic, output_italic);
			UMBRA_PROPERTY_RW(always_use_consoles, always_use_consoles);
			UMBRA_PROPERTY_RW(use_command_welcome, use_command_welcome);
			UMBRA_PROPERTY_RW(use_default_responses, use_default_responses);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
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
			pUseDefaultResponses = PageCtrl.create_check     (GroupID, "", "Use Default Responses");

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
		
		bool dialog_to_attributes(InterfaceData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
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
			exr.exchange(Attr.use_default_responses, (*pUseDefaultResponses));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(InterfaceData& RemoteData)
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