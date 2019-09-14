#pragma once
#include <string>

class Crypto
{
public: // static conversions
	static String md5(std::string Input);
	static int crc32(std::string Input);
};