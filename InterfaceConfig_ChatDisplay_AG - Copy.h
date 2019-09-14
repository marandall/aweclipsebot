#pragma once
#include "serializer.h"
#include "tsb.h"
#include "umbra_remote_class.h"
#include "raw_param_editor.h"
#include "PropertyScriptCtrl.h"
#include "GlobalDex.h"

namespace InterfaceConfig { 
namespace AutoTemplates 
{
	class ChatDisplayPropertyBuilder;

	class ChatDisplayData : 
		public Umbra::RemoteClass,
		public IRawParamEditor
	{
	public: // type defenition for property builder
		typedef ChatDisplayPropertyBuilder PropertyBuilder;
	
	public: // source data
		AW::Colour  default_citizen_colour;
		AW::Colour  default_tourist_colour;
		AW::Colour  default_bot_colour;
		bool        prefix_chat_with_time;
		bool        prefix_consoles_with_time;

		
	public:
		ChatDisplayData()
		{
			default_citizen_colour = 0x00;
			default_tourist_colour = 0x777777;
			default_bot_colour = 0x00;
			prefix_chat_with_time = false;
			prefix_consoles_with_time = false;

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
			char* section_id = const_cast<char*>("ifg_ChatDisplay");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "default_citizen_colour", default_citizen_colour);
			SerializerSystem.Exchange(ExchangeMode, section_id, "default_tourist_colour", default_tourist_colour);
			SerializerSystem.Exchange(ExchangeMode, section_id, "default_bot_colour", default_bot_colour);
			SerializerSystem.Exchange(ExchangeMode, section_id, "prefix_chat_with_time", prefix_chat_with_time);
			SerializerSystem.Exchange(ExchangeMode, section_id, "prefix_consoles_with_time", prefix_consoles_with_time);

		}
		
		ChatDisplayData& GetDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual void DataUpdated() { }
		
	public: // read only operators
		AW::Colour   get_DefaultCitizenColour          () const { return default_citizen_colour; }
		AW::Colour   get_DefaultTouristColour          () const { return default_tourist_colour; }
		AW::Colour   get_DefaultBotColour              () const { return default_bot_colour; }
		bool         get_PrefixChatWithTime            () const { return prefix_chat_with_time; }
		bool         get_PrefixConsolesWithTime        () const { return prefix_consoles_with_time; }

		virtual void set_DefaultCitizenColour          (AW::Colour   Assgn) { default_citizen_colour = Assgn; }
		virtual void set_DefaultTouristColour          (AW::Colour   Assgn) { default_tourist_colour = Assgn; }
		virtual void set_DefaultBotColour              (AW::Colour   Assgn) { default_bot_colour = Assgn; }
		virtual void set_PrefixChatWithTime            (bool         Assgn) { prefix_chat_with_time = Assgn; }
		virtual void set_PrefixConsolesWithTime        (bool         Assgn) { prefix_consoles_with_time = Assgn; }

		
	public: // string conversion
		virtual Ascii param_read(Ascii ParamName)
		{
			if (ParamName == "default_citizen_colour") {
				return Conversion::ToString(default_citizen_colour);
			}
			if (ParamName == "default_tourist_colour") {
				return Conversion::ToString(default_tourist_colour);
			}
			if (ParamName == "default_bot_colour") {
				return Conversion::ToString(default_bot_colour);
			}
			if (ParamName == "prefix_chat_with_time") {
				return Conversion::ToString(prefix_chat_with_time);
			}
			if (ParamName == "prefix_consoles_with_time") {
				return Conversion::ToString(prefix_consoles_with_time);
			}

			return "";
		}
		
		virtual bool param_write(Ascii ParamName, Ascii Value)
		{
			if (ParamName == "default_citizen_colour") { 
				default_citizen_colour = Conversion::ToColour(Value);
				return true;
			}
			if (ParamName == "default_tourist_colour") { 
				default_tourist_colour = Conversion::ToColour(Value);
				return true;
			}
			if (ParamName == "default_bot_colour") { 
				default_bot_colour = Conversion::ToColour(Value);
				return true;
			}
			if (ParamName == "prefix_chat_with_time") { 
				prefix_chat_with_time = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "prefix_consoles_with_time") { 
				prefix_consoles_with_time = Conversion::ToBool(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		Umbra::Variant Umbra_GetProperty(Umbra::string ID, Umbra::ES_GetProperty& ES)
		{
			if (strcmp(ID.c_str(), "default_citizen_colour"      ) == 0) return default_citizen_colour.value;
			if (strcmp(ID.c_str(), "default_tourist_colour"      ) == 0) return default_tourist_colour.value;
			if (strcmp(ID.c_str(), "default_bot_colour"          ) == 0) return default_bot_colour.value;
			if (strcmp(ID.c_str(), "prefix_chat_with_time"       ) == 0) return prefix_chat_with_time;
			if (strcmp(ID.c_str(), "prefix_consoles_with_time"   ) == 0) return prefix_consoles_with_time;

			
			// nothing found
			throw Umbra::UnsupportedPropertyException(ID);
		}	
	};

	
	class ChatDisplayPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		ColourButton     *pDefaultCitizenColour;
		ColourButton     *pDefaultTouristColour;
		ColourButton     *pDefaultBotColour;
		CButton          *pPrefixChatWithTime;
		CButton          *pPrefixConsolesWithTime;

		
	public: // a set of the special property type
		ChatDisplayData data;
		ChatDisplayData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pDefaultCitizenColour = PageCtrl.create_colour    (GroupID, "", "Default Citizen Colour");
			pDefaultTouristColour = PageCtrl.create_colour    (GroupID, "", "Default Tourist Colour");
			pDefaultBotColour = PageCtrl.create_colour    (GroupID, "", "Default Bot Colour");
			pPrefixChatWithTime = PageCtrl.create_check     (GroupID, "", "Prefix Chat With Time");
			pPrefixConsolesWithTime = PageCtrl.create_check     (GroupID, "", "Prefix Consoles With Time");

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
		
		void dialog_to_attributes(ChatDisplayData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
		}
		
		void attributes_to_dialog(ChatDisplayData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(ChatDisplayData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.default_citizen_colour, (*pDefaultCitizenColour));
			exr.exchange(Attr.default_tourist_colour, (*pDefaultTouristColour));
			exr.exchange(Attr.default_bot_colour, (*pDefaultBotColour));
			exr.exchange(Attr.prefix_chat_with_time, (*pPrefixChatWithTime));
			exr.exchange(Attr.prefix_consoles_with_time, (*pPrefixConsolesWithTime));

			
			if (Mode == DataExchanger::FROM_CONTROL)
			{
				Attr.DataUpdated();
			}
			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(ChatDisplayData& RemoteData)
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