#pragma once
#include <vector>

class CommandParser
{
public:
	struct Argument { CStringA key, text; };

public:
	std::vector<Argument> args;
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
			if (*p == '-')
			{		
				p++;
				char* p_begin = p;

				// we have a flag, onwards!
				while (*p != '\0' && *p != ' ' && *p != '=')
					p++;

				// set the flag id
				flag_id = CStringA(p_begin).Left(static_cast<int>(p - p_begin));
				insert_arg( flag_id, "" );
				if (*p == '=') p++;
			}

			// single quote encapsulated string
			else if (*p == '\'')
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
			else if (*p != ' ')
			{
				char* p_begin = p;
				while (*p != '\0' && *p != '-')
					p++;
				
				// check what it is
				data = CStringA(p_begin).Left(static_cast<int>(p - p_begin - (*p == 0 ? 0 : 1) ));
				insert_arg( flag_id, data );
			}

			// nothing, nothing at all
			else
			{
				p++;
			}
		}

		Text.ReleaseBuffer();
		return 0;
	}

	CommandParser(CStringA Text) { ToTokens(Text); }
	CommandParser() {  }

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

	size_t	size() const	{ return args.size();	}
	Argument index(size_t I) const { return args[I];		}
};