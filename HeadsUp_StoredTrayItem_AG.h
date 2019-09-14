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

namespace HeadsUp { 
namespace AutoTemplates 
{
	class StoredTrayItemPropertyBuilder;

	//
	// Import Data
	//
	// a:10:{i:0;a:4:{s:4:"name";s:7:"enabled";s:4:"type";s:4:"bool";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:1;a:4:{s:4:"name";s:3:"key";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:2;a:4:{s:4:"name";s:3:"row";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:3;a:4:{s:4:"name";s:5:"index";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:4;a:4:{s:4:"name";s:7:"pattern";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:5;a:4:{s:4:"name";s:5:"image";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:6;a:4:{s:4:"name";s:6:"size_x";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:7;a:4:{s:4:"name";s:6:"size_z";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:8;a:4:{s:4:"name";s:12:"click_method";s:4:"type";s:3:"int";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}i:9;a:4:{s:4:"name";s:14:"click_function";s:4:"type";s:6:"string";s:4:"info";s:0:"";s:2:"rw";s:2:"rw";}}
	//
	
	class StoredTrayItemData : 
		public Umbra::RemoteClass,
		public IRawParamEditor,
		public AutoNumberIdentifier,
		public IConfigLoader
	{
	public: // type defenition for property builder
		typedef StoredTrayItemPropertyBuilder PropertyBuilder;
	
	public: // source data
		bool enabled;
		String key;
		String row;
		int         index;
		String pattern;
		String image;
		int         size_x;
		int         size_z;
		int         click_method;
		String click_function;

		
	public:
		StoredTrayItemData()
		{
			enabled = false;
			key = "";
			row = "";
			index = 0;
			pattern = "";
			image = "";
			size_x = 64;
			size_z = 64;
			click_method = 0;
			click_function = "";

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
			char* section_id = const_cast<char*>("hud_StoredTrayItem");
			if (IniKey) section_id = const_cast<char*>(IniKey);
			
			SerializerSystem.Exchange(ExchangeMode, section_id, "enabled", enabled);
			SerializerSystem.Exchange(ExchangeMode, section_id, "key", key);
			SerializerSystem.Exchange(ExchangeMode, section_id, "row", row);
			SerializerSystem.Exchange(ExchangeMode, section_id, "index", index);
			SerializerSystem.Exchange(ExchangeMode, section_id, "pattern", pattern);
			SerializerSystem.Exchange(ExchangeMode, section_id, "image", image);
			SerializerSystem.Exchange(ExchangeMode, section_id, "size_x", size_x);
			SerializerSystem.Exchange(ExchangeMode, section_id, "size_z", size_z);
			SerializerSystem.Exchange(ExchangeMode, section_id, "click_method", click_method);
			SerializerSystem.Exchange(ExchangeMode, section_id, "click_function", click_function);

		}
		
		StoredTrayItemData& getDataRef()
		{
			return *this;
		}
		
	public: // parent notification methods
		virtual bool onUpdated() { return true; }
		
	public: // read only operators
		bool  getEnabled                       () const { return enabled; }
		String  getKey                           () const { return key; }
		String  getRow                           () const { return row; }
		int          getIndex                         () const { return index; }
		String  getPattern                       () const { return pattern; }
		String  getImage                         () const { return image; }
		int          getSizeX                         () const { return size_x; }
		int          getSizeZ                         () const { return size_z; }
		int          getClickMethod                   () const { return click_method; }
		String  getClickFunction                 () const { return click_function; }

		virtual void setEnabled                       (bool  Assgn) { enabled = Assgn; }
		virtual void setKey                           (String  Assgn) { key = Assgn; }
		virtual void setRow                           (String  Assgn) { row = Assgn; }
		virtual void setIndex                         (int          Assgn) { index = Assgn; }
		virtual void setPattern                       (String  Assgn) { pattern = Assgn; }
		virtual void setImage                         (String  Assgn) { image = Assgn; }
		virtual void setSizeX                         (int          Assgn) { size_x = Assgn; }
		virtual void setSizeZ                         (int          Assgn) { size_z = Assgn; }
		virtual void setClickMethod                   (int          Assgn) { click_method = Assgn; }
		virtual void setClickFunction                 (String  Assgn) { click_function = Assgn; }

		
	public: // string conversion
		virtual String param_read(String ParamName)
		{
			if (ParamName == "enabled") {
				return Conversion::ToString(enabled);
			}
			if (ParamName == "key") {
				return Conversion::ToString(key);
			}
			if (ParamName == "row") {
				return Conversion::ToString(row);
			}
			if (ParamName == "index") {
				return Conversion::ToString(index);
			}
			if (ParamName == "pattern") {
				return Conversion::ToString(pattern);
			}
			if (ParamName == "image") {
				return Conversion::ToString(image);
			}
			if (ParamName == "size_x") {
				return Conversion::ToString(size_x);
			}
			if (ParamName == "size_z") {
				return Conversion::ToString(size_z);
			}
			if (ParamName == "click_method") {
				return Conversion::ToString(click_method);
			}
			if (ParamName == "click_function") {
				return Conversion::ToString(click_function);
			}

			return "";
		}
		
		virtual bool param_write(String ParamName, String Value)
		{
			if (ParamName == "enabled") { 
				enabled = Conversion::ToBool(Value);
				return true;
			}
			if (ParamName == "key") { 
				key = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "row") { 
				row = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "index") { 
				index = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "pattern") { 
				pattern = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "image") { 
				image = Conversion::ToString(Value);
				return true;
			}
			if (ParamName == "size_x") { 
				size_x = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "size_z") { 
				size_z = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "click_method") { 
				click_method = Conversion::ToInt(Value);
				return true;
			}
			if (ParamName == "click_function") { 
				click_function = Conversion::ToString(Value);
				return true;
			}

			return false;
		}	
		
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
		    UMBRA_PROPERTY_RW(enabled, enabled);
			UMBRA_PROPERTY_RW(key, key);
			UMBRA_PROPERTY_RW(row, row);
			UMBRA_PROPERTY_RW(index, index);
			UMBRA_PROPERTY_RW(pattern, pattern);
			UMBRA_PROPERTY_RW(image, image);
			UMBRA_PROPERTY_RW(size_x, size_x);
			UMBRA_PROPERTY_RW(size_z, size_z);
			UMBRA_PROPERTY_RW(click_method, click_method);
			UMBRA_PROPERTY_RW(click_function, click_function);

			
			// nothing found
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	
	class StoredTrayItemPropertyBuilder : public PropertyBuilderBase
	{
	public: // list of object variables
		CButton          *pEnabled       ;
		TextBox          *pKey           ;
		TextBox          *pRow           ;
		TextBox          *pIndex         ;
		TextBox          *pPattern       ;
		CStatic			 *pOverrides     ;
		TextBox          *pImage         ;
		TextBox          *pSizeX         ;
		TextBox          *pSizeZ         ;
		CStatic			 *pCm            ;
		ComboBox         *pClickMethod   ;
		TextBox          *pClickFunction ;

		
	public: // a set of the special property type
		StoredTrayItemData data;
		StoredTrayItemData* pRemoteData;
		
	public: // functions
		void create_objects(PropertyScriptCtrl &PageCtrl, int GroupID)
		{
			pEnabled        = PageCtrl.create_check     (GroupID, "", "Enabled");
			pKey            = PageCtrl.create_edit      (GroupID, "", "Key");
			pRow            = PageCtrl.create_edit      (GroupID, "", "Row");
			pIndex          = PageCtrl.create_edit      (GroupID, "", "Index");
			pPattern        = PageCtrl.create_edit      (GroupID, "", "Pattern");
			pOverrides      = PageCtrl.create_label     (GroupID, "", "Image Setup");
			pImage          = PageCtrl.create_edit      (GroupID, "", "Image");
			pSizeX          = PageCtrl.create_edit      (GroupID, "", "Size X");
			pSizeZ          = PageCtrl.create_edit      (GroupID, "", "Size Z");
			pCm             = PageCtrl.create_label     (GroupID, "", "Click Method");
			pClickMethod    = PageCtrl.create_combo     (GroupID, "", "Click Method");
			pClickFunction  = PageCtrl.create_edit      (GroupID, "", "Click Function");

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
		
		bool dialog_to_attributes(StoredTrayItemData& Attr)
		{
			data_exchange(Attr, DataExchanger::FROM_CONTROL);
			return Attr.onUpdated();
		}
		
		void attributes_to_dialog(StoredTrayItemData& Attr)
		{
			data_exchange(Attr, DataExchanger::TO_CONTROL);
		}
		
		void data_exchange(StoredTrayItemData& Attr, bool Mode)
		{
			DataExchanger exr(Mode);
			exr.exchange(Attr.enabled, (*pEnabled       ));
			exr.exchange(Attr.key, (*pKey           ));
			exr.exchange(Attr.row, (*pRow           ));
			exr.exchange(Attr.index, (*pIndex         ));
			exr.exchange(Attr.pattern, (*pPattern       ));
			exr.exchange(Attr.image, (*pImage         ));
			exr.exchange(Attr.size_x, (*pSizeX         ));
			exr.exchange(Attr.size_z, (*pSizeZ         ));
			exr.exchange(Attr.click_method, (*pClickMethod   ));
			exr.exchange(Attr.click_function, (*pClickFunction ));


			return;
		}
		
		// upgrades for the property builder helper
		PropertyBuilderBase& assign_remote(StoredTrayItemData& RemoteData)
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