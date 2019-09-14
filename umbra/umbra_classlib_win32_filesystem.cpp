#include "stdafx.h"
#include "umbra_classlib_win32_filesystem.h"
#include <windows.h>

using namespace Umbra;

// start of Win32.FileSystem namespace
namespace Umbra { namespace ClassLib { namespace Win32 { namespace FileSystem {

	//
	// Static Classes for Enumeration
	//

	FileAttributesDef	NamespaceDef::_file_attributes_def;
	DirectorySearchDef	NamespaceDef::_directory_search_def;

	void NamespaceDef::doUmbraExtensibleDefSetup()
	{
		//_children["FileAttributes"] = _file_attributes_def;
		//_children["DirectorySearch"]= _directory_search_def;
	}

	bool NamespaceDef::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		//
		// Copy Files using API
		//

		UMBRA_FUNCTION_BEGIN(copyFile) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Source,  Umbra::VT_STRING);
			UMBRA_FUNCTION_DA(1, Dest,    Umbra::VT_STRING);
	
			// code information break
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( ::CopyFileA(vaSource.toString().c_str(), vaDest.toString().c_str(), TRUE) );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// Copy Files using API and overwrites if it exists
		//

		UMBRA_FUNCTION_BEGIN(copyOverWriteFile) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Source,  Umbra::VT_STRING);
			UMBRA_FUNCTION_DA(1, Dest,    Umbra::VT_STRING);
	
			// code information break
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( ::CopyFileA(vaSource.toString().c_str(), vaDest.toString().c_str(), FALSE) );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// Move Files using API
		//

		UMBRA_FUNCTION_BEGIN(moveFile) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Source,  Umbra::VT_STRING);
			UMBRA_FUNCTION_DA(1, Dest,    Umbra::VT_STRING);
	
