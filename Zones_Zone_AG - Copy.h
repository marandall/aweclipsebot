#pragma once
#include "serializer.h"
#include "tsb.h"
#include "umbra_remote_class.h"
#include "raw_param_editor.h"
#include "PropertyScriptCtrl.h"
#include "GlobalDex.h"

namespace Zones { 
namespace AutoTemplates 
{
	class ZonePropertyBuilder;

	class ZoneData : 
		public Umbra::RemoteClass,
		public IRawParamEditor
	{
	public: // type defenition for property builder
		typedef ZonePropertyBuilder PropertyBuilder;
	
	public: // source data
		Ascii       name;
		Ascii       id;
		bool        use_for_lookups;
		tsb_t       override_fly;
		tsb_t       override_shift;
		tsb_t       override_teleport;
		Ascii       override_template;
		Ascii       override_final;
		Ascii       chat_channel;

		
	public:
		ZoneData()
		{
			name = "";
			id = "";
			use_for_lookups = true;
			override_fly = B_EMPTY;
			override_shift = B_EMPTY;
			override_teleport = B_EMPTY;
			override_template = "";
			override_final = "";
			chat_channel = "";

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
			char* section_id = const_cast<char*>("zones_Zone");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "name", name);
			SerializerSystem.Exchange(ExchangeMode, section_id, "id", id);
			SerializerSystem.Exchange(ExchangeMode, section_id, "use_for_lookups", use_for_lookups);
			SerializerSystem.Exchange(ExchangeMode, section_id, "override_fly", override_fly);
			SerializerSystem.Exchange(ExchangeMode, section_id, "override_shift", override_shift);
			SerializerSystem.Exchange(ExchangeMode, section_id, "override_teleport", override_teleport);
			SerializerSystem.Exchange(ExchangeMode, section_id, "override_template", override_template);
			SerializerSystem.Exchange(ExchangeMode, section_id, "override_final", override_final);
			SerializerSystem.Exchange(ExchangeMode, section_id, "chat_channel", chat_channel);

		}
		
		ZoneData& GetDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual void DataUpdated() { }
		
	public: // read only operators
		Ascii        get_Name                          () const { return name; }
		Ascii        get_Id                            () const { return id; }
		bool         get_UseForLookups                 () const { return use_for_lookups; }
		tsb_t        get_OverrideFly                   () const { return override_fly; }
		tsb_t        get_OverrideShift                 () const { return override_shift; }
		tsb_t        get_OverrideTeleport              () const { return override_teleport; }
		Ascii        get_OverrideTemplate              () const { return override_template; }
		Ascii        get_OverrideFinal                 () const { return override_final; }
		Ascii        get_ChatChannel                   () const { return chat_channel; }

		virtual void set_Name                          (Ascii        Assgn) { name = Assgn; }
		virtual void set_Id                            (Ascii        Assgn) { id = Assgn; }
		virtual void set_UseForLookups                 (bool         Assgn) { use_for_lookups = Assgn; }
		virtual void set_OverrideFly                   (tsb_t        Assgn) { override_fly = Assgn; }
		virtual void set_OverrideShift                 (tsb_t        Assgn) { override_shift = Assgn; }
		virtual void set_OverrideTeleport              (tsb_t        Assgn) { override_teleport = Assgn; }
		virtual void set_OverrideTemplate              (Ascii        Assgn) { override_template = Assgn; }
		virtual void set_OverrideFinal                 (Ascii        Assgn) { override_final = Assgn; }
		virtual void set_ChatChannel                   (Ascii        Assgn) { chat_channel = Assgn; }

		
	public: // string conversion
		virtual Ascii param_read(Ascii ParamName)
		{
			if (ParamName == "name") {
				return Conversion::ToString(name);
			}
			if (ParamName == "id") {
				return Conversion::ToString(id);
			}
			if (ParamName == "use_for_lookups") {
				return Conversion::ToString(use_for_lookups);
			}
			if (ParamName == "override_fly") {
				return Conversion::ToStringTSB(override_fly);
			}
			if (ParamName == "override_shift") {
				return Conversion::ToStringTSB(override_shift);
			}
			if (ParamName == "override_teleport") {
				return Conversion::ToStringTSB(override_teleport);
			}
			if (ParamName == "override_template") {
				return Conversion::ToString(override_template);
			}
			if (ParamName == "override_final") {
				return Conversion::ToString(override_final);
			}
			if (ParamName == "chat_channel") {
				return Conversion::ToString(chat_channel);
			}

			return "";
		}
		
