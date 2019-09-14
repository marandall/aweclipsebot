#pragma once
#include "AWSDK_Support.h"

class InputFlags
{
public:
	String final;
private:
	vector<String>		m_ptr;

public:
	/* we have public constructors and such */
	InputFlags() 
	{
	}

	InputFlags(LPCSTR pSource) 
	{
		search(pSource);
	}

	~InputFlags() 
	{
	};

	/* we split the information up using the splitter class */
	void search(LPCSTR p)
	{
		/* the variables */
		AW::SplitString split;
		size_t count	= split.split(p, " ", -1);
		int length		= 0;
		size_t i		= 0;

		/* the code execution */
		
		for (i = 0; i < count; i++){
			if (*(split[i]) == '-') {
				push(split[i]);
				length+= (int)strlen(split[i]) + 1;
			} else {
				break;
			}
		}

		size_t pre = i;
		for (i = i; i < count; i++)
		{
			/* first time we miss the instruction*/
			if (i != pre)
				final+= " ";
			/* but we always copy the string */
			final+= split[i];
		}

	}

	void push(LPCSTR pData)
	{
		/* push onto pointer */
		String data = pData;
		m_ptr.push_back(data);
	}

	bool key(LPCSTR Key) const
	{
		for (unsigned int i = 0; i < m_ptr.size(); i++)
		{
			if (m_ptr[i] == Key)
				return true;
		}
		return false;
	}

	bool operator[](LPCSTR input_key) const
	{
		return key(input_key);
	}

};