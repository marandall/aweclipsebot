#pragma once
#include "awsdk_location.h"
#include <list>
#include "uptr.h"

namespace Recorder
{
	// Msg
	// -------------------------------
	// A single message by a user
	// 
	class Msg
	{
	protected:
		String			text;
		time_t			timestamp;
		String			av_name;
		int				av_session;
		AW::Location	av_pos;
		int				av_index;	// history index
		COLORREF		colour;
	};

	// the avatar
	class Session :
		public AW::AvatarData,
		public u_ptrbase
	{
	protected:
		__int64		 m_enter_time;				// when person entered
		AW::Location m_enter_position;			// where person entered
		__int64		 m_exit_time;				// when person left
		AW::Location m_exit_position;			// where person left
		int			 m_unique_id;				// the tracking id

	public: // accessor methods for the position
		__int64		 getEnterTime()		const	{ return m_enter_time;		}
		AW::Location getEnterPosition() const	{ return m_enter_position;	}
		__int64		 getExitTime()		const	{ return m_exit_time;		}
		AW::Location getExitPosition()	const	{ return m_exit_position;	}
		int			 getUniqueId()		const	{ return m_unique_id;		}

	public: // data copying mechanisms
		void		 begin			(const Avatar& Source);
		void		 update			(const Avatar& Source);
		void		 finalise		(const Avatar& Source);
	};

	class Record : public u_ptrbase
	{
	public: // the elements
		u_ptr<Session>		avatar;
		time_t				time;
	
	public: // constant types
		static const int TYPE_UNKNOWN			= 0;
		static const int TYPE_AVATAR_ADD		= 1;
		static const int TYPE_AVATAR_CHANGE		= 2;
		static const int TYPE_AVATAR_DELETE		= 3;
		static const int TYPE_AVATAR_CLICK		= 4;
		static const int TYPE_CHAT				= 5;
		static const int TYPE_CONSOLE_MESSAGE	= 6;
		static const int TYPE_OBJECT_ADD		= 7;
		static const int TYPE_OBJECT_DELETE		= 8;
		static const int TYPE_OBJECT_CLICK		= 9;
		static const int TYPE_OBJECT_SELECT		= 10;

	public: // constructor
		Record()
		{
			time = ::time64(0);
		}

		time_t getTime() { return time; }

	public: // overrides
		virtual const char*	getRecordName() 
		{
			return "Record";
		}

		virtual int getRecordType() 
		{
			return TYPE_UNKNOWN;
		}

		virtual CStringA getText()
		{
			return "[no details]";
		}
	};

	//
	// Avatar Events
	//

	class AvatarAdd : public Record
	{
	public: // type identification for this record
		const char* getRecordName() { return "AVATAR_ADD";			}
		int getRecordType()			{ return TYPE_AVATAR_ADD;		}
		CStringA getText()			{ return avatar->getCitizenInfo();	}

	public: // custom constructor
		AvatarAdd(Avatar& Av) { avatar = Av.history_session; }
	};

	class AvatarChange : public Record
	{
	public: // type identification for this record
		const char* getRecordName() { return "AVATAR_CHANGE";		}
		int getRecordType()			{ return TYPE_AVATAR_CHANGE;	}
		CStringA getText()			{ return avatar->getCoordinates();	}

	public: // custom constructor
		AvatarChange(Avatar& Av) { avatar = Av.history_session; }
	};

	class AvatarDelete : public Record
	{
	public: // type identification for this record
		const char* getRecordName() { return "AVATAR_DELETE";		}
		int getRecordType()			{ return TYPE_AVATAR_DELETE;	}
		CStringA getText()			{ return avatar->getCoordinates();	}

	public: // custom constructor
		AvatarDelete(Avatar& Av) { avatar = Av.history_session; }
	};

	class AvatarClick : public Record
	{
	public: // type identification for this record
		const char* getRecordName() { return "AVATAR_CLICK";		}
		int getRecordType()			{ return TYPE_AVATAR_CLICK;		}
	
	public: // additional information
		u_ptr<Session>	clicked;

	public: // custom constructor
		AvatarClick(Avatar& Av, Avatar* sAv) { avatar = Av.history_session; sAv ? clicked = sAv->history_session : 0; }
	};

	//
	// Communications
	//

	class Chat : public Record
	{
	public: // type identification for this record
		const char* getRecordName() { return "CHAT";				}
		int getRecordType()			{ return TYPE_CHAT;				}
		CStringA getText()			{ return message;				}

	public: // additional information
		CStringA	message;

	public: // custom constructor
		Chat(Avatar& Av, const char* Message) { avatar = Av.history_session; message = Message; }
	};

	class ConsoleMessage : public Record
	{
	public: // type identification for this record
		const char* getRecordName() { return "CONSOLE_MESSAGE";		}
		int getRecordType()			{ return TYPE_CONSOLE_MESSAGE;	}
		CStringA getText()			{ return message;	}

	public: // additional information
		CStringA	message;
	};

	//
	// Object Events
	//

	class ObjectAdd : public Record
	{
	public: // type identification for this record
		const char* getRecordName() { return "OBJECT_ADD";			}
		int getRecordType()			{ return TYPE_OBJECT_ADD;		}
	};

	class ObjectDelete : public Record
	{
	public: // type identification for this record
		const char* getRecordName() { return "OBJECT_DELETE";		}
		int getRecordType()			{ return TYPE_OBJECT_DELETE;	}
	};

	class ObjectClick : public Record
	{
	public: // type identification for this record
		const char* getRecordName() { return "OBJECT_CLICK";		}
		int getRecordType()			{ return TYPE_OBJECT_CLICK;		}
	};

	class ObjectSelect : public Record
	{
	public: // type identification for this record
		const char* getRecordName() { return "OBJECT_SELECT";		}
		int getRecordType()			{ return TYPE_OBJECT_SELECT;	}
	};

	//
	// Event Recorder
	//

	class Recording
	{	
	public: // type defenitions for a message
		typedef std::list< u_ptr<Record> >	ListType;
		typedef ListType::iterator			iterator;
		typedef ListType::const_iterator	const_iterator;
		typedef ListType::reverse_iterator	reverse_iterator;
		typedef ListType::pointer			pointer;

	public: // handling methods
		void	onAvatarAdd		(Avatar& Av);
		void	onAvatarChange	(Avatar& Av);
		void	onAvatarDelete	(Avatar& Av);
		void	onAvatarClick	(Avatar& Av, Avatar* Cl);
		void	onObjectAdd		(ObjectData& O, Avatar* Av);
		void	onObjectDelete	(ObjectData& O, Avatar* Av);
		void	onObjectClick	(ObjectData& O, Avatar* Av);
		void	onObjectSelect	(ObjectData& O, Avatar* Av);
		void	onChat			(Avatar& Av, const char* Message, int Type);
		void	onConsoleMessage(const char* Message);

	protected: // list variables
		ListType			list;

	public: // access to the list
		iterator			begin(void)		  { return list.begin();	}
		iterator			end	 (void)		  { return list.end();	}
		const_iterator		begin(void)	const { return list.begin();	}
		const_iterator		end  (void) const { return list.end();	}
		reverse_iterator	rbegin(void)	  { return list.rbegin();	}
		reverse_iterator	rend (void)		  { return list.rend();	}
		size_t				size (void) const { return list.size();	}
		void				push_back(u_ptr<Record> M) { list.push_back(M);		}

	public: // copy filters
		void				copy_in_radius	(const AW::Location& Center, int Radius);

	public: // static singleton
		static Recording& getInstance() { static Recording r; return r; }
	};
}