		virtual bool param_write(Ascii ParamName, Ascii Value)
		{
			if (ParamName == "name") { 
				name = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "id") { 
				id = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "use_for_lookups") { 
				use_for_lookups = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "override_fly") { 
				override_fly = Conversion::ToTSB(Value);
				return true;
			}
			if (ParamName == "override_shift") { 
				override_shift = Conversion::ToTSB(Value);
				return true;
			}
			if (ParamName == "override_teleport") { 
				override_teleport = Conversion::ToTSB(Value);
				return true;
			}
			if (ParamName == "override_template") { 
				override_template = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "override_final") { 
				override_final = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "chat_channel") { 
				chat_channel = Conversion::ToString(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		Umbra::Variant Umbra_GetProperty(Umbra::string ID, Umbra::ES_GetProperty& ES)
		{
			if (strcmp(ID.c_str(), "name"                        ) == 0) return (const char*)name;
			if (strcmp(ID.c_str(), "id"                          ) == 0) return (const char*)id;
			if (strcmp(ID.c_str(), "use_for_lookups"             ) == 0) return use_for_lookups;
			if (strcmp(ID.c_str(), "override_fly"                ) == 0) return override_fly;
			if (strcmp(ID.c_str(), "override_shift"              ) == 0) return override_shift;
			if (strcmp(ID.c_str(), "override_teleport"           ) == 0) return override_teleport;
			if (strcmp(ID.c_str(), "override_template"           ) == 0) return (const char*)override_template;
			if (strcmp(ID.c_str(), "override_final"              ) == 0) return (const char*)override_final;
			if (strcmp(ID.c_str(), "chat_channel"                ) == 0) return (const char*)chat_channel;

			
			// nothing found
			throw Umbra::UnsupportedPropertyException(ID);
		}	
	};

	
	class ZonePropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pName          ;
		TextBox          *pId            ;
		CButton          *pUseForLookups ;
		CStatic			 *pOverrides     ;
		CButton          *pOverrideFly   ;
		CButton          *pOverrideShift ;
		CButton          *pOverrideTeleport;
		TextBox          *pOverrideTemplate;
		TextBox          *pOverrideFinal ;
		CStatic			 *pComms         ;
		TextBox          *pChatChannel   ;

		
	public: // a set of the special property type
		ZoneData data;
		ZoneData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pName           = PageCtrl.create_edit      (GroupID, "", "Name");
			pId             = PageCtrl.create_edit      (GroupID, "", "Id");
			pUseForLookups  = PageCtrl.create_check     (GroupID, "", "Use For Lookups");
			pOverrides      = PageCtrl.create_label     (GroupID, "", "Overrides");
			pOverrideFly    = PageCtrl.create_tristate  (GroupID, "", "Override Fly");
			pOverrideShift  = PageCtrl.create_tristate  (GroupID, "", "Override Shift");
			pOverrideTeleport = PageCtrl.create_tristate  (GroupID, "", "Override Teleport");
			pOverrideTemplate = PageCtrl.create_edit      (GroupID, "", "Override Template");
			pOverrideFinal  = PageCtrl.create_edit      (GroupID, "", "Override Final");
			pComms          = PageCtrl.create_label     (GroupID, "", "Communications");
			pChatChannel    = PageCtrl.create_edit      (GroupID, "", "Chat Channel");

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
		
		void dialog_to_attributes(ZoneData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
		}
		
		void attributes_to_dialog(ZoneData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(ZoneData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.name, (*pName          ));
			exr.exchange(Attr.id, (*pId            ));
			exr.exchange(Attr.use_for_lookups, (*pUseForLookups ));
			exr.exchange(Attr.override_fly, (*pOverrideFly   ));
			exr.exchange(Attr.override_shift, (*pOverrideShift ));
			exr.exchange(Attr.override_teleport, (*pOverrideTeleport));
			exr.exchange(Attr.override_template, (*pOverrideTemplate));
			exr.exchange(Attr.override_final, (*pOverrideFinal ));
			exr.exchange(Attr.chat_channel, (*pChatChannel   ));

			
			if (Mode == DataExchanger::FROM_CONTROL)
			{
				Attr.DataUpdated();
			}
			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(ZoneData& RemoteData)
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