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
	class AdvancedPropertyBuilder;

	//
	// Import Data
	//
	// a:24:{i:0;a:4:{s:4:"name";s:22:"server_neighbour_range";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:9:"enable_ps";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:22:"default_citizen_colour";s:4:"type";s:10:"aw::colour";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:22:"default_tourist_colour";s:4:"type";s:10:"aw::colour";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:18:"default_bot_colour";s:4:"type";s:10:"aw::colour";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:5;a:4:{s:4:"name";s:16:"restore_sessions";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:6;a:4:{s:4:"name";s:26:"allow_user_channel_options";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:7;a:4:{s:4:"name";s:10:"chat_range";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:8;a:4:{s:4:"name";s:20:"default_channel_mode";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:9;a:4:{s:4:"name";s:18:"default_first_join";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:10;a:4:{s:4:"name";s:24:"idle_channel_persistance";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:11;a:4:{s:4:"name";s:27:"maximum_channels_per_person";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:12;a:4:{s:4:"name";s:20:"channel_notify_color";s:4:"type";s:10:"aw::colour";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:13;a:4:{s:4:"name";s:19:"channel_notify_bold";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:14;a:4:{s:4:"name";s:21:"channel_notify_italic";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:15;a:4:{s:4:"name";s:19:"announce_by_default";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:16;a:4:{s:4:"name";s:20:"channel_join_message";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:17;a:4:{s:4:"name";s:20:"channel_quit_message";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:18;a:4:{s:4:"name";s:21:"default_channel_color";s:4:"type";s:10:"aw::colour";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:19;a:4:{s:4:"name";s:17:"alt_channel_color";s:4:"type";s:10:"aw::colour";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:20;a:4:{s:4:"name";s:21:"channel_format_prefix";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:21;a:4:{s:4:"name";s:22:"channel_format_message";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:22;a:4:{s:4:"name";s:26:"channel_format_alt_message";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:23;a:4:{s:4:"name";s:22:"text_appears_backwards";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class AdvancedData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef AdvancedPropertyBuilder PropertyBuilder;
	
	public: // source data
		int         server_neighbour_range;
		bool enable_ps;
		AW::Colour  default_citizen_colour;
		AW::Colour  default_tourist_colour;
		AW::Colour  default_bot_colour;
		bool restore_sessions;
		bool allow_user_channel_options;
		int         chat_range;
		int         default_channel_mode;
		bool default_first_join;
		int         idle_channel_persistance;
		int         maximum_channels_per_person;
		AW::Colour  channel_notify_color;
		bool channel_notify_bold;
		bool channel_notify_italic;
		bool announce_by_default;
		String channel_join_message;
		String channel_quit_message;
		AW::Colour  default_channel_color;
		AW::Colour  alt_channel_color;
		String channel_format_prefix;
		String channel_format_message;
		String channel_format_alt_message;
		bool text_appears_backwards;

		
	public:
		AdvancedData()
		{
			server_neighbour_range = 20000;
			enable_ps = true;
			default_citizen_colour = 0x772222;
			default_tourist_colour = 0x777777;
			restore_sessions = true;
			allow_user_channel_options = true;
			chat_range = 200;
			default_channel_mode = 0;
			default_first_join = true;
			idle_channel_persistance = 10800;
			maximum_channels_per_person = 5;
			channel_notify_color = 0x777777;
			channel_notify_bold = false;
			channel_notify_italic = false;
			announce_by_default = true;
			channel_join_message = "{$user.name} has joined {$channelname}";
			channel_quit_message = "{$user.name} has quit {$channelname}";
			default_channel_color = 0x007722;
			alt_channel_color = 0x227700;
			channel_format_prefix = "{$user.name}";
			channel_format_message = "{$message}";
			channel_format_alt_message = "- {$channelname} - {$message}";
			text_appears_backwards = false;

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
			char* section_id = const_cast<char*>("relay_Advanced");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "server_neighbour_range", server_neighbour_range);
			SerializerSystem.Exchange(ExchangeMode, section_id, "enable_ps", enable_ps);
			SerializerSystem.Exchange(ExchangeMode, section_id, "default_citizen_colour", default_citizen_colour);
			SerializerSystem.Exchange(ExchangeMode, section_id, "default_tourist_colour", default_tourist_colour);
			SerializerSystem.Exchange(ExchangeMode, section_id, "default_bot_colour", default_bot_colour);
			SerializerSystem.Exchange(ExchangeMode, section_id, "restore_sessions", restore_sessions);
			SerializerSystem.Exchange(ExchangeMode, section_id, "allow_user_channel_options", allow_user_channel_options);
			SerializerSystem.Exchange(ExchangeMode, section_id, "chat_range", chat_range);
			SerializerSystem.Exchange(ExchangeMode, section_id, "default_channel_mode", default_channel_mode);
			SerializerSystem.Exchange(ExchangeMode, section_id, "default_first_join", default_first_join);
			SerializerSystem.Exchange(ExchangeMode, section_id, "idle_channel_persistance", idle_channel_persistance);
			SerializerSystem.Exchange(ExchangeMode, section_id, "maximum_channels_per_person", maximum_channels_per_person);
			SerializerSystem.Exchange(ExchangeMode, section_id, "channel_notify_color", channel_notify_color);
			SerializerSystem.Exchange(ExchangeMode, section_id, "channel_notify_bold", channel_notify_bold);
			SerializerSystem.Exchange(ExchangeMode, section_id, "channel_notify_italic", channel_notify_italic);
			SerializerSystem.Exchange(ExchangeMode, section_id, "announce_by_default", announce_by_default);
			SerializerSystem.Exchange(ExchangeMode, section_id, "channel_join_message", channel_join_message);
			SerializerSystem.Exchange(ExchangeMode, section_id, "channel_quit_message", channel_quit_message);
			SerializerSystem.Exchange(ExchangeMode, section_id, "default_channel_color", default_channel_color);
			SerializerSystem.Exchange(ExchangeMode, section_id, "alt_channel_color", alt_channel_color);
			SerializerSystem.Exchange(ExchangeMode, section_id, "channel_format_prefix", channel_format_prefix);
			SerializerSystem.Exchange(ExchangeMode, section_id, "channel_format_message", channel_format_message);
			SerializerSystem.Exchange(ExchangeMode, section_id, "channel_format_alt_message", channel_format_alt_message);
			SerializerSystem.Exchange(ExchangeMode, section_id, "text_appears_backwards", text_appears_backwards);

		}
		
		AdvancedData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		int          getServerNeighbourRange          () const { return server_neighbour_range; }
		bool  getEnablePs                      () const { return enable_ps; }
		AW::Colour   getDefaultCitizenColour          () const { return default_citizen_colour; }
		AW::Colour   getDefaultTouristColour          () const { return default_tourist_colour; }
		AW::Colour   getDefaultBotColour              () const { return default_bot_colour; }
		bool  getRestoreSessions               () const { return restore_sessions; }
		bool  getAllowUserChannelOptions       () const { return allow_user_channel_options; }
		int          getChatRange                     () const { return chat_range; }
		int          getDefaultChannelMode            () const { return default_channel_mode; }
		bool  getDefaultFirstJoin              () const { return default_first_join; }
		int          getIdleChannelPersistance        () const { return idle_channel_persistance; }
		int          getMaximumChannelsPerPerson      () const { return maximum_channels_per_person; }
		AW::Colour   getChannelNotifyColor            () const { return channel_notify_color; }
		bool  getChannelNotifyBold             () const { return channel_notify_bold; }
		bool  getChannelNotifyItalic           () const { return channel_notify_italic; }
		bool  getAnnounceByDefault             () const { return announce_by_default; }
		String  getChannelJoinMessage            () const { return channel_join_message; }
		String  getChannelQuitMessage            () const { return channel_quit_message; }
		AW::Colour   getDefaultChannelColor           () const { return default_channel_color; }
		AW::Colour   getAltChannelColor               () const { return alt_channel_color; }
		String  getChannelFormatPrefix           () const { return channel_format_prefix; }
		String  getChannelFormatMessage          () const { return channel_format_message; }
		String  getChannelFormatAltMessage       () const { return channel_format_alt_message; }
		bool  getTextAppearsBackwards          () const { return text_appears_backwards; }

		virtual void setServerNeighbourRange          (int          Assgn) { server_neighbour_range = Assgn; }
		virtual void setEnablePs                      (bool  Assgn) { enable_ps = Assgn; }
		virtual void setDefaultCitizenColour          (AW::Colour   Assgn) { default_citizen_colour = Assgn; }
		virtual void setDefaultTouristColour          (AW::Colour   Assgn) { default_tourist_colour = Assgn; }
		virtual void setDefaultBotColour              (AW::Colour   Assgn) { default_bot_colour = Assgn; }
		virtual void setRestoreSessions               (bool  Assgn) { restore_sessions = Assgn; }
		virtual void setAllowUserChannelOptions       (bool  Assgn) { allow_user_channel_options = Assgn; }
		virtual void setChatRange                     (int          Assgn) { chat_range = Assgn; }
		virtual void setDefaultChannelMode            (int          Assgn) { default_channel_mode = Assgn; }
		virtual void setDefaultFirstJoin              (bool  Assgn) { default_first_join = Assgn; }
		virtual void setIdleChannelPersistance        (int          Assgn) { idle_channel_persistance = Assgn; }
		virtual void setMaximumChannelsPerPerson      (int          Assgn) { maximum_channels_per_person = Assgn; }
		virtual void setChannelNotifyColor            (AW::Colour   Assgn) { channel_notify_color = Assgn; }
		virtual void setChannelNotifyBold             (bool  Assgn) { channel_notify_bold = Assgn; }
		virtual void setChannelNotifyItalic           (bool  Assgn) { channel_notify_italic = Assgn; }
		virtual void setAnnounceByDefault             (bool  Assgn) { announce_by_default = Assgn; }
		virtual void setChannelJoinMessage            (String  Assgn) { channel_join_message = Assgn; }
		virtual void setChannelQuitMessage            (String  Assgn) { channel_quit_message = Assgn; }
		virtual void setDefaultChannelColor           (AW::Colour   Assgn) { default_channel_color = Assgn; }
		virtual void setAltChannelColor               (AW::Colour   Assgn) { alt_channel_color = Assgn; }
		virtual void setChannelFormatPrefix           (String  Assgn) { channel_format_prefix = Assgn; }
		virtual void setChannelFormatMessage          (String  Assgn) { channel_format_message = Assgn; }
		virtual void setChannelFormatAltMessage       (String  Assgn) { channel_format_alt_message = Assgn; }
		virtual void setTextAppearsBackwards          (bool  Assgn) { text_appears_backwards = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "server_neighbour_range") {
				return Conversion::ToString(server_neighbour_range);
			}
			if (ParamName == "enable_ps") {
				return Conversion::ToString(enable_ps);
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
			if (ParamName == "restore_sessions") {
				return Conversion::ToString(restore_sessions);
			}
			if (ParamName == "allow_user_channel_options") {
				return Conversion::ToString(allow_user_channel_options);
			}
			if (ParamName == "chat_range") {
				return Conversion::ToString(chat_range);
			}
			if (ParamName == "default_channel_mode") {
				return Conversion::ToString(default_channel_mode);
			}
			if (ParamName == "default_first_join") {
				return Conversion::ToString(default_first_join);
			}
			if (ParamName == "idle_channel_persistance") {
				return Conversion::ToString(idle_channel_persistance);
			}
			if (ParamName == "maximum_channels_per_person") {
				return Conversion::ToString(maximum_channels_per_person);
			}
			if (ParamName == "channel_notify_color") {
				return Conversion::ToString(channel_notify_color);
			}
			if (ParamName == "channel_notify_bold") {
				return Conversion::ToString(channel_notify_bold);
			}
			if (ParamName == "channel_notify_italic") {
				return Conversion::ToString(channel_notify_italic);
			}
			if (ParamName == "announce_by_default") {
				return Conversion::ToString(announce_by_default);
			}
			if (ParamName == "channel_join_message") {
				return Conversion::ToString(channel_join_message);
			}
			if (ParamName == "channel_quit_message") {
				return Conversion::ToString(channel_quit_message);
			}
			if (ParamName == "default_channel_color") {
				return Conversion::ToString(default_channel_color);
			}
			if (ParamName == "alt_channel_color") {
				return Conversion::ToString(alt_channel_color);
			}
			if (ParamName == "channel_format_prefix") {
				return Conversion::ToString(channel_format_prefix);
			}
			if (ParamName == "channel_format_message") {
				return Conversion::ToString(channel_format_message);
			}
			if (ParamName == "channel_format_alt_message") {
				return Conversion::ToString(channel_format_alt_message);
			}
			if (ParamName == "text_appears_backwards") {
				return Conversion::ToString(text_appears_backwards);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "server_neighbour_range") { 
				server_neighbour_range = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "enable_ps") { 
				enable_ps = Conversion::ToBool(Value);
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
			if (ParamName == "restore_sessions") { 
				restore_sessions = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "allow_user_channel_options") { 
				allow_user_channel_options = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "chat_range") { 
				chat_range = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "default_channel_mode") { 
				default_channel_mode = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "default_first_join") { 
				default_first_join = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "idle_channel_persistance") { 
				idle_channel_persistance = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "maximum_channels_per_person") { 
				maximum_channels_per_person = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "channel_notify_color") { 
				channel_notify_color = Conversion::ToColour(Value);
				return true;
			}
			if (ParamName == "channel_notify_bold") { 
				channel_notify_bold = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "channel_notify_italic") { 
				channel_notify_italic = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "announce_by_default") { 
				announce_by_default = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "channel_join_message") { 
				channel_join_message = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "channel_quit_message") { 
				channel_quit_message = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "default_channel_color") { 
				default_channel_color = Conversion::ToColour(Value);
				return true;
			}
			if (ParamName == "alt_channel_color") { 
				alt_channel_color = Conversion::ToColour(Value);
				return true;
			}
			if (ParamName == "channel_format_prefix") { 
				channel_format_prefix = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "channel_format_message") { 
				channel_format_message = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "channel_format_alt_message") { 
				channel_format_alt_message = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "text_appears_backwards") { 
				text_appears_backwards = Conversion::ToBool(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(server_neighbour_range, server_neighbour_range);
			UMBRA_PROPERTY_RW(enable_ps, enable_ps);
			UMBRA_PROPERTY_RW(default_citizen_colour, default_citizen_colour);
			UMBRA_PROPERTY_RW(default_tourist_colour, default_tourist_colour);
			UMBRA_PROPERTY_RW(default_bot_colour, default_bot_colour);
			UMBRA_PROPERTY_RW(restore_sessions, restore_sessions);
			UMBRA_PROPERTY_RW(allow_user_channel_options, allow_user_channel_options);
			UMBRA_PROPERTY_RW(chat_range, chat_range);
			UMBRA_PROPERTY_RW(default_channel_mode, default_channel_mode);
			UMBRA_PROPERTY_RW(default_first_join, default_first_join);
			UMBRA_PROPERTY_RW(idle_channel_persistance, idle_channel_persistance);
			UMBRA_PROPERTY_RW(maximum_channels_per_person, maximum_channels_per_person);
			UMBRA_PROPERTY_RW(channel_notify_color, channel_notify_color);
			UMBRA_PROPERTY_RW(channel_notify_bold, channel_notify_bold);
			UMBRA_PROPERTY_RW(channel_notify_italic, channel_notify_italic);
			UMBRA_PROPERTY_RW(announce_by_default, announce_by_default);
			UMBRA_PROPERTY_RW(channel_join_message, channel_join_message);
			UMBRA_PROPERTY_RW(channel_quit_message, channel_quit_message);
			UMBRA_PROPERTY_RW(default_channel_color, default_channel_color);
			UMBRA_PROPERTY_RW(alt_channel_color, alt_channel_color);
			UMBRA_PROPERTY_RW(channel_format_prefix, channel_format_prefix);
			UMBRA_PROPERTY_RW(channel_format_message, channel_format_message);
			UMBRA_PROPERTY_RW(channel_format_alt_message, channel_format_alt_message);
			UMBRA_PROPERTY_RW(text_appears_backwards, text_appears_backwards);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class AdvancedPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CStatic			 *pL1            ;
		TextBox          *pServerNeighbourRange;
		CStatic			 *pL2            ;
		CButton          *pEnablePs      ;
		ColourButton     *pDefaultCitizenColour;
		ColourButton     *pDefaultTouristColour;
		ColourButton     *pDefaultBotColour;
		CStatic			 *pL3a           ;
		CButton          *pRestoreSessions;
		CButton          *pAllowUserChannelOptions;
		CStatic			 *pL3            ;
		TextBox          *pChatRange     ;
		ComboBox         *pDefaultChannelMode;
		CStatic			 *pL4            ;
		CButton          *pDefaultFirstJoin;
		TextBox          *pIdleChannelPersistance;
		TextBox          *pMaximumChannelsPerPerson;
		CStatic			 *pL5            ;
		ColourButton     *pChannelNotifyColor;
		CButton          *pChannelNotifyBold;
		CButton          *pChannelNotifyItalic;
		CButton          *pAnnounceByDefault;
		TextBox          *pChannelJoinMessage;
		TextBox          *pChannelQuitMessage;
		ColourButton     *pDefaultChannelColor;
		ColourButton     *pAltChannelColor;
		TextBox          *pChannelFormatPrefix;
		TextBox          *pChannelFormatMessage;
		TextBox          *pChannelFormatAltMessage;
		CStatic			 *pL6            ;
		CButton          *pTextAppearsBackwards;

		
	public: // a set of the special property type
		AdvancedData data;
		AdvancedData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pL1             = PageCtrl.create_label     (GroupID, "", "Server Options");
			pServerNeighbourRange = PageCtrl.create_edit      (GroupID, "", "Server Neighbour Range");
			pL2             = PageCtrl.create_label     (GroupID, "", "Style Options");
			pEnablePs       = PageCtrl.create_check     (GroupID, "", "Enable Ps");
			pDefaultCitizenColour = PageCtrl.create_colour    (GroupID, "", "Default Citizen Colour");
			pDefaultTouristColour = PageCtrl.create_colour    (GroupID, "", "Default Tourist Colour");
			pDefaultBotColour = PageCtrl.create_colour    (GroupID, "", "Default Bot Colour");
			pL3a            = PageCtrl.create_label     (GroupID, "", "Triggers");
			pRestoreSessions = PageCtrl.create_check     (GroupID, "", "Restore Sessions");
			pAllowUserChannelOptions = PageCtrl.create_check     (GroupID, "", "Allow User Channel Options");
			pL3             = PageCtrl.create_label     (GroupID, "", "Extender Information");
			pChatRange      = PageCtrl.create_edit      (GroupID, "", "Chat Range");
			pDefaultChannelMode = PageCtrl.create_combo     (GroupID, "", "Default Channel Mode");
			pL4             = PageCtrl.create_label     (GroupID, "", "Channel Information");
			pDefaultFirstJoin = PageCtrl.create_check     (GroupID, "", "Default First Join");
			pIdleChannelPersistance = PageCtrl.create_edit      (GroupID, "", "Idle Channel Persistance");
			pMaximumChannelsPerPerson = PageCtrl.create_edit      (GroupID, "", "Maximum Channels Per Person");
			pL5             = PageCtrl.create_label     (GroupID, "", "Channel Configuration");
			pChannelNotifyColor = PageCtrl.create_colour    (GroupID, "", "Channel Notify Color");
			pChannelNotifyBold = PageCtrl.create_check     (GroupID, "", "Channel Notify Bold");
			pChannelNotifyItalic = PageCtrl.create_check     (GroupID, "", "Channel Notify Italic");
			pAnnounceByDefault = PageCtrl.create_check     (GroupID, "", "Announce By Default");
			pChannelJoinMessage = PageCtrl.create_edit      (GroupID, "", "Channel Join Message");
			pChannelQuitMessage = PageCtrl.create_edit      (GroupID, "", "Channel Quit Message");
			pDefaultChannelColor = PageCtrl.create_colour    (GroupID, "", "Default Channel Color");
			pAltChannelColor = PageCtrl.create_colour    (GroupID, "", "Alt Channel Color");
			pChannelFormatPrefix = PageCtrl.create_edit      (GroupID, "", "Channel Format Prefix");
			pChannelFormatMessage = PageCtrl.create_edit      (GroupID, "", "Channel Format Message");
			pChannelFormatAltMessage = PageCtrl.create_edit      (GroupID, "", "Channel Format Alt Message");
			pL6             = PageCtrl.create_label     (GroupID, "", "Modifications");
			pTextAppearsBackwards = PageCtrl.create_check     (GroupID, "", "Text Appears Backwards");

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
		
		bool dialog_to_attributes(AdvancedData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(AdvancedData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(AdvancedData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.server_neighbour_range, (*pServerNeighbourRange));
			exr.exchange(Attr.enable_ps, (*pEnablePs      ));
			exr.exchange(Attr.default_citizen_colour, (*pDefaultCitizenColour));
			exr.exchange(Attr.default_tourist_colour, (*pDefaultTouristColour));
			exr.exchange(Attr.default_bot_colour, (*pDefaultBotColour));
			exr.exchange(Attr.restore_sessions, (*pRestoreSessions));
			exr.exchange(Attr.allow_user_channel_options, (*pAllowUserChannelOptions));
			exr.exchange(Attr.chat_range, (*pChatRange     ));
			exr.exchange(Attr.default_channel_mode, (*pDefaultChannelMode));
			exr.exchange(Attr.default_first_join, (*pDefaultFirstJoin));
			exr.exchange(Attr.idle_channel_persistance, (*pIdleChannelPersistance));
			exr.exchange(Attr.maximum_channels_per_person, (*pMaximumChannelsPerPerson));
			exr.exchange(Attr.channel_notify_color, (*pChannelNotifyColor));
			exr.exchange(Attr.channel_notify_bold, (*pChannelNotifyBold));
			exr.exchange(Attr.channel_notify_italic, (*pChannelNotifyItalic));
			exr.exchange(Attr.announce_by_default, (*pAnnounceByDefault));
			exr.exchange(Attr.channel_join_message, (*pChannelJoinMessage));
			exr.exchange(Attr.channel_quit_message, (*pChannelQuitMessage));
			exr.exchange(Attr.default_channel_color, (*pDefaultChannelColor));
			exr.exchange(Attr.alt_channel_color, (*pAltChannelColor));
			exr.exchange(Attr.channel_format_prefix, (*pChannelFormatPrefix));
			exr.exchange(Attr.channel_format_message, (*pChannelFormatMessage));
			exr.exchange(Attr.channel_format_alt_message, (*pChannelFormatAltMessage));
			exr.exchange(Attr.text_appears_backwards, (*pTextAppearsBackwards));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(AdvancedData& RemoteData)
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