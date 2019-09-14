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

namespace CIS { namespace Config { 
namespace AutoTemplates 
{
	class ScriptedCommandPropertyBuilder;

	//
	// Import Data
	//
	// a:9:{i:0;a:4:{s:4:"name";s:2:"id";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:14:"required_right";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:15:"script_function";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:14:"context_avatar";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:15:"context_console";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:5;a:4:{s:4:"name";s:14:"requires_world";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:6;a:4:{s:4:"name";s:17:"requires_universe";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:7;a:4:{s:4:"name";s:14:"requires_eject";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:8;a:4:{s:4:"name";s:18:"requires_caretaker";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class ScriptedCommandData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef ScriptedCommandPropertyBuilder PropertyBuilder;
	
	public: // source data
		String id;
		String required_right;
		String script_function;
		bool context_avatar;
		bool context_console;
		bool requires_world;
		bool requires_universe;
		bool requires_eject;
		bool requires_caretaker;

		
	public:
		ScriptedCommandData()
		{
			id = "";
			required_right = "";
			script_function = "";
			context_avatar = false;
			context_console = false;
			requires_world = false;
			requires_universe = true;
			requires_eject = false;
			requires_caretaker = false;

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
			char* section_id = const_cast<char*>("cis_ScriptedCommand");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "id", id);
			SerializerSystem.Exchange(ExchangeMode, section_id, "required_right", required_right);
			SerializerSystem.Exchange(ExchangeMode, section_id, "script_function", script_function);
			SerializerSystem.Exchange(ExchangeMode, section_id, "context_avatar", context_avatar);
			SerializerSystem.Exchange(ExchangeMode, section_id, "context_console", context_console);
			SerializerSystem.Exchange(ExchangeMode, section_id, "requires_world", requires_world);
			SerializerSystem.Exchange(ExchangeMode, section_id, "requires_universe", requires_universe);
			SerializerSystem.Exchange(ExchangeMode, section_id, "requires_eject", requires_eject);
			SerializerSystem.Exchange(ExchangeMode, section_id, "requires_caretaker", requires_caretaker);

		}
		
		ScriptedCommandData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		String  getId                            () const { return id; }
		String  getRequiredRight                 () const { return required_right; }
		String  getScriptFunction                () const { return script_function; }
		bool  getContextAvatar                 () const { return context_avatar; }
		bool  getContextConsole                () const { return context_console; }
		bool  getRequiresWorld                 () const { return requires_world; }
		bool  getRequiresUniverse              () const { return requires_universe; }
		bool  getRequiresEject                 () const { return requires_eject; }
		bool  getRequiresCaretaker             () const { return requires_caretaker; }

		virtual void setId                            (String  Assgn) { id = Assgn; }
		virtual void setRequiredRight                 (String  Assgn) { required_right = Assgn; }
		virtual void setScriptFunction                (String  Assgn) { script_function = Assgn; }
		virtual void setContextAvatar                 (bool  Assgn) { context_avatar = Assgn; }
		virtual void setContextConsole                (bool  Assgn) { context_console = Assgn; }
		virtual void setRequiresWorld                 (bool  Assgn) { requires_world = Assgn; }
		virtual void setRequiresUniverse              (bool  Assgn) { requires_universe = Assgn; }
		virtual void setRequiresEject                 (bool  Assgn) { requires_eject = Assgn; }
		virtual void setRequiresCaretaker             (bool  Assgn) { requires_caretaker = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "id") {
				return Conversion::ToString(id);
			}
			if (ParamName == "required_right") {
				return Conversion::ToString(required_right);
			}
			if (ParamName == "script_function") {
				return Conversion::ToString(script_function);
			}
			if (ParamName == "context_avatar") {
				return Conversion::ToString(context_avatar);
			}
			if (ParamName == "context_console") {
				return Conversion::ToString(context_console);
			}
			if (ParamName == "requires_world") {
				return Conversion::ToString(requires_world);
			}
			if (ParamName == "requires_universe") {
				return Conversion::ToString(requires_universe);
			}
			if (ParamName == "requires_eject") {
				return Conversion::ToString(requires_eject);
			}
			if (ParamName == "requires_caretaker") {
				return Conversion::ToString(requires_caretaker);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "id") { 
				id = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "required_right") { 
				required_right = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "script_function") { 
				script_function = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "context_avatar") { 
				context_avatar = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "context_console") { 
				context_console = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "requires_world") { 
				requires_world = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "requires_universe") { 
				requires_universe = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "requires_eject") { 
				requires_eject = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "requires_caretaker") { 
				requires_caretaker = Conversion::ToBool(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(id, id);
			UMBRA_PROPERTY_RW(required_right, required_right);
			UMBRA_PROPERTY_RW(script_function, script_function);
			UMBRA_PROPERTY_RW(context_avatar, context_avatar);
			UMBRA_PROPERTY_RW(context_console, context_console);
			UMBRA_PROPERTY_RW(requires_world, requires_world);
			UMBRA_PROPERTY_RW(requires_universe, requires_universe);
			UMBRA_PROPERTY_RW(requires_eject, requires_eject);
			UMBRA_PROPERTY_RW(requires_caretaker, requires_caretaker);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class ScriptedCommandPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pId            ;
		TextBox          *pRequiredRight ;
		TextBox          *pScriptFunction;
		CButton          *pContextAvatar ;
		CButton          *pContextConsole;
		CButton          *pRequiresWorld ;
		CButton          *pRequiresUniverse;
		CButton          *pRequiresEject ;
		CButton          *pRequiresCaretaker;

		
	public: // a set of the special property type
		ScriptedCommandData data;
		ScriptedCommandData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pId             = PageCtrl.create_edit      (GroupID, "", "Id");
			pRequiredRight  = PageCtrl.create_edit      (GroupID, "", "Required Right");
			pScriptFunction = PageCtrl.create_edit      (GroupID, "", "Script Function");
			pContextAvatar  = PageCtrl.create_check     (GroupID, "", "Context Avatar");
			pContextConsole = PageCtrl.create_check     (GroupID, "", "Context Console");
			pRequiresWorld  = PageCtrl.create_check     (GroupID, "", "Requires World");
			pRequiresUniverse = PageCtrl.create_check     (GroupID, "", "Requires Universe");
			pRequiresEject  = PageCtrl.create_check     (GroupID, "", "Requires Eject");
			pRequiresCaretaker = PageCtrl.create_check     (GroupID, "", "Requires Caretaker");

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
		
		bool dialog_to_attributes(ScriptedCommandData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(ScriptedCommandData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(ScriptedCommandData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.id, (*pId            ));
			exr.exchange(Attr.required_right, (*pRequiredRight ));
			exr.exchange(Attr.script_function, (*pScriptFunction));
			exr.exchange(Attr.context_avatar, (*pContextAvatar ));
			exr.exchange(Attr.context_console, (*pContextConsole));
			exr.exchange(Attr.requires_world, (*pRequiresWorld ));
			exr.exchange(Attr.requires_universe, (*pRequiresUniverse));
			exr.exchange(Attr.requires_eject, (*pRequiresEject ));
			exr.exchange(Attr.requires_caretaker, (*pRequiresCaretaker));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(ScriptedCommandData& RemoteData)
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
} }