#pragma once
#include <string>

namespace Umbra
{
	class Tools
	{
	public: // static html manipulation functions
		static std::string urlencode(std::string Src);
		static std::string urldecode(std::string Src);
		static std::string htmlspecialchars(std::string Src);
		static std::string htmlspecialchars_decode(std::string Src);

	public: // string protection
		static std::string addslashes(std::string Src);

	public: // conversion between representations
		static std::string bin2hex(std::string Src);
		static std::string hex2bin(std::string Src);
		static std::string base64_encode(std::string Src);
		static std::string base64_decode(std::string Src);

	public: // case modifications
		static std::string ucfirst(std::string Src);
		static std::string ucwords(std::string Src);

	public: // string editing
		static std::string str_repeat(std::string Src, int Freq);
		static std::string str_pad(int Freq);

	public: // string conversions
		static std::string long2ip(int Addr);
		static int ip2long(std::string Src);

	public: // conversion of dates and times
		static __int64 parseint(std::string Src, int Radix);
		static double parsedouble(std::string Src);

	public: // file handling routines
		static std::string file_get_contents(std::string Path, errno_t* Err = 0);
		static errno_t file_put_contents(std::string Path);

	public: // find data
		static WIN32_FIND_DATAA* get_find_file(std::string Path); 
	};
}