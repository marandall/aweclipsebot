#pragma once
#include "serializer.h"
#include "tsb.h"
#include "autonumber.h"

// umbra includes
#include "umbra/umbra_remoteclass.h"
#include "umbra/umbra_variant.h"

#include "raw_param_editor.h"
#include "PropertyScriptCtrl.h"
#include "GlobalDex.h"


namespace AutoTemplates 
{
	class CallListPropertyBuilder;

	//
	// Import Data
	//
	// a:22:{i:0;a:4:{s:4:"name";s:16:"event_avatar_add";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:19:"event_avatar_change";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:19:"event_avatar_delete";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:18:"event_avatar_click";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:17:"event_avatar_zone";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:5;a:4:{s:4:"name";s:10:"event_chat";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:6;a:4:{s:4:"name";s:16:"event_object_add";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:7;a:4:{s:4:"name";s:19:"event_object_delete";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:8;a:4:{s:4:"name";s:18:"event_object_click";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:9;a:4:{s:4:"name";s:19:"event_object_select";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:10;a:4:{s:4:"name";s:19:"event_chanserv_join";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:11;a:4:{s:4:"name";s:20:"event_chanserv_leave";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:12;a:4:{s:4:"name";s:20:"avatar_eject_prepare";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:13;a:4:{s:4:"name";s:21:"avatar_eject_complete";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:14;a:4:{s:4:"name";s:16:"avatar_build_drs";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:15;a:4:{s:4:"name";s:21:"avatar_build_hyperion";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:16;a:4:{s:4:"name";s:14:"world_list_add";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:17;a:4:{s:4:"name";s:17:"world_list_delete";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:18;a:4:{s:4:"name";s:17:"world_list_change";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:19;a:4:{s:4:"name";s:18:"event_timer_second";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:20;a:4:{s:4:"name";s:18:"event_timer_minute";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:21;a:4:{s:4:"name";s:16:"event_timer_hour";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class CallListData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier
	{
	public: // type defenition for property builder
		typedef CallListPropertyBuilder PropertyBuilder;
	
	public: // source data
		String event_avatar_add;
		String event_avatar_change;
		String event_avatar_delete;
		String event_avatar_click;
		String event_avatar_zone;
		String event_chat;
		String event_object_add;
		String event_object_delete;
		String event_object_click;
		String event_object_select;
		String event_chanserv_join;
		String event_chanserv_leave;
		String avatar_eject_prepare;
		String avatar_eject_complete;
		String avatar_build_drs;
		String avatar_build_hyperion;
		String world_list_add;
		String world_list_delete;
		String world_list_change;
		String event_timer_second;
		String event_timer_minute;
		String event_timer_hour;

		
	public:
		CallListData()
		{
			event_avatar_add = "";
			event_avatar_change = "";
			event_avatar_delete = "";
			event_avatar_click = "";
			event_avatar_zone = "";
			event_chat = "";
			event_object_add = "";
			event_object_delete = "";
			event_object_click = "";
			event_object_select = "";
			event_chanserv_join = "";
			event_chanserv_leave = "";
			avatar_eject_prepare = "";
			avatar_eject_complete = "";
			avatar_build_drs = "";
			avatar_build_hyperion = "";
			world_list_add = "";
			world_list_delete = "";
			world_list_change = "";
			event_timer_second = "";
			event_timer_minute = "";
			event_timer_hour = "";

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
			char* section_id = const_cast<char*>("CallList");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "event_avatar_add", event_avatar_add);
			SerializerSystem.Exchange(ExchangeMode, section_id, "event_avatar_change", event_avatar_change);
			SerializerSystem.Exchange(ExchangeMode, section_id, "event_avatar_delete", event_avatar_delete);
			SerializerSystem.Exchange(ExchangeMode, section_id, "event_avatar_click", event_avatar_click);
			SerializerSystem.Exchange(ExchangeMode, section_id, "event_avatar_zone", event_avatar_zone);
			SerializerSystem.Exchange(ExchangeMode, section_id, "event_chat", event_chat);
			SerializerSystem.Exchange(ExchangeMode, section_id, "event_object_add", event_object_add);
			SerializerSystem.Exchange(ExchangeMode, section_id, "event_object_delete", event_object_delete);
			SerializerSystem.Exchange(ExchangeMode, section_id, "event_object_click", event_object_click);
			SerializerSystem.Exchange(ExchangeMode, section_id, "event_object_select", event_object_select);
			SerializerSystem.Exchange(ExchangeMode, section_id, "event_chanserv_join", event_chanserv_join);
			SerializerSystem.Exchange(ExchangeMode, section_id, "event_chanserv_leave", event_chanserv_leave);
			SerializerSystem.Exchange(ExchangeMode, section_id, "avatar_eject_prepare", avatar_eject_prepare);
			SerializerSystem.Exchange(ExchangeMode, section_id, "avatar_eject_complete", avatar_eject_complete);
			SerializerSystem.Exchange(ExchangeMode, section_id, "avatar_build_drs", avatar_build_drs);
			SerializerSystem.Exchange(ExchangeMode, section_id, "avatar_build_hyperion", avatar_build_hyperion);
			SerializerSystem.Exchange(ExchangeMode, section_id, "world_list_add", world_list_add);
			SerializerSystem.Exchange(ExchangeMode, section_id, "world_list_delete", world_list_delete);
			SerializerSystem.Exchange(ExchangeMode, section_id, "world_list_change", world_list_change);
			SerializerSystem.Exchange(ExchangeMode, section_id, "event_timer_second", event_timer_second);
			SerializerSystem.Exchange(ExchangeMode, section_id, "event_timer_minute", event_timer_minute);
			SerializerSystem.Exchange(ExchangeMode, section_id, "event_timer_hour", event_timer_hour);

		}
		
