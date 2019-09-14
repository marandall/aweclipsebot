#include "stdafx.h"
#include "crypto.h"
#include "crypto_md5.h"
#include "crypto_crc32.h"

class CrcTool;

String Crypto::md5(std::string Input)
{
	unsigned char output_16[16];
	::md5((unsigned char*)Input.c_str(), (int)Input.length(), output_16);

	// convert to string
	String out;
	for (size_t i = 0; i < 16; i++)
		out.AppendFormat("%02x", (unsigned char)output_16[i]);
	return out;
}

int Crypto::crc32(std::string Input)
{
	static CrcTool crc;
	return crc.Get_CRC(Input);
}