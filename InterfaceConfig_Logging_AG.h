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

namespace InterfaceConfig { 
namespace AutoTemplates 
{
	class LoggingPropertyBuilder;

	//
	// Import Data
	//
	// a:13:{i:0;a:4:{s:4:"name";s:11:"time_format";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:11:"date_format";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:15:"datetime_format";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:14:"use_local_time";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:14:"use_utc_offset";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:5;a:4:{s:4:"name";s:13:"display_lines";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:6;a:4:{s:4:"name";s:12:"show_avatars";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:7;a:4:{s:4:"name";s:9:"show_chat";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:8;a:4:{s:4:"name";s:13:"show_consoles";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:9;a:4:{s:4:"name";s:13:"show_botgrams";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:10;a:4:{s:4:"name";s:27:"show_protocol_botgrams_only";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:11;a:4:{s:4:"name";s:8:"show_aka";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:12;a:4:{s:4:"name";s:19:"show_aka_repetition";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class LoggingData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef LoggingPropertyBuilder PropertyBuilder;
	
	public: // source data
		String time_format;
		String date_format;
		String datetime_format;
		bool use_local_time;
		int         use_utc_offset;
		int         display_lines;
		bool show_avatars;
		bool show_chat;
		bool show_consoles;
		bool show_botgrams;
		bool show_protocol_botgrams_only;
		bool show_aka;
		int         show_aka_repetition;

		
	public:
		LoggingData()
		{
			time_format = "%H:%M:%S";
			date_format = "%H:%M:%S";
			datetime_format = "%d/%b/%Y %H:%M:%S";
			use_local_time = true;
			use_utc_offset = -2;
			display_lines = 5000;
			show_avatars = true;
			show_chat = true;
			show_consoles = true;
			show_botgrams = true;
			show_protocol_botgrams_only = true;
			show_aka = true;
			show_aka_repetition = 60 * 30;

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
			char* section_id = const_cast<char*>("ifg_Logging");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "time_format", time_format);
			SerializerSystem.Exchange(ExchangeMode, section_id, "date_format", date_format);
			SerializerSystem.Exchange(ExchangeMode, section_id, "datetime_format", datetime_format);
			SerializerSystem.Exchange(ExchangeMode, section_id, "use_local_time", use_local_time);
			SerializerSystem.Exchange(ExchangeMode, section_id, "use_utc_offset", use_utc_offset);
			SerializerSystem.Exchange(ExchangeMode, section_id, "display_lines", display_lines);
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_avatars", show_avatars);
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_chat", show_chat);
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_consoles", show_consoles);
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_botgrams", show_botgrams);
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_protocol_botgrams_only", show_protocol_botgrams_only);
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_aka", show_aka);
			SerializerSystem.Exchange(ExchangeMode, section_id, "show_aka_repetition", show_aka_repetition);

		}
		
		LoggingData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		String  getTimeFormat                    () const { return time_format; }
		String  getDateFormat                    () const { return date_format; }
		String  getDatetimeFormat                () const { return datetime_format; }
		bool  getUseLocalTime                  () const { return use_local_time; }
		int          getUseUtcOffset                  () const { return use_utc_offset; }
		int          getDisplayLines                  () const { return display_lines; }
		bool  getShowAvatars                   () const { return show_avatars; }
		bool  getShowChat                      () const { return show_chat; }
		bool  getShowConsoles                  () const { return show_consoles; }
		bool  getShowBotgrams                  () const { return show_botgrams; }
		bool  getShowProtocolBotgramsOnly      () const { return show_protocol_botgrams_only; }
		bool  getShowAka                       () const { return show_aka; }
		int          getShowAkaRepetition             () const { return show_aka_repetition; }

