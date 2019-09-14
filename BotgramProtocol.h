#pragma once
#include <list>

namespace BotgramProtocol
{
	class Header
	{
	public: // the heeader info
		String key, value;
	};

	class Packet
	{
	public: // the datagram
		String				data;			// the binary data
		std::vector<Header>	headers;		// header information

	public: // packet building
		void	decode(String Code);
		String	getHeader(const char* Key, const char* Default = "");

	};

	class ReceiveThread
	{
	public: // tracking
		String	id;
		int		source_citno;
		String	source_name;
		int		packets_received;
		int		packets_needed;
		int		length;
		String	content_type;

	public: // the packet array
		std::vector<Packet>	packets;

	public: // constructor
		ReceiveThread()
		{
			source_citno	 = 0;
			packets_received = 0;
			packets_needed	 = 0;
			length			 = 0;
		}
	};

	class ReceiverKernel
	{
	public: // list defenition
		typedef std::list<ReceiveThread> ThreadList;
		typedef ThreadList::iterator	 iterator;

	public: // the actual data
		ThreadList threads;

	public: // support information
		ReceiveThread* search(String Id, int Citizen);
		void onReceive(String Name, int Citizen, String Data);

	public: // static singleton
		static ReceiverKernel& getInstance()
		{
			static ReceiverKernel k;
			return k;
		}
	};

	//
	//
	//
	//
	//

	class SendingThread
	{
	public: // the information
		String	id;
		int		target_citizen;
		String	target_user;
		String	content_type;
		int		packet_index;
	
	public: // data packets to send
		std::vector<String> packets;

	public: // body loading
		void open(String Text);

	public: // constructor
		SendingThread()
		{
			target_citizen = 0;
			packet_index   = 0;
		}
	};

	class SendingKernel
	{
	public: // list defenition
		typedef std::list<SendingThread> ThreadList;
		typedef ThreadList::iterator	 iterator;

	public: // the actual data
		ThreadList threads;

	public: // automatic handling
		void add(SendingThread& T) { threads.push_back(T); }
		void onTick();

	public: // static singleton
		static SendingKernel& getInstance()
		{
			static SendingKernel k;
			return k;
		}
	};

	//
	//
	// Conversation Exchanges
	//
	//

	class Conversation
	{
	public: // conversational information
		String	id;
		String	name;
		int		citizen;
		int		type;

	public: // what type of connection is it
		static const int ST_RECIPIENT = 0;
		static const int ST_REQUESTER = 1;
		static const int ST_DUPLEX	  = 2;
	};

	class Conversations
	{
	public: // type defenition for conversations
		typedef std::list<Conversation> ConvList;
		typedef ConvList::iterator		iterator;

	public: // list imp
		ConvList list;
		iterator begin() { return list.begin(); }
		iterator end()   { return list.end();   }
		void     clear() { return list.clear();	}
		void	 add(Conversation C) { list.push_back(C); }
	
	public: // static access
		static Conversations& getInstance()
		{
			static Conversations c;
			return c;
		}
	};
}