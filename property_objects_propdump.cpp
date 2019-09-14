#include "stdafx.h"

#include "property_objects.h"
#include "property_support.h"

namespace Property
{
	errno_t Objects::write_propdump(const char* Path, int Version)
	{
		// open the given file path for writing
		FILE* fptr = NULL;
		if (errno_t err = fopen_s(&fptr, Path, "w"))
			return err;

		// write the header style
		if (Version != 0)
			fprintf(fptr, "propdump version %d\n", Version);
		else
			Version = 4;	// default version

		// write each of the object elements
		for (iterator objPtr = begin(); objPtr != end(); objPtr++)
			if (objPtr->type == AW_OBJECT_TYPE_V3 || Version >= 4)			// no particles in v3
				fprintf(fptr, "%s\n", (const char*)objPtr->FileIO_getPropdump(Version));
	
		// close down the file
		fclose(fptr);
		return 0;
	}

	errno_t Objects::read_propdump(const char* Path)
	{
	// open the given file path for reading
		FILE* fptr = NULL;
		if (errno_t err = fopen_s(&fptr, Path, "r"))
			return err;

		// read settings
		int version = 4;
		int failed_objects = 0;
		ObjectData objData;

		// read file through loop
		char line_buffer[1024 * 2] = { 0 };
		String line;
		int   line_count = 0;
		while (fgets(line_buffer, sizeof line_buffer, fptr))
		{
			line_count++;
			line = line_buffer; line.Trim();

			// check for the first line
			if (line_count == 1)
			{
				String prefix_search = "propdump version ";
				if (line.Left(prefix_search.GetLength()) == prefix_search)
				{
					version = atoi(line.Mid(prefix_search.GetLength()));
					if (version != 2 && version != 3 && version != 3 && version != 4)
					{
						fclose(fptr);
						return -1;
					}
				}
			}
			else
			{
				bool success = false;
				if (version == 2)
					success = objData.FileIO_UsePropdump_v2(line);
				else if (version == 3)
					success = objData.FileIO_UsePropdump_v3(line);
				else if (version == 4)
					success = objData.FileIO_UsePropdump_v4(line);
				else
					failed_objects++;

				// add only if we were successful
				if (success) insert_object(objData);
			}
		}

		// close down the file
		fclose(fptr);
		return 0;
	}

	errno_t Objects::write_project_v1 (const char* Path)
	{
		// open the given file path for writing
		FILE* fptr = NULL;
		if (errno_t err = fopen_s(&fptr, Path, "w"))
			return err;

		// write the header style
		String header; 
		header.Format("@PROPERTY_PROJECT\n\n"
			          "# Property Dump Generated by Eclipse Evolution XE2\n"
					  "# Generated:       %s\n"
					  "# Timestamp:       %d\n"
					  "# Total Objects:   %d\n"
					  "# Boundary Top:    %s\n"
					  "# Boundary Bottom: %s\n\n"
					  "@PROPERTY_PROJECT_VERSION 1\n"
					  "@ENCODING BASE_10\n"
					  "@RADIX 16\n\n",
					  getTime(),
					  (int)time64(0),
					  (int)m_objects.size(),
					  cell_boundary.hi.getLocation().getCoordinates(),
					  cell_boundary.lo.getLocation().getCoordinates()
				);
		fputs(header, fptr);

		// write each of the object elements
		CStringA src_buffer;
		int count = 0;
		for (iterator objPtr = begin(); objPtr != end(); objPtr++) {
			src_buffer.AppendFormat("%s\n", objPtr->FileIO_getPropertyProjectLine_v1());
			
			// output as required
			if ((++count % 20) == 0) {
				fprintf(fptr, "%s", (LPCSTR)src_buffer);
				src_buffer = "";
			}
		}

		// anything left in the buffer
		if (src_buffer.GetLength())
			fprintf(fptr, "%s", (LPCSTR)src_buffer);

		// close down the file
		fclose(fptr);
		return 0;
	}

	errno_t	Objects::write_cache(const char* Path, int SectorX, int SectorZ)
	{
		// open the given file path for writing
		FILE* fptr = NULL;
		if (errno_t err = fopen_s(&fptr, Path, "w"))
			return err;

		// write the header style
		String header; 
		header.Format("@PROPERTY_PROJECT\n\n"
			          "# Property Dump Generated by Eclipse Evolution XE2\n"
					  "@PROPERTY_PROJECT_VERSION 1\n"
					  "@ENCODING SIGNED_HEX\n"
					  "@RADIX 16\n\n");
		fputs(header, fptr);

		// write each of the object elements
		/* for (iterator objPtr = begin(); objPtr != end(); objPtr++)
		{
			int sector_x = objPtr->getSectorX();
			int sector_z = objPtr->getSectorZ();

			if (sector_x == SectorX && sector_z == SectorZ)
				fprintf(fptr, "%s\n", objPtr->FileIO_getPropertyProjectLine_v1());
		}*/

		// close down the file
		fclose(fptr);
		return 0;
	}

	errno_t	Objects::read_project_v1(const char* Path)
	{
		// open the given file path for reading
		FILE* fptr = NULL;
		if (errno_t err = fopen_s(&fptr, Path, "r"))
			return err;

		// read settings
		int version = 1;
		int failed_objects = 0;

		//  turn off indexing
		bool track_cells	= this->track_cells;
		bool track_sectors	= this->track_sectors;
		this->track_cells	= false;
		this->track_sectors = false;

		// time tracing
		DWORD ticks = GetTickCount();

		// read file through loop
		char		line_buffer[1024 * 2] = { 0 };
		String		line;
		ObjectData	objData;

		while (fgets(line_buffer, sizeof line_buffer, fptr))
		{
			// copy line and trim end bytes
			line = line_buffer; line.Trim();

			// if first byte is @ then treat as command
			if (line[0] == '@')
			{
			}
			else if (line[0] == '#')
			{
			}
			else if (line.GetLength() == 0)
			{
			}
			else
			{
				objData.FileIO_UsePropertyProjectLine_v1(line);
				insert_object(objData);
			}
		}

		// restore settings
		this->track_cells	= track_cells;
		this->track_sectors	= track_sectors;
		this->rebuild_trackers();

		// time tracking
		String time_debug;
		time_debug.Format("e2pp Load '%s' @ %d ms", Path, GetTickCount() - ticks);
		WriteMsg(time_debug, 0, OT_DEBUG | OM_TIME);

		// close down the file
		fclose(fptr);
		return 0;
	}
}



