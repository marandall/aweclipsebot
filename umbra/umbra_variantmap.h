#pragma once
#include "umbra_variant.h"
#include "umbra_typedef.h"
#include <map>

namespace Umbra
{
	class VariantMap
	{
	public: // required types for this map
		typedef std::map<string, Variant>			MapType;
		typedef MapType::iterator					iterator;
		typedef MapType::reverse_iterator			reverse_iterator;
		typedef MapType::reference					reference;

	public: // variable employed
		MapType				m_map;

	public: // forward functions
		iterator			begin()				{ return m_map.begin();		}
		iterator			end()				{ return m_map.end();		}
		reverse_iterator	rbegin()			{ return m_map.rbegin();	}
		reverse_iterator	rend()				{ return m_map.rend();		}
		size_t				size() const		{ return m_map.size();		}

	public: // attribute checking
		iterator id_exists(string id)
		{
			return m_map.find(id);
		}

		Variant* search(string id)
		{
			iterator itr = id_exists(id);
			if (itr == end())
			{
				return 0;
			}
			else
			{
				return &itr->second;;
			}
		}

		Variant& operator[](string id)
		{
			return m_map[id];
		}

	public: // overloaded constructors
		VariantMap() 
		{
		}

		VariantMap(const char* K1, Variant V1) 
		{
			m_map[K1] = V1;
		}

		VariantMap(const char* K1, Variant V1, const char* K2, Variant V2) 
		{
			m_map[K1] = V1;
			m_map[K2] = V2;
		}

		VariantMap(const char* K1, Variant V1, const char* K2, Variant V2, const char* K3, Variant V3) 
		{
			m_map[K1] = V1;
			m_map[K2] = V2;
			m_map[K3] = V3;
		}

	public: // parsing routine
		Variant     queryVariant(const char* Param);
		std::string parse_string(const char* Input);
	};

	//
	//
	// Variant Array - an association of variant data
	//
	//

	class VariantArray
	{
	public: // variant data types
		std::vector<Variant> items;

	public: // constructor copy
		VariantArray()
		{
		}

		VariantArray(Variant v1)
		{
			items.push_back(v1);
		}

		VariantArray(Variant v1, Variant v2)
		{
			items.push_back(v1);
			items.push_back(v2);
		}

		VariantArray(Variant v1, Variant v2, Variant v3)
		{
			items.push_back(v1);
			items.push_back(v2);
			items.push_back(v3);
		}

		VariantArray(Variant v1, Variant v2, Variant v3, Variant v4)
		{
			items.push_back(v1);
			items.push_back(v2);
			items.push_back(v3);
		}

	public: // add item
		void push_back(Variant V)
		{
			items.push_back(V);
		}

	public: // access operators
		size_t size() const { return items.size(); }
		Variant& operator[](size_t I) { return items[I]; }
	};
}