#include "stdafx.h"
#include "listener.h"

	bool Listener::connectListener(EventBroadcaster& Broadcaster)
	{
		event_broadcasters.push_back(&Broadcaster);
		Broadcaster.acceptListener(this);
		return true;
	}

	void Listener::disconnectListener(EventBroadcaster* Broadcaster)
	{
		for (size_t i = 0; i < event_broadcasters.size(); i++)
			if (event_broadcasters[i] == Broadcaster)
			{
				event_broadcasters[i]->closeListener(this);
				event_broadcasters.erase(event_broadcasters.begin() + i);
				return;
			}
	}

	Listener::Listener()
	{
	}

	Listener::~Listener()
	{
		while (event_broadcasters.size())
		{
			EventBroadcaster* pb = event_broadcasters.back();
			event_broadcasters.pop_back();

			if (pb->listener_list.size())
			{
				// 
				for (EventBroadcaster::ListenerIterator ptr = pb->listener_list.begin(); ptr != pb->listener_list.end(); )
				{
					if ((*ptr) == this)
						ptr = pb->listener_list.erase(ptr);
					else
						ptr++;
				}
			}
		}
	}

	Listener::Listener(const Listener& Copy)
	{
		// copy the connection list
		for (size_t i = 0; i < Copy.event_broadcasters.size(); i++)
			connectListener(*((EventBroadcaster*)Copy.event_broadcasters[i]));
	}

	void EventBroadcaster::broadcastToListeners(int Id, EventMessage* EventInfo)
	{
		for (ListenerIterator ptr = listener_list.begin(); ptr != listener_list.end(); ptr++)
			(*ptr)->onListener(this, Id, EventInfo);
	}

	EventBroadcaster::EventBroadcaster()
	{

	}

	EventBroadcaster::~EventBroadcaster()
	{
		for (ListenerIterator ptr = listener_list.begin(); ptr != listener_list.end(); ptr++)
		{
			size_t cx = (*ptr)->event_broadcasters.size();
			for (size_t i = 0; i < cx; i++)
			{
				if ((*ptr)->event_broadcasters[i] == this)
				{
					(*ptr)->event_broadcasters.erase((*ptr)->event_broadcasters.begin() + i);
				}
			}
		}
	}

	void EventBroadcaster::acceptListener(Listener* Source)
	{
		listener_list.push_back(Source);
	}

	void EventBroadcaster::closeListener(Listener* Source)
	{
		for (ListenerIterator ptr = listener_list.begin(); ptr != listener_list.end(); )
		{
			if ((*ptr) == Source)
				ptr = listener_list.erase(ptr);
			else
				ptr++;
		}
	}