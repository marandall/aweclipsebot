#include "stdafx.h"
#include "umbra_tools.h"
#include "winsock2.h"

namespace Umbra
{
	unsigned char byte_to_nibble(char C)
	{
		switch (C) {
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;

		// lower case characters
		case 'a': return 10;
		case 'b': return 11;
		case 'c': return 12;
		case 'd': return 13;
		case 'e': return 14;
		case 'f': return 15;
		
		// upper case characters
		case 'A': return 10;
		case 'B': return 11;
		case 'C': return 12;
		case 'D': return 13;
		case 'E': return 14;
		case 'F': return 15;

		// nothing found
		default: return -1;
		};
	}

	unsigned char hex_to_byte(const char* Ptr)
	{
		return (byte_to_nibble(*Ptr) << 4) | byte_to_nibble(*(Ptr + 1));
	}

	std::string Tools::urlencode(std::string Src)
	{
		// reserve the size for the given strike and presume nothing will be used
		std::string line;
		line.reserve(Src.size());

		// loop for every byte
		for (size_t i = 0; i < Src.size(); i++)
		{
			unsigned char byte = (unsigned char)Src[i];

			// encoding ranges
			bool r1 = byte >= 0x00 && byte <= 0x1f;
			bool r2 = byte >= 0x7f && byte <= 0xff;
			bool r3 = false;

			// hex codes
			unsigned char also[] = { 
				0x20, 0x22, 0x3c, 0x3e, 0x23, 0x25, 0x7b, 0x7d, 
				0x7c, 0x5c, 0x5e, 0x7e, 0x5b, 0x5d, 0x60, 0x00,
				0x24, 0x26, 0x2b, 0x2c, 0x2f, 0x3a, 0x3b, 0x3d, 0x3f, 0x40 };
			
			// loop for every hidden character and check of its existance
			for (size_t j = 0; j < (sizeof(also) / sizeof(also[0])); j++) {
				if (also[j] == byte) {
					r3 = true;
					break;
				} }

			// any character here has to be encoded using the double quote
			if (r1 || r2 || r3)
			{
				char align[16];
				sprintf_s(align, sizeof(align), "%%%02X", (unsigned int)byte);
				line.append(align);
			}
			else
			{
				// append the character
				line.push_back((char)byte);
			}
		}

		// return output
		return line;
	}

	std::string Tools::urldecode(std::string Src)
	{
		// reserve the size for the given strike and presume nothing will be used
		std::string line;
		line.reserve(Src.size());

		// loop for every byte
		char* p = (char*)Src.c_str();
		while (*p != 0)
		{
			// if there is a percentage opening
			if (*p == '%')
			{
				// convert to byte and push back
				unsigned char c = hex_to_byte(p + 1);
				line.push_back((char)c);

				// p continues to the next element
				p+= 3;
			}
			else
			{
				line.push_back(*p);
				p++;
			}
		}

		// return output
		return line;
	}

	std::string Tools::htmlspecialchars(std::string Src)
	{
		// reserve the size for the given strike and presume nothing will be used
		std::string line;
		line.reserve(Src.size());
		char align[8] = { 0 };

		// loop for every byte
		for (size_t i = 0; i < Src.size(); i++)
		{
			unsigned char byte = (unsigned char)Src[i];

			// encoding rangess
			if (byte == '&') strcpy_s(align, "&amp;");
			else if (byte == '"') strcpy_s(align, "&quot;");
			else if (byte == '\'') strcpy_s(align, "&#039;");
			else if (byte == '<') strcpy_s(align, "&lt;");
			else if (byte == '>') strcpy_s(align, "&gt;");
			else
			{
				line.push_back((char)byte);
				continue;
			}

			// use the handler
			line.append(align);
		}

		// return output
		return line;
	}

