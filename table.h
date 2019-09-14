#pragma once
#include <list>
#include <vector>

namespace Reports
{
	class Report;
}

class TableWriter
{
public: // row information
	class Row
	{
	public: // access elements
		typedef std::map<int, String> CellMap;
		CellMap elements;

	public: // operator
		String& operator[](int Index)
		{
			return elements[Index];
		}
	};

public: // row elements
	typedef std::list<Row> RowList;
	RowList rows;

public: // header information
	Row header;

public: // add a row element
	void add(Row R)
	{
		rows.push_back(R);
	}

protected: // generate padding
	String pad(int Count, const char* Symbol = " ")
	{
		String padding;
		for (int i = 0; i < Count; i++)
			padding+= Symbol;
		return padding;
	}

public: // writing functions
	void write(Reports::Report& Rep);
};

class ITableWriter
{
public: // the writer
	virtual TableWriter getReportTable() = 0;
};