			// code information break
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( ::MoveFileA(vaSource.toString().c_str(), vaDest.toString().c_str()) );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// no such element
		return Umbra::FunctionRequest::NOT_HANDLED;
	}

	FileInfoImpl::FileInfoImpl(Umbra::Host* Owner, bool AutoTidy)
	{
		// register as managed resource
		if (AutoTidy) setUmbraGarbageCollected();
	}

	bool FileInfoImpl::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		// file times are ignored
		// ftCreationTime;
		// ftLastAccessTime;
		// ftLastWriteTime;

		// quick reference
		DWORD dwFileAttributes = data.dwFileAttributes;

		__int64 filesize = (((__int64)data.nFileSizeHigh) << 32) | data.nFileSizeLow;

		// basic information
		UMBRA_PROPERTY_R(attributes,	(int)data.dwFileAttributes);
		UMBRA_PROPERTY_R(size,			filesize);
		UMBRA_PROPERTY_R(name,			data.cFileName);
		UMBRA_PROPERTY_R(short_name,	data.cAlternateFileName);
	
		// read only attributes
		UMBRA_PROPERTY_R(read_only,		(dwFileAttributes & FILE_ATTRIBUTE_READONLY)		!= 0);
		UMBRA_PROPERTY_R(hidden,		(dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)			!= 0);
		UMBRA_PROPERTY_R(system,		(dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)			!= 0);
		UMBRA_PROPERTY_R(directory,		(dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)		!= 0);
		UMBRA_PROPERTY_R(archive,		(dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)			!= 0);
		UMBRA_PROPERTY_R(device,		(dwFileAttributes & FILE_ATTRIBUTE_DEVICE)			!= 0);
		UMBRA_PROPERTY_R(normal,		(dwFileAttributes & FILE_ATTRIBUTE_NORMAL)			!= 0); 
		UMBRA_PROPERTY_R(tempoary,		(dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY)		!= 0);
		UMBRA_PROPERTY_R(sparse_file,	(dwFileAttributes & FILE_ATTRIBUTE_SPARSE_FILE)		!= 0);  
		UMBRA_PROPERTY_R(reparse_point,	(dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)	!= 0);
		UMBRA_PROPERTY_R(compressed,	(dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED)		!= 0);
		UMBRA_PROPERTY_R(offline,		(dwFileAttributes & FILE_ATTRIBUTE_OFFLINE)			!= 0); 
		UMBRA_PROPERTY_R(not_indexed,	(dwFileAttributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED) != 0);
		UMBRA_PROPERTY_R(encrypted,		(dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)		!= 0);

		// is it an ignorable directory
		bool dot_directory = strcmp(data.cFileName, ".") == 0 || strcmp(data.cFileName, "..") == 0;
		UMBRA_PROPERTY_R(dot_directory,	dot_directory);

		// no such element
		return Umbra::PropertyRequest::NOT_HANDLED;
	}

	DirectorySearchImpl::DirectorySearchImpl(Umbra::Host* Owner, bool AutoTidy)
	{
		// register as managed resource
		if (AutoTidy) setUmbraGarbageCollected();

		// allow all masks
		filter = 0;
	}

	RemoteClass* DirectorySearchDef::doUmbraNewInstance(Host* HostPtr)
	{
		return new DirectorySearchImpl(HostPtr, true);
	}

	bool DirectorySearchImpl::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		//
		// searches through the allocated directory
		//

		UMBRA_FUNCTION_BEGIN(search) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Path,  Umbra::VT_STRING);
	
			// code information break
			UMBRA_FUNCTION_CS() {
				// erase the current list
				found.clear();
				found.reserve(50);

				// load the first file
				WIN32_FIND_DATAA find_data;
				HANDLE h = FindFirstFileA(vaPath.toString().c_str(), &find_data);

				// check if invalid
				if (h == INVALID_HANDLE_VALUE)
				{
					UMBRA_FUNCTION_RET( -1 );
				}
				else
				{
					BOOL should_continue = FALSE;
					do
					{
						// create new instance of file info
						if ((find_data.dwFileAttributes & filter) == filter)
						{
							FileInfoImpl* pImpl = new FileInfoImpl(0, true);
							pImpl->data = find_data;
							found.push_back(*pImpl);

							// almost exponential growth
							if (found.size() == 50)
								found.resize(100);
							else if (found.size() == 100)
								found.resize(200);
							else if (found.size() == 200)
								found.resize(800);
							else if (found.size() == 800)
								found.resize(2000);
						}

						// proceed to next index
						should_continue = FindNextFileA(h, &find_data);
					} while (should_continue != FALSE);
				
					// restrict the size to what is needed
					if (found.size() > 50)
						found.resize(found.size());

					// close search and return total number of files found
					FindClose(h);
					UMBRA_FUNCTION_RET( (int)found.size() );
				}
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		//
		// accesses a information
		//

		UMBRA_FUNCTION_BEGIN(get) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Index, Umbra::VT_INTEGER);
	
			// code information break
			UMBRA_FUNCTION_CS() {
				// copy the index
				int index = vaIndex.toInt32();

				// return the index
				if (index >= 0 && index < (int)found.size())
				{
					UMBRA_FUNCTION_RET( found[index] );
				}
				else
				{
					UMBRA_FUNCTION_RET( Variant() );
				}
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// no such element
		return Umbra::FunctionRequest::NOT_HANDLED;
	}

	bool DirectorySearchImpl::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		// only the filter can be read and written
		UMBRA_PROPERTY_RW(filter, filter);

		// count of items
		UMBRA_PROPERTY_R(found_count, (int)found.size());

		// no such element
		return Umbra::PropertyRequest::NOT_HANDLED;
	}

	bool FileAttributesDef::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		//
		// The enumeration types for the file system
		//

		UMBRA_PROPERTY_R(READONLY,				FILE_ATTRIBUTE_READONLY);
		UMBRA_PROPERTY_R(HIDDEN,				FILE_ATTRIBUTE_HIDDEN);
		UMBRA_PROPERTY_R(SYSTEM,				FILE_ATTRIBUTE_SYSTEM);
		UMBRA_PROPERTY_R(DIRECTORY,				FILE_ATTRIBUTE_DIRECTORY);
		UMBRA_PROPERTY_R(ARCHIVE,				FILE_ATTRIBUTE_ARCHIVE);
		UMBRA_PROPERTY_R(DEVICE,				FILE_ATTRIBUTE_DEVICE);
		UMBRA_PROPERTY_R(NORMAL,				FILE_ATTRIBUTE_NORMAL);
		UMBRA_PROPERTY_R(TEMPOARY,				FILE_ATTRIBUTE_TEMPORARY);
		UMBRA_PROPERTY_R(SPARSE_FILE,			FILE_ATTRIBUTE_SPARSE_FILE);
		UMBRA_PROPERTY_R(REPARSE_POINT,			FILE_ATTRIBUTE_REPARSE_POINT);
		UMBRA_PROPERTY_R(COMPRESSED,			FILE_ATTRIBUTE_COMPRESSED);
		UMBRA_PROPERTY_R(OFFLINE,				FILE_ATTRIBUTE_OFFLINE);
		UMBRA_PROPERTY_R(NOT_CONTENT_INDEXED,	FILE_ATTRIBUTE_NOT_CONTENT_INDEXED);
		UMBRA_PROPERTY_R(ENCRYPTED,				FILE_ATTRIBUTE_ENCRYPTED);

		// no such element
		return Umbra::PropertyRequest::NOT_HANDLED;
	}

// end of Win32.FileSystem namespace
} } } }