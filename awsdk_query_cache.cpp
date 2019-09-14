#include "stdafx.h"
#include "awsdk_query.h"
#include "awsdk_objects_v3.h"

namespace AW
{
	void Query_Cache::Reset()
	{
		m_list.clear();
	}

	void Query_Cache::InsertObject(ObjectData& Object)
	{
		m_list.push_back(Object);
	}
	
	void Query_Cache::RemoveObject(int ObjectId)
	{
		for (Iterator pObj = m_list.begin(); pObj != m_list.end(); pObj++)
		{
			if (pObj->id != ObjectId)
				continue;

			// eliminate the object and delete
			m_list.erase(pObj);
			return;
		}
	}

	bool Query_Cache::Read_Propdump(String FilePath)
	{
		// open the file path
		FILE* fptr = NULL;
		if (fopen_s(&fptr, FilePath, "r"))
			return false;

		// read in line by line
		char	huge_buffer[1024 * 2];
		String	temp;
		while (fgets(huge_buffer, 1024 * 2, fptr))
		{
			temp = huge_buffer;
			temp.Trim();

			// extract object data and parse
			ObjectData obj;
			if (obj.PropConvert_ReadVersion4(temp))
			{
				InsertObject(obj);
			}
			else
			{
				int k = 0;
			}
		}

		// all good
		fclose(fptr);
		return true;
	}

	bool Query_Cache::Write_Propdump(String FilePath)
	{
		// open the file path
		FILE* fptr = NULL;
		if (fopen_s(&fptr, FilePath, "w"))
			return false;

		// write header, propdump version 4
		fputs("propdump version 4\n", fptr);

		// write each object
		for (Iterator pObj = m_list.begin(); pObj != m_list.end(); pObj++)
			fputs(pObj->PropConvert_WriteVersion4() + "\n", fptr);

		// close dump
		fclose(fptr);
		return true;
	}

}