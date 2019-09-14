#pragma once
#include "AWSDK_Avatar.h"			// for AW::Avatar
#include "AWSDK_WorldRights.h"		// for AW::WorldRights
#include "ZoneConnector.h"			// for Zones::ZoneConnector
#include "zone_access_entry.h"		// for zone access entry check
#include "LaserTag_AvatarClass.h"	// for laser tag
#include "Avatar_DataConnector.h"	// for connection class
#include "umbra/umbra_remoteclass.h"
#include "uptr.h"
#include "citizen.h"
#include "autonumber.h"
#include "umbra/umbra_uptr.h"

// avatar extenions
#include "aex_flood_prevention.h"

class Serializer;

namespace AW 
{ 
	namespace World 
	{ 
		class Attributes; 
	} 
}

namespace ChatControllers 
{ 
	class GlobalChatAexPlugin; 
	class MuteList; 

	namespace Channels 
	{
		class MembershipBindings;
	}
}

namespace HeadsUp  { 
	class IconTray; 
	class FreeTray; 
}

namespace UserProfile {
	class FreeJoin; 
}

namespace DynamicRights { 
	class Permissions; 
	class User; 
}

class WorldAttributes;

// constants for the user override state
const int WORLD_OVERRIDE_TEMPLATE	= 1;		// override as a result of a template
const int WORLD_OVERRIDE_PRIVATE    = 2;		// override as a result of a personal profile

namespace Umbra {
	namespace ClassLib {
		namespace Collections {
			class C_AutoMap;
		}
	}
}

namespace Recorder
{
	class Session;
}

class AvatarOptions;

class Avatar : 
	public AW::Avatar ,
	public Zones::ZoneConnector,
	public Zones::AccessEntry,
	public AutoNumberIdentifier,
	public AEX::FloodPrevetion
{
	friend class EclipseBot;
	friend class CommonBot;

public:
	/* constructor and deconstructor */
	Avatar							(void);
	~Avatar							(void);

public: // umbra additional info
	Umbra::VariantMap	script_vars;		// stored properties
	Umbra::Variant		script_wrapper;		// the wrapper class around this avatar

public: // umbra interface
	bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
	bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);
	int  getUmbraFlags() { return UMBRA_FLAGS_ALLOCATE_ONCE; }

protected: // additional umbra function groups
	bool doUmbraFunctionForWorldRights(Umbra::Variant& V, Umbra::FunctionRequest& R);
	bool doUmbraFunctionForPermissions(Umbra::Variant& V, Umbra::FunctionRequest& R);
	bool doUmbraFunctionForHeadsUpTray(Umbra::Variant& V, Umbra::FunctionRequest& R);
	bool doUmbraFunctionForCommunication(Umbra::Variant& V, Umbra::FunctionRequest& R);

public: // the citizen and privilege information
	CitizenInfo	citizen_info;			// resolved citizen information
	CitizenInfo privilege_info;			// resolved privilege information
	bool		citizen_info_available;	
	bool		privilege_info_available;

public: // avatar options
	AvatarOptions* m_pAvatarOptions;
	AvatarOptions& getOptions() { return *m_pAvatarOptions; }

public: // the history
	u_ptr<Recorder::Session>		history_session;	

	/* functions to get the number of icons */
	int		getIconIP				(void)	const;
	int		getIconPrivName			(void)	const;
	int		getIconState			(void)	const;
	int		getIconDNS				(void)	const;
	int		getIconSide				(void)	const;
	int		getIconCitizen			(void)	const;
	int		getIconPrivilege		(void)	const;
	int		getIconBuild			(void)	const;

	/* features for the build */
	int		 getBuildState			(void)	const;
	String	 getBuildDescription	(void)	const;

	/* these are direct database access functions */
	int		WriteSecurityLog		(const char* pMsg);
	void	WriteEnterReport		(void);
	void	UpdateAvatarReport		(void);
	void	WriteExitReport			(void);

	/* string lookup abilities */
	String	getCID					(void) const;
	String	getAddressInfo			(void) const;
	String	GetBestLock				(void) const;
	String	UserStrings				(String str);

	/* mods for ip and dns lookup */
	void	BeginLookupIp			(void);
	void	BeginLookupDns			(void);
	void	EndLookupDns			(void);

