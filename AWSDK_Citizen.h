#pragma once
/*****************************************************************************
**                                                                          **
**                                                                          **
**                              AW::Citizen                                **
**               Represents a citizen account and attributes                **
**                                                                          **
** The base citizen is simply a representation of citizen attributes on     **
**   the uniserver as a result of a lookup or next/previous etc.            **
**                                                                          **
** get functions are defined as const to allow easy passing, unlike         **
**   most other classes the set functions are public however being un-const **
**   means that it is up to the user to ensure that an object which should  **
**   not be changed is not.                                                 **
**                                                                          **
** Design History:                                                          **
**    Initial Design:	Mark Randall, August 20, 2005                       **
**                                                                          **
**                                                                          **
*****************************************************************************/
namespace AW
{
	class Bot;

	class Citizen
#ifdef _AWSDK_USE_UMBRA
		: public Umbra::RemoteClass
#endif
	{
	public:
		Citizen();
		virtual ~Citizen();

	protected: // tracking variables
		int		m_number;
		String	m_name;
		String	m_password;
		String	m_email;
		int		m_time_left;
		String	m_privilege_password;
		int		m_immigration_time;
		int		m_expiration_time;
		BOOL	m_beta;
		int		m_last_login;
		int		m_bot_limit;
		int		m_total_time;
		BOOL	m_enabled;
		String	m_comment;
		String	m_url;
		BOOL	m_privacy;
		BOOL	m_trial;
		BOOL	m_cav_enabled;
		BOOL	m_cav_template;
		int		m_last_address;
	
	public: // read access to the known attributes
		int		getCitizen()			const { return m_number;					}
		int		getNumber()			const { return m_number;					}
		String	getName()				const { return m_name;						}
		String	getPassword()			const { return m_password;					}
		String	getEmail()				const { return m_email;						}
		int		getTimeLeft()			const { return m_time_left;					}
		String	getPrivilegePassword() const { return m_privilege_password;		}
		int		getImmigrationTime()	const { return m_immigration_time;			}
		int		getExpirationTime()	const { return m_expiration_time;			}
		BOOL	getBeta()				const { return m_beta;						}
		int		getLastLogin()			const { return m_last_login;				}
		int		getBotLimit()			const { return m_bot_limit;					}
		int		getTotalTime()			const { return m_total_time;				}
		int		getLastAddress()		const { return m_last_address;				}
		BOOL	getEnabled()			const { return m_enabled;					}
		String	getComment()			const { return m_comment;					}
		String	getUrl()				const { return m_url;						}
		BOOL	getPrivacy()			const { return m_privacy;					}
		BOOL	getTrial()				const { return m_trial;						}
		BOOL	getCavEnabled()		const { return m_cav_enabled;				}
		BOOL	getCavTemplate()		const { return m_cav_template;				}
		bool	getDisableTelegrams()	const;
		String	getLastAddressString() const;

	public: // read access to the known attributes
		int		setNumber				(int New)	{ return m_number				= New;	}
		String	setName				(String New)	{ return m_name					= New;	}
		String	setPassword			(String New)	{ return m_password				= New;	}
		String	setEmail				(String New)	{ return m_email				= New;	}
		String	setPrivilegePassword	(String New) { return m_privilege_password	= New;	}
		int		setExpirationTime		(int New)	{ return m_expiration_time		= New;	}
		BOOL	setBeta				(BOOL New)	{ return m_beta					= New;	}
		int		setBotLimit			(int New)	{ return m_bot_limit			= New;	}
		BOOL	setEnabled				(BOOL New)	{ return m_enabled				= New;	}
		String	setComment				(String New)	{ return m_comment				= New;	}
		String	setUrl					(String New)	{ return m_url					= New;	}
		BOOL	setPrivacy				(BOOL New)	{ return m_privacy				= New;	}
		BOOL	setTrial				(BOOL New)	{ return m_trial				= New;	}
		BOOL	setCavEnabled			(BOOL New)	{ return m_cav_enabled			= New;	}
		BOOL	setCavTemplate			(BOOL New)	{ return m_cav_template			= New;	}

	public: // extraction methods
		int		from_sdk(Bot& bot);
		int		to_sdk(Bot& bot);

#ifdef _AWSDK_USE_UMBRA
	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest&);
#endif

	};
};