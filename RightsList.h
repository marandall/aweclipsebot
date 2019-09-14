#pragma once
#include <string>
#include <vector>

class RightsList
{
public:
	class Entry
	{
	public:
		int	 from;			// lowest citizen number
		int	 to;			// highest citizen number
		bool deny;			// deny this group

	public: // constructor
		Entry(int User, int User2, bool Deny)
		{
			from = User;
			to   = User2;
			deny = Deny;
		}

	public: // checking
		bool isSingleUser(int User)
		{
			return (from == to && from == User);
		}

	public: // strings
		std::string conv(int T)
		{
			char t[16];
			sprintf_s(t, sizeof t, "%d", T);
			return t;
		}

		std::string text()
		{
			std::string temp	= deny ? "-" : "";
			std::string tidle	= "~";
			if (to != from) temp.append(conv(from)).append(tidle).append(conv(to));
			else temp.append(conv(from));
			return temp;
		}
	};

public: // list of rights
	std::vector<Entry> rights;
	bool			   everyone;

protected: // insert symbolic
	void insert(std::string A)
	{
		// check it has got an item
		if (strlen(A.c_str()) == 0)
			return;

		Entry e(0, 0, false);

		// group
		std::string::size_type pos = A.find("~");
		if (pos != std::string::npos)
		{
			std::string p1 = A.substr(0, pos);
			std::string p2 = A.substr(pos + 1);
		
			// convert sections
			e.deny = (p1[0] == '-');
			e.from = atoi(p1.c_str());
			e.to   = atoi(p2.c_str());
		}
		else
		{
			std::string p1 = A;

			// convert sections
			e.deny = (p1[0] == '-');
			e.from = abs(atoi(p1.c_str()));
			e.to   = e.from;
		}

		// add item
		e.from = abs(e.from);
		e.to   = abs(e.to);
		rights.push_back(e);
	}

public: // operations parse
	void parse(const char* data)
	{
		char* start = const_cast<char*>(data);
		char* p     = start;

		// check the start
		everyone = (*data == '*');
		if (everyone) start = ++p;

		// split the string
		while (*p != '\0')
		{
			// split by this token
			start = p;
			while (*p != ',' && *p != ' ' && *p != 0)
				p++;

			// copy
			std::string em(start, p - start);
			insert(em);

			// equals
			if (*p == '\0')
				break;
			p++;
		}
	}

public: // valid
	bool valid()
	{
		return true;
	}

public: // the checking
	bool singleExists(int N)
	{
		for (size_t i = 0; i < rights.size(); i++)
			if (rights[i].isSingleUser(N) && rights[i].deny == false)
				return true;
		return false;
	}


public: // granting permissions
	bool add(int N, int C, int Pos = -1)
	{
		// if equal
		if (N == C && singleExists(N))
			return false;

		// if we insert it at the end
		if (Pos == -1)
		{
			Entry e(N, C, false);
			rights.push_back(e);
		}
		else
		{
			Entry e(N, C, false);
			rights.insert(rights.begin() + Pos, e);
		}

		// is this list valid
		return valid();
	}

public: // unblock
	bool block(int N, int C, int Pos = -1)
	{
		// if we insert it at the end
		if (Pos == -1)
		{
			Entry e(N, C, true);
			rights.push_back(e);
		}
		else
		{
			Entry e(N, C, true);
			rights.insert(rights.begin() + Pos, e);
		}

		// is this list valid
		return valid();
	}

	bool unblock(int N)
	{
		// has it been changed
		bool changed = false;
		for (size_t i = 0; i < rights.size(); i++)
		{
			if (rights[i].isSingleUser(N) == true && rights[i].deny == true)
			{
				rights.erase(rights.begin() + i);
				changed = true;
			}
		}

		// return if the item has changed
		return changed;
	}

public: // denying groups
	bool remove(int N)
	{
		bool changed = false;
		for (size_t i = 0; i < rights.size(); i++)
		{
			if (rights[i].isSingleUser(N) == true && rights[i].deny == false)
			{
				rights.erase(rights.begin() + i);
				i--;
				changed = true;
			}
		}

		// edits
		return changed;
	}

public: // get the list of rights
	std::string list()
	{
		std::string text = everyone ? "*" : "";
		if (rights.size() > 0)
		{
			// check for text
			text.append( everyone ? "," : "" ).append( rights[0].text() );
			
			for (int i = 1; i < rights.size(); i++)
				text.append( "," ).append( rights[i].text() );

			text.append( "," ) . append ( rights[rights.size() - 1].text() );
		}
		return text;
	}
};