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
	class GlobalChatAexPropertyBuilder;

	class GlobalChatAexData : 
		public Umbra::RemoteClass,
		public IRawParamEditor
	{
	public: // type defenition for property builder
		typedef GlobalChatAexPropertyBuilder PropertyBuilder;
	
	public: // source data
		Ascii       impersonation;
		Ascii       tag;
		AW::Colour  colour;
		tsb_t       allow_user_colour_changes;
		tsb_t       bold;
		tsb_t       italic;

		
	public:
		GlobalChatAexData()
		{
			impersonation = "";
			tag = "";
			colour = 0xFFFFFF;
			allow_user_colour_changes = B_EMPTY;
			bold = 0x777777;
			italic = B_EMPTY;

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
			char* section_id = const_cast<char*>("relay_GlobalChatAex");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "impersonation", impersonation);
			SerializerSystem.Exchange(ExchangeMode, section_id, "tag", tag);
			SerializerSystem.Exchange(ExchangeMode, section_id, "colour", colour);
			SerializerSystem.Exchange(ExchangeMode, section_id, "allow_user_colour_changes", allow_user_colour_changes);
			SerializerSystem.Exchange(ExchangeMode, section_id, "bold", bold);
			SerializerSystem.Exchange(ExchangeMode, section_id, "italic", italic);

		}
		
		GlobalChatAexData& GetDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual void DataUpdated() { }
		
	public: // read only operators
		Ascii        get_Impersonation                 () const { return impersonation; }
		Ascii        get_Tag                           () const { return tag; }
		AW::Colour   get_Colour                        () const { return colour; }
		tsb_t        get_AllowUserColourChanges        () const { return allow_user_colour_changes; }
		tsb_t        get_Bold                          () const { return bold; }
		tsb_t        get_Italic                        () const { return italic; }

		virtual void set_Impersonation                 (Ascii        Assgn) { impersonation = Assgn; }
		virtual void set_Tag                           (Ascii        Assgn) { tag = Assgn; }
		virtual void set_Colour                        (AW::Colour   Assgn) { colour = Assgn; }
		virtual void set_AllowUserColourChanges        (tsb_t        Assgn) { allow_user_colour_changes = Assgn; }
		virtual void set_Bold                          (tsb_t        Assgn) { bold = Assgn; }
		virtual void set_Italic                        (tsb_t        Assgn) { italic = Assgn; }

		
	public: // string conversion
		virtual Ascii param_read(Ascii ParamName)
		{
			if (ParamName == "impersonation") {
				return Conversion::ToString(impersonation);
			}
			if (ParamName == "tag") {
				return Conversion::ToString(tag);
			}
			if (ParamName == "colour") {
				return Conversion::ToString(colour);
			}
			if (ParamName == "allow_user_colour_changes") {
				return Conversion::ToStringTSB(allow_user_colour_changes);
			}
			if (ParamName == "bold") {
				return Conversion::ToStringTSB(bold);
			}
			if (ParamName == "italic") {
				return Conversion::ToStringTSB(italic);
			}

			return "";
		}
		
		virtual bool param_write(Ascii ParamName, Ascii Value)
		{
			if (ParamName == "impersonation") { 
				impersonation = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "tag") { 
				tag = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "colour") { 
				colour = Conversion::ToColour(Value);
				return true;
			}
			if (ParamName == "allow_user_colour_changes") { 
				allow_user_colour_changes = Conversion::ToTSB(Value);
				return true;
			}
			if (ParamName == "bold") { 
				bold = Conversion::ToTSB(Value);
				return true;
			}
			if (ParamName == "italic") { 
				italic = Conversion::ToTSB(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		Umbra::Variant Umbra_GetProperty(Umbra::string ID, Umbra::ES_GetProperty& ES)
		{
			if (strcmp(ID.c_str(), "impersonation"               ) == 0) return (const char*)impersonation;
			if (strcmp(ID.c_str(), "tag"                         ) == 0) return (const char*)tag;
			if (strcmp(ID.c_str(), "colour"                      ) == 0) return colour.value;
			if (strcmp(ID.c_str(), "allow_user_colour_changes"   ) == 0) return allow_user_colour_changes;
			if (strcmp(ID.c_str(), "bold"                        ) == 0) return bold;
			if (strcmp(ID.c_str(), "italic"                      ) == 0) return italic;

			
			// nothing found
			throw Umbra::UnsupportedPropertyException(ID);
		}	
	};

	
	class GlobalChatAexPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		TextBox          *pImpersonation ;
		TextBox          *pTag           ;
		ColourButton     *pColour        ;
		CButton          *pAllowUserColourChanges;
		CButton          *pBold          ;
		CButton          *pItalic        ;

		
	public: // a set of the special property type
		GlobalChatAexData data;
		GlobalChatAexData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pImpersonation  = PageCtrl.create_edit      (GroupID, "", "Impersonation");
			pTag            = PageCtrl.create_edit      (GroupID, "", "Tag");
			pColour         = PageCtrl.create_colour    (GroupID, "", "Colour");
			pAllowUserColourChanges = PageCtrl.create_tristate  (GroupID, "", "Allow User Colour Changes");
			pBold           = PageCtrl.create_tristate  (GroupID, "", "Bold");
			pItalic         = PageCtrl.create_tristate  (GroupID, "", "Italic");

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
		
		void dialog_to_attributes(GlobalChatAexData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
		}
		
		void attributes_to_dialog(GlobalChatAexData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(GlobalChatAexData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.impersonation, (*pImpersonation ));
			exr.exchange(Attr.tag, (*pTag           ));
			exr.exchange(Attr.colour, (*pColour        ));
			exr.exchange(Attr.allow_user_colour_changes, (*pAllowUserColourChanges));
			exr.exchange(Attr.bold, (*pBold          ));
			exr.exchange(Attr.italic, (*pItalic        ));

			
			if (Mode == DataExchanger::FROM_CONTROL)
			{
				Attr.DataUpdated();
			}
			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(GlobalChatAexData& RemoteData)
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