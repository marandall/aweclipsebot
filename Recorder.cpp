#include "stdafx.h"
#include "Recorder.h"

namespace Recorder
{
	void Session::begin(const Avatar& Source)
	{
		// effective initialisation
		m_enter_time = 0;
		m_exit_time	 = 0;

		// autonumber
		static int unique_id = 0;
		m_unique_id = (unique_id++);

		// copy routunes
		CopyData(Source);
		m_enter_time	 = time64(0);
		m_enter_position = Source;
	}

	void Session::update(const Avatar& Source)
	{
		CopyData(Source);
	}
	
	void Session::finalise(const Avatar& Source)
	{
		CopyData(Source);
		m_exit_time		 = time64(0);
		m_exit_position	 = Source;
	}

	void Recording::onAvatarAdd(Avatar& Av)
	{
		Av.history_session = new Session();
		Av.history_session->begin(Av);

		// new avatar add instance
		list.push_back( new AvatarAdd(Av) );
	}

	void Recording::onAvatarChange(Avatar& Av)
	{
		list.push_back(new AvatarChange(Av));
	}

	void Recording::onAvatarDelete(Avatar& Av)
	{
		list.push_back(new AvatarDelete(Av));
	}

	void Recording::onAvatarClick(Avatar& Av, Avatar* Cl)
	{
		list.push_back(new AvatarClick(Av, Cl));
	}

	void Recording::onObjectAdd(ObjectData& O, Avatar* Av)
	{
	}

	void Recording::onObjectDelete(ObjectData& O, Avatar* Av)
	{
	}

	void Recording::onObjectClick(ObjectData& O, Avatar* Av)
	{
	}

	void Recording::onObjectSelect(ObjectData& O, Avatar* Av)
	{
	}

	void Recording::onChat(Avatar& Av, const char* Message, int Type)
	{
		list.push_back( new Chat(Av, Message) );
	}

	void Recording::onConsoleMessage(const char* Message)
	{
	}

}