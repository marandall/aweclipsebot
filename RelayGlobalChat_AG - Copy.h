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
	class GlobalChatPropertyBuilder;

	class GlobalChatData : 
		public Umbra::RemoteClass,
		public IRawParamEditor
	{
	public: // type defenition for property builder
		typedef GlobalChatPropertyBuilder PropertyBuilder;
	
	public: // source data
		bool        use_broadcast_method;
		int         chat_range;
		Ascii       format;
		AW::Colour  default_citizen_colour;
		AW::Colour  default_tourist_colour;
		AW::Colour  default_bot_colour;
		bool        allow_citizen_colour_changes;
		bool        allow_tourist_colour_changes;
		int         max_white_percentage;
		bool        enable_impersonation;

		
	public:
		GlobalChatData()
		{
			use_broadcast_method = false;
			chat_range = 0;
			format = "{$name}:\t{$text}";
			default_citizen_colour = 0x772222;
			default_tourist_colour = 0x777777;
			allow_citizen_colour_changes = true;
			allow_tourist_colour_changes = true;
			max_white_percentage = 80;
			enable_impersonation = true;

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
			char* section_id = const_cast<char*>("relay_GlobalChat");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "use_broadcast_method", use_broadcast_method);
			SerializerSystem.Exchange(ExchangeMode, section_id, "chat_range", chat_range);
			SerializerSystem.Exchange(ExchangeMode, section_id, "format", format);
			SerializerSystem.Exchange(ExchangeMode, section_id, "default_citizen_colour", default_citizen_colour);
			SerializerSystem.Exchange(ExchangeMode, section_id, "default_tourist_colour", default_tourist_colour);
			SerializerSystem.Exchange(ExchangeMode, section_id, "default_bot_colour", default_bot_colour);
			SerializerSystem.Exchange(ExchangeMode, section_id, "allow_citizen_colour_changes", allow_citizen_colour_changes);
			SerializerSystem.Exchange(ExchangeMode, section_id, "allow_tourist_colour_changes", allow_tourist_colour_changes);
			SerializerSystem.Exchange(ExchangeMode, section_id, "max_white_percentage", max_white_percentage);
			SerializerSystem.Exchange(ExchangeMode, section_id, "enable_impersonation", enable_impersonation);

		}
		
		GlobalChatData& GetDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual void DataUpdated() { }
		
	public: // read only operators
		bool         get_UseBroadcastMethod            () const { return use_broadcast_method; }
		int          get_ChatRange                     () const { return chat_range; }
		Ascii        get_Format                        () const { return format; }
		AW::Colour   get_DefaultCitizenColour          () const { return default_citizen_colour; }
		AW::Colour   get_DefaultTouristColour          () const { return default_tourist_colour; }
		AW::Colour   get_DefaultBotColour              () const { return default_bot_colour; }
		bool         get_AllowCitizenColourChanges     () const { return allow_citizen_colour_changes; }
		bool         get_AllowTouristColourChanges     () const { return allow_tourist_colour_changes; }
		int          get_MaxWhitePercentage            () const { return max_white_percentage; }
		bool         get_EnableImpersonation           () const { return enable_impersonation; }

		virtual void set_UseBroadcastMethod            (bool         Assgn) { use_broadcast_method = Assgn; }
		virtual void set_ChatRange                     (int          Assgn) { chat_range = Assgn; }
		virtual void set_Format                        (Ascii        Assgn) { format = Assgn; }
		virtual void set_DefaultCitizenColour          (AW::Colour   Assgn) { default_citizen_colour = Assgn; }
		virtual void set_DefaultTouristColour          (AW::Colour   Assgn) { default_tourist_colour = Assgn; }
		virtual void set_DefaultBotColour              (AW::Colour   Assgn) { default_bot_colour = Assgn; }
		virtual void set_AllowCitizenColourChanges     (bool         Assgn) { allow_citizen_colour_changes = Assgn; }
		virtual void set_AllowTouristColourChanges     (bool         Assgn) { allow_tourist_colour_changes = Assgn; }
		virtual void set_MaxWhitePercentage            (int          Assgn) { max_white_percentage = Assgn; }
		virtual void set_EnableImpersonation           (bool         Assgn) { enable_impersonation = Assgn; }

		
	public: // string conversion
		virtual Ascii param_read(Ascii ParamName)
		{
			if (ParamName == "use_broadcast_method") {
				return Conversion::ToString(use_broadcast_method);
			}
			if (ParamName == "chat_range") {
				return Conversion::ToString(chat_range);
			}
			if (ParamName == "format") {
				return Conversion::ToString(format);
			}
			if (ParamName == "default_citizen_colour") {
				return Conversion::ToString(default_citizen_colour);
			}
			if (ParamName == "default_tourist_colour") {
				return Conversion::ToString(default_tourist_colour);
			}
			if (ParamName == "default_bot_colour") {
				return Conversion::ToString(default_bot_colour);
			}
			if (ParamName == "allow_citizen_colour_changes") {
				return Conversion::ToString(allow_citizen_colour_changes);
			}
			if (ParamName == "allow_tourist_colour_changes") {
				return Conversion::ToString(allow_tourist_colour_changes);
			}
			if (ParamName == "max_white_percentage") {
				return Conversion::ToString(max_white_percentage);
			}
			if (ParamName == "enable_impersonation") {
				return Conversion::ToString(enable_impersonation);
			}

			return "";
		}
		
		virtual bool param_write(Ascii ParamName, Ascii Value)
		{
			if (ParamName == "use_broadcast_method") { 
				use_broadcast_method = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "chat_range") { 
				chat_range = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "format") { 
				format = Conversion::ToString(Value);
				return true;
			}
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
			if (ParamName == "allow_citizen_colour_changes") { 
				allow_citizen_colour_changes = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "allow_tourist_colour_changes") { 
				allow_tourist_colour_changes = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "max_white_percentage") { 
				max_white_percentage = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "enable_impersonation") { 
				enable_impersonation = Conversion::ToBool(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		Umbra::Variant Umbra_GetProperty(Umbra::string ID, Umbra::ES_GetProperty& ES)
		{
			if (strcmp(ID.c_str(), "use_broadcast_method"        ) == 0) return use_broadcast_method;
			if (strcmp(ID.c_str(), "chat_range"                  ) == 0) return chat_range;
			if (strcmp(ID.c_str(), "format"                      ) == 0) return (const char*)format;
			if (strcmp(ID.c_str(), "default_citizen_colour"      ) == 0) return default_citizen_colour.value;
			if (strcmp(ID.c_str(), "default_tourist_colour"      ) == 0) return default_tourist_colour.value;
			if (strcmp(ID.c_str(), "default_bot_colour"          ) == 0) return default_bot_colour.value;
			if (strcmp(ID.c_str(), "allow_citizen_colour_changes") == 0) return allow_citizen_colour_changes;
			if (strcmp(ID.c_str(), "allow_tourist_colour_changes") == 0) return allow_tourist_colour_changes;
			if (strcmp(ID.c_str(), "max_white_percentage"        ) == 0) return max_white_percentage;
			if (strcmp(ID.c_str(), "enable_impersonation"        ) == 0) return enable_impersonation;

			
			// nothing found
			throw Umbra::UnsupportedPropertyException(ID);
		}	
	};

	
	class GlobalChatPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CButton          *pUseBroadcastMethod;
		TextBox          *pChatRange     ;
		TextBox          *pFormat        ;
		ColourButton     *pDefaultCitizenColour;
		ColourButton     *pDefaultTouristColour;
		ColourButton     *pDefaultBotColour;
		CButton          *pAllowCitizenColourChanges;
		CButton          *pAllowTouristColourChanges;
		TextBox          *pMaxWhitePercentage;
		CButton          *pEnableImpersonation;

		
	public: // a set of the special property type
		GlobalChatData data;
		GlobalChatData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pUseBroadcastMethod = PageCtrl.create_check     (GroupID, "", "Use Broadcast Method");
			pChatRange      = PageCtrl.create_edit      (GroupID, "", "Chat Range");
			pFormat         = PageCtrl.create_edit      (GroupID, "", "Format");
			pDefaultCitizenColour = PageCtrl.create_colour    (GroupID, "", "Default Citizen Colour");
			pDefaultTouristColour = PageCtrl.create_colour    (GroupID, "", "Default Tourist Colour");
			pDefaultBotColour = PageCtrl.create_colour    (GroupID, "", "Default Bot Colour");
			pAllowCitizenColourChanges = PageCtrl.create_check     (GroupID, "", "Allow Citizen Colour Changes");
			pAllowTouristColourChanges = PageCtrl.create_check     (GroupID, "", "Allow Tourist Colour Changes");
			pMaxWhitePercentage = PageCtrl.create_edit      (GroupID, "", "Max White Percentage");
			pEnableImpersonation = PageCtrl.create_check     (GroupID, "", "Enable Impersonation");

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
		
		void dialog_to_attributes(GlobalChatData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
		}
		
		void attributes_to_dialog(GlobalChatData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(GlobalChatData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.use_broadcast_method, (*pUseBroadcastMethod));
			exr.exchange(Attr.chat_range, (*pChatRange     ));
			exr.exchange(Attr.format, (*pFormat        ));
			exr.exchange(Attr.default_citizen_colour, (*pDefaultCitizenColour));
			exr.exchange(Attr.default_tourist_colour, (*pDefaultTouristColour));
			exr.exchange(Attr.default_bot_colour, (*pDefaultBotColour));
			exr.exchange(Attr.allow_citizen_colour_changes, (*pAllowCitizenColourChanges));
			exr.exchange(Attr.allow_tourist_colour_changes, (*pAllowTouristColourChanges));
			exr.exchange(Attr.max_white_percentage, (*pMaxWhitePercentage));
			exr.exchange(Attr.enable_impersonation, (*pEnableImpersonation));

			
			if (Mode == DataExchanger::FROM_CONTROL)
			{
				Attr.DataUpdated();
			}
			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(GlobalChatData& RemoteData)
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