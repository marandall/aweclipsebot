#pragma once

class StringToolkit
{
public:
	// character sets
	static const char* AlphaChars;		//= "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVQXYZ";
	static const char* AlphaNumChars;	//= "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVQXYZ1234567890";
	static const char* NumChars;		//= "1234567890";
	static const char* WhiteChars;		//

	static bool OnlyContainsCharacters(const char* Text, const char* CharRange)
	{
		size_t text_len = strlen(Text);
		size_t range_len = strlen(CharRange);

		// for each letter
		for (size_t i = 0; i < text_len; i++)
		{
			// check it exists in byte set
			bool is_ok = false;
			for (size_t j = 0; j < range_len; j++)
				if (Text[i] == CharRange[j])
				{
					is_ok = true;
					break;
				}
			
			if (is_ok == true)
			{
				continue;
			}

			// its a string set
			return false;
		}

		// ok
		return true;
	}

	static bool ContainsCharacters(const char* Text, const char* CharRange)
	{
		size_t text_len = strlen(Text);
		size_t range_len = strlen(CharRange);

		// for each letter
		for (size_t i = 0; i < text_len; i++)
		{
			// check it exists in byte set
			for (size_t j = 0; j < range_len; j++)
				if (Text[i] == CharRange[j])
					return true;
		}

		// characters not found in set
		return false;
	}

	static bool LengthInRange(const char* Text, int MinLen, int MaxLen)
	{
		int len = (int)strlen(Text);
		return len >= MinLen && len <= MaxLen;
	}

	static String EscapeBinaryString(BYTE* Buffer, int Length)
	{

	}
};