#pragma once
#include <vector>

namespace ADODB
{
	class SQLBuilder
	{
	protected:
		struct KeyPair { CStringA name, value; };
		std::vector<KeyPair> m_pairs;
	
	public: // insert mechanisms
		void Insert(const char* KeyId, const char* Data);
		void Insert(const char* KeyId, int Data);
		void Insert(const char* KeyId, float Data);

	public: // builders
		CStringA CreateInsert(CStringA Table);
		CStringA CreateUpdate(CStringA Table, CStringA Where);
	};
}