	std::string Tools::htmlspecialchars_decode(std::string Src)
	{
		// reserve a line
		std::string line;
		line.reserve(Src.length() + 1);

		// iterator settings
		char* p = (char*)Src.c_str();
		while (*p != '\0')
		{
			// ampersand encoding
			if (*p == '&')
			{
				// track th start and iterator over the characters
				char* pStart = p;
				while (*p != ';' && *p != '\0')
					p++;

				// is ending semicolon
				if (*p == ';')
				{
					std::string elem(pStart, p - pStart);
					if (elem.compare("&lt") == 0) line.append("<");
					else if (elem.compare("&gt") == 0) line.append(">");
					else if (elem.compare("&quot") == 0) line.append("\"");
					else if (elem.compare("&#039;") == 0) line.append("'");
					else line.append(elem);
					p++;
				}
			}
			else
			{
				line.push_back(*p);
				p++;
			}
		}

		// return output
		return line;
	}

	std::string Tools::addslashes(std::string Src)
	{
		// reserve the size for the given strike and presume nothing will be used
		std::string line;
		line.reserve(Src.size() + 10);
		char align[8] = { 0 };

		// loop for every byte
		for (size_t i = 0; i < Src.size(); i++)
		{
			unsigned char byte = (unsigned char)Src[i];

			// encoding rangess
			if (byte == '\'') line.append("\\'");
			else if (byte == '"') line.append("\\\"");
			else if (byte == '\\') line.append("\\\\");
			else if (byte == '0') line.append("\\0");
			else if (byte == '>') line.append("\\'");
			else line.push_back((char)byte);
		}

		// return output
		return line;
	}

	std::string Tools::bin2hex(std::string Src)
	{
		// reserve the size for the given strike and presume nothing will be used
		std::string line;
		line.reserve(Src.size() * 2 + 5);
		char align[8] = { 0 };

		// loop for every byte
		for (size_t i = 0; i < Src.size(); i++)
		{
			// encoding rangess
			sprintf_s(align, "%02x", (int)(unsigned char)Src[i]);
			line.append(align);
		}

		// return output
		return line;
	}

	std::string Tools::hex2bin(std::string Src)
	{
		std::string line;
		line.reserve(Src.size() / 2);

		// copy the structure backwards
		char byte_dual[3] = { 0, 0, 0 };
		char* pEnd;
		const char* pString = (LPCSTR)Src.c_str();
		for (size_t i = 0; i < Src.size(); i+=2)
		{
			byte_dual[0] = *(pString + i);
			byte_dual[1] = *(pString + i + 1);
			
			// allocate byte to range buffer
			line.push_back((char)(unsigned char)strtol(byte_dual, &pEnd, 16));
		}

		// return output
		return line;
	}

	std::string Tools::base64_encode(std::string Src)
	{
		static char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvqxyz0123456789+/=";
		static char padding[]  = "=";

		// reserve a new string of the initial length plus a third
		size_t len = Src.length();
		std::string line;
		line.reserve((size_t)(Src.length() * 1.4F));

		// clip the bits for the next 3 bytes
		for (size_t i = 0; i < Src.length(); i+= 3)
		{
			// single calculation sizes
			size_t p1 = i;
			size_t p2 = i + 1;
			size_t p3 = i + 3;

			// convert into long int pattern
			BYTE b3 = p1 < len ? Src[i + 0] : 0;
			BYTE b2 = p2 < len ? Src[i + 1] : 0;
			BYTE b1 = p3 < len ? Src[i + 2] : 0;
			unsigned int align = b1 | (b2 << 8) | (b3 << 16);

			// first byte
			int n1 = ((align >> 0)  & 0x3F);
			int n2 = ((align >> 6)  & 0x3F);
			int n3 = ((align >> 12) & 0x3F);
			int n4 = ((align >> 18) & 0x3F);

			// assign new bytes
			if (p3 < len)
			{
				char text[5] = { alphabet[n4], alphabet[n3], alphabet[n2], alphabet[n1], 0 };
				line.append(text);
			}
		}

		// the completed line
		return line;
	}

	std::string Tools::base64_decode(std::string Src)
	{
		return "";
	}

