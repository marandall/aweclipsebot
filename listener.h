#pragma once

class EventBroadcaster;

class EventMessage
{
public:
	virtual ~EventMessage()
	{
	}
};

class Listener
{
	friend class EventBroadcaster;

public: // the listener information
	virtual void onListener(EventBroadcaster* Broadcaster, int Id, EventMessage* EventInfo) = 0;

public: // connection event
	bool connectListener(EventBroadcaster& Broadcaster);
	void disconnectListener(EventBroadcaster* Broadcaster);

protected: // list of broadcaster targetted
	std::vector<EventBroadcaster*>	event_broadcasters;

public: // constructor
	Listener();
	virtual ~Listener();
	Listener(const Listener& Copy);

public: // notification events
	static const int LE_DESTROY	= 0;
};

class EventBroadcaster
{
	friend class Listener;

private: // list of broadcasting targets
	typedef std::list<Listener*>	ListenerList;
	typedef ListenerList::iterator	ListenerIterator;

protected: // listener list data
	ListenerList listener_list;

public: // send a message
	void broadcastToListeners(int Id, EventMessage* EventInfo);

public: // constructor
	EventBroadcaster();
	virtual ~EventBroadcaster();

public: // connection data
	void acceptListener(Listener* Source);
	void closeListener(Listener* Source);
};