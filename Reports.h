#pragma once
#include <list>
#include <vector>
#include "table.h"

namespace Reports
{
	class Table;

	class Report
	{
	public: // the report lines
		typedef std::list<String> LineList;
		LineList output;

	public: // override
		virtual String generateReport() = 0;
		virtual ~Report() { }

	public: // run import
		void add(String S);
		void add(TableWriter& Tw);

	public: // convert to string
		String toString()
		{
			String outstr;
			for (LineList::iterator itr = output.begin(); itr != output.end(); itr++)
			{
				outstr.AppendFormat("%s\r\n", *itr);
			}

			// out string is determined
			return outstr;
		}
	};

	class IpCollisions : public Report
	{
	public: // generate a report
		String generateReport();
	};


	class AvatarList : public Report
	{
	public: // generate a report
		String generateReport();
	};
}