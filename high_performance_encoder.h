#pragma once

class StringEncoder
{
public: // slashing
	static void encode_slashes(char* Dest, const char* Copy, bool WrapQuotes = false)
	{
		char* p = (char*)Copy;
		char* s = Dest;
		
		// wrap with quotes
		if (WrapQuotes) *(s++) = '"';

		// for each
		while (*p != '\0')
		{
			if (*p == '\t')			{ *(s++) = '\\'; *(s++) = 't';  }
			else if (*p == '\n')	{ *(s++) = '\\'; *(s++) = 'n';  }
			else if (*p == '\r')	{ *(s++) = '\\'; *(s++) = 'r';  }
			else if (*p == '\\')	{ *(s++) = '\\'; *(s++) = '\\'; }
			else if (*p == '"')		{ *(s++) = '\\'; *(s++) = '"';  }
			else					{ *(s++) = *(p);			    }
			p++;
		}
		
		// wrap with quotes
		if (WrapQuotes) *(s++) = '"';

		// copy
		*s = 0;
	}

	static void decode_slashes(char* Dest, const char* Copy)
	{
		char* p = (char*)Copy;
		char* s = Dest;
		bool  escaped = false;

		// itr each
		while (*p != '\0')
		{
			if (escaped)
			{
				if (*p == '\\')		*(s++) = '\\';
				else if (*p == 'n')	*(s++) = '\n';
				else if (*p == 'r')	*(s++) = '\r';
				else if (*p == '"')	*(s++) = '"';
				else if (*p == '0')	*(s++) = '\0';
				else if (*p == 't')	*(s++) = '\t';
				else				*(s++) = *p;
				escaped = false;
				p++;
			}
			else if (*p == '\\')
			{
				escaped = true;
				p++;
			}
			else
			{
				*(s++) = *(p++);
			}
		}

		// end
		*s = '\0';
	}
};

class SpaceSeperator
{
public:
	static const int Size = 50;
	static const int Buffer = 1024;

private: // the list of resulting tokens
	char	list[Size][Buffer];

public: // operators
	char*	operator[](int Index) { return list[Index]; }

public: // encoder
	void reset()
	{
		for (int i = 0; i < Size; i++)
			strcpy_s(list[i], Buffer, "");
	}

	void build(char* Dest, int ElementCount)
	{
		char* s = Dest;

		// go through each element
		for (size_t i = 0; i < (size_t)ElementCount; i++) {
			// copy each byte
			char* p = list[i];
			while (*p != '\0') *(s++) = *(p++);
			*(s++) = ' ';
		}

		// en
		*s = '\0';
	}

public: // token generating list
	int parse(const char* Input)
	{
		char* start = const_cast<char*>(Input);
		char* p		= start;
		int   index = 0;
		char* last  = p;

		string s(Input);

		// tokenise
		while (true)
		{
			if (index > 20 || index < 0)
			{
				int k = 0;
			}

			// loop until next symbol
			printf("%c\n", *p);
			while (*p == ' ')
				p++;

			start = p;

			if (*p == 0)
				break;

			// find the next space
			if (*p == '"')
			{
				if (*(p + 1) == '"')
				{
					strcpy(list[index++], "");
					p++;
					p++;
				}
				else
				{
					if (s.substr(0, 5).compare("20164") == 0)
					{
						int j = 0;
					}

					bool escaped = false;
					p++;
					char* start2 = start;
					while (true)
					{
						if (*p == '"' && escaped == false)
						{
							// reached the end
							char token[Buffer] = { 0 };
							char* z = token;
						
							// copy into the buffer
							while (start != p) *(z++) = *(start++);
							*z = '\0';

							// decode
							StringEncoder::decode_slashes(list[index++], token + 1);
							p++;
							break;
						}
						else if (*p == '"')
						{
							int v = 0;
						}
						else if (*p == '\\')
						{
							escaped = !escaped;
						}
						else if (*p == 0)
						{
							int v = 0;
						}
						else
						{
							escaped = false;
						}
						p++;
					}
				}

				// copy
				int j = 0;
			}
			else
			{
				// go through each element
				while (*p != ' ' && *p != '\0') {
					p++;
				}

				// reached the end
				char token[Buffer] = { 0 };
				char* z = token;
			
				// copy into the buffer
				while (start != p) *(z++) = *(start++);
				*z = '\0';

				// decode
				strcpy_s(list[index++], Buffer, token);
				p++;

				// end or clear
				int sl = (int)strlen(p);
				if (sl == 0 || *p == 10)
					return index;
				// else
				//	p++;

				int j = 0;
			}


			// end
			last = p;
			if (*p == '\0')
				break;
		}
	
		return index;
	}
};