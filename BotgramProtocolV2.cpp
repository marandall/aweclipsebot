#include "stdafx.h"
#include "BotgramProtocolV2.h"
#include "crypto.h"
#include "scripting_host.h"
#include <Winerror.h>

namespace BotgramTransferProtocol
{
	bool ReceiverThread::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		// checkstate
		UMBRA_PROPERTY_R(id, id);
		UMBRA_PROPERTY_R(from, from);
		UMBRA_PROPERTY_R(from_name, from_name);
		UMBRA_PROPERTY_R(origin, origin);
		UMBRA_PROPERTY_R(content_type, content_type);
		UMBRA_PROPERTY_R(content_name, content_name);
		UMBRA_PROPERTY_R(size, size);
		UMBRA_PROPERTY_R(body, body);

		// not able to determine
		return R.NOT_HANDLED;
	}

	ReceiverThread* ReceiveProcessor::getThread(String ID)
	{
		for (iterator i = begin(); i != end(); i++)
			if (i->id == ID)
				return &(*i);
		return 0;
	}

	bool ReceiveProcessor::onReceive(int Citizen, String From, String Text)
	{
		// no message was able to be determined
		if (Text.Left(8) != "BGTP-1: ")
			return false;

		// explode against double new line
		SplitString bo(Text, "\n\n", 2);
		if (bo.size() != 2)
			return false;

		// copy segments
		String header = bo[0];
		String body   = bo[1];

		// explode the header into lines
		SplitString lines(header, "\n", -1);
		std::map<String, String> headers;

		// copy each segment
		for (size_t i = 0; i < lines.size(); i++)
		{
			// split by a colon space
			SplitString li(lines[i], ": ", 2);
			if (li.size() != 2)
				continue;

			// copy into headers
			headers[li[0]] = li[1]; 
		}

		// extract the various parts of the message
		String msg_uid= headers["BGTP-1"];
		String to			= headers["to"].Trim();
		String next		= headers["next"].Trim();;
		int index			= atoi(headers["part"]);
		int size			= atoi(headers["size"]);

		// must be addressed to us
		if (to.GetLength() != 0)
		{
			// $ - session identifier
			if (to.Left(1) == "$")
			{
				if (atoi(to.Mid(1)) != bot.getSession())
					return false;
			}

			// & - world name
			else if (to.Left(1) == "&")
			{
				if (to.Mid(1).CompareNoCase(bot.getWorld()) != 0)
					return false;
			}

			// [ - bot name
			else if (to.Left(1) == "[")
			{
				String bn;
				bn.Format("[%s]", bot.getBotName());
				if (bn.CompareNoCase(to.Mid(1)) != 0)
					return false;
			}
		}
		
		// form the message id
		String msg_id;
		msg_id.Format("%d-%s", Citizen, msg_uid);

		// lookup the thread
		ReceiverThread* pThread = getThread(msg_id);
		if (pThread == 0 && index == 1)
		{
			// create a new thread
			ReceiverThread rt;
			rt.from					= Citizen;
			rt.id						= msg_id;
			rt.from_name		= From;
			rt.body					= "";
			rt.next_msg			= 1;
			rt.next_hash		= "";
			rt.size					= size;
			rt.content_type	= headers["content-type"];
			rt.content_name	= headers["content-name"];

			// allocate thread
			threads.push_back(rt);
			pThread = &threads.back();

			// trace output
			String log;
			log.Format("Receiving '%s' named '%s' via BGTP from %s", rt.content_type, rt.content_name, rt.from_name);
			WriteMsg(log, 0, OT_TECH);
		}
		else if (pThread == 0)
		{
			int j = 0;
			return false;
		}

		// is it the thread we are expecting
		if (pThread->next_msg == index)
		{
			// is the next thread having a hash
			if (pThread->next_hash.GetLength() == 0)
				;
			else
			{
				// check the hash codes align
				String md5_actual = Crypto::md5(string(Text));
				if (pThread->next_hash != md5_actual)
				{
					return false;
				}
			}

			// extend the body and ready for the next message
			pThread->body.Append(body);
			pThread->next_msg++;
			pThread->next_hash = next;

			// check length
			int len = pThread->body.GetLength();
			if (len == pThread->size)
			{
				// scripting host
				ScriptingHost::getInst().doFunction("EventProtocolBotgram", 
					&Umbra::VariantArray(*pThread), 0, &bot);

				// done, clean it up
				for (iterator i = begin(); i != end(); i++)
					if (i->id == msg_id)
					{
						threads.erase(i);
						return true;
					}
			}
		}

		// all is good
		return true;
	}

	int BroadcastThread::send(int Citno)
	{
		const int message_size = 500;			// how many bytes per message
		std::vector<String> packets;			// list of output text

		// structures needed
		const char* padding  = "{hash-hash-hash-hash-hash-hash-}";
		const char* searcher = "next: {hash-hash-hash-hash-hash-hash-}\n";
		int size			= (int)body.GetLength();
		int pack_id		= 0;
		String body		= this->body;
		String header	= "";

		// allocate an id
		String msg_id;
		msg_id.Format("%d%d", rand(), rand());

		// until we have absolutely nothing left
		while (body.GetLength() != 0)
		{
			pack_id++;

			// build up a header
			header.Format("BGTP-1: %s\n", (const char*)msg_id);
			header.AppendFormat("part: %d\n", pack_id);
			header.AppendFormat("next: %s\n", padding);
			
			// is there a to filter
			if (to.GetLength() != 0) 
				header.AppendFormat("to: %s\n", to);
			
			// special headers for package 1
			if (pack_id == 1)
			{
				header.AppendFormat("size: %d\n", this->body.GetLength());
				header.AppendFormat("content-type: %s\n", content_type);
				header.AppendFormat("content-name: %s\n", content_name);
			}

			// double space
			header.AppendFormat("\n");

			// how much is left for data
			int bytes_available = message_size - header.GetLength();
			String seg = body.Mid(0, bytes_available);
			body = body.Mid(bytes_available);

			// add packet
			packets.push_back(header + seg);
		}

		// go backwards though each packet
		String prev_md5 = "";
		String replacer;

		// replace md5 for all fields
		for (int i = (int)(packets.size() - 1); i>= 0; i--)
		{
			size_t j = i;
			if (prev_md5.GetLength() == 0)
			{
				replacer = "";
			}
			else
			{
				replacer.Format("next: %s\n", prev_md5);
			}

			// replace only once
			int pos = packets[j].Find(searcher, 0);
			packets[j] = packets[j].Left(pos) + replacer + packets[j].Mid(pos + strlen(searcher));

			// overwrite last md5
			prev_md5 = Crypto::md5(string(packets[i]));
		}

		// send botgrams
		for (size_t i = 0; i < packets.size(); i++)
			bot.Botgram(Citno, packets[i]);
		return 0;
	}

	bool BroadcastThread::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		UMBRA_FUNCTION_BEGIN(send) {
			// required arguments
			UMBRA_FUNCTION_DA(0, To, Umbra::VT_INTEGER);

			// code information break
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( send(vaTo.toInt32()) );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		return R.NOT_HANDLED;
	}

	bool BroadcastThread::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		// content type is ok
		UMBRA_PROPERTY_RW(content_type, content_type);
		UMBRA_PROPERTY_RW(content_name, content_name);
		UMBRA_PROPERTY_RW(body, body);
		
		return R.NOT_HANDLED;
	}
}