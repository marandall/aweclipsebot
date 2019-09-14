#pragma once
#include "umbra_classlib.h"
#include <vector>

namespace Umbra { namespace ClassLib { namespace Usl {

	//
	// Usl.FileInfo
	//

	class FileInfoImpl : public RemoteClass
	{
	public: // interface access
		WIN32_FIND_DATAA data;

	public: // constructor
		FileInfoImpl(Umbra::Host* HostObject = 0, bool GarbageCollect = false)
		{
			if (GarbageCollect) setUmbraGarbageCollected();
		}

		~FileInfoImpl()
		{
		}

	public: // property access from system
		bool doUmbraFunction(Variant& V, FunctionRequest& R)
		{
			UMBRA_FUNCTION_BEGIN(__construct) {
				UMBRA_FUNCTION_OA(0, FilePath, Umbra::VT_ANY, "c:");
				UMBRA_FUNCTION_CS() {
					HANDLE hFind = FindFirstFileA(vaFilePath.toString().c_str(), &data);
					if (hFind != INVALID_HANDLE_VALUE)
					{
						FindClose(hFind);
					}
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// No such function was known
			//

			return R.NOT_HANDLED;
		}

		bool doUmbraProperty(Variant& V, PropertyRequest& R)
		{
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

			// unable to service request
			return Umbra::PropertyRequest::NOT_HANDLED;
		}
	};

	//
	// Defenition for the Usl.FileInfo
	//

	class FileInfoClass : public RemoteClass, public INewInstanceCreator
	{
	public: // the instance creator interface
		RemoteClass* doUmbraNewInstance(Host* HostPtr)
		{
			return new FileInfoImpl(HostPtr, true);
		}
	};

} } }