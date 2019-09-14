#pragma once
#include "umbra/umbra_remoteclass.h"
#include <list>
#include <map>

namespace BotgramTransferProtocol
{
	class ReceiverThread : public Umbra::RemoteClass
	{
	public: // thread information
		String	id;									// message id
		int			from;								// citizen number it is from
		String	from_name;					// which bot name it is from
		int			next_msg;						// the id of the next message
		String	next_hash;					// which hash is the next to be expected
		String	origin;							// self identification of origin

	public: // content information
		String	content_type;				// type aka evo2/help-msg
		String	content_name;				// aka nothing
		int			size;								// how big is the message

	public: // the body code
		String	body;

	public: // scripting interface
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);
	};

	class ReceiveProcessor
	{
	public: // list of receiver threads
		typedef std::list<ReceiverThread> ThreadList;
		typedef std::list<ReceiverThread>::iterator			iterator;

	public: // the storage
		ThreadList threads;
		ThreadList completed;

	public: // front list functions
		iterator	begin() { return threads.begin(); }
		iterator	end()		{ return threads.end();		}
		size_t		size()	{ return threads.size();	}
		void			clear()	{ threads.clear();				}

	public: // processing
		ReceiverThread* getThread(String ID);
		bool			onReceive(int Citizen, String From, String Text);
	};

	class BroadcastThread : public Umbra::RemoteClass
	{
	public: // broadcast options
		String	content_type;				// type aka evo2/help-msg
		String	content_name;				// aka nothing
		String	body;
		String	to;

	public: // storage of output stream
		std::vector<String> packets;

	public: // processing
		int send(int Citnum);

	public: // umbra interface
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);
	};
}