		CallListData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		String  getEventAvatarAdd                () const { return event_avatar_add; }
		String  getEventAvatarChange             () const { return event_avatar_change; }
		String  getEventAvatarDelete             () const { return event_avatar_delete; }
		String  getEventAvatarClick              () const { return event_avatar_click; }
		String  getEventAvatarZone               () const { return event_avatar_zone; }
		String  getEventChat                     () const { return event_chat; }
		String  getEventObjectAdd                () const { return event_object_add; }
		String  getEventObjectDelete             () const { return event_object_delete; }
		String  getEventObjectClick              () const { return event_object_click; }
		String  getEventObjectSelect             () const { return event_object_select; }
		String  getEventChanservJoin             () const { return event_chanserv_join; }
		String  getEventChanservLeave            () const { return event_chanserv_leave; }
		String  getAvatarEjectPrepare            () const { return avatar_eject_prepare; }
		String  getAvatarEjectComplete           () const { return avatar_eject_complete; }
		String  getAvatarBuildDrs                () const { return avatar_build_drs; }
		String  getAvatarBuildHyperion           () const { return avatar_build_hyperion; }
		String  getWorldListAdd                  () const { return world_list_add; }
		String  getWorldListDelete               () const { return world_list_delete; }
		String  getWorldListChange               () const { return world_list_change; }
		String  getEventTimerSecond              () const { return event_timer_second; }
		String  getEventTimerMinute              () const { return event_timer_minute; }
		String  getEventTimerHour                () const { return event_timer_hour; }

