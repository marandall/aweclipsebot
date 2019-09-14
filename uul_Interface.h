#pragma once

namespace UUL
{
	class User;
	
	/**
		Interface to the host container from a list manager
	*/
	class Interface
	{
	public:
		virtual void	OnUUL_Offline		(User& Entity)	{}
		virtual void	OnUUL_Clear			()				{}
		virtual void	OnUUL_Online		(User& Entity)	{}
		virtual void	OnUUL_UserStatus	(User& Prev, User& Entity) {}

		/* gui specific changes for use with the user list */
		virtual void	OnUUL_GuiAdd		(User& Entity)	{}
		virtual void	OnUUL_GuiChange		(User& Entity)	{}
		virtual void	OnUUL_GuiDelete		(User& Entity)	{}
	};
}