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
	class GlobalChatPropertyBuilder;

	//
	// Import Data
	//
	// a:11:{i:0;a:4:{s:4:"name";s:20:"use_broadcast_method";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:10:"chat_range";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:19:"use_global_channels";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:6:"format";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:20:"enable_impersonation";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:5;a:4:{s:4:"name";s:22:"default_citizen_colour";s:4:"type";s:10:"aw::colour";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:6;a:4:{s:4:"name";s:22:"default_tourist_colour";s:4:"type";s:10:"aw::colour";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:7;a:4:{s:4:"name";s:18:"default_bot_colour";s:4:"type";s:10:"aw::colour";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:8;a:4:{s:4:"name";s:28:"allow_citizen_colour_changes";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:9;a:4:{s:4:"name";s:28:"allow_tourist_colour_changes";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:10;a:4:{s:4:"name";s:20:"max_white_percentage";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class GlobalChatData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef GlobalChatPropertyBuilder PropertyBuilder;
	
	public: // source data
		bool use_broadcast_method;
		int         chat_range;
		bool use_global_channels;
		String format;
		bool enable_impersonation;
		AW::Colour  default_citizen_colour;
		AW::Colour  default_tourist_colour;
		AW::Colour  default_bot_colour;
		bool allow_citizen_colour_changes;
		bool allow_tourist_colour_changes;
		int         max_white_percentage;

		
	public:
		GlobalChatData()
		{
			use_broadcast_method = false;
			chat_range = 0;
			use_global_channels = false;
			format = "{$name}:\t{$text}";
			enable_impersonation = true;
			default_citizen_colour = 0x772222;
			default_tourist_colour = 0x777777;
			allow_citizen_colour_changes = true;
			allow_tourist_colour_changes = true;
			max_white_percentage = 80;

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
			char* section_id = const_cast<char*>("relay_GlobalChat");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "use_broadcast_method", use_broadcast_method);
			SerializerSystem.Exchange(ExchangeMode, section_id, "chat_range", chat_range);
			SerializerSystem.Exchange(ExchangeMode, section_id, "use_global_channels", use_global_channels);
			SerializerSystem.Exchange(ExchangeMode, section_id, "format", format);
			SerializerSystem.Exchange(ExchangeMode, section_id, "enable_impersonation", enable_impersonation);
			SerializerSystem.Exchange(ExchangeMode, section_id, "default_citizen_colour", default_citizen_colour);
			SerializerSystem.Exchange(ExchangeMode, section_id, "default_tourist_colour", default_tourist_colour);
			SerializerSystem.Exchange(ExchangeMode, section_id, "default_bot_colour", default_bot_colour);
			SerializerSystem.Exchange(ExchangeMode, section_id, "allow_citizen_colour_changes", allow_citizen_colour_changes);
			SerializerSystem.Exchange(ExchangeMode, section_id, "allow_tourist_colour_changes", allow_tourist_colour_changes);
			SerializerSystem.Exchange(ExchangeMode, section_id, "max_white_percentage", max_white_percentage);

		}
		
		GlobalChatData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		bool  getUseBroadcastMethod            () const { return use_broadcast_method; }
		int          getChatRange                     () const { return chat_range; }
		bool  getUseGlobalChannels             () const { return use_global_channels; }
		String  getFormat                        () const { return format; }
		bool  getEnableImpersonation           () const { return enable_impersonation; }
		AW::Colour   getDefaultCitizenColour          () const { return default_citizen_colour; }
		AW::Colour   getDefaultTouristColour          () const { return default_tourist_colour; }
		AW::Colour   getDefaultBotColour              () const { return default_bot_colour; }
		bool  getAllowCitizenColourChanges     () const { return allow_citizen_colour_changes; }
		bool  getAllowTouristColourChanges     () const { return allow_tourist_colour_changes; }
		int          getMaxWhitePercentage            () const { return max_white_percentage; }

		virtual void setUseBroadcastMethod            (bool  Assgn) { use_broadcast_method = Assgn; }
		virtual void setChatRange                     (int          Assgn) { chat_range = Assgn; }
		virtual void setUseGlobalChannels             (bool  Assgn) { use_global_channels = Assgn; }
		virtual void setFormat                        (String  Assgn) { format = Assgn; }
		virtual void setEnableImpersonation           (bool  Assgn) { enable_impersonation = Assgn; }
		virtual void setDefaultCitizenColour          (AW::Colour   Assgn) { default_citizen_colour = Assgn; }
		virtual void setDefaultTouristColour          (AW::Colour   Assgn) { default_tourist_colour = Assgn; }
		virtual void setDefaultBotColour              (AW::Colour   Assgn) { default_bot_colour = Assgn; }
		virtual void setAllowCitizenColourChanges     (bool  Assgn) { allow_citizen_colour_changes = Assgn; }
		virtual void setAllowTouristColourChanges     (bool  Assgn) { allow_tourist_colour_changes = Assgn; }
		virtual void setMaxWhitePercentage            (int          Assgn) { max_white_percentage = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "use_broadcast_method") {
				return Conversion::ToString(use_broadcast_method);
			}
			if (ParamName == "chat_range") {
				return Conversion::ToString(chat_range);
			}
			if (ParamName == "use_global_channels") {
				return Conversion::ToString(use_global_channels);
			}
			if (ParamName == "format") {
				return Conversion::ToString(format);
			}
			if (ParamName == "enable_impersonation") {
				return Conversion::ToString(enable_impersonation);
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

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "use_broadcast_method") { 
				use_broadcast_method = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "chat_range") { 
				chat_range = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "use_global_channels") { 
				use_global_channels = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "format") { 
				format = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "enable_impersonation") { 
				enable_impersonation = Conversion::ToBool(Value);
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

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(use_broadcast_method, use_broadcast_method);
			UMBRA_PROPERTY_RW(chat_range, chat_range);
			UMBRA_PROPERTY_RW(use_global_channels, use_global_channels);
			UMBRA_PROPERTY_RW(format, format);
			UMBRA_PROPERTY_RW(enable_impersonation, enable_impersonation);
			UMBRA_PROPERTY_RW(default_citizen_colour, default_citizen_colour);
			UMBRA_PROPERTY_RW(default_tourist_colour, default_tourist_colour);
			UMBRA_PROPERTY_RW(default_bot_colour, default_bot_colour);
			UMBRA_PROPERTY_RW(allow_citizen_colour_changes, allow_citizen_colour_changes);
			UMBRA_PROPERTY_RW(allow_tourist_colour_changes, allow_tourist_colour_changes);
			UMBRA_PROPERTY_RW(max_white_percentage, max_white_percentage);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class GlobalChatPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CButton          *pUseBroadcastMethod;
		TextBox          *pChatRange     ;
		CButton          *pUseGlobalChannels;
		TextBox          *pFormat        ;
		CButton          *pEnableImpersonation;
		CStatic			 *pColors        ;
		ColourButton     *pDefaultCitizenColour;
		ColourButton     *pDefaultTouristColour;
		ColourButton     *pDefaultBotColour;
		CButton          *pAllowCitizenColourChanges;
		CButton          *pAllowTouristColourChanges;
		TextBox          *pMaxWhitePercentage;

		
	public: // a set of the special property type
		GlobalChatData data;
		GlobalChatData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pUseBroadcastMethod = PageCtrl.create_check     (GroupID, "", "Use Broadcast Method");
			pChatRange      = PageCtrl.create_edit      (GroupID, "", "Chat Range");
			pUseGlobalChannels = PageCtrl.create_check     (GroupID, "", "Use Global Channels");
			pFormat         = PageCtrl.create_edit      (GroupID, "", "Format");
			pEnableImpersonation = PageCtrl.create_check     (GroupID, "", "Enable Impersonation");
			pColors         = PageCtrl.create_label     (GroupID, "", "Colour Options");
			pDefaultCitizenColour = PageCtrl.create_colour    (GroupID, "", "Default Citizen Colour");
			pDefaultTouristColour = PageCtrl.create_colour    (GroupID, "", "Default Tourist Colour");
			pDefaultBotColour = PageCtrl.create_colour    (GroupID, "", "Default Bot Colour");
			pAllowCitizenColourChanges = PageCtrl.create_check     (GroupID, "", "Allow Citizen Colour Changes");
			pAllowTouristColourChanges = PageCtrl.create_check     (GroupID, "", "Allow Tourist Colour Changes");
			pMaxWhitePercentage = PageCtrl.create_edit      (GroupID, "", "Max White Percentage");

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
		
		bool dialog_to_attributes(GlobalChatData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
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
			exr.exchange(Attr.use_global_channels, (*pUseGlobalChannels));
			exr.exchange(Attr.format, (*pFormat        ));
			exr.exchange(Attr.enable_impersonation, (*pEnableImpersonation));
			exr.exchange(Attr.default_citizen_colour, (*pDefaultCitizenColour));
			exr.exchange(Attr.default_tourist_colour, (*pDefaultTouristColour));
			exr.exchange(Attr.default_bot_colour, (*pDefaultBotColour));
			exr.exchange(Attr.allow_citizen_colour_changes, (*pAllowCitizenColourChanges));
			exr.exchange(Attr.allow_tourist_colour_changes, (*pAllowTouristColourChanges));
			exr.exchange(Attr.max_white_percentage, (*pMaxWhitePercentage));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(GlobalChatData& RemoteData)
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