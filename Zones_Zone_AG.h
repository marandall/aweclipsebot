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

namespace Zones { 
namespace AutoTemplates 
{
	class ZonePropertyBuilder;

	//
	// Import Data
	//
	// a:14:{i:0;a:4:{s:4:"name";s:4:"name";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:2:"id";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:15:"use_for_lookups";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:12:"override_fly";s:4:"type";s:5:"tsb_t";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:14:"override_shift";s:4:"type";s:5:"tsb_t";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:5;a:4:{s:4:"name";s:17:"override_teleport";s:4:"type";s:5:"tsb_t";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:6;a:4:{s:4:"name";s:17:"override_template";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:7;a:4:{s:4:"name";s:14:"override_final";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:8;a:4:{s:4:"name";s:12:"enter_camera";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:9;a:4:{s:4:"name";s:11:"enter_noise";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:10;a:4:{s:4:"name";s:11:"access_list";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:11;a:4:{s:4:"name";s:15:"denial_argument";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:12;a:4:{s:4:"name";s:12:"chat_channel";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:13;a:4:{s:4:"name";s:16:"chat_requires_ps";s:4:"type";s:5:"tsb_t";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class ZoneData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef ZonePropertyBuilder PropertyBuilder;
	
	public: // source data
		String name;
		String id;
		bool use_for_lookups;
		tsb_t       override_fly;
		tsb_t       override_shift;
		tsb_t       override_teleport;
		String override_template;
		String override_final;
		String enter_camera;
		String enter_noise;
		String access_list;
		String denial_argument;
		String chat_channel;
		tsb_t       chat_requires_ps;

		
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
			enter_camera = "";
			enter_noise = "";
			access_list = "";
			denial_argument = "";
			chat_channel = "";
			chat_requires_ps = B_EMPTY;

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
			SerializerSystem.Exchange(ExchangeMode, section_id, "enter_camera", enter_camera);
			SerializerSystem.Exchange(ExchangeMode, section_id, "enter_noise", enter_noise);
			SerializerSystem.Exchange(ExchangeMode, section_id, "access_list", access_list);
			SerializerSystem.Exchange(ExchangeMode, section_id, "denial_argument", denial_argument);
			SerializerSystem.Exchange(ExchangeMode, section_id, "chat_channel", chat_channel);
			SerializerSystem.Exchange(ExchangeMode, section_id, "chat_requires_ps", chat_requires_ps);

		}
		
		ZoneData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		String  getName                          () const { return name; }
		String  getId                            () const { return id; }
		bool  getUseForLookups                 () const { return use_for_lookups; }
		tsb_t        getOverrideFly                   () const { return override_fly; }
		tsb_t        getOverrideShift                 () const { return override_shift; }
		tsb_t        getOverrideTeleport              () const { return override_teleport; }
		String  getOverrideTemplate              () const { return override_template; }
		String  getOverrideFinal                 () const { return override_final; }
		String  getEnterCamera                   () const { return enter_camera; }
		String  getEnterNoise                    () const { return enter_noise; }
		String  getAccessList                    () const { return access_list; }
		String  getDenialArgument                () const { return denial_argument; }
		String  getChatChannel                   () const { return chat_channel; }
		tsb_t        getChatRequiresPs                () const { return chat_requires_ps; }