public:
	/* security methods */
	void	LookupEjectIfNeeded		(void);
	int		LookupPrivilegeIfNeeded (void);
	long	Eject					(const long Duration, String MessageText, String LogText, Avatar* Caller = NULL);
	long	Ban						(const long Duration, String ReasonText, Avatar* Caller = NULL);

	/* rights access and the modification of rights */
	AW::WorldRights wr_own, wr_privs;
	void	CalculateRights			(void);
	bool	HasCT									(bool cit = false) const;
	bool	HasEject							(bool sing = false, bool cit = false) const;
	bool	HasPS									(bool sing = false, bool cit = false) const;
	bool	HasRoot								(bool sing = false, bool cit = false) const;
	bool	HasBots								(bool sing = false, bool cit = false) const;
	bool	HasEminent						(bool sing = false, bool cit = false) const;
	bool	HasBuild							(bool sing = false, bool cit = false) const;
	bool	HasSpecialObjects			(bool sing = false, bool cit = false) const;
	bool	HasSpecialCommands		(bool sing = false, bool cit = false) const;
	bool	HasSpeak							(bool sing = false, bool cit = false) const;
	bool	HasEnter							(bool sing = false, bool cit = false) const;
	bool	HasV4									(bool sing = false, bool cit = false) const;
	bool	HasVoIP								(bool sing = false, bool cit = false) const;
	bool	HasIndividualRights		(bool citizen = false) const;
	String	GetRightsString			(bool sing = false, bool cit = false) const;

protected: // dynamic rights controller
	DynamicRights::Permissions		*_permissions;

public:
	DynamicRights::Permissions&		getPermissions() { return *_permissions; }
	DynamicRights::User*					allocDynamicRights();

	void	ResetRights				(void);
	bool	GetRight				(String KeyCode) const;
	
	/* support methods called w	hen we wish to carry out some task */
	void	OnEnter					(void);
	void	OnExit					(void);
	void	OnWorldAttributes		(void);
	void	CompileData				(void);

	void	EventCitizenLookup		(int rc, long citizen, LPCSTR name);
	long	WhisperEx				(String MsgText, bool supress = false);
	long	ConsoleMessageEx		(String MsgText, COLORREF Colour, int Style);
	int		ConsoleWhisper			(String MsgText, COLORREF Colour, int Style);
	int		ConsoleMessagePrefixed	(String Prefix, String MsgText, COLORREF Colour, int Style);
	int		SendURL					(String Url, String Target = "", bool Target3D = false, String Post = "");
	long	Click					(int count = 1);
	bool	Freeze					(BOOL freeze);
	int		SendSound				(String SoundUrl);

	bool	whisper_target;
	
	/* get a report*/
	String	GetReport				(void) const;
	String	getStateName			(void) const;
	String	getOutputReport			(bool full = false, bool eject = false, bool self = false) const;
	String	getAvatarDispatch		(void) const;
	String	getXML_Type1			(void) const;
	bool	IsPattern				(String PatternMatch, bool IpAuthorised = true) const;

	/* to do with the user profile */
	String	getFileKey				(void) const;
	void	SyncData				(bool Load);

private:
	CTime			m_enter_time;
	AW::Location	m_enter_location;

public:
	CTime	getEnterTime			(void)		{ return m_enter_time; }

private:
	bool	m_privname_resolved;

	/* privilege internal functions */
	void	ResolvePrivilegeIfAvailable	(void);
	void	ResolvePrivilegeIfUndefined	(void);

public:
	long		uuid_l;
	int			clist_id;
	int			gui_main_index;
	int			gui_side_index;

	/* a muting feature to set avatar quietness */
protected:
	bool		m_muted;
public:
	bool		getMuted			(void)   const { return m_muted;	}
	void		Mute				(void)	 { m_muted = true;	}
	void		Unmute				(void)	 { m_muted = false;	}
	void		setMuted			(bool X) { m_muted = X;		}


	/* the beyond visual range relay */
protected:
	ChatControllers::GlobalChatAexPlugin*	m_global_chat_plugin;
	ChatControllers::MuteList*				m_mute_list;
public:
	ChatControllers::GlobalChatAexPlugin&	getGlobalChatPlugin();
	ChatControllers::MuteList&				getMuteList();

	/* a colour coding system to set an avatars colour in chat */
protected:
	COLORREF	m_display_colour;
public:
	COLORREF	getDisplayColour	(void) const	  { return m_display_colour; }
	void		setDisplayColour	(COLORREF Colour) { m_display_colour = Colour; }

public: // channel membership
	ChatControllers::Channels::MembershipBindings* m_pChannelBindings;
	ChatControllers::Channels::MembershipBindings& getChannelBindings() { return *m_pChannelBindings; }


	/* the tracking of the individual avatar for this sesison */
protected:
	int			m_historic_id;
public:
	void		EnableHistory		(void);
	void		HistoryEvent		(LPCSTR event_id, LPCSTR description);
	int			getHistoricId		(void) const	  { return m_historic_id; }
	void		setHistoricId		(int Id)		  { m_historic_id = Id;   }

	/* zone related functions */
