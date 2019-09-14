#pragma once
#include "serializer.h"
#include "tsb.h"
#include "umbra_remote_class.h"
#include "raw_param_editor.h"
#include "PropertyScriptCtrl.h"
#include "GlobalDex.h"

namespace CIS { namespace Config { 
namespace AutoTemplates 
{
	class EjectionPropertyBuilder;

	class EjectionData : 
		public Umbra::RemoteClass,
		public IRawParamEditor
	{
	public: // type defenition for property builder
		typedef EjectionPropertyBuilder PropertyBuilder;
	
	public: // source data
		bool        enable;
		int         max_eject_length_users;
		int         max_eject_length_admin;
		bool        enable_ent;
		int         max_radial_teleport_size;

		
	public:
		EjectionData()
		{
			enable = true;
			max_eject_length_users = 1440;
			max_eject_length_admin = 0;
			enable_ent = true;
			max_radial_teleport_size = 100;

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
			char* section_id = const_cast<char*>("cis_Ejection");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "enable", enable);
			SerializerSystem.Exchange(ExchangeMode, section_id, "max_eject_length_users", max_eject_length_users);
			SerializerSystem.Exchange(ExchangeMode, section_id, "max_eject_length_admin", max_eject_length_admin);
			SerializerSystem.Exchange(ExchangeMode, section_id, "enable_ent", enable_ent);
			SerializerSystem.Exchange(ExchangeMode, section_id, "max_radial_teleport_size", max_radial_teleport_size);

		}
		
		EjectionData& GetDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual void DataUpdated() { }
		
	public: // read only operators
		bool         get_Enable                        () const { return enable; }
		int          get_MaxEjectLengthUsers           () const { return max_eject_length_users; }
		int          get_MaxEjectLengthAdmin           () const { return max_eject_length_admin; }
		bool         get_EnableEnt                     () const { return enable_ent; }
		int          get_MaxRadialTeleportSize         () const { return max_radial_teleport_size; }

		virtual void set_Enable                        (bool         Assgn) { enable = Assgn; }
		virtual void set_MaxEjectLengthUsers           (int          Assgn) { max_eject_length_users = Assgn; }
		virtual void set_MaxEjectLengthAdmin           (int          Assgn) { max_eject_length_admin = Assgn; }
		virtual void set_EnableEnt                     (bool         Assgn) { enable_ent = Assgn; }
		virtual void set_MaxRadialTeleportSize         (int          Assgn) { max_radial_teleport_size = Assgn; }

		
	public: // string conversion
		virtual Ascii param_read(Ascii ParamName)
		{
			if (ParamName == "enable") {
				return Conversion::ToString(enable);
			}
			if (ParamName == "max_eject_length_users") {
				return Conversion::ToString(max_eject_length_users);
			}
			if (ParamName == "max_eject_length_admin") {
				return Conversion::ToString(max_eject_length_admin);
			}
			if (ParamName == "enable_ent") {
				return Conversion::ToString(enable_ent);
			}
			if (ParamName == "max_radial_teleport_size") {
				return Conversion::ToString(max_radial_teleport_size);
			}

			return "";
		}
		
		virtual bool param_write(Ascii ParamName, Ascii Value)
		{
			if (ParamName == "enable") { 
				enable = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "max_eject_length_users") { 
				max_eject_length_users = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "max_eject_length_admin") { 
				max_eject_length_admin = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "enable_ent") { 
				enable_ent = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "max_radial_teleport_size") { 
				max_radial_teleport_size = Conversion::ToInt(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		Umbra::Variant Umbra_GetProperty(Umbra::string ID, Umbra::ES_GetProperty& ES)
		{
			if (strcmp(ID.c_str(), "enable"                      ) == 0) return enable;
			if (strcmp(ID.c_str(), "max_eject_length_users"      ) == 0) return max_eject_length_users;
			if (strcmp(ID.c_str(), "max_eject_length_admin"      ) == 0) return max_eject_length_admin;
			if (strcmp(ID.c_str(), "enable_ent"                  ) == 0) return enable_ent;
			if (strcmp(ID.c_str(), "max_radial_teleport_size"    ) == 0) return max_radial_teleport_size;

			
			// nothing found
			throw Umbra::UnsupportedPropertyException(ID);
		}	
	};

	
	class EjectionPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CButton          *pEnable        ;
		TextBox          *pMaxEjectLengthUsers;
		TextBox          *pMaxEjectLengthAdmin;
		CButton          *pEnableEnt     ;
		TextBox          *pMaxRadialTeleportSize;

		
	public: // a set of the special property type
		EjectionData data;
		EjectionData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pEnable         = PageCtrl.create_check     (GroupID, "", "Enable Ejection");
			pMaxEjectLengthUsers = PageCtrl.create_edit      (GroupID, "", "Max Eject Length (std)");
			pMaxEjectLengthAdmin = PageCtrl.create_edit      (GroupID, "", "Max Eject Length (admin)");
			pEnableEnt      = PageCtrl.create_check     (GroupID, "", "Enable Traps");
			pMaxRadialTeleportSize = PageCtrl.create_edit      (GroupID, "", "Max Radial Teleport Size");

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
		
		void dialog_to_attributes(EjectionData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
		}
		
		void attributes_to_dialog(EjectionData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(EjectionData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.enable, (*pEnable        ));
			exr.exchange(Attr.max_eject_length_users, (*pMaxEjectLengthUsers));
			exr.exchange(Attr.max_eject_length_admin, (*pMaxEjectLengthAdmin));
			exr.exchange(Attr.enable_ent, (*pEnableEnt     ));
			exr.exchange(Attr.max_radial_teleport_size, (*pMaxRadialTeleportSize));

			
			if (Mode == DataExchanger::FROM_CONTROL)
			{
				Attr.DataUpdated();
			}
			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(EjectionData& RemoteData)
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
} }