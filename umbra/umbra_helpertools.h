#pragma once

#include <list>
#include <string>

using std::string;

namespace Umbra
{
	namespace HelperTools
	{
		class AutoNumber
		{
		public:
			static int getNext()
			{
				static int x = 0;
				return ++x;
			}
		};

		class MiniINI
		{
		protected: // defenition of the value
			struct KeyPair
			{ 
				string key, value;
			};

		public: // the list keypair
			typedef std::list<KeyPair>	List;
			typedef List::iterator		iterator;

		public:
			List m_list;

		public: // loading routine
			string operator[](const char* Value)
			{
				for (iterator ptr = m_list.begin(); ptr != m_list.end(); ptr++)
					if (_stricmp(ptr->key.c_str(), Value) == 0)
						return ptr->value;
				return "";
			}

			void write(const char* Key, const char* Value)
			{
				for (iterator ptr = m_list.begin(); ptr != m_list.end(); ptr++)
					if (_stricmp(ptr->key.c_str(), Key) == 0) {
						ptr->value = Value;
						return;
					}
				KeyPair kp = { Key, Value };
				m_list.push_back(kp);
			}

			string read(const char* Key, const char* Default = "")
			{
				for (iterator ptr = m_list.begin(); ptr != m_list.end(); ptr++)
					if (_stricmp(ptr->key.c_str(), Key) == 0)
						return ptr->value;
				return Default;
			}

			errno_t load(const char* FilePath)
			{
				FILE* fptr = NULL;
				
				// attempt to open the ini list
				errno_t err = fopen_s(&fptr, FilePath, "r");
				if (err) 
					return err;

				// open each item
				char buffer[1024] = { 0 };
				while (fgets(buffer, 1024, fptr) != NULL)
				{
					// trimming
					size_t len = strlen(buffer);
					if (buffer[len-1] != '\r')
						buffer[len-1] = '\0';

					// search for return
					char* eq = strstr(buffer, "=");
					if (eq == 0)
						continue;

					// the strings
					std::string line_key = std::string(buffer, (int)(eq - buffer));
					std::string line_val = std::string(eq + 1);

					// insert tag
					KeyPair kp = { line_key, line_val };
					m_list.push_back(kp);
				}

				// close, return nada
				fclose(fptr);
				return 0;
			}

			errno_t save(const char* FilePath)
			{
				// file for writing
				FILE* fptr = NULL;
				errno_t err = fopen_s(&fptr, FilePath, "w");
				if (err) 
					return err;

				// each section
				for (iterator ptr = m_list.begin(); ptr != m_list.end(); ptr++)
					fprintf_s(fptr, "%s=%s\n", ptr->key.c_str(), ptr->value.c_str());

				// close
				fclose(fptr);
				return 0;
			}
		};
	}
}