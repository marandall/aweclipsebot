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
	class EjectionPropertyBuilder;

	//
	// Import Data
	//
	// a:6:{i:0;a:4:{s:4:"name";s:6:"enable";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:22:"max_eject_length_users";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:22:"max_eject_length_admin";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:15:"require_comment";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:20:"send_ejection_notice";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:5;a:4:{s:4:"name";s:24:"max_radial_teleport_size";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class EjectionData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef EjectionPropertyBuilder PropertyBuilder;
	
	public: // source data
		bool enable;
		int         max_eject_length_users;
		int         max_eject_length_admin;
		bool require_comment;
		bool send_ejection_notice;
		int         max_radial_teleport_size;

		
	public:
		EjectionData()
		{
			enable = true;
			max_eject_length_users = 1440;
			max_eject_length_admin = 0;
			require_comment = true;
			send_ejection_notice = true;
			max_radial_teleport_size = 100;

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
			char* section_id = const_cast<char*>("cis_Ejection");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "enable", enable);
			SerializerSystem.Exchange(ExchangeMode, section_id, "max_eject_length_users", max_eject_length_users);
			SerializerSystem.Exchange(ExchangeMode, section_id, "max_eject_length_admin", max_eject_length_admin);
			SerializerSystem.Exchange(ExchangeMode, section_id, "require_comment", require_comment);
			SerializerSystem.Exchange(ExchangeMode, section_id, "send_ejection_notice", send_ejection_notice);
			SerializerSystem.Exchange(ExchangeMode, section_id, "max_radial_teleport_size", max_radial_teleport_size);

		}
		
		EjectionData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		bool  getEnable                        () const { return enable; }
		int          getMaxEjectLengthUsers           () const { return max_eject_length_users; }
		int          getMaxEjectLengthAdmin           () const { return max_eject_length_admin; }
		bool  getRequireComment                () const { return require_comment; }
		bool  getSendEjectionNotice            () const { return send_ejection_notice; }
		int          getMaxRadialTeleportSize         () const { return max_radial_teleport_size; }

		virtual void setEnable                        (bool  Assgn) { enable = Assgn; }
		virtual void setMaxEjectLengthUsers           (int          Assgn) { max_eject_length_users = Assgn; }
		virtual void setMaxEjectLengthAdmin           (int          Assgn) { max_eject_length_admin = Assgn; }
		virtual void setRequireComment                (bool  Assgn) { require_comment = Assgn; }
		virtual void setSendEjectionNotice            (bool  Assgn) { send_ejection_notice = Assgn; }
		virtual void setMaxRadialTeleportSize         (int          Assgn) { max_radial_teleport_size = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
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
			if (ParamName == "require_comment") {
				return Conversion::ToString(require_comment);
			}
			if (ParamName == "send_ejection_notice") {
				return Conversion::ToString(send_ejection_notice);
			}
			if (ParamName == "max_radial_teleport_size") {
				return Conversion::ToString(max_radial_teleport_size);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
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
			if (ParamName == "require_comment") { 
				require_comment = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "send_ejection_notice") { 
				send_ejection_notice = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "max_radial_teleport_size") { 
				max_radial_teleport_size = Conversion::ToInt(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(enable, enable);
			UMBRA_PROPERTY_RW(max_eject_length_users, max_eject_length_users);
			UMBRA_PROPERTY_RW(max_eject_length_admin, max_eject_length_admin);
			UMBRA_PROPERTY_RW(require_comment, require_comment);
			UMBRA_PROPERTY_RW(send_ejection_notice, send_ejection_notice);
			UMBRA_PROPERTY_RW(max_radial_teleport_size, max_radial_teleport_size);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class EjectionPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CButton          *pEnable        ;
		TextBox          *pMaxEjectLengthUsers;
		TextBox          *pMaxEjectLengthAdmin;
		CButton          *pRequireComment;
		CButton          *pSendEjectionNotice;
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
			pRequireComment = PageCtrl.create_check     (GroupID, "", "Require Comment");
			pSendEjectionNotice = PageCtrl.create_check     (GroupID, "", "Send Notice");
			pMaxRadialTeleportSize = PageCtrl.create_edit      (GroupID, "", "Max Radial Teleport Size");

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
		
		bool dialog_to_attributes(EjectionData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
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
			exr.exchange(Attr.require_comment, (*pRequireComment));
			exr.exchange(Attr.send_ejection_notice, (*pSendEjectionNotice));
			exr.exchange(Attr.max_radial_teleport_size, (*pMaxRadialTeleportSize));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(EjectionData& RemoteData)
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