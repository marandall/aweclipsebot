#pragma once
#include <vector>

class MiniParser
{
public:
	struct Argument { CStringA key, text; };
	std::vector<Argument> args;
protected:
	void insert_arg(CStringA flag_id, CStringA text)
	{
		for (size_t i = 0; i < args.size(); i++) 
		{
			if (args[i].key == flag_id)
			{
				args[i].text = text;
				return;
			}
		}

		// add to back
		text.Trim();
		Argument arg = { flag_id, text };
		args.push_back(arg);
	}

public:
	int ToTokens(CStringA Text)
	{
		char* p = Text.GetBuffer();
		CStringA flag_id, data;

		// outer loop
		while (*p != '\0')
		{
			if (*p != ' ')
			{
				char* p_begin = p;

				// we have a flag, onwards!
				while (*p != '\0' && *p != '=')
					p++;

				// set the flag id
				flag_id = CStringA(p_begin).Left(static_cast<int>(p - p_begin));
				insert_arg( flag_id, "" );
				if (*p == '\0')
					break;	
				p++;

				if (*p == '\'')
				{
					char* p_begin = ++p;
					while (*p != '\0' && *p != '\'')
						p++;
					
					// check what it is
					data = CStringA(p_begin).Left( static_cast<int>(p - p_begin) );
					insert_arg( flag_id, data );
					if (*p != 0) p++;
				}

				// ignore spaces, treat as words
				else
				{
					char* p_begin = p;
					while (*p != '\0' && *p != ' ')
						p++;
					
					// check what it is
					data = CStringA(p_begin).Left(static_cast<int>(p - p_begin));
					insert_arg( flag_id, data );
				}
			}
			// nothing, nothing at all
			else
			{
				p++;
			}
		}

		for (size_t i =0; i < args.size(); i++)
			printf("%s=%s\n", (const char*)args[i].key, (const char*)args[i].text);

		Text.ReleaseBuffer();
		return 0;
	}

	MiniParser(CStringA Text) { ToTokens(Text); }
	MiniParser() {  }

	bool Exists(CStringA Flag_Id) const
	{
		for (size_t i = 0; i < args.size(); i++)
			if (args[i].key == Flag_Id)
				return true;
		return false;
	}

	bool getExists(CStringA Flag_ID) const
	{
		return Exists(Flag_ID);
	}

	CStringA operator[](const char* Key)
	{
		for (size_t i = 0; i < args.size(); i++)
			if (args[i].key == Key)
				return args[i].text;
		return "";
	}

	CStringA getText(const char* Key, const char* Default = "") const
	{
		for (size_t i = 0; i < args.size(); i++)
			if (args[i].key == Key)
				return args[i].text;
		return Default;
	}
};