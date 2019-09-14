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
	class GlobalChatAexPropertyBuilder;

	//
	// Import Data
	//
	// a:9:{i:0;a:4:{s:4:"name";s:13:"impersonation";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:3:"tag";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:6:"colour";s:4:"type";s:10:"aw::colour";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:25:"allow_user_colour_changes";s:4:"type";s:5:"tsb_t";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:4:"bold";s:4:"type";s:5:"tsb_t";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:5;a:4:{s:4:"name";s:6:"italic";s:4:"type";s:5:"tsb_t";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:6;a:4:{s:4:"name";s:12:"session_bold";s:4:"type";s:5:"tsb_t";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:7;a:4:{s:4:"name";s:14:"session_italic";s:4:"type";s:5:"tsb_t";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:8;a:4:{s:4:"name";s:13:"session_color";s:4:"type";s:10:"aw::colour";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class GlobalChatAexData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef GlobalChatAexPropertyBuilder PropertyBuilder;
	
	public: // source data
		String impersonation;
		String tag;
		AW::Colour  colour;
		tsb_t       allow_user_colour_changes;
		tsb_t       bold;
		tsb_t       italic;
		tsb_t       session_bold;
		tsb_t       session_italic;
		AW::Colour  session_color;

		
	public:
		GlobalChatAexData()
		{
			impersonation = "";
			tag = "";
			colour = 0xFFFFFF;
			allow_user_colour_changes = B_EMPTY;
			bold = B_EMPTY;
			italic = B_EMPTY;
			session_bold = B_EMPTY;
			session_italic = B_EMPTY;
			session_color = 0xFFFFFF;

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
			char* section_id = const_cast<char*>("relay_GlobalChatAex");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "impersonation", impersonation);
			SerializerSystem.Exchange(ExchangeMode, section_id, "tag", tag);
			SerializerSystem.Exchange(ExchangeMode, section_id, "colour", colour);
			SerializerSystem.Exchange(ExchangeMode, section_id, "allow_user_colour_changes", allow_user_colour_changes);
			SerializerSystem.Exchange(ExchangeMode, section_id, "bold", bold);
			SerializerSystem.Exchange(ExchangeMode, section_id, "italic", italic);
			SerializerSystem.Exchange(ExchangeMode, section_id, "session_bold", session_bold);
			SerializerSystem.Exchange(ExchangeMode, section_id, "session_italic", session_italic);
			SerializerSystem.Exchange(ExchangeMode, section_id, "session_color", session_color);

		}
		
		GlobalChatAexData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		String  getImpersonation                 () const { return impersonation; }
		String  getTag                           () const { return tag; }
		AW::Colour   getColour                        () const { return colour; }
		tsb_t        getAllowUserColourChanges        () const { return allow_user_colour_changes; }
		tsb_t        getBold                          () const { return bold; }
		tsb_t        getItalic                        () const { return italic; }
		tsb_t        getSessionBold                   () const { return session_bold; }
		tsb_t        getSessionItalic                 () const { return session_italic; }
		AW::Colour   getSessionColor                  () const { return session_color; }

		virtual void setImpersonation                 (String  Assgn) { impersonation = Assgn; }
		virtual void setTag                           (String  Assgn) { tag = Assgn; }
		virtual void setColour                        (AW::Colour   Assgn) { colour = Assgn; }
		virtual void setAllowUserColourChanges        (tsb_t        Assgn) { allow_user_colour_changes = Assgn; }
		virtual void setBold                          (tsb_t        Assgn) { bold = Assgn; }
		virtual void setItalic                        (tsb_t        Assgn) { italic = Assgn; }
		virtual void setSessionBold                   (tsb_t        Assgn) { session_bold = Assgn; }
		virtual void setSessionItalic                 (tsb_t        Assgn) { session_italic = Assgn; }
		virtual void setSessionColor                  (AW::Colour   Assgn) { session_color = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
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
			if (ParamName == "session_bold") {
				return Conversion::ToStringTSB(session_bold);
			}
			if (ParamName == "session_italic") {
				return Conversion::ToStringTSB(session_italic);
			}
			if (ParamName == "session_color") {
				return Conversion::ToString(session_color);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
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
			if (ParamName == "session_bold") { 
				session_bold = Conversion::ToTSB(Value);
				return true;
			}
			if (ParamName == "session_italic") { 
				session_italic = Conversion::ToTSB(Value);
				return true;
			}
			if (ParamName == "session_color") { 
				session_color = Conversion::ToColour(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(impersonation, impersonation);
			UMBRA_PROPERTY_RW(tag, tag);
			UMBRA_PROPERTY_RW(colour, colour);
			UMBRA_PROPERTY_RW(allow_user_colour_changes, allow_user_colour_changes);
			UMBRA_PROPERTY_RW(bold, bold);
			UMBRA_PROPERTY_RW(italic, italic);
			UMBRA_PROPERTY_RW(session_bold, session_bold);
			UMBRA_PROPERTY_RW(session_italic, session_italic);
			UMBRA_PROPERTY_RW(session_color, session_color);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
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
		CButton          *pSessionBold   ;
		CButton          *pSessionItalic ;
		ColourButton     *pSessionColor  ;

		
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
			pSessionBold    = PageCtrl.create_tristate  (GroupID, "", "Session Bold");
			pSessionItalic  = PageCtrl.create_tristate  (GroupID, "", "Session Italic");
			pSessionColor   = PageCtrl.create_colour    (GroupID, "", "Session Color");

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
		
		bool dialog_to_attributes(GlobalChatAexData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
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
			exr.exchange(Attr.session_bold, (*pSessionBold   ));
			exr.exchange(Attr.session_italic, (*pSessionItalic ));
			exr.exchange(Attr.session_color, (*pSessionColor  ));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(GlobalChatAexData& RemoteData)
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