	std::string Tools::ucfirst(std::string Src)
	{
		// loop for every byte
		char byte = Src[0];
		if (__isascii(byte) && islower(byte))
			Src[0] = toupper(byte);

		// return output
		return Src;
	}

	std::string Tools::ucwords(std::string Src)
	{
		// loop for every byte
		bool new_word = true;

		// for every byte
		for (size_t i = 0; i < Src.size(); i++)
		{
			// copy byte
			char byte = Src[i];

			// for a new word switch to uppercase
			if (new_word) 
			{
				Src[i] = toupper(Src[i]);
				new_word = false;
			}

			// check for new word byte
			if (byte == ' ' || byte == '\t')
				new_word = true;
		}

		// return output
		return Src;
	}

	std::string Tools::str_repeat(std::string Src, int Freq)
	{
		// frequent time
		std::string line;
		line.reserve(Src.size() * Freq + 2);

		// copy the structure backwards
		for (int i = 0; i < Freq; i++)
			line.append(Src);

		// return output
		return line;
	}

	std::string Tools::str_pad(int Freq)
	{
		// frequent time
		std::string line;
		line.reserve(Freq + 2);

		// copy the structure backwards
		for (int i = 0; i < Freq; i++)
			line.append(" ");

		// return output
		return line;
	}

	__int64 Tools::parseint(std::string Src, int Radix)
	{
		// convert and check range
		char* pEnd = 0;
		return strtol(Src.c_str(), &pEnd, Radix);
	}
	
	double Tools::parsedouble(std::string Src)
	{
		// convert and check range
		char* pEnd = 0;
		return strtod(Src.c_str(), &pEnd);
	}

	WIN32_FIND_DATAA* Tools::get_find_file(std::string Path)
	{
		static WIN32_FIND_DATAA filedata;
		HANDLE hFind = FindFirstFile(Path.c_str(), &filedata);

		// valid file?
		if (hFind == INVALID_HANDLE_VALUE)
		{
			return 0;
		}
		else
		{
			FindClose(hFind);
			return &filedata;
		}
	}

	std::string Tools::file_get_contents(std::string Path, errno_t* Err)
	{
		// open the designated file for reading
		FILE* fptr = 0;
		errno_t err = fopen_s(&fptr, Path.c_str(), "rbS");

		// dummy error
		errno_t edum;
		if (Err == 0) Err = &edum;

		// read every element
		WIN32_FIND_DATAA* pFd = get_find_file(Path);
		if (pFd == 0)
		{
			*Err = (errno_t)GetLastError();
			return "";
		}
		else
		{
			// open a file handle for the reading
			HANDLE hFile = CreateFile(Path.c_str(),
				GENERIC_READ,								// only read information
				FILE_SHARE_READ,		// deny write and delete access
				NULL, OPEN_EXISTING, 0, NULL);

			// if we dont have a handle
			if (hFile == INVALID_HANDLE_VALUE)
			{
				*Err = (errno_t)GetLastError();
				return "";
			}
			else
			{
				// copy buffer
				std::string output;
				output.resize(pFd->nFileSizeLow);

				// read the number of bytes
				DWORD bytes_read = 0;
				BOOL read = ReadFile(hFile, (void*)&output[0], pFd->nFileSizeLow, &bytes_read, NULL);
				CloseHandle(hFile);

				// return resultant
				*Err = 0;
				std::string x(output);
				return x;
			}
		}
	}

	std::string Tools::long2ip(int ip)
	{
		char iptext[24];
		sprintf_s(iptext, sizeof(iptext), "%d.%d.%d.%d",
			ip & 0xFF, (BYTE)(ip >> 8),(BYTE)(ip>> 16),(BYTE)(ip>> 24));
		return iptext;
	}

	int Tools::ip2long(std::string Src)
	{
		return inet_addr(Src.c_str());
	}

	errno_t Tools::file_put_contents(std::string Path)
	{
		return 0;
	}
}