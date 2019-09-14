#pragma once
#include <vector>

namespace Helpers
{
	namespace Parsing {
		CStringA ExtractBrackets(CStringA _Identify) ;
	}

	namespace Text
	{
		// config stream
		// ---------------------------------------------------------------------------------
		// a class containing a boolean and a string
		class ConfString {
		private:
			CStringA	m_str;
			bool		m_enabled;
		public:
			operator bool()   const { return m_enabled; }
			operator LPCSTR() const { return m_str;     }
			bool Enabled() const	{ return m_enabled; }
		public:
			ConfString& operator=(LPCSTR _InStr) { m_str = _InStr;     return *this; }
			ConfString& operator=(bool   _InBoo) { m_enabled = _InBoo; return *this; }
		public:
			void set(bool _Enabled, LPCSTR _Input) { m_str = _Input; m_enabled = _Enabled; }
		};

		// config stream
		// ---------------------------------------------------------------------------------
		// a class containing a boolean and a string
		class UKMgr {
		private:
			class UDC {
			public:
				CStringA	key, value;
			public:
				UDC(LPCSTR Key, LPCSTR Value) {
					key = Key; value = Value;
				}

				UDC(LPCSTR Key, int Value) {
					key = Key;
					value.Format("%d", Value);
				}
			};

		private:
			vector<UDC>	list;

		public:
			void Insert(LPCSTR Key, LPCSTR Value)	{ UDC tk(Key, Value); list.push_back(tk); } 
			void Insert(LPCSTR Key, int Value)		{ UDC tk(Key, Value); list.push_back(tk); }

		public:
			CStringA	operator[](LPCSTR Key)		{ for (size_t i = 0; i < list.size(); i++)
														if (list[i].key == Key)
															return list[i].value;
														return "";
													}
			void Erase() { list.clear(); }
		};

		CStringA FormatUKS(LPCSTR InputString, UKMgr& List);
		CStringA CreateGUID();
	};
}