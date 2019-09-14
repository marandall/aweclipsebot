#pragma once

#include "umbra_classlib.h"
#include <stdio.h>
#include <windows.h>

// start of FileSystem namespace
namespace Umbra { namespace ClassLib { namespace Usl {

	//
	// File Attributes
	//

	class Win32FileData  : public RemoteClass
	{
	public: // file information
		  DWORD		dwFileAttributes;  
		  FILETIME	ftCreationTime; 
		  FILETIME	ftLastAccessTime; 
		  FILETIME	ftLastWriteTime; 
		  __int64	file_size; 
		  TCHAR		cFileName[MAX_PATH];  
		  TCHAR		cAlternateFileName[14];
	};

	//
	// File Pointer IO
	//

	class FileImpl : public RemoteClass
	{
	public: // file information
		FILE* file_ptr;

	public: // constructor and destructor
		FileImpl()
		{
			file_ptr = 0;
		}

		~FileImpl()
		{
			if (file_ptr)
			{
				fclose(file_ptr);
				file_ptr = 0;
			}
		}

	public: // umbra function handling
		bool doUmbraFunction(Variant& V, FunctionRequest& R)
		{
			// constructor
			//
			
			UMBRA_FUNCTION_BEGIN(__construct) {
				UMBRA_FUNCTION_OA(0, Path, Umbra::VT_STRING, Variant());		// expecting file path
				UMBRA_FUNCTION_OA(1, Mode, Umbra::VT_STRING, Variant());		// expecting file open mode
				UMBRA_FUNCTION_CS() 
				{
					// check for validity
					if (vaPath.IsNull() == true)
					{
					}
					else
					{
						// is the file pointer already open
						if (file_ptr != 0)
						{
								UMBRA_FUNCTION_RET( -1 );
						}
						else
						{
							errno_t err = fopen_s(&file_ptr, vaPath.toString().c_str(), vaMode.toString().c_str());
							UMBRA_FUNCTION_RET( (int)err );
						}
					}
				} 
				UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();


			UMBRA_FUNCTION_BEGIN(open) {
				UMBRA_FUNCTION_DA(0, Path, Umbra::VT_STRING);		// expecting file path
				UMBRA_FUNCTION_DA(1, Mode, Umbra::VT_STRING);		// expecting file open mode
				UMBRA_FUNCTION_CS() 
				{
					// is the file pointer already open
					if (file_ptr != 0)
					{
							UMBRA_FUNCTION_RET( -1 );
					}
					else
					{
						errno_t err = fopen_s(&file_ptr, vaPath.toString().c_str(), vaMode.toString().c_str());
						UMBRA_FUNCTION_RET( (int)err );
					}
				} 
				UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			// close
			//

			UMBRA_FUNCTION_BEGIN(close) {
				UMBRA_FUNCTION_CS() 
				{
					// is the file pointer already open
					if (file_ptr == 0)
					{
							UMBRA_FUNCTION_RET( -1 );
					}
					else
					{
						fclose(file_ptr);
						file_ptr = 0;
					}
				} 
				UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			// read a line
			//

			UMBRA_FUNCTION_BEGIN(readLine) {
				UMBRA_FUNCTION_CS() 
				{
					// is the file pointer already open
					if (file_ptr == 0)
					{
						UMBRA_FUNCTION_RET( Umbra::Variant() );
					}
					else
					{
						static char line_input[1024 * 10];
						if( fgets(line_input, sizeof (line_input), file_ptr) == 0)
						{
							UMBRA_FUNCTION_RET( Umbra::Variant() );
						}
						else
						{
							UMBRA_FUNCTION_RET( line_input );
						}
					}
				} 
				UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			// read a single line from the input stream
			//

			UMBRA_FUNCTION_BEGIN(readChar) {
				UMBRA_FUNCTION_CS() 
				{
					// is the file pointer already open
					if (file_ptr == 0)
					{
						UMBRA_FUNCTION_RET( Umbra::Variant() );
					}
					else
					{
						int ret = fgetc(file_ptr);
						if( ret == EOF)
						{
							UMBRA_FUNCTION_RET( Umbra::Variant() );
						}
						else
						{
							UMBRA_FUNCTION_RET( ret );
						}
					}
				} 
				UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			// write the file stream
			//

			UMBRA_FUNCTION_BEGIN(write) {
				UMBRA_FUNCTION_DA(0, Text, Umbra::VT_STRING);
				UMBRA_FUNCTION_CS() 
				{
					// is the file pointer already open
					if (file_ptr == 0)
					{
						UMBRA_FUNCTION_RET( Umbra::Variant() );
					}
					else
					{
						std::string text = vaText.toString();
						int ret = (int)fwrite(text.c_str(), 1, (int)text.length(), file_ptr);
						UMBRA_FUNCTION_RET( ret );
					}
				} 
				UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			// write the file stream
			//

			UMBRA_FUNCTION_BEGIN(writeLine) {
				UMBRA_FUNCTION_DA(0, Text, Umbra::VT_STRING);
				UMBRA_FUNCTION_CS() 
				{
					// is the file pointer already open
					if (file_ptr == 0)
					{
						UMBRA_FUNCTION_RET( Umbra::Variant() );
					}
					else
					{
						std::string text = vaText.toString();
						text.append("\r\n");
						int ret = (int)fwrite(text.c_str(), 1, (int)text.length(), file_ptr);
						UMBRA_FUNCTION_RET( ret );
					}
				} 
				UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			// no function identified
			return R.NOT_HANDLED;
		}
	};

	//
	// File Class
	//

	class FileClass : public RemoteClass, public Umbra::INewInstanceCreator
	{
	public:
		RemoteClass* doUmbraNewInstance(Umbra::Host* HostPtr)
		{
			return new FileImpl();
		}
	};

// end of FileSystem namespace
} } }