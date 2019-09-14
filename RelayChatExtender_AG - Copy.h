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
	class ChatExtenderPropertyBuilder;

	class ChatExtenderData : 
		public Umbra::RemoteClass,
		public IRawParamEditor
	{
	public: // type defenition for property builder
		typedef ChatExtenderPropertyBuilder PropertyBuilder;
	
	public: // source data
		bool        automatically_join;
		bool        allow_passive_selection;
		bool        force_bots_passive;
		AW::Colour  standard_colour;
		AW::Colour  passive_colour;
		bool        use_hud_icons;

		
	public:
		ChatExtenderData()
		{
			automatically_join = true;
			allow_passive_selection = true;
			force_bots_passive = false;
			standard_colour = 0x772277;
			passive_colour = 0x882222;
			use_hud_icons = false;

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
			char* section_id = const_cast<char*>("relay_ChatExtender");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "automatically_join", automatically_join);
			SerializerSystem.Exchange(ExchangeMode, section_id, "allow_passive_selection", allow_passive_selection);
			SerializerSystem.Exchange(ExchangeMode, section_id, "force_bots_passive", force_bots_passive);
			SerializerSystem.Exchange(ExchangeMode, section_id, "standard_colour", standard_colour);
			SerializerSystem.Exchange(ExchangeMode, section_id, "passive_colour", passive_colour);
			SerializerSystem.Exchange(ExchangeMode, section_id, "use_hud_icons", use_hud_icons);

		}
		
		ChatExtenderData& GetDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual void DataUpdated() { }
		
	public: // read only operators
		bool         get_AutomaticallyJoin             () const { return automatically_join; }
		bool         get_AllowPassiveSelection         () const { return allow_passive_selection; }
		bool         get_ForceBotsPassive              () const { return force_bots_passive; }
		AW::Colour   get_StandardColour                () const { return standard_colour; }
		AW::Colour   get_PassiveColour                 () const { return passive_colour; }
		bool         get_UseHudIcons                   () const { return use_hud_icons; }

		virtual void set_AutomaticallyJoin             (bool         Assgn) { automatically_join = Assgn; }
		virtual void set_AllowPassiveSelection         (bool         Assgn) { allow_passive_selection = Assgn; }
		virtual void set_ForceBotsPassive              (bool         Assgn) { force_bots_passive = Assgn; }
		virtual void set_StandardColour                (AW::Colour   Assgn) { standard_colour = Assgn; }
		virtual void set_PassiveColour                 (AW::Colour   Assgn) { passive_colour = Assgn; }
		virtual void set_UseHudIcons                   (bool         Assgn) { use_hud_icons = Assgn; }

		
	public: // string conversion
		virtual Ascii param_read(Ascii ParamName)
		{
			if (ParamName == "automatically_join") {
				return Conversion::ToString(automatically_join);
			}
			if (ParamName == "allow_passive_selection") {
				return Conversion::ToString(allow_passive_selection);
			}
			if (ParamName == "force_bots_passive") {
				return Conversion::ToString(force_bots_passive);
			}
			if (ParamName == "standard_colour") {
				return Conversion::ToString(standard_colour);
			}
			if (ParamName == "passive_colour") {
				return Conversion::ToString(passive_colour);
			}
			if (ParamName == "use_hud_icons") {
				return Conversion::ToString(use_hud_icons);
			}

			return "";
		}
		
		virtual bool param_write(Ascii ParamName, Ascii Value)
		{
			if (ParamName == "automatically_join") { 
				automatically_join = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "allow_passive_selection") { 
				allow_passive_selection = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "force_bots_passive") { 
				force_bots_passive = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "standard_colour") { 
				standard_colour = Conversion::ToColour(Value);
				return true;
			}
			if (ParamName == "passive_colour") { 
				passive_colour = Conversion::ToColour(Value);
				return true;
			}
			if (ParamName == "use_hud_icons") { 
				use_hud_icons = Conversion::ToBool(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		Umbra::Variant Umbra_GetProperty(Umbra::string ID, Umbra::ES_GetProperty& ES)
		{
			if (strcmp(ID.c_str(), "automatically_join"          ) == 0) return automatically_join;
			if (strcmp(ID.c_str(), "allow_passive_selection"     ) == 0) return allow_passive_selection;
			if (strcmp(ID.c_str(), "force_bots_passive"          ) == 0) return force_bots_passive;
			if (strcmp(ID.c_str(), "standard_colour"             ) == 0) return standard_colour.value;
			if (strcmp(ID.c_str(), "passive_colour"              ) == 0) return passive_colour.value;
			if (strcmp(ID.c_str(), "use_hud_icons"               ) == 0) return use_hud_icons;

			
			// nothing found
			throw Umbra::UnsupportedPropertyException(ID);
		}	
	};

	
	class ChatExtenderPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CButton          *pAutomaticallyJoin;
		CButton          *pAllowPassiveSelection;
		CButton          *pForceBotsPassive;
		ColourButton     *pStandardColour;
		ColourButton     *pPassiveColour ;
		CButton          *pUseHudIcons   ;

		
	public: // a set of the special property type
		ChatExtenderData data;
		ChatExtenderData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pAutomaticallyJoin = PageCtrl.create_check     (GroupID, "", "Automatically Join");
			pAllowPassiveSelection = PageCtrl.create_check     (GroupID, "", "Allow Passive Selection");
			pForceBotsPassive = PageCtrl.create_check     (GroupID, "", "Force Bots Passive");
			pStandardColour = PageCtrl.create_colour    (GroupID, "", "Standard Colour");
			pPassiveColour  = PageCtrl.create_colour    (GroupID, "", "Passive Colour");
			pUseHudIcons    = PageCtrl.create_check     (GroupID, "", "Use Hud Icons");

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
		
		void dialog_to_attributes(ChatExtenderData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
		}
		
		void attributes_to_dialog(ChatExtenderData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(ChatExtenderData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.automatically_join, (*pAutomaticallyJoin));
			exr.exchange(Attr.allow_passive_selection, (*pAllowPassiveSelection));
			exr.exchange(Attr.force_bots_passive, (*pForceBotsPassive));
			exr.exchange(Attr.standard_colour, (*pStandardColour));
			exr.exchange(Attr.passive_colour, (*pPassiveColour ));
			exr.exchange(Attr.use_hud_icons, (*pUseHudIcons   ));

			
			if (Mode == DataExchanger::FROM_CONTROL)
			{
				Attr.DataUpdated();
			}
			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(ChatExtenderData& RemoteData)
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