protected:
	void		onEventZoneChanged	(Zones::Zone* pOld, Zones::Zone* pNew);
	void		onEventZoneReset	(void);
	void		ZoneDetach			(Zones::Zone& ptr);
	void		ZoneAttach			(Zones::Zone& ptr);
	bool		ZoneDoWelcome		(Zones::Zone& TheZone);
	bool		ZoneDoWelcome2		(Zones::Zone& TheZone);
	bool		ZoneDoRestriction	(Zones::Zone& TheZone);
	bool		ZAE_KeyConforms		(String Key);
	bool		m_zone_confirmed;

public:
	bool		getZoneConfirmed	(void) { return m_zone_confirmed; } 


public: // hyperion conversion system
	WorldAttributes*	m_pWorldOverride;
	int					m_WorldOverrideState;								// is there an override that has been applied
	bool				m_WorldOverrideComplete;							// is the override complete (for queues)
	WorldAttributes&	getWorldOverride() { return *m_pWorldOverride; }
	int					getWorldOverrideState() { return m_WorldOverrideState; }

public: // hyperion based systems
	void		activateHyperion();									// causes the system to begin looking for changes
	bool		activateHyperionByRefreshConditions();				// only triggers a change if refresh conditions met
	void		useHyperionZone(WorldAttributes& Attr);		// activate the system using zone settings
	void		useHyperionPersonal(WorldAttributes& Attr);	// activate the system using personal settings
	void		useHyperionScript(WorldAttributes& Attr);		// activate the system using scripting overrides

protected: // zone updates
	time_t		last_hyperion_update_time;
	AW::Location last_hyperion_update_position;

public: // updates
	time_t		getLastHyperionUpdateTime() const { return last_hyperion_update_time; }
	AW::Location getLastHyperionUpdatePosition() const { return last_hyperion_update_position; }

public: // database control
	BOOL		db_WriteEnter();
	BOOL		db_WriteUpdate();
	BOOL		db_WriteExit();

public: // umbra interfacing systems
	Umbra::ClassLib::Collections::C_AutoMap* m_automap;
	Umbra::ClassLib::Collections::C_AutoMap& getAutoMap() { return *m_automap; }

public: // heads up display
	HeadsUp::IconTray* m_pHud;
	HeadsUp::IconTray& getHud() { return *m_pHud; }
	HeadsUp::FreeTray* _pFreeTray;
	HeadsUp::FreeTray& getFreeTray() { return *_pFreeTray; }

public: // free join
	UserProfile::FreeJoin* m_pFreeJoin;
	UserProfile::FreeJoin& getFreeJoin() { return *m_pFreeJoin; }

public: // profile loading / saving
	errno_t		profileLoad();
	errno_t		profileSave();
	void		profileExchange(bool Direction, Serializer& Sz);

protected: // last time they did things
	time_t		last_event_time;			// any of the below events except enter
	time_t		last_chat_time;				// message either chat or whisper
	time_t		last_object_time;			// click, add, delete or select
	time_t		last_movement_time;			// state changed
	time_t		last_enter_time;			// user detected by the bot
		
public: // reading the last change
	time_t		getLastEventTime()		const { return last_event_time;		}
	time_t		getLastChatTime()		const { return last_chat_time;		}
	time_t		getLastObjectTime()		const { return last_object_time;	}
	time_t		getLastMovementTime()	const { return last_movement_time;	}
	time_t		getLastEnterTime()		const { return last_enter_time;		}

protected: // last encountered events
	time_t		first_encounter_time;		// time the user left
	String		first_encounter_world;		// which world they occupied
	String		first_encounter_position;	// which coordinates they occupied
	time_t		last_encounter_time;		// time the user left
	String		last_encounter_world;		// which world they occupied
	String		last_encounter_position;	// which coordinates they occupied
	int			previous_encounter_count;		// how many times has been encountered

public: // read access
	time_t		getFirstEncounterTime()		const { return first_encounter_time; }
	String		getFirstEncounterWorld()	const { return first_encounter_world; }
	String		getFirstEncounterPosition()	const { return first_encounter_position; }
	time_t		getLastEncounterTime()		const { return last_encounter_time;	}
	String		getLastEncounterWorld()		const { return last_encounter_world; }
	String		getLastEncounterPosition()	const { return last_encounter_position;	}
	int			getPreviousEncounterCount()	const { return previous_encounter_count;	}

public: // writing the changes
	void		updateLastChatTime()		{ last_event_time = last_chat_time = time64(0);		}
	void		updateLastObjectTime()		{ last_event_time = last_object_time = time64(0);	}
	void		updateLastMovementTime()	{ last_event_time = last_movement_time = time64(0);	}

public: // the mover we are connected to
	int			connected_mover_id;
	int			getConnectedMoverId() const	{ return connected_mover_id;		}
};