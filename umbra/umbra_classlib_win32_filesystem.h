#pragma once

#include "umbra_remoteclass.h"
#include "umbra_classlib.h"

#include <gdiplus.h>

// UMBRA_NAMESPACE: Win32.Gdi
namespace Umbra { namespace ClassLib { namespace Win32 { namespace FileSystem {

	//
	// IMP_CLASS: Win32.FileSystem.FileInfo
	//   Info:    Read only class representing WIN32_FIND_DATA;
	//   Exposes: 
	//

	class FileInfoImpl : public Umbra::RemoteClass
	{
	public: // interface access
		WIN32_FIND_DATAA data;

	public: // lifetime
		FileInfoImpl(Umbra::Host* Owner, bool AutoTidy);

	public: // access interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest&);
	};

	//
	// IMP_CLASS: Win32.FileSystem.DirectorySearch
	//   Info:    Read only class representing WIN32_FIND_DATA;
	//   Exposes: 
	//

	class DirectorySearchImpl : public Umbra::RemoteClass
	{
	public: // searching filters
		int						filter;
		std::string				file_path;
		std::vector<Variant>	found;

	public: // lifetime
		DirectorySearchImpl(Umbra::Host* Owner, bool AutoTidy);

	public: // access interfaces
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);
	};

	//
	// DEF_CLASS: Win32.FileSystem.DirectorySearch
	//   Info:    Searches for files;
	//   Exposes: 
	//

	class DirectorySearchDef : public Umbra::RemoteClass,
							   public Umbra::INewInstanceCreator 
	{
	public: // access interfaces
		RemoteClass* doUmbraNewInstance(Host*);
	};

	//
	// ENU_CLASS: Win32.FileSystem.FileAttributes
	//   Info:    Enumeration of win32 file attributes;
	//   Exposes: 
	//

	class FileAttributesDef : public Umbra::RemoteClass
	{
	public: // access interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest&);
	};

	//
	// Win32.FileSystem
	//

	class NamespaceDef : public Umbra::RemoteClass
	{
	protected: // static enumerations
		static FileAttributesDef	_file_attributes_def;
		static DirectorySearchDef	_directory_search_def;

	public: // interface access
		void doUmbraExtensibleDefSetup();

	public: // access interfaces
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);

	public: // static instance
		static NamespaceDef& getInst() { static NamespaceDef ns; return ns; }
	};

} } } }