		virtual void setName                          (String  Assgn) { name = Assgn; }
		virtual void setId                            (String  Assgn) { id = Assgn; }
		virtual void setUseForLookups                 (bool  Assgn) { use_for_lookups = Assgn; }
		virtual void setOverrideFly                   (tsb_t        Assgn) { override_fly = Assgn; }
		virtual void setOverrideShift                 (tsb_t        Assgn) { override_shift = Assgn; }
		virtual void setOverrideTeleport              (tsb_t        Assgn) { override_teleport = Assgn; }
		virtual void setOverrideTemplate              (String  Assgn) { override_template = Assgn; }
		virtual void setOverrideFinal                 (String  Assgn) { override_final = Assgn; }
		virtual void setEnterCamera                   (String  Assgn) { enter_camera = Assgn; }
		virtual void setEnterNoise                    (String  Assgn) { enter_noise = Assgn; }
		virtual void setAccessList                    (String  Assgn) { access_list = Assgn; }
		virtual void setDenialArgument                (String  Assgn) { denial_argument = Assgn; }
		virtual void setChatChannel                   (String  Assgn) { chat_channel = Assgn; }
		virtual void setChatRequiresPs                (tsb_t        Assgn) { chat_requires_ps = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
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
			if (ParamName == "enter_camera") {
				return Conversion::ToString(enter_camera);
			}
			if (ParamName == "enter_noise") {
				return Conversion::ToString(enter_noise);
			}
			if (ParamName == "access_list") {
				return Conversion::ToString(access_list);
			}
			if (ParamName == "denial_argument") {
				return Conversion::ToString(denial_argument);
			}
			if (ParamName == "chat_channel") {
				return Conversion::ToString(chat_channel);
			}
			if (ParamName == "chat_requires_ps") {
				return Conversion::ToStringTSB(chat_requires_ps);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
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
			if (ParamName == "enter_camera") { 
				enter_camera = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "enter_noise") { 
				enter_noise = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "access_list") { 
				access_list = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "denial_argument") { 
				denial_argument = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "chat_channel") { 
				chat_channel = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "chat_requires_ps") { 
				chat_requires_ps = Conversion::ToTSB(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(name, name);
			UMBRA_PROPERTY_RW(id, id);
			UMBRA_PROPERTY_RW(use_for_lookups, use_for_lookups);
			UMBRA_PROPERTY_RW(override_fly, override_fly);
			UMBRA_PROPERTY_RW(override_shift, override_shift);
			UMBRA_PROPERTY_RW(override_teleport, override_teleport);
			UMBRA_PROPERTY_RW(override_template, override_template);
			UMBRA_PROPERTY_RW(override_final, override_final);
			UMBRA_PROPERTY_RW(enter_camera, enter_camera);
			UMBRA_PROPERTY_RW(enter_noise, enter_noise);
			UMBRA_PROPERTY_RW(access_list, access_list);
			UMBRA_PROPERTY_RW(denial_argument, denial_argument);
			UMBRA_PROPERTY_RW(chat_channel, chat_channel);
			UMBRA_PROPERTY_RW(chat_requires_ps, chat_requires_ps);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
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
		CStatic			 *pTriggers      ;
		TextBox          *pEnterCamera   ;
		TextBox          *pEnterNoise    ;
		CStatic			 *pSec           ;
		TextBox          *pAccessList    ;
		TextBox          *pDenialArgument;
		CStatic			 *pComms         ;
		TextBox          *pChatChannel   ;
		CButton          *pChatRequiresPs;

		
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
			pTriggers       = PageCtrl.create_label     (GroupID, "", "Triggers");
			pEnterCamera    = PageCtrl.create_edit      (GroupID, "", "Enter Camera");
			pEnterNoise     = PageCtrl.create_edit      (GroupID, "", "Enter Noise");
			pSec            = PageCtrl.create_label     (GroupID, "", "Security");
			pAccessList     = PageCtrl.create_edit      (GroupID, "", "Access List");
			pDenialArgument = PageCtrl.create_edit      (GroupID, "", "Denial Argument");
			pComms          = PageCtrl.create_label     (GroupID, "", "Communications");
			pChatChannel    = PageCtrl.create_edit      (GroupID, "", "Chat Channel");
			pChatRequiresPs = PageCtrl.create_tristate  (GroupID, "", "Chat Requires Ps");

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
		
		bool dialog_to_attributes(ZoneData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
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
			exr.exchange(Attr.enter_camera, (*pEnterCamera   ));
			exr.exchange(Attr.enter_noise, (*pEnterNoise    ));
			exr.exchange(Attr.access_list, (*pAccessList    ));
			exr.exchange(Attr.denial_argument, (*pDenialArgument));
			exr.exchange(Attr.chat_channel, (*pChatChannel   ));
			exr.exchange(Attr.chat_requires_ps, (*pChatRequiresPs));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(ZoneData& RemoteData)
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