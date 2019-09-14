#pragma once
#include "serializer.h"
#include "tsb.h"
#include "umbra/umbra_remoteclass.h"
#include "raw_param_editor.h"
#include "PropertyScriptCtrl.h"
#include "GlobalDex.h"


namespace AutoTemplates 
{
	class CallListPropertyBuilder;

	class CallListData : 
		public Umbra::RemoteClass,
		public IRawParamEditor
	{
	public: // type defenition for property builder
		typedef CallListPropertyBuilder PropertyBuilder;
	
	public: // source data
		String      event_avatar_add;
		String      event_avatar_change;
		String      event_avatar_delete;
		String      event_avatar_click;
		String      event_object_add;
		String      event_object_delete;
		String      event_object_click;
		String      event_object_select;
		String      avatar_eject_prepare;
		String      avatar_eject_complete;
		String      avatar_drs_build;
		String      avatar_hyperion_build;
		String      world_list_add;
		String      world_list_delete;
		String      world_list_change;

		
	public:
		CallListData()
		{
			event_avatar_add = "";
			event_avatar_change = "";
			event_avatar_delete = "";
			event_avatar_click = "";
			event_object_add = "";
			event_object_delete = "";
			event_object_click = "";
			event_object_select = "";
			avatar_eject_prepare = "";
			avatar_eject_complete = "";
			avatar_drs_build = "";
			avatar_hyperion_build = "";
			world_list_add = "";
			world_list_delete = "";
			world_list_change = "";

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
			SerializerSystem.Exchange(ExchangeMode, section_id, "event_object_add", event_object_add);
			SerializerSystem.Exchange(ExchangeMode, section_id, "event_object_delete", event_object_delete);
			SerializerSystem.Exchange(ExchangeMode, section_id, "event_object_click", event_object_click);
			SerializerSystem.Exchange(ExchangeMode, section_id, "event_object_select", event_object_select);
			SerializerSystem.Exchange(ExchangeMode, section_id, "avatar_eject_prepare", avatar_eject_prepare);
			SerializerSystem.Exchange(ExchangeMode, section_id, "avatar_eject_complete", avatar_eject_complete);
			SerializerSystem.Exchange(ExchangeMode, section_id, "avatar_drs_build", avatar_drs_build);
			SerializerSystem.Exchange(ExchangeMode, section_id, "avatar_hyperion_build", avatar_hyperion_build);
			SerializerSystem.Exchange(ExchangeMode, section_id, "world_list_add", world_list_add);
			SerializerSystem.Exchange(ExchangeMode, section_id, "world_list_delete", world_list_delete);
			SerializerSystem.Exchange(ExchangeMode, section_id, "world_list_change", world_list_change);

		}
		
		CallListData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		String       getEventAvatarAdd                () const { return event_avatar_add; }
		String       getEventAvatarChange             () const { return event_avatar_change; }
		String       getEventAvatarDelete             () const { return event_avatar_delete; }
		String       getEventAvatarClick              () const { return event_avatar_click; }
		String       getEventObjectAdd                () const { return event_object_add; }
		String       getEventObjectDelete             () const { return event_object_delete; }
		String       getEventObjectClick              () const { return event_object_click; }
		String       getEventObjectSelect             () const { return event_object_select; }
		String       getAvatarEjectPrepare            () const { return avatar_eject_prepare; }
		String       getAvatarEjectComplete           () const { return avatar_eject_complete; }
		String       getAvatarDrsBuild                () const { return avatar_drs_build; }
		String       getAvatarHyperionBuild           () const { return avatar_hyperion_build; }
		String       getWorldListAdd                  () const { return world_list_add; }
		String       getWorldListDelete               () const { return world_list_delete; }
		String       getWorldListChange               () const { return world_list_change; }

		virtual void setEventAvatarAdd                (String       Assgn) { event_avatar_add = Assgn; }
		virtual void setEventAvatarChange             (String       Assgn) { event_avatar_change = Assgn; }
		virtual void setEventAvatarDelete             (String       Assgn) { event_avatar_delete = Assgn; }
		virtual void setEventAvatarClick              (String       Assgn) { event_avatar_click = Assgn; }
		virtual void setEventObjectAdd                (String       Assgn) { event_object_add = Assgn; }
		virtual void setEventObjectDelete             (String       Assgn) { event_object_delete = Assgn; }
		virtual void setEventObjectClick              (String       Assgn) { event_object_click = Assgn; }
		virtual void setEventObjectSelect             (String       Assgn) { event_object_select = Assgn; }
		virtual void setAvatarEjectPrepare            (String       Assgn) { avatar_eject_prepare = Assgn; }
		virtual void setAvatarEjectComplete           (String       Assgn) { avatar_eject_complete = Assgn; }
		virtual void setAvatarDrsBuild                (String       Assgn) { avatar_drs_build = Assgn; }
		virtual void setAvatarHyperionBuild           (String       Assgn) { avatar_hyperion_build = Assgn; }
		virtual void setWorldListAdd                  (String       Assgn) { world_list_add = Assgn; }
		virtual void setWorldListDelete               (String       Assgn) { world_list_delete = Assgn; }
		virtual void setWorldListChange               (String       Assgn) { world_list_change = Assgn; }

		
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
			if (ParamName == "avatar_eject_prepare") {
				return Conversion::ToString(avatar_eject_prepare);
			}
			if (ParamName == "avatar_eject_complete") {
				return Conversion::ToString(avatar_eject_complete);
			}
			if (ParamName == "avatar_drs_build") {
				return Conversion::ToString(avatar_drs_build);
			}
			if (ParamName == "avatar_hyperion_build") {
				return Conversion::ToString(avatar_hyperion_build);
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
			if (ParamName == "avatar_eject_prepare") { 
				avatar_eject_prepare = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "avatar_eject_complete") { 
				avatar_eject_complete = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "avatar_drs_build") { 
				avatar_drs_build = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "avatar_hyperion_build") { 
				avatar_hyperion_build = Conversion::ToString(Value);
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

			return false;
		}	
		
	public: // umbra interfaces
		void doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(event_avatar_add, event_avatar_add);
			UMBRA_PROPERTY_RW(event_avatar_change, event_avatar_change);
			UMBRA_PROPERTY_RW(event_avatar_delete, event_avatar_delete);
			UMBRA_PROPERTY_RW(event_avatar_click, event_avatar_click);
			UMBRA_PROPERTY_RW(event_object_add, event_object_add);
			UMBRA_PROPERTY_RW(event_object_delete, event_object_delete);
			UMBRA_PROPERTY_RW(event_object_click, event_object_click);
			UMBRA_PROPERTY_RW(event_object_select, event_object_select);
			UMBRA_PROPERTY_RW(avatar_eject_prepare, avatar_eject_prepare);
			UMBRA_PROPERTY_RW(avatar_eject_complete, avatar_eject_complete);
			UMBRA_PROPERTY_RW(avatar_drs_build, avatar_drs_build);
			UMBRA_PROPERTY_RW(avatar_hyperion_build, avatar_hyperion_build);
			UMBRA_PROPERTY_RW(world_list_add, world_list_add);
			UMBRA_PROPERTY_RW(world_list_delete, world_list_delete);
			UMBRA_PROPERTY_RW(world_list_change, world_list_change);

			
			// nothing found
			throw Umbra::UnsupportedPropertyException(R);
		}
	};

	
	class CallListPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pEventAvatarAdd;
		TextBox          *pEventAvatarChange;
		TextBox          *pEventAvatarDelete;
		TextBox          *pEventAvatarClick;
		TextBox          *pEventObjectAdd;
		TextBox          *pEventObjectDelete;
		TextBox          *pEventObjectClick;
		TextBox          *pEventObjectSelect;
		TextBox          *pAvatarEjectPrepare;
		TextBox          *pAvatarEjectComplete;
		TextBox          *pAvatarDrsBuild;
		TextBox          *pAvatarHyperionBuild;
		TextBox          *pWorldListAdd  ;
		TextBox          *pWorldListDelete;
		TextBox          *pWorldListChange;

		
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
			pEventObjectAdd = PageCtrl.create_edit      (GroupID, "", "Event Object Add");
			pEventObjectDelete = PageCtrl.create_edit      (GroupID, "", "Event Object Delete");
			pEventObjectClick = PageCtrl.create_edit      (GroupID, "", "Event Object Click");
			pEventObjectSelect = PageCtrl.create_edit      (GroupID, "", "Event Object Select");
			pAvatarEjectPrepare = PageCtrl.create_edit      (GroupID, "", "Avatar Eject Prepare");
			pAvatarEjectComplete = PageCtrl.create_edit      (GroupID, "", "Avatar Eject Complete");
			pAvatarDrsBuild = PageCtrl.create_edit      (GroupID, "", "Avatar Drs Build");
			pAvatarHyperionBuild = PageCtrl.create_edit      (GroupID, "", "Avatar Hyperion Build");
			pWorldListAdd   = PageCtrl.create_edit      (GroupID, "", "World List Add");
			pWorldListDelete = PageCtrl.create_edit      (GroupID, "", "World List Delete");
			pWorldListChange = PageCtrl.create_edit      (GroupID, "", "World List Change");

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
			exr.exchange(Attr.event_object_add, (*pEventObjectAdd));
			exr.exchange(Attr.event_object_delete, (*pEventObjectDelete));
			exr.exchange(Attr.event_object_click, (*pEventObjectClick));
			exr.exchange(Attr.event_object_select, (*pEventObjectSelect));
			exr.exchange(Attr.avatar_eject_prepare, (*pAvatarEjectPrepare));
			exr.exchange(Attr.avatar_eject_complete, (*pAvatarEjectComplete));
			exr.exchange(Attr.avatar_drs_build, (*pAvatarDrsBuild));
			exr.exchange(Attr.avatar_hyperion_build, (*pAvatarHyperionBuild));
			exr.exchange(Attr.world_list_add, (*pWorldListAdd  ));
			exr.exchange(Attr.world_list_delete, (*pWorldListDelete));
			exr.exchange(Attr.world_list_change, (*pWorldListChange));


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
 