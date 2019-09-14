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
	class LoggingPropertyBuilder;

	class LoggingData : 
		public Umbra::RemoteClass,
		public IRawParamEditor
	{
	public: // type defenition for property builder
		typedef LoggingPropertyBuilder PropertyBuilder;
	
	public: // source data
		Ascii       time_format;
		Ascii       date_format;
		Ascii       datetime_format;
		bool        show_avatars;
		bool        show_chat;
		bool        show_consoles;
		bool        show_botgrams;
		bool        show_protocol_botgrams_only;

		
	public:
		LoggingData()
		{
			time_format = "%H:%M:%S";
			date_format = "%H:%M:%S";
			datetime_format = "%H:%M:%S";
			show_avatars = true;
			show_chat = true;
			show_consoles = true;
			show_botgrams = true;
			show_protocol_botgrams_only = true;

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
			char* section_id = const_cast<char*>("ifg_Logging");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "time_format", time_format);
			SerializerSystem.Exchange(ExchangeMode, section_id, "date_format", date_format);
			SerializerSystem.Exchange(ExchangeMode, section_id, "datetime_format", datetime_format);
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_avatars", show_avatars);
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_chat", show_chat);
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_consoles", show_consoles);
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_botgrams", show_botgrams);
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_protocol_botgrams_only", show_protocol_botgrams_only);

		}
		
		LoggingData& GetDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual void DataUpdated() { }
		
	public: // read only operators
		Ascii        get_TimeFormat                    () const { return time_format; }
		Ascii        get_DateFormat                    () const { return date_format; }
		Ascii        get_DatetimeFormat                () const { return datetime_format; }
		bool         get_ShowAvatars                   () const { return show_avatars; }
		bool         get_ShowChat                      () const { return show_chat; }
		bool         get_ShowConsoles                  () const { return show_consoles; }
		bool         get_ShowBotgrams                  () const { return show_botgrams; }
		bool         get_ShowProtocolBotgramsOnly      () const { return show_protocol_botgrams_only; }

		virtual void set_TimeFormat                    (Ascii        Assgn) { time_format = Assgn; }
		virtual void set_DateFormat                    (Ascii        Assgn) { date_format = Assgn; }
		virtual void set_DatetimeFormat                (Ascii        Assgn) { datetime_format = Assgn; }
		virtual void set_ShowAvatars                   (bool         Assgn) { show_avatars = Assgn; }
		virtual void set_ShowChat                      (bool         Assgn) { show_chat = Assgn; }
		virtual void set_ShowConsoles                  (bool         Assgn) { show_consoles = Assgn; }
		virtual void set_ShowBotgrams                  (bool         Assgn) { show_botgrams = Assgn; }
		virtual void set_ShowProtocolBotgramsOnly      (bool         Assgn) { show_protocol_botgrams_only = Assgn; }

		
	public: // string conversion
		virtual Ascii param_read(Ascii ParamName)
		{
			if (ParamName == "time_format") {
				return Conversion::ToString(time_format);
			}
			if (ParamName == "date_format") {
				return Conversion::ToString(date_format);
			}
			if (ParamName == "datetime_format") {
				return Conversion::ToString(datetime_format);
			}
			if (ParamName == "show_avatars") {
				return Conversion::ToString(show_avatars);
			}
			if (ParamName == "show_chat") {
				return Conversion::ToString(show_chat);
			}
			if (ParamName == "show_consoles") {
				return Conversion::ToString(show_consoles);
			}
			if (ParamName == "show_botgrams") {
				return Conversion::ToString(show_botgrams);
			}
			if (ParamName == "show_protocol_botgrams_only") {
				return Conversion::ToString(show_protocol_botgrams_only);
			}

			return "";
		}
		
		virtual bool param_write(Ascii ParamName, Ascii Value)
		{
			if (ParamName == "time_format") { 
				time_format = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "date_format") { 
				date_format = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "datetime_format") { 
				datetime_format = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "show_avatars") { 
				show_avatars = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "show_chat") { 
				show_chat = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "show_consoles") { 
				show_consoles = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "show_botgrams") { 
				show_botgrams = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "show_protocol_botgrams_only") { 
				show_protocol_botgrams_only = Conversion::ToBool(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		Umbra::Variant Umbra_GetProperty(Umbra::string ID, Umbra::ES_GetProperty& ES)
		{
			if (strcmp(ID.c_str(), "time_format"                 ) == 0) return (const char*)time_format;
			if (strcmp(ID.c_str(), "date_format"                 ) == 0) return (const char*)date_format;
			if (strcmp(ID.c_str(), "datetime_format"             ) == 0) return (const char*)datetime_format;
			if (strcmp(ID.c_str(), "show_avatars"                ) == 0) return show_avatars;
			if (strcmp(ID.c_str(), "show_chat"                   ) == 0) return show_chat;
			if (strcmp(ID.c_str(), "show_consoles"               ) == 0) return show_consoles;
			if (strcmp(ID.c_str(), "show_botgrams"               ) == 0) return show_botgrams;
			if (strcmp(ID.c_str(), "show_protocol_botgrams_only" ) == 0) return show_protocol_botgrams_only;

			
			// nothing found
			throw Umbra::UnsupportedPropertyException(ID);
		}	
	};

	
	class LoggingPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pTimeFormat    ;
		TextBox          *pDateFormat    ;
		TextBox          *pDatetimeFormat;
		CButton          *pShowAvatars   ;
		CButton          *pShowChat      ;
		CButton          *pShowConsoles  ;
		CButton          *pShowBotgrams  ;
		CButton          *pShowProtocolBotgramsOnly;

		
	public: // a set of the special property type
		LoggingData data;
		LoggingData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pTimeFormat     = PageCtrl.create_edit      (GroupID, "", "Time Format");
			pDateFormat     = PageCtrl.create_edit      (GroupID, "", "Date Format");
			pDatetimeFormat = PageCtrl.create_edit      (GroupID, "", "Datetime Format");
			pShowAvatars    = PageCtrl.create_check     (GroupID, "", "Show Avatars");
			pShowChat       = PageCtrl.create_check     (GroupID, "", "Show Chat");
			pShowConsoles   = PageCtrl.create_check     (GroupID, "", "Show Consoles");
			pShowBotgrams   = PageCtrl.create_check     (GroupID, "", "Show Botgrams");
			pShowProtocolBotgramsOnly = PageCtrl.create_check     (GroupID, "", "Show Protocol Botgrams Only");

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
		
		void dialog_to_attributes(LoggingData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
		}
		
		void attributes_to_dialog(LoggingData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(LoggingData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.time_format, (*pTimeFormat    ));
			exr.exchange(Attr.date_format, (*pDateFormat    ));
			exr.exchange(Attr.datetime_format, (*pDatetimeFormat));
			exr.exchange(Attr.show_avatars, (*pShowAvatars   ));
			exr.exchange(Attr.show_chat, (*pShowChat      ));
			exr.exchange(Attr.show_consoles, (*pShowConsoles  ));
			exr.exchange(Attr.show_botgrams, (*pShowBotgrams  ));
			exr.exchange(Attr.show_protocol_botgrams_only, (*pShowProtocolBotgramsOnly));

			
			if (Mode == DataExchanger::FROM_CONTROL)
			{
				Attr.DataUpdated();
			}
			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(LoggingData& RemoteData)
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