#pragma once

namespace Property
{
	class PartialExtractor
	{
	private:
		String	buffer;
		int		cur_pos;

	public: 
		PartialExtractor(String Data) 
		{ 
			buffer	= Data; 
			cur_pos = 0; 
		}

		String getNext(int Length)
		{
			cur_pos+= Length;
			return buffer.Mid(cur_pos - Length, Length);
		}

		String getLast()
		{
			return buffer.Mid(cur_pos);
		}
	};

	inline __int64 hex_int(const char* Start)
	{
		static unsigned char jump_map[] = { 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13,
				14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		unsigned int result = 0;
		char* p = const_cast<char*>(Start);
		while (*p != '\0')
			result = ((result << 0x04) | jump_map[*p++]);
		return result;
	}

	// conversion
	inline __int64 signed_hex_to_int(const char* Value)
	{
		if (*Value == '-') return -(hex_int(Value+1));
		else return hex_int(Value);
	}

	inline void FileIO_CopyHexToBinary(std::vector<char>& Array, String Data)
	{
		int len = Data.GetLength();
		Array.resize(len / 2);

		if (len == 0)
			return;

		// copy the structure backwards
		char  byte_dual[3] = { 0, 0, 0 };
		char* pEnd;
		const char* pString = (LPCSTR)Data;

		for (int i = 0; i < len; i+=2)
		{
			int pos = (i / 2);
			byte_dual[0] = *(pString + i);
			byte_dual[1] = *(pString + i + 1);

			int bv =  strtol(byte_dual, &pEnd, 16);
			Array[pos] = bv;

			char x2[5];
			sprintf(x2, "%02x", (unsigned char)Array[pos]);
			if (strcmp(x2, byte_dual) != 0)
			{
				int l = 0;
			}
		}
	}

	inline String FileIO_ArrayToHex(std::vector<char>& Array)
	{
		String var;
		for (size_t i = 0; i < Array.size(); i++)
			var.AppendFormat("%02x", (unsigned char)Array[i]);
		return var;
	}

}