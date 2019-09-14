#pragma once

template<size_t MaxLength, size_t MaxElements>
class HighPerformance_SpaceParser
{
protected: // the element performance comes from its maximum length
	char m_array[MaxElements][MaxLength];
	size_t total_count;

public: // inserts a string at a given position
	inline void insert_string(char* Box, const char* src, const char* end)
	{
		// TODO: Fix to make variable length and pointerable
		size_t len = (size_t)(end - src + 1) ;

		/* for through and copy each item */
		char* ptr = Box;
		size_t count = 0;
		while (count < MaxLength && count < len)
		{
			*(ptr + count) = *(src + count);
			count++;
		}

		/* null terminate */
		*(ptr + len) = '\0';
	}

public: // split the string
	size_t split(const char* Input, int MaxElements)
	{
		size_t box_count = 0;
		total_count		 = 0;
		char* p = const_cast<char*>(Input);
	
		// split at the spaces
		char* pStart = p;
		while (*p != '\0')
		{
			if (*p == ' ')
			{
				insert_string(&m_array[box_count][0], pStart, p - 1);
				box_count++;
				pStart = (p+1);
			}

			// check the size
			if (box_count == MaxElements - 1)
				break;

			p++;
		}

		// split up to the final point
		insert_string(&m_array[box_count][0], pStart, Input + strlen(Input));
		box_count++;

		total_count = box_count;
		return box_count;
	}

public: // return the box size
	const char* operator[](size_t Index) { return &m_array[Index][0]; }
	size_t size() const { return total_count; }
};