		virtual void setTimeFormat                    (String  Assgn) { time_format = Assgn; }
		virtual void setDateFormat                    (String  Assgn) { date_format = Assgn; }
		virtual void setDatetimeFormat                (String  Assgn) { datetime_format = Assgn; }
		virtual void setUseLocalTime                  (bool  Assgn) { use_local_time = Assgn; }
		virtual void setUseUtcOffset                  (int          Assgn) { use_utc_offset = Assgn; }
		virtual void setDisplayLines                  (int          Assgn) { display_lines = Assgn; }
		virtual void setShowAvatars                   (bool  Assgn) { show_avatars = Assgn; }
		virtual void setShowChat                      (bool  Assgn) { show_chat = Assgn; }
		virtual void setShowConsoles                  (bool  Assgn) { show_consoles = Assgn; }
		virtual void setShowBotgrams                  (bool  Assgn) { show_botgrams = Assgn; }
		virtual void setShowProtocolBotgramsOnly      (bool  Assgn) { show_protocol_botgrams_only = Assgn; }
		virtual void setShowAka                       (bool  Assgn) { show_aka = Assgn; }
		virtual void setShowAkaRepetition             (int          Assgn) { show_aka_repetition = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
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
			if (ParamName == "use_local_time") {
				return Conversion::ToString(use_local_time);
			}
			if (ParamName == "use_utc_offset") {
				return Conversion::ToString(use_utc_offset);
			}
			if (ParamName == "display_lines") {
				return Conversion::ToString(display_lines);
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
			if (ParamName == "show_aka") {
				return Conversion::ToString(show_aka);
			}
			if (ParamName == "show_aka_repetition") {
				return Conversion::ToString(show_aka_repetition);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
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
			if (ParamName == "use_local_time") { 
				use_local_time = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "use_utc_offset") { 
				use_utc_offset = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "display_lines") { 
				display_lines = Conversion::ToInt(Value);
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
			if (ParamName == "show_aka") { 
				show_aka = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "show_aka_repetition") { 
				show_aka_repetition = Conversion::ToInt(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(time_format, time_format);
			UMBRA_PROPERTY_RW(date_format, date_format);
			UMBRA_PROPERTY_RW(datetime_format, datetime_format);
			UMBRA_PROPERTY_RW(use_local_time, use_local_time);
			UMBRA_PROPERTY_RW(use_utc_offset, use_utc_offset);
			UMBRA_PROPERTY_RW(display_lines, display_lines);
			UMBRA_PROPERTY_RW(show_avatars, show_avatars);
			UMBRA_PROPERTY_RW(show_chat, show_chat);
			UMBRA_PROPERTY_RW(show_consoles, show_consoles);
			UMBRA_PROPERTY_RW(show_botgrams, show_botgrams);
			UMBRA_PROPERTY_RW(show_protocol_botgrams_only, show_protocol_botgrams_only);
			UMBRA_PROPERTY_RW(show_aka, show_aka);
			UMBRA_PROPERTY_RW(show_aka_repetition, show_aka_repetition);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class LoggingPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CStatic			 *pDatetime      ;
		TextBox          *pTimeFormat    ;
		TextBox          *pDateFormat    ;
		TextBox          *pDatetimeFormat;
		CButton          *pUseLocalTime  ;
		TextBox          *pUseUtcOffset  ;
		CStatic			 *pDisplay       ;
		TextBox          *pDisplayLines  ;
		CStatic			 *pEvents        ;
		CButton          *pShowAvatars   ;
		CButton          *pShowChat      ;
		CButton          *pShowConsoles  ;
		CButton          *pShowBotgrams  ;
		CButton          *pShowProtocolBotgramsOnly;
		CStatic			 *pAka           ;
		CButton          *pShowAka       ;
		TextBox          *pShowAkaRepetition;

		
	public: // a set of the special property type
		LoggingData data;
		LoggingData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pDatetime       = PageCtrl.create_label     (GroupID, "", "Date and Time");
			pTimeFormat     = PageCtrl.create_edit      (GroupID, "", "Time Format");
			pDateFormat     = PageCtrl.create_edit      (GroupID, "", "Date Format");
			pDatetimeFormat = PageCtrl.create_edit      (GroupID, "", "Datetime Format");
			pUseLocalTime   = PageCtrl.create_check     (GroupID, "", "Use Local Time");
			pUseUtcOffset   = PageCtrl.create_edit      (GroupID, "", "Use Utc Offset");
			pDisplay        = PageCtrl.create_label     (GroupID, "", "Display");
			pDisplayLines   = PageCtrl.create_edit      (GroupID, "", "Display Lines");
			pEvents         = PageCtrl.create_label     (GroupID, "", "Event Visibility");
			pShowAvatars    = PageCtrl.create_check     (GroupID, "", "Show Avatars");
			pShowChat       = PageCtrl.create_check     (GroupID, "", "Show Chat");
			pShowConsoles   = PageCtrl.create_check     (GroupID, "", "Show Consoles");
			pShowBotgrams   = PageCtrl.create_check     (GroupID, "", "Show Botgrams");
			pShowProtocolBotgramsOnly = PageCtrl.create_check     (GroupID, "", "Show Protocol Botgrams Only");
			pAka            = PageCtrl.create_label     (GroupID, "", "Historic IP Checking (aka AKA)");
			pShowAka        = PageCtrl.create_check     (GroupID, "", "Show Aka");
			pShowAkaRepetition = PageCtrl.create_edit      (GroupID, "", "Show Aka Repetition");

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
		
		bool dialog_to_attributes(LoggingData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
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
			exr.exchange(Attr.use_local_time, (*pUseLocalTime  ));
			exr.exchange(Attr.use_utc_offset, (*pUseUtcOffset  ));
			exr.exchange(Attr.display_lines, (*pDisplayLines  ));
			exr.exchange(Attr.show_avatars, (*pShowAvatars   ));
			exr.exchange(Attr.show_chat, (*pShowChat      ));
			exr.exchange(Attr.show_consoles, (*pShowConsoles  ));
			exr.exchange(Attr.show_botgrams, (*pShowBotgrams  ));
			exr.exchange(Attr.show_protocol_botgrams_only, (*pShowProtocolBotgramsOnly));
			exr.exchange(Attr.show_aka, (*pShowAka       ));
			exr.exchange(Attr.show_aka_repetition, (*pShowAkaRepetition));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(LoggingData& RemoteData)
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