		virtual void setEventAvatarAdd                (String  Assgn) { event_avatar_add = Assgn; }
		virtual void setEventAvatarChange             (String  Assgn) { event_avatar_change = Assgn; }
		virtual void setEventAvatarDelete             (String  Assgn) { event_avatar_delete = Assgn; }
		virtual void setEventAvatarClick              (String  Assgn) { event_avatar_click = Assgn; }
		virtual void setEventAvatarZone               (String  Assgn) { event_avatar_zone = Assgn; }
		virtual void setEventChat                     (String  Assgn) { event_chat = Assgn; }
		virtual void setEventObjectAdd                (String  Assgn) { event_object_add = Assgn; }
		virtual void setEventObjectDelete             (String  Assgn) { event_object_delete = Assgn; }
		virtual void setEventObjectClick              (String  Assgn) { event_object_click = Assgn; }
		virtual void setEventObjectSelect             (String  Assgn) { event_object_select = Assgn; }
		virtual void setEventChanservJoin             (String  Assgn) { event_chanserv_join = Assgn; }
		virtual void setEventChanservLeave            (String  Assgn) { event_chanserv_leave = Assgn; }
		virtual void setAvatarEjectPrepare            (String  Assgn) { avatar_eject_prepare = Assgn; }
		virtual void setAvatarEjectComplete           (String  Assgn) { avatar_eject_complete = Assgn; }
		virtual void setAvatarBuildDrs                (String  Assgn) { avatar_build_drs = Assgn; }
		virtual void setAvatarBuildHyperion           (String  Assgn) { avatar_build_hyperion = Assgn; }
		virtual void setWorldListAdd                  (String  Assgn) { world_list_add = Assgn; }
		virtual void setWorldListDelete               (String  Assgn) { world_list_delete = Assgn; }
		virtual void setWorldListChange               (String  Assgn) { world_list_change = Assgn; }
		virtual void setEventTimerSecond              (String  Assgn) { event_timer_second = Assgn; }
		virtual void setEventTimerMinute              (String  Assgn) { event_timer_minute = Assgn; }
		virtual void setEventTimerHour                (String  Assgn) { event_timer_hour = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "event_avatar_add") {
				return Conversion::ToString(event_avatar_add);
			}
			if (ParamName == "event_avatar_change") {
				return Conversion::ToString(event_avatar_change);
			}
			if (ParamName == "event_avatar_delete") {
				return Conversion::ToString(event_avatar_delete);
			}
			if (ParamName == "event_avatar_click") {
				return Conversion::ToString(event_avatar_click);
			}
			if (ParamName == "event_avatar_zone") {
				return Conversion::ToString(event_avatar_zone);
			}
			if (ParamName == "event_chat") {
				return Conversion::ToString(event_chat);
			}
			if (ParamName == "event_object_add") {
				return Conversion::ToString(event_object_add);
			}
			if (ParamName == "event_object_delete") {
				return Conversion::ToString(event_object_delete);
			}
			if (ParamName == "event_object_click") {
				return Conversion::ToString(event_object_click);
			}
			if (ParamName == "event_object_select") {
				return Conversion::ToString(event_object_select);
			}
			if (ParamName == "event_chanserv_join") {
				return Conversion::ToString(event_chanserv_join);
			}
			if (ParamName == "event_chanserv_leave") {
				return Conversion::ToString(event_chanserv_leave);
			}
			if (ParamName == "avatar_eject_prepare") {
				return Conversion::ToString(avatar_eject_prepare);
			}
			if (ParamName == "avatar_eject_complete") {
				return Conversion::ToString(avatar_eject_complete);
			}
			if (ParamName == "avatar_build_drs") {
				return Conversion::ToString(avatar_build_drs);
			}
			if (ParamName == "avatar_build_hyperion") {
				return Conversion::ToString(avatar_build_hyperion);
			}
			if (ParamName == "world_list_add") {
				return Conversion::ToString(world_list_add);
			}
			if (ParamName == "world_list_delete") {
				return Conversion::ToString(world_list_delete);
			}
			if (ParamName == "world_list_change") {
				return Conversion::ToString(world_list_change);
			}
			if (ParamName == "event_timer_second") {
				return Conversion::ToString(event_timer_second);
			}
			if (ParamName == "event_timer_minute") {
				return Conversion::ToString(event_timer_minute);
			}
			if (ParamName == "event_timer_hour") {
				return Conversion::ToString(event_timer_hour);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "event_avatar_add") { 
				event_avatar_add = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "event_avatar_change") { 
				event_avatar_change = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "event_avatar_delete") { 
				event_avatar_delete = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "event_avatar_click") { 
				event_avatar_click = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "event_avatar_zone") { 
				event_avatar_zone = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "event_chat") { 
				event_chat = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "event_object_add") { 
				event_object_add = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "event_object_delete") { 
				event_object_delete = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "event_object_click") { 
				event_object_click = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "event_object_select") { 
				event_object_select = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "event_chanserv_join") { 
				event_chanserv_join = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "event_chanserv_leave") { 
				event_chanserv_leave = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "avatar_eject_prepare") { 
				avatar_eject_prepare = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "avatar_eject_complete") { 
				avatar_eject_complete = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "avatar_build_drs") { 
				avatar_build_drs = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "avatar_build_hyperion") { 
				avatar_build_hyperion = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "world_list_add") { 
				world_list_add = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "world_list_delete") { 
				world_list_delete = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "world_list_change") { 
				world_list_change = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "event_timer_second") { 
				event_timer_second = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "event_timer_minute") { 
				event_timer_minute = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "event_timer_hour") { 
				event_timer_hour = Conversion::ToString(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(event_avatar_add, event_avatar_add);
			UMBRA_PROPERTY_RW(event_avatar_change, event_avatar_change);
			UMBRA_PROPERTY_RW(event_avatar_delete, event_avatar_delete);
			UMBRA_PROPERTY_RW(event_avatar_click, event_avatar_click);
			UMBRA_PROPERTY_RW(event_avatar_zone, event_avatar_zone);
			UMBRA_PROPERTY_RW(event_chat, event_chat);
			UMBRA_PROPERTY_RW(event_object_add, event_object_add);
			UMBRA_PROPERTY_RW(event_object_delete, event_object_delete);
			UMBRA_PROPERTY_RW(event_object_click, event_object_click);
			UMBRA_PROPERTY_RW(event_object_select, event_object_select);
			UMBRA_PROPERTY_RW(event_chanserv_join, event_chanserv_join);
			UMBRA_PROPERTY_RW(event_chanserv_leave, event_chanserv_leave);
			UMBRA_PROPERTY_RW(avatar_eject_prepare, avatar_eject_prepare);
			UMBRA_PROPERTY_RW(avatar_eject_complete, avatar_eject_complete);
			UMBRA_PROPERTY_RW(avatar_build_drs, avatar_build_drs);
			UMBRA_PROPERTY_RW(avatar_build_hyperion, avatar_build_hyperion);
			UMBRA_PROPERTY_RW(world_list_add, world_list_add);
			UMBRA_PROPERTY_RW(world_list_delete, world_list_delete);
			UMBRA_PROPERTY_RW(world_list_change, world_list_change);
			UMBRA_PROPERTY_RW(event_timer_second, event_timer_second);
			UMBRA_PROPERTY_RW(event_timer_minute, event_timer_minute);
			UMBRA_PROPERTY_RW(event_timer_hour, event_timer_hour);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class CallListPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pEventAvatarAdd;
		TextBox          *pEventAvatarChange;
		TextBox          *pEventAvatarDelete;
		TextBox          *pEventAvatarClick;
		TextBox          *pEventAvatarZone;
		TextBox          *pEventChat     ;
		TextBox          *pEventObjectAdd;
		TextBox          *pEventObjectDelete;
		TextBox          *pEventObjectClick;
		TextBox          *pEventObjectSelect;
		TextBox          *pEventChanservJoin;
		TextBox          *pEventChanservLeave;
		CStatic			 *pAvatarList    ;
		TextBox          *pAvatarEjectPrepare;
		TextBox          *pAvatarEjectComplete;
		TextBox          *pAvatarBuildDrs;
		TextBox          *pAvatarBuildHyperion;
		CStatic			 *pWorldList     ;
		TextBox          *pWorldListAdd  ;
		TextBox          *pWorldListDelete;
		TextBox          *pWorldListChange;
		CStatic			 *pTimers        ;
		TextBox          *pEventTimerSecond;
		TextBox          *pEventTimerMinute;
		TextBox          *pEventTimerHour;

		
	public: // a set of the special property type
		CallListData data;
		CallListData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pEventAvatarAdd = PageCtrl.create_edit      (GroupID, "", "Event Avatar Add");
			pEventAvatarChange = PageCtrl.create_edit      (GroupID, "", "Event Avatar Change");
			pEventAvatarDelete = PageCtrl.create_edit      (GroupID, "", "Event Avatar Delete");
			pEventAvatarClick = PageCtrl.create_edit      (GroupID, "", "Event Avatar Click");
			pEventAvatarZone = PageCtrl.create_edit      (GroupID, "", "Event Avatar Zone");
			pEventChat      = PageCtrl.create_edit      (GroupID, "", "Event Chat");
			pEventObjectAdd = PageCtrl.create_edit      (GroupID, "", "Event Object Add");
			pEventObjectDelete = PageCtrl.create_edit      (GroupID, "", "Event Object Delete");
			pEventObjectClick = PageCtrl.create_edit      (GroupID, "", "Event Object Click");
			pEventObjectSelect = PageCtrl.create_edit      (GroupID, "", "Event Object Select");
			pEventChanservJoin = PageCtrl.create_edit      (GroupID, "", "Event Chanserv Join");
			pEventChanservLeave = PageCtrl.create_edit      (GroupID, "", "Event Chanserv Leave");
			pAvatarList     = PageCtrl.create_label     (GroupID, "", "Avatar Functions");
			pAvatarEjectPrepare = PageCtrl.create_edit      (GroupID, "", "Avatar Eject Prepare");
			pAvatarEjectComplete = PageCtrl.create_edit      (GroupID, "", "Avatar Eject Complete");
			pAvatarBuildDrs = PageCtrl.create_edit      (GroupID, "", "Avatar Build Drs");
			pAvatarBuildHyperion = PageCtrl.create_edit      (GroupID, "", "Avatar Build Hyperion");
			pWorldList      = PageCtrl.create_label     (GroupID, "", "World List Changes");
			pWorldListAdd   = PageCtrl.create_edit      (GroupID, "", "World List Add");
			pWorldListDelete = PageCtrl.create_edit      (GroupID, "", "World List Delete");
			pWorldListChange = PageCtrl.create_edit      (GroupID, "", "World List Change");
			pTimers         = PageCtrl.create_label     (GroupID, "", "Timers");
			pEventTimerSecond = PageCtrl.create_edit      (GroupID, "", "Event Timer Second");
			pEventTimerMinute = PageCtrl.create_edit      (GroupID, "", "Event Timer Minute");
			pEventTimerHour = PageCtrl.create_edit      (GroupID, "", "Event Timer Hour");

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
		
		bool dialog_to_attributes(CallListData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(CallListData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(CallListData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.event_avatar_add, (*pEventAvatarAdd));
			exr.exchange(Attr.event_avatar_change, (*pEventAvatarChange));
			exr.exchange(Attr.event_avatar_delete, (*pEventAvatarDelete));
			exr.exchange(Attr.event_avatar_click, (*pEventAvatarClick));
			exr.exchange(Attr.event_avatar_zone, (*pEventAvatarZone));
			exr.exchange(Attr.event_chat, (*pEventChat     ));
			exr.exchange(Attr.event_object_add, (*pEventObjectAdd));
			exr.exchange(Attr.event_object_delete, (*pEventObjectDelete));
			exr.exchange(Attr.event_object_click, (*pEventObjectClick));
			exr.exchange(Attr.event_object_select, (*pEventObjectSelect));
			exr.exchange(Attr.event_chanserv_join, (*pEventChanservJoin));
			exr.exchange(Attr.event_chanserv_leave, (*pEventChanservLeave));
			exr.exchange(Attr.avatar_eject_prepare, (*pAvatarEjectPrepare));
			exr.exchange(Attr.avatar_eject_complete, (*pAvatarEjectComplete));
			exr.exchange(Attr.avatar_build_drs, (*pAvatarBuildDrs));
			exr.exchange(Attr.avatar_build_hyperion, (*pAvatarBuildHyperion));
			exr.exchange(Attr.world_list_add, (*pWorldListAdd  ));
			exr.exchange(Attr.world_list_delete, (*pWorldListDelete));
			exr.exchange(Attr.world_list_change, (*pWorldListChange));
			exr.exchange(Attr.event_timer_second, (*pEventTimerSecond));
			exr.exchange(Attr.event_timer_minute, (*pEventTimerMinute));
			exr.exchange(Attr.event_timer_hour, (*pEventTimerHour));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(CallListData& RemoteData)
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
 