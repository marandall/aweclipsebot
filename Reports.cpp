#include "stdafx.h"
#include "reports.h"
#include "zones.h"

namespace Reports
{
	void Report::add(TableWriter& Tw)
	{
		Tw.write(*this);
	}

	void Report::add(String S)
	{
		output.push_back(S);
	}


	String IpCollisions::generateReport()
	{
		Avatars& avlist = avatars;
		
		// structure set
		std::vector<int> matched_ips;

		// the table rows
		const int TD_SESSION	= 0;
		const int TD_NAME		= 1;
		const int TD_CITIZEN	= 2;
		const int TD_PRIVILEGE	= 3;
		const int TD_ADDRESS	= 4;
		const int TD_DNS		= 5;

		// header
		add("IP Collision Report");
		add("------------------------------------------");
		add("");

		// for each of the avatars in the list build up a collection of IP addresses
		for (size_t i = 0; i < avlist.size(); i++)
		{
			// avatar in question
			Avatar& av = avlist[i];

			// check if the avatar has already eben matched
			bool already_found = false;
			for (size_t j = 0; j < matched_ips.size(); j++)
			{
				if (matched_ips[j] == av.getAddressLng())
				{	
					already_found = true;
					break;
				}
			}

			// resultant check
			if (already_found == true)
			{
				continue;
			}

			// tracking data
			TableWriter table;
			int additional_found = 0;

			// configure table
			table.header[TD_SESSION]	= "Session";
			table.header[TD_NAME]		= "Name";
			table.header[TD_CITIZEN]	= "Citizen";
			table.header[TD_PRIVILEGE]	= "Privilege";
			table.header[TD_ADDRESS]	= "Address";
			table.header[TD_DNS]		= "DNS";
			
			// find each avatar matching this one
			for (size_t j = 0; j < avlist.size(); j++)
			{
				if (avlist[j].getAddressLng() == av.getAddressLng())
				{
					TableWriter::Row r;
					r[TD_SESSION]	= ToString(avlist[j].getSession());
					r[TD_NAME]		= avlist[j].getName();
					r[TD_CITIZEN]	= ToString(avlist[j].getCitizen());
					r[TD_PRIVILEGE]	= avlist[j].getPrivName();
					r[TD_ADDRESS]	= avlist[j].getAddress();
					r[TD_DNS]		= avlist[j].getDns();
				
					// append row
					table.add(r);
				}
			}

			// write this element
			add(table);

			// ip already known
			matched_ips.push_back(av.getAddressLng());
		}

		// our report
		return toString();
	}

	String AvatarList::generateReport()
	{
		// tracking data
		TableWriter table;
		int additional_found = 0;
		Avatars& avlist = avatars;


		// the table rows
		const int TD_SESSION	= 0;
		const int TD_NAME		= 1;
		const int TD_CITIZEN	= 2;
		const int TD_PRIVILEGE	= 3;
		const int TD_ADDRESS	= 4;
		const int TD_DNS		= 5;
		const int TD_POSITION	= 6;
		const int TD_ZONE		= 7;

		// configure table
		table.header[TD_SESSION]	= "Session";
		table.header[TD_NAME]		= "Name";
		table.header[TD_CITIZEN]	= "Citizen";
		table.header[TD_PRIVILEGE]	= "Privilege";
		table.header[TD_ADDRESS]	= "Address";
		table.header[TD_DNS]		= "DNS";
		table.header[TD_POSITION]	= "Position";
		table.header[TD_ZONE]		= "Zone";
		
		// find each avatar matching this one
		for (size_t j = 0; j < avatars.size(); j++)
		{
			// generate a new row
			TableWriter::Row r;
			r[TD_SESSION]	= ToString(avlist[j].getSession());
			r[TD_NAME]		= avlist[j].getName();
			r[TD_CITIZEN]	= ToString(avlist[j].getCitizen());
			r[TD_PRIVILEGE]	= avlist[j].getPrivName();
			r[TD_ADDRESS]	= avlist[j].getAddress();
			r[TD_DNS]		= avlist[j].getDns();

			// location
			r[TD_POSITION]	= avlist[j].getCoordinates();
			r[TD_ZONE]		= avlist[j].getZone()->getName();
			
			// append row
			table.add(r);
		}

		// write this element
		add(table);
		return toString();
	}
}

void TableWriter::write(Reports::Report& Rep)
{
	// item counter
	std::vector<int> widths;
	widths.resize(header.elements.size(), 0);
	int total_width = 1;

	// the header information
	rows.insert(rows.begin(), header);

	// extend for each element row as required, full iterator
	for (RowList::iterator row = rows.begin(); row != rows.end(); row++)
	{
		// iteration of inner loop
		for (Row::CellMap::iterator h = row->elements.begin(); h != row->elements.end(); h++)
		{
			if (h->second.GetLength() > widths[h->first])
			{
				widths[h->first] = h->second.GetLength();
			}
		}
	}

	// append all the widths
	for (size_t i = 0; i < widths.size(); i++)
	{
		total_width+= widths[i] + 3;
	}

	// write out header
	Rep.add(pad(total_width, "-"));

	// write each element row as required, full iterator
	int row_index = 0;
	for (RowList::iterator row = rows.begin(); row != rows.end(); row++)
	{
		// iteration of inner loop
		String line;
		for (Row::CellMap::iterator h = row->elements.begin(); h != row->elements.end(); h++)
		{
			line.AppendFormat( "| %s %s", h->second, pad(widths[h->first] - h->second.GetLength()) );
		}

		// append the final line
		line+= "|";
		Rep.add(line);

		// row item
		if (row_index++ == 0)
		{
			Rep.add(pad(total_width, "-"));
		}
	}

	// write out header
	Rep.add(pad(total_width, "-"));
}