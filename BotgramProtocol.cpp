#include "stdafx.h"
#include "botgramprotocol.h"
#include "fileloader.h"

namespace BotgramProtocol
{
	void Packet::decode(String Code)
	{
		SplitString parts(Code, "\n\n", 2);

		// copy each of the headers
		SplitString headertext(parts[0], "\n", -1);
		for (size_t i = 0; i < headertext.size(); i++) 
		{
			// create a new header
			SplitString line(headertext[i], ": ", 2);
			Header h = { line[0], line[1] };
			headers.push_back(h);
		}

		// copy the binary information
		data = parts[1];
	}

	String Packet::getHeader(const char* Key, const char* Default)
	{
		for (size_t i = 0; i < headers.size(); i++)
			if (headers[i].key.CompareNoCase(Key) == 0)
				return headers[i].value;
		return Default;
	}

	ReceiveThread* ReceiverKernel::search(String Id, int Citizen)
	{
		for (iterator ptr = threads.begin(); ptr != threads.end(); ptr++)
			if (ptr->id == Id && ptr->source_citno == Citizen)
				return &(*ptr);
		return 0;
	}

	void ReceiverKernel::onReceive(String Name, int Citizen, String Data)
	{
		// create a packet
		Packet p;
		p.decode(Data);

		ReceiveThread* pThread = search(p.getHeader("id"), Citizen);
		if (pThread == 0)
		{
			ReceiveThread t;
			int pages = abs(atoi( p.getHeader("pages") ));
			
			// we do not have a list of pages here
			if (pages == 0)
				return;

			// check the broadcast range
			String target = p.getHeader("target");
			bool is_target = false;


			// assign to new thread
			t.source_name		= Name;
			t.source_citno		= Citizen;
			t.id				= p.getHeader("id");
			t.length			= atoi(p.getHeader("length"));
			t.content_type		= p.getHeader("content-type");
			t.packets.resize(pages);

			// assign packet data
			t.packets[0] = p;
			t.packets_received = 1;
			t.packets_needed   = pages;

			// debug information
			String debug;
			debug.Format("Receiving '%s' botgram from %s (%d) - %0.1fK expected.", t.content_type, Name, Citizen, t.length / 1024.0);
			WriteMsg(debug, 0x00, OT_TECH | OM_TIME);

			// push it back
			threads.push_back(t);
			pThread = &threads.back();
		}
		else
		{
			int page = abs(atoi( p.getHeader("page") ));
			pThread->packets[page - 1] = p;
			pThread->packets_received++;
		}

		// do they equal the same
		if (pThread && pThread->packets_needed == pThread->packets_received)
		{
			// build the string
			String body;
			for (size_t i = 0; i < pThread->packets.size(); i++)
			{
				body.Append(pThread->packets[i].data);
			}

			// write to the disk
			String path = "c:\\data.txt";
			FileWriter::WriteBinaryFile(path, body);

			// jump through and erase
			for (iterator ptr = threads.begin(); ptr != threads.end(); ptr++)
			{
				if (&(*ptr) == pThread)
				{
					threads.erase(ptr);
					return;
				}
			}

			// 
		}
	}

	class Prepage
	{
	public: // variables
		String header;
		String body;

	public: // constructor
		Prepage(String H, String B)
		{
			header = H;
			body   = B;
		}
	};

	void SendingKernel::onTick()
	{
		for (iterator ptr = threads.begin(); ptr != threads.end(); )
		{
			bool breaker = false;
			bool completed = false;
			for (size_t i = ptr->packet_index; i < ptr->packets.size(); i++)
			{
				// send the packet information while possible
				int rc = bot.Botgram(ptr->target_citizen, ptr->packets[i], true);
				if (rc) {
					breaker = true;
					break;
				}
			}

			// check breaker
			if (breaker)
			{
				break;
			}
			else
			{
				ptr = threads.erase(ptr);
			}
		}
	}

	void SendingThread::open(String Text)
	{
		const int BOTGRAM_SIZE = 500;
		const int MD5_LENGTH   = 32;

		// length of page information
		const int RESERVED_FOR_PAGES = 15;
		const int RESERVED_FOR_PAGE  = 10;
		const int RESERVED_FOR_NEXT	 = MD5_LENGTH + 7;
		const int RESERVED_FOR_NL	 = 3;

		// header function variables
		String header;
		String body;
		int    start_index = 0;
		int	   page_no = 1;

		// prebuild pages
		std::vector<Prepage> inprog_pages;

		// create an ID string
		id.Format("%4x%4x%4x%4x%4x%4x%4x%4x%4x", rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand());
		id = id.Left(32);

		// create the initial header
		header = "TRANSFER botgram/1.0";
		header.AppendFormat("\nlength: %d", Text.GetLength());
		header.AppendFormat("\nid: %s", id);
		header.AppendFormat("\ncontent-type: %s", content_type);

		// calculate the length of the data
		int major_header_length = header.GetLength() + RESERVED_FOR_PAGES + RESERVED_FOR_PAGE + RESERVED_FOR_NEXT;
		body = Text.Mid(start_index, BOTGRAM_SIZE - major_header_length);
		inprog_pages.push_back( Prepage(header, body) );

		// assign the next index
		start_index = body.GetLength();

		// while we are not at the limit
		while (start_index < Text.GetLength())
		{
			header = "TRANSFER followup/1.0";
			header.AppendFormat("\nid: %s", id);
			header.AppendFormat("\npage: %d", ++page_no);

			// get the header length
			int header_length = header.GetLength() + RESERVED_FOR_NEXT + RESERVED_FOR_NL;
			body = Text.Mid(start_index, BOTGRAM_SIZE - header_length);
			
			// create an in progress page
			inprog_pages.push_back( Prepage(header, body) );

			// assign to the next index
			start_index+= body.GetLength();
		}

		// set the number of pages in the first
		inprog_pages[0].header.AppendFormat("\npages: %d", (int)inprog_pages.size());

		// go through each page
		//for (size_t i = 0; inprog_pages.size() - 1; i++)
		{
			int k = 0;
		}

		// build the number of pages
		for (size_t i = 0; i < inprog_pages.size(); i++)
		{
			body.Format("%s\n\n%s", inprog_pages[i].header, inprog_pages[i].body);
			packets.push_back(body);

			// write to the file
			String fp;
			fp.Format("pages_%d.txt", (int)i);
			FileWriter::WriteBinaryFile(fp, body);
		}
	}
}