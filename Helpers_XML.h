#pragma once
#include <map>

namespace Helpers
{
	namespace XML
	{
		// a single line parser feed is the data from a single line
		class DataKeyset
		{
		public: // type defenition expansions
			typedef std::map<String, String>		ListType;
			typedef ListType::iterator			iterator;
			typedef ListType::reverse_iterator	reverse_iterator;
			typedef ListType::reference			reference;
			typedef ListType::size_type			size_type;

		public: // var
			ListType list;

		public: // read operations
			iterator begin()			{ return list.begin();		}
			iterator end()				{ return list.end();		}
			reverse_iterator rbegin()	{ return list.rbegin();		}
			reverse_iterator rend()		{ return list.rend();		}
			size_type size()			{ return list.size();		}

		public: // type
			CStringA				type;

		public:
			CStringA&	operator[](LPCSTR Index) { return list[Index]; }
			bool		KeyExists(LPCSTR Index);
			void		InsertKey(LPCSTR Index, LPCSTR Data);
			void		InsertKey(LPCSTR Index, int Data);

		public:
			CStringA	ToSingleXML(LPCSTR _Type);
		};

		// parser function
		bool		ParseSingleLine(const char* _Input, DataKeyset& _Info);
		bool		valid_key(char by);
		CStringA	HtmlDecode(CStringA _Input);
		CStringA	HtmlEncode(CStringA _Input);
	};
}