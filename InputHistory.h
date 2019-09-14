#pragma once
#include <list>
#include <vector>

class ConsoleInputHistory
{
public: // the history
	std::vector<CStringA>	data;

public: // add and remove
	void	add(CStringA T)			{ data.push_back(T);	}
	size_t	size()					{ return data.size();	}
	CString	operator[](size_t I)	{ return data[I];		}
};