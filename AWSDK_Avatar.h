#pragma once
#include "AWSDK_Location.h"			// for AW::Location
#include "AWSDK_Enums.h"			// for enums

namespace AW 
{
	class HUD_Element;

	class AvatarData :
		public Location
	{
	public:
		virtual ~AvatarData() { };

	protected:
		String			m_name;
		int				m_session, m_type;

		int				m_state, m_avatar, m_gesture, m_build, m_ticks;
		double			m_ryaw, m_rpitch;

		/* extended avatar information for 4.1 */
		int				m_flags;
		BOOL			m_locked;

		/* citizenship and privilege information */
		int				m_citizen, m_privilege;
		String			m_priv_name;
		int				m_priv_lus;

		/* avatar address and dns information */
		unsigned int	m_address_lng;
		String			m_address, m_dns;
		int				m_address_lus, m_dns_lus;

	public:
		AvatarData::AvatarData()
		{
			/* avatar data */
			m_session		= 0;
			m_type			= 0;
			m_state			= 0;
			m_avatar		= 0;
			m_gesture		= 0;
			m_build			= 0;
			m_ticks			= 0;
			
			/* extended avatar information for 4.1 */
			m_flags			= 0;
			m_locked		= 0;

			/* citizenship and privilege information */
			m_citizen		= 0;
			m_privilege		= 0;
			m_priv_lus		= LookupState::None;

			/* avatar address and dns information */
			m_address_lng	= 0;
			m_address_lus	= LookupState::None;
			m_dns_lus		= LookupState::None;
		}

	public:
		String			getName			(void) const	{ return m_name;	}
		int				getSession		(void) const	{ return m_session; }
		int				getType			(void) const	{ return m_type;	}
		
		/* avatar data */
		int				getState		(void) const	{ return m_state;	}
		int				getAvatar		(void) const	{ return m_avatar;	}
		int				getGesture		(void) const	{ return m_gesture; }
		int				getBuild		(void) const	{ return m_build;	}
		int				getTicks		(void) const	{ return m_ticks;	}
		double			getrYaw			(void) const	{ return m_ryaw;	}
		double			getPitch		(void) const	{ return getTilt();	}

		/* citizen stuff */
		int				getCitizen		(void) const	{ return m_citizen;		}
		int				getPrivilege	(void) const	{ return m_privilege;	}
		String			getPrivName		(void) const	{ return m_priv_name;	}
		int				getPrivLUS		(void) const	{ return m_priv_lus;	}

		/* the ip and dns */
		unsigned int	getAddressLng	(void) const	{ return m_address_lng;	}
		String			getAddress		(void) const	{ return m_address;		}
		int				getAddressLUS	(void) const	{ return m_address_lus;	}
		String			getDns			(void) const	{ return m_dns;			}
		int				getDnsLUS		(void) const	{ return m_dns_lus;		}

	protected:
		virtual void	setName			(LPCSTR Name)	{ m_name	= Name;		}
		virtual void	setSession		(int Session)	{ m_session	= Session;	}
		virtual void	setType			(int Type)		{ m_type = Type;		}
		virtual void	setState		(int State)		{ m_state = State;		}

		/* extra data */
		virtual void	setAvatar		(int Avatar)	{ m_avatar = Avatar;	}
		virtual void	setGesture		(int Gesture)	{ m_gesture = Gesture;	}
		virtual void	setBuild		(int Build)		{ m_build = Build;		}
		virtual void	setTicks		(int Ticks)		{ m_ticks = Ticks;		}
		
		/* citizen and privilege */
		virtual void	setCitizen		(int Citizen)	{ m_citizen = Citizen;	}
		virtual void	setPrivilege	(int Privilege)	{ m_privilege = Privilege;	}
		virtual void	setPrivLUS		(int LUS)		{ m_priv_lus = LUS;		}
		virtual void	setPrivName		(LPCSTR Name)	{ m_priv_name = Name;	}

		/* address information */
		virtual void	setAddressLUS	(int LUS)		{ m_address_lus = LUS;	}
		virtual void	setDnsLUS		(int LUS)		{ m_dns_lus	= LUS;		}
		virtual void	setAddress		(LPCSTR Address){ m_address = Address;	}
		virtual void	setDns			(LPCSTR Dns)	{ m_dns  = Dns;			}

	protected: /* copy class */
		virtual void	CopyData		(const AvatarData& Data){ *this = Data;	}

	public: /* relating to type checks */
		bool			IsCitizen		(void) const { return m_citizen && m_privilege;		}
		bool			IsTourist		(void) const { return !m_citizen && !m_privilege;	}
		bool			IsBot			(void) const { return !m_citizen && m_privilege;	}

	public: /* creating user strings */
		virtual String	getCitizenInfo	(void) const;
		virtual String	getAddressInfo	(void) const;
		virtual bool	IsPattern		(String Pattern, bool IpAuthorised) const;

#ifdef _AWSDK_USE_UMBRA
	public: // umbra enhancements
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest&);
#endif
	};

	class Avatar :
		public AvatarData 
	{
	public:
		/* Communications actions to carry out against the user */
		virtual int		Whisper		(LPCSTR message);
		virtual int		ConsoleMsg	(LPCSTR message, COLORREF colour, int Style);
		
		/* support methods for name and location */
		virtual int		getStartup	(const AW::Bot& bot);
		virtual int		getChanges	(const AW::Bot& bot);

		/* warping and teleport methods against the user */
		virtual int		Teleport	(int x, int y, int z, int yaw, LPCSTR world = NULL, bool warp = false);
		virtual int		Teleport	(const AW::Location& pt, LPCSTR world = NULL, bool warp = false);
		virtual int		Eject		(int duration);
		virtual int		Avatarset	(int avatar, int gesture, int state = -1);
		
		/* loopup methods for the IP */
		virtual int		LookupIP	();
		virtual void	OnIpLookupResult(unsigned int ip, int rc);
		virtual void	OnDnsLookupResult(LPCSTR dns, int state);

		/* feedback messages for the privilege name */
		virtual void	OnPrivilegeResolved(LPCSTR name);

		/* 4.1 communications features - always enabled */
		virtual int		Noise		(LPCSTR sound);
		virtual int		CreateHUD	(HUD_Element& Element);
		virtual int		DestroyHUD	(int ElementID);
		virtual int		ClearHUD	();

		AW::Bot*		plc_bot;
	};
};