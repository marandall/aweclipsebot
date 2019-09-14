#pragma once

#include "aw.h"								// for sdk
#include "AWSDK_Location.h"					// for AW::Location
#include "AWSDK_Config.h"					// for access macros
#include "AWSDK_Enums.h"					// for function ptr
#include "AWSDK_Macros.h"
#include "AWSDK_ReasonCodes.h"				// for reason code defines

/*****************************************************************************
**                                                                          **
**                                 AW::Bot                                  **
**               Superclass of all functionality within Orion               **
**         Primary controller of sdk instances, fully multi-instance		**
**                                                                          **
** The base bot is the root of an SDK interface, supporting all other       **
**   communications and actions with the server, it binds directly to the   **
**   AW::Bots interface.                                                    **
**                                                                          **
** Each orion application should have at least one base bot, unique IDs     **
**   allow for selecting the sdk instance to use, names that beging with    **
**   '@' are not automatically deleted from the sdk upon clearing           **
**   however all other instances are automatically deleted.                 **
**                                                                          **
** Design History:                                                          **
**    Initial Design:	Mark Randall, August 20, 2005                       **
**    Stepped Update:	Mark Randall, August 25, 2005                       **
**    #ifdef'd Sects:	Mark Randall, August 26, 2005                       **
**                                                                          **
**                                                                          **
*****************************************************************************/

#define AW_USERLIST_STATE_OFFLINE		0
#define AW_USERLIST_STATE_ONLINE		1
#define AW_USERLIST_STATE_INVITE		2

namespace AW
{
	class Citizen;

	const bool EXCHANGE_UPDATE_SDK	= false;
	const bool EXCHANGE_UPDATE_DATA	= true;

	class ColourTripletWrapper;

	class Bot : 
		public Location
	{
	public:
		// *****************************************************************************************
		//					      Standard constructor and deconstructor
		// *****************************************************************************************
		Bot(void);
		virtual ~Bot(void);
		void Lock() { }
		void Unlock() { }


	// *****************************************************************************************
	//					Primary bot identification including citnum, name etc
	// *****************************************************************************************
	private:
		/* identification and the likes */
		String			m_name;				// String literal only
		unsigned int	m_session;			// 4.1 requirement
		String			m_login_password;
		String			m_login_application;
		long			m_login_citizen;

	public:
		String			getName							(void) const	{ return m_name;	};
		String			getBotName						(void) const	{ CString id; id.Format("[%s]", m_name); return id; }
		UINT			getSession						(void) const	{ return m_session; }
		int				getOwner						(void)			{ return _int(AW_LOGIN_OWNER); }
		String			getOwnerName					(void)			{ return _string(AW_LOGIN_PRIVILEGE_NAME); }
		void			setName							(LPCSTR name);
		String			m_UniqueID;
		String			getUniqueID						(void)			{ return m_UniqueID; }
		long			getCitizen						(void)			{ return m_login_citizen; }

		static CString	GetReasonCode					(int rc);
		static CString	getUniverseVerID				(int build);

	// *****************************************************************************************
	//					        Multiple Instance related controls
	// *****************************************************************************************
	private:
		void*			m_instance;			// Instance Ptr (mutlisession)
	public:
		void*			getInstance						(void) const			{ return m_instance; };
		BOOL			SelectInstance					(void) const;


		/* universe attributes that should only be set by the create call */
	private:
		String			m_universe_host, m_universe_name;
		int				m_universe_port;
	public:
		String			getUniverseHost					(void) const		{ return m_universe_host; }
		String			getUniverseName					(void) const		{ return m_universe_name; }
		int				getUniversePort					(void) const		{ return m_universe_port; }

		/* properties for the setting of the world and the invisible state */
	protected:
		void			setWorld						(const char* world);
		void			setInvisible					(BOOL mode);
		void			SetGlobalState					(int state);
	public:
		inline String	getWorld						(void) const	{ return _string(AW_WORLD_NAME);	};
		inline int		GetGlobalState					(void) const	{ return m_global_state;			}
		inline BOOL		getInvisible					(void) const	{ return m_invisible;				}	
		inline int		getAvatar						(void) const	{ return m_avatar;					}
		inline int		GetGesture						(void) const	{ return m_gesture;					}
		inline int		getState						(void) const	{ return m_state;					}
		DUAL_PROPERTY(Speed, m_speed, int);
		inline void		setPassiveAvatar				(int av, int state, int gesture) { m_avatar = av; m_state = state; m_gesture = gesture; } 

	private:
		String			m_world;
		int				m_global_state;
		BOOL			m_invisible;
		int				m_avatar, m_gesture, m_state;

		//////////////////////////////////////////////////////////////////////////////////////
		/* event and callback settings, using booleans and switch lookup tables */
	public:
		int				EventSet						(AW_EVENT_ATTRIBUTE a, bool set = TRUE);
		int				CallbackSet						(AW_CALLBACK c, bool set);

	protected:
		int				_event_set						(AW_EVENT_ATTRIBUTE, pfnc_event fptr);
		int				_callback_set					(AW_CALLBACK, pfnc_callback fptr);
	public:
		void*			_event							(AW_EVENT_ATTRIBUTE);
		void*			_callback						(AW_CALLBACK);


		/* the per-bot output message, for when a global message is not available*/
		virtual void	WriteMsgB						(LPCSTR msg, long col, int opts);

		//////////////////////////////////////////////////////////////////////////////////////
		/* access methods to the server connection status */
	public:
		int				getWorldConnection				(void) const					{ return m_world_connection;    }
		int				getUniverseConnection			(void) const					{ return m_universe_connection; }
		static CString	ConnectionToType				(int type, bool ext);
	protected:
		int				setWorldConnection				(int state);
		int				setUniverseConnection			(int state);
		int				m_universe_connection;
		int				m_world_connection;	
	public:
		bool			IsWorldConnected				(void) const;
		bool			IsWorldClosed					(void) const;
		bool			IsUniConnected					(void) const;
		bool			IsUniClosed						(void) const;

		/* simple diagnostics */
	public:
		int				_traffic_count					(int& in, int& out);

		/* initialisation routines and variables */
	private:
		static bool		m_initialised;
	public:
		virtual int		Initialise						(void) ;
		BOOL			getInitialised					(void) const { return m_initialised;	  }

		//////////////////////////////////////////////////////////////////////////////////////
		/* access to the session number, which is constant, and the aw_wait trigger for no good reason */
		unsigned int	_session						(void) const;
		virtual int		TriggerEvents					(int ms = 0);

		//////////////////////////////////////////////////////////////////////////////////////
		/* direct access to the sdk's variables from inside */
	XORION_ATTRIBUTE_SETTING_ACCESS:
		int				_int_set	(AW_ATTRIBUTE attribute, int value)		const;
		int				_string_set	(AW_ATTRIBUTE attribute, LPCSTR value)	const;
		int				_float_set	(AW_ATTRIBUTE attribute, float value)	const;
		int				_bool_set	(AW_ATTRIBUTE attribute, BOOL value)	const;
		int				_data_set	(AW_ATTRIBUTE attribute, LPCSTR data, 	unsigned int length) const;
	public:
		int				_int		(AW_ATTRIBUTE attribute)				const;
		String			_string		(AW_ATTRIBUTE attribute)				const;
		float			_float		(AW_ATTRIBUTE attribute)				const;
		BOOL			_bool		(AW_ATTRIBUTE attribute)				const;
		char*			_data		(AW_ATTRIBUTE attribute, UINT* length)	const;
	public:
		void			_exchange	(bool ToSdk, AW_ATTRIBUTE Attribute, int& Var);
		void			_exchange	(bool ToSdk, AW_ATTRIBUTE Attribute, float& Var);
		void			_exchange	(bool ToSdk, AW_ATTRIBUTE Attribute, String& Var);
		void			_exchange   (bool ToSdk, AW_ATTRIBUTE Attribute, ColourTripletWrapper& Var);

		//////////////////////////////////////////////////////////////////////////////////////
		/* universe connection related functions */
	XORION_EXPLICIT_MODIFIER_ACCESS:
		int				_create		(LPCSTR universe_host, int universe_port);
		int				_login		(void);
		int				_destroy	(void);
	public:
		virtual int		Destroy		(void);
		virtual int		Create		(LPCSTR universe_host, int universe_port, LPCSTR universe_name, BOOL uses_callback);
		virtual int		Login		(int citno, LPCSTR password, LPCSTR name, LPCSTR application, BOOL uses_callback);
	protected:
		virtual void	onCallbackCreate	(int rc);

		/* world connection related functions */
	XORION_EXPLICIT_MODIFIER_ACCESS:
		int				_enter		(LPCSTR world);
		int				_exit		(void);
	public:
		virtual int		EnterWorld	(LPCSTR world, BOOL global, BOOL uses_callback);
		virtual int		ExitWorld	(void);
		virtual void	onEventWorldLinkClosed() { }
		virtual void	onEventWorldLinkOpened() { }

		//////////////////////////////////////////////////////////////////////////////////////
		/* world rights lookup added in 4.1 */
		int				_check_right		(int citizen, char* str)	const;
		int				_check_right_all	(char* str)					const;
		int				_has_world_right	(int citizen, int right)	const;
		int				_has_world_right_all(int right)					const;

		//////////////////////////////////////////////////////////////////////////////////////
		/* object related functions */
		int				_query				(int x, int z, int sequence[3][3]);
		int				_query_5x5			(int x, int z, int sequence[5][5]);
		int				_cell_next			(void);

		//////////////////////////////////////////////////////////////////////////////////////
		/* avatar related functions */
		int				_teleport			(int session_id);
		int				_avatar_set			(int session_id);
		int				_avatar_click		(int session_id);
		int				_url_send			(int session_id, LPCSTR url, LPCSTR target);
		int				_address			(int session_id);
		int				_eject				(void);
		int				_world_attributes_to_avatar		(int session_id);
		virtual int		TeleportUser		(int session_id, int x, int y, int z, int yaw, CStringA world, bool warp);
		virtual int		TeleportUser		(int session_id, AW::Location& pt, LPCSTR world, bool warp);
		virtual int		Eject				(int session_id, int duration);

		//////////////////////////////////////////////////////////////////////////////////////
		/* movement and other related changes */
	XORION_EXPLICIT_MODIFIER_ACCESS:
		int				_state_change		(void);
	public:
		virtual int		Move				(int x, int y, int z, int yaw, int pitch);
		virtual int		Move				(const AW::Location& coords);
		virtual int		FaceTowards			(int x, int z);
		virtual int		FaceAway			(int x, int z);
		virtual int		FaceTowards			(const AW::Location& loc);
		virtual int		FaceAway			(const AW::Location& loc);
		virtual int		MoveInDirectionR	(double angle, int distance, int height);
		virtual int		MoveInDirectionRevR	(double angle, int distance, int height);
		virtual int		ChangeAvatar		(int avatar, int gesture = -1, int state = -1, bool apply = true);
		virtual BOOL	dspc_OnPreMove		(int x, int y, int z, int yaw, int pitch)				{ return TRUE; }

		//////////////////////////////////////////////////////////////////////////////////////
		/* communications */
		int				_say				(LPCSTR msg);
		int				_whisper			(int session, LPCSTR msg);
		int				_console_message	(int session);
		int				_noise				(int session_id);
		int				_user_list			(void);
		int				_world_list			(void);
		virtual int		Say					(LPCSTR msg);
		virtual int		Whipser				(int session, LPCSTR msg);
		virtual int		ConsoleMessage		(int session, LPCSTR msg, COLORREF colour, int Format);
		virtual int		Botgram				(int citizen, LPCSTR message);
		virtual int		SendNoise			(int session_id, LPCSTR sound);

		//////////////////////////////////////////////////////////////////////////////////////
		/* world changing functions */
		int				_world_attributes_change		(void);
		int				_world_attribute_set			(int attribute, char* value);
		int				_world_attribute_get			(int attribute, int* read_only, char* value);
		int				_world_attributes_send			(int Session);

		//////////////////////////////////////////////////////////////////////////////////////
		/* mover changing functions */
		int				_mover_set_state	(int id, int state, int model_num);
		int				_mover_set_position (int id, int x, int y, int z, int yaw, int pitch, int roll);
		int				_mover_rider_add	(int id, int session, int dist, int angle, int y_delta, int yaw_delta, int pitch_delta);
		int				_mover_rider_change (int id, int session, int dist, int angle, int y_delta, int yaw_delta, int pitch_delta);
		int				_mover_rider_delete (int id, int session);
		int				_mover_links		(int id);

		//////////////////////////////////////////////////////////////////////////////////////
		/* bot immitation procedures */
		int				_url_click						(LPCSTR url);
		int				_object_click					(void);
		int				_object_bump					(void);
		//virtual int		ObjectClick						(int x, int z, int number,	BOOL synced);
		//virtual int		ObjectClick						(const AW::Object& obj,	BOOL synced);
		//virtual int		ObjectBump						(int x, int z, int number,	BOOL synced);
		//virtual int		ObjectBump						(const AW::Object& obj,	BOOL synced);

#if (AW_BUILD > 50)
		int				_object_zone_set (
							aw_type_vector size, unsigned char shape, char priority,
							float gravity, float friction, unsigned short flags, unsigned long color,
							unsigned short fog_min, unsigned short fog_max,
							const char* footstep, const char* ambient, const char* camera);
		int				_object_particles_set (
							aw_type_vector_range volume, aw_type_vector_range speed,
							aw_type_vector_range accel,  aw_type_vector_range angle,
							aw_type_vector_range spin,   aw_type_vector_range size,
							unsigned long release_min, unsigned long release_max, unsigned short release_size,
							unsigned long lifespan, unsigned long emitter_lifespan,
							unsigned long fade_in, unsigned long fade_out,
							unsigned long color_start, unsigned long color_end,
							float opacity, unsigned char render_style, unsigned short flags, unsigned char style,
							const char* assetlist, const char* name);
		int				_object_mover_set (unsigned char type, unsigned short flags,
							const char* name, const char* script, const char* sound, 
							unsigned short waypoints_len, void* waypoints, const char* bump_name);
		int				_object_camera_set (unsigned short flags, float zoom, const char* name);
		
		/* new data getters */
		aw_object_data_zone*		_object_zone		(int* len);
		aw_object_data_particles*	_object_particles	(int* len);
		aw_object_data_camera*		_object_camera		(int* len);
		aw_object_data_mover*		_object_mover		(int* len);
#endif

		//////////////////////////////////////////////////////////////////////////////////////
		/* heads up display */
		int				_hud_create						(void);
		int				_hud_click						(void);
		int				_hud_destroy					(int session, int id);
		int				_hud_clear						(int session);

		//////////////////////////////////////////////////////////////////////////////////////
		/* terrain related functions */
		int				_terrain_set					(int x, int z, int count, int texture, int* heights);
		int				_terrain_query					(int page_x, int page_z, unsigned long sequence);
		int				_terrain_next					(void);
		int				_terrain_delete_all				(void);
		int				_terrain_load_node				(void);

		//////////////////////////////////////////////////////////////////////////////////////
		/* object related functions */
		int				_object_add						(void);
		int				_object_load					(void);
		int				_object_change					(void);
		int				_object_delete					(void);

		//////////////////////////////////////////////////////////////////////////////////////
		/* world license related functions */
		int				_license_add					(void);
		int				_license_attributes				(LPCSTR name);
		int				_license_change					(void);
		int				_license_next					(void);
		int				_license_delete					(LPCSTR name);
		int				_license_previous				(void);

		//////////////////////////////////////////////////////////////////////////////////////
		/* citizen related functions */
		int				_citizen_attributes_by_number	(int citizen);
		int				_citizen_attributes_by_name		(LPCSTR name);
		int				_citizen_add					(void);
		int				_citizen_change					(void);
		int				_citizen_delete					(int citizen_id);
		int				_citizen_next					(void);
		int				_citizen_previous				(void);
		int				CitizenAdd						(LPCSTR name,	LPCSTR password,	LPCSTR email,		BOOL beta);
		int				CitizenAdd						(const AW::Citizen& cit);
		int				CitizenChange					(int number,	LPCSTR name,		LPCSTR password,	LPCSTR privilege, LPCSTR email, LPCSTR url, LPCSTR comment, BOOL beta, BOOL enabled, int bot_limit, int expires);
		int				CitizenChange					(const AW::Citizen& cit);

		//////////////////////////////////////////////////////////////////////////////////////
		/* server related functions */
		int				_server_world_add				(void);
		int				_server_world_change			(int server_id);
		int				_server_world_delete			(int server_id);
		int				_server_world_list				(void);
		int				_server_world_set				(int server_id);
		int				_server_world_start				(int server_id);
		int				_server_world_stop				(int server_id);

		//////////////////////////////////////////////////////////////////////////////////////
		/* world ejection related functions */
		int				_world_eject					(void);
		int				_world_ejection_add				(void);
		int				_world_ejection_delete			(void);
		int				_world_ejection_lookup			(void);
		int				_world_ejection_next			(void);
		int				_world_ejection_previous		(void);
		virtual int		WorldEjection_DeleteAddress		(int ip_addr);
		virtual int		WorldEjection_DeleteCompID		(int comp_id);
		virtual int		WorldEjection_DeleteCitizen		(int citizen_id);
		virtual int		WorldEjection_DeleteByType		(int code_id,		int type_id);
		virtual int		WorldEjection_AddAddress		(int ip_addr,		LPCSTR comment, int expires);
		virtual int		WorldEjection_AddCompID			(int comp_id,		LPCSTR comment, int expires);
		virtual int		WorldEjection_AddCitizen		(int citizen_id,	LPCSTR comment, int expires);
		virtual int		WorldEjection_AddByType			(int code_id,		int type_id,	LPCSTR comment, int expires);

		//////////////////////////////////////////////////////////////////////////////////////
		/* universe ejection related functions */
		int				_universe_ejection_add			(void);
		int				_universe_ejection_delete		(int address_id);
		int				_universe_ejection_lookup		(void);
		int				_universe_ejection_next			(void);
		int				_universe_ejection_previous		(void);

		/* camera modification */
		int				_camera_set						(int session_id);
		int				CameraFaceObject				(int session_id, LPCSTR object_name);
		int				CameraFaceObject				(int session_id, LPCSTR object_from, LPCSTR object_to);
		int				CameraFaceSession				(int session_id, int targetid);
		int				CameraFirstPerson				(int session_id);
		int				CameraObjectTracksSession		(int session_id, LPCSTR tracking_object, int tracking_session);
		int				CameraSessionTracksObject		(int session_id, LPCSTR tracking_object, int tracking_session);

		//////////////////////////////////////////////////////////////////////////////////////
		/* universe attributes related functions */
		int				_universe_attributes_change		(void);
		/* support methods */
		int				UniAttributes_ChangeBrowserRange(int min, int max, int beta);
		int				UniAttributes_ChangeWorldRange	(int min, int max, int beta);
		int				UniAttributes_ChangeIO			(LPCSTR new_io);

		/* quick information on if a right is available (as provided by SDK) */
		bool			HasEject						(void) const;
		bool			HasPS							(void) const;
		bool			HasCT							(void) const;
		bool			HasRoot							(void) const;
		bool			HasUniUserList					(void) const;

	protected:
		/* events that are used for a change in visible or connection state*/
		virtual void	onEventWorldConnectionChange   (int state)							{}
		virtual void	onEventUniverseConnectionChange(int state)							{}
		virtual void	onEventVisible					(void)								{}
		virtual void	onEventMove					(void)								{}

		//////////////////////////////////////////////////////////////////////////////////////
		/* avatar related handlers */
		static void		AWAPI_event_avatar_add			(void);
		static void		AWAPI_event_avatar_change		(void);
		static void		AWAPI_event_avatar_delete		(void);
		static void		AWAPI_event_avatar_click		(void);
		static void		AWAPI_callback_address			(int ReasonCode);
		/* event handlers */
		virtual void	onEventAvatarAdd				(void)								{}
		virtual void	onEventAvatarChange			(void)								{}
		virtual void	onEventAvatarDelete			(void)								{}
		virtual void	onEventAvatarClick				(void)								{}
		/* callback handlers */
		virtual void	onCallbackAddress				(int ReasonCode)					{}

		//////////////////////////////////////////////////////////////////////////////////////
		/* object overrrides */
		static void		AWAPI_event_object_add			(void);
		static void		AWAPI_event_object_delete		(void);
		static void		AWAPI_event_object_click		(void);
		static void		AWAPI_event_object_select		(void);
		/* event handlers */
		virtual void	onEventObjectAdd				(void)								{}
		virtual void	onEventObjectDelete			(void)								{}
		virtual void	onEventObjectSelect			(void)								{}
		virtual void	onEventObjectClick				(void)								{}
		virtual void	onEventObjectBump				(void)								{}
	
		//////////////////////////////////////////////////////////////////////////////////////
		/* terrain overrrides */
		static void		AWAPI_event_terrain_begin		(void);
		static void		AWAPI_event_terrain_changed		(void);
		static void		AWAPI_event_terrain_data		(void);
		static void		AWAPI_event_terrain_end			(void);
		/* event handlers */
		virtual void	onEventTerrainBegin			(void)								{}
		virtual void	onEventTerrainChanged			(void)								{}
		virtual void	onEventTerrainData				(void)								{}
		virtual void	onEventTerrainEnd				(void)								{}

		//////////////////////////////////////////////////////////////////////////////////////
		/* Heads Up Display */
		virtual void	onEventHudClick				(void)								{}
		virtual void	onEventHudCreate				(void)								{}
		virtual void	onEventHudDestroy				(void)								{}
		virtual void	onEventHudClear				(void)								{}
		virtual void	onCallbackHudResult			(int ReasonCode)					{}
		// event handlers
		static void		AWAPI_event_hud_click			(void);
		static void		AWAPI_event_hud_create			(void);
		static void		AWAPI_event_hud_destroy			(void);
		static void		AWAPI_event_hud_clear			(void);
		static void		AWAPI_callback_hud_result		(int ReasonCode);

		//////////////////////////////////////////////////////////////////////////////////////
		/* cell related handlers*/
		static void		AWAPI_event_cell_begin			(void);
		static void		AWAPI_event_cell_end			(void);
		static void		AWAPI_event_cell_object			(void);
		static void		AWAPI_callback_cell_result		(int ReasonCode);

		/* event handlers */
		virtual void	onEventCellBegin				(void)								{ }
		virtual void	onEventCellEnd					(void)								{ }
		virtual void	onEventCellObject				(void)								{ }

		/* overrides used for the query mechanism */
		virtual void	onCallbackQuery				(int ReasonCode)				{ }
		virtual void	onCallbackCellResult			(int ReasonCode)		{ }

		//////////////////////////////////////////////////////////////////////////////////////
		/* override events used for the universe server */
		static void		AWAPI_event_universe_attributes	(void);
		virtual void	OnSys_UniverseAttributes		(void);
		/* event handlers */
		virtual void	onEventUniverseAttributes		(void)						{ }
		virtual void	onEventUniverseIOChange		(void)							{ }

		//////////////////////////////////////////////////////////////////////////////////////
		/* special overrides used for universe disconnect */
		static void		AWAPI_event_universe_disconnect	(void);
		virtual void	OnSys_UniverseDisconnect		(void);
		/* event handlers */
		virtual void	onEventUniverseDisconnect		(int reason)			{ }
		virtual void	onEventUniverseRecover			(void)						{ }
		virtual void	onEventUniverseLinkClosed		(void)						{ }
		virtual void	onEventUniverseLinkOpened		(void)						{ }

		//////////////////////////////////////////////////////////////////////////////////////
		/* special overrides used for universe disconnect */
		static void		AWAPI_event_world_info				(void);
		static void		AWAPI_callback_world_list			(int rc);
		static void		AWAPI_callback_world_ejection	(int rc);
		static void		AWAPI_callback_world_ejection_result(int rc);
		virtual void	onCallbackWorldEjection				(int rc)				{ }
		virtual void	onCallbackWorldEjectionResult	(int rc)				{ }

		/* callback handlers */
		virtual void	onCallbackWorldList			(int rc)							{ }
		virtual void	onEventWorldInfo				(void)								{ }

		//////////////////////////////////////////////////////////////////////////////////////
		/* events used for sdk teleportation */
		static void		AWAPI_event_teleport	(void);
		virtual void	onEventTeleport				(void)									{ }

		//////////////////////////////////////////////////////////////////////////////////////
		/* special overrides used for universe login*/
		static void		AWAPI_callback_login			(int ReasonCode);
		virtual void	OnSys_CallbackLogin				(int ReasonCode);
		/* callback handlers */
		virtual void	onCallbackLogin				(int ReasonCode)					{}
		/* event handlers */
		virtual void	onEventLogin					(void)								{}
		virtual void	onEventLoginFailed				(int ReasonCode)					{}

		//////////////////////////////////////////////////////////////////////////////////////
		/* callbacks used by citizen queries to the universe server */
		virtual void	onCallbackCitizenResult		(int ReasonCode)					{}
		virtual void	onCallbackCitizenAttributes	(int ReasonCode)					{}

		//////////////////////////////////////////////////////////////////////////////////////
		/* world attributes and related events / callbacks */
		static void		AWAPI_event_world_attributes	(void);
		static void		AWAPI_event_world_disconnect	(void);
		/* events handlers */
		virtual void	onEventWorldAttributes			(void)								{}
		virtual void	onEventWorldDisconnect			(int ReasonCode)					{}
		virtual void	onEventWorldIOChange			(void)								{}
		virtual void	onEventWorldRecover			(void)								{}
		virtual void	onEventWorldRecoverFailed		(int ReasonCode)					{}
		virtual void	onEventWorldEjected			(void)								{}
		/* managed handlers */
		virtual void	OnSys_WorldAttributes			(void);
		virtual void	OnSys_WorldDisconnect			(int ReasonCode);
		virtual void	OnSys_CallbackEnter				(int ReasonCode);
		/* callback handlers */
		virtual void	onCallbackEnter				(int ReasonCode)					{}

		//////////////////////////////////////////////////////////////////////////////////////
		/* events used for communications events */
		static void		AWAPI_event_chat				(void);
		static void		AWAPI_event_console_message		(void);
		static void		AWAPI_event_botgram				(void);
		static void		AWAPI_event_url_click			(void);
		static void		AWAPI_callback_botgram_result	(int rc);
		
		/* event handlers */
		virtual void	onEventChat						(void)								{}
		virtual void	onEventConsoleMessage			(void)								{}
		virtual void	onEventBotgram					(void)								{}
		virtual void	onEventUrlClick					(void)								{}
		virtual void	onCallbackBotgramResult			(int rc)							{}

		//////////////////////////////////////////////////////////////////////////////////////
		/* the event handlers for entity */
		static void		AWAPI_event_entity_add			(void);
		static void		AWAPI_event_entity_change		(void);
		static void		AWAPI_event_entity_delete		(void);
		static void		AWAPI_event_entity_rider_add	(void);
		static void		AWAPI_event_entity_rider_change	(void);
		static void		AWAPI_event_entity_rider_delete	(void);
		static void		AWAPI_event_entity_links		(void);

		/* event handlers */
		virtual void	onEventEntityAdd				(void)								{}
		virtual void	onEventEntityChange			(void)								{}
		virtual void	onEventEntityDelete			(void)								{}
		virtual void	onEventEntityRiderAdd			(void)								{}
		virtual void	onEventEntityRiderChange		(void)								{}
		virtual void	onEventEntityRiderDelete		(void)								{}
		virtual void	onEventEntityLinks				(void)								{}

		/* static methods used as the destination pointers for the events and callbacks */
		static void		AWAPI_callback_create			(int ReasonCode);
		static void		AWAPI_callback_enter			(int ReasonCode);

		/* world changes */

		/* citizen related callbacks */
		static void		AWAPI_callback_citizen_result	(int ReasonCode);
		static void		AWAPI_callback_citizen_attributes(int ReasonCode);


		/* object related callback handlers */
		static void		AWAPI_callback_query			(int ReasonCode);
		static void		AWAPI_callback_object_result	(int ReasonCode);
		virtual void	onCallbackObjectResult			(int ReasonCode)					{};

		/* related to the citizen list */
		static void		AWAPI_event_user_info			(void);
		static void		AWAPI_callback_user_list		(int ReasonCode);
		/* callbacks and events used by the user list */
		virtual void	onEventUserInfo				(void)								{}
		virtual void	onCallbackUserList				(int ReasonCode)					{}


		/* variables for storing the local universe profile */
	private:
		String		m_world_io, m_universe_io;

		/* major helpers for setting the various different forms of events */
	protected:
		virtual void	setAvatarEvents					(bool yes);
		virtual void	setObjectEvents					(bool yes);
		virtual void	setCellEvents					(bool yes);
		virtual void	setWorldEvents					(bool yes);
		virtual void	setUniverseEvents				(bool yes);
		virtual void	setCommunicationEvents			(bool yes);
		virtual void	setUserListEvents				(bool yes);
		virtual void	setTerrainEvents				(bool yes);
		virtual void	setHudEvents					(bool yes);
		virtual void	setEntityEvents					(bool yes);
		virtual void	SetRiderEvents					(bool yes);

		/* automatic login variables for the async login */
	protected:
		String		m_al_uni_host, m_al_uni_name;
		int			m_al_uni_port;
		int			m_al_citizen;
		String		m_al_password, m_al_name;
		String		m_al_world,	m_al_coordinates;
		bool		m_al_global, m_al_invisible;
		bool		m_al_enabled;

		/* retriever functions */
	public:
		inline String	getAL_UniHost()					{ return m_al_uni_host;	}
		inline int		getAL_UniPort()					{ return m_al_uni_port;	}
		inline String	getAL_UniName()					{ return m_al_uni_name; }
		inline int		getAL_Citizen()					{ return m_al_citizen;	}
		inline String	getAL_Password()				{ return m_al_password;	}
		inline String	getAL_World()					{ return m_al_world;	}
		inline String	getAL_Coordinates()				{ return m_al_coordinates;}
		inline bool		getAL_Global()					{ return m_al_global;	  }
		inline bool		getAL_Invisible()				{ return m_al_invisible;  }
		inline bool		getAL_Enabled()					{ return m_al_enabled;	  }
		inline void		setAL_UniHost(LPCSTR host)		{ m_al_uni_host	= host;		}
		inline void		setAL_UniPort(int port)			{ m_al_uni_port = port;		}
		inline void		setAL_UniName(LPCSTR name)		{ m_al_uni_name = name;		}
		inline void		setAL_Citizen(int citizen)		{ m_al_citizen  = citizen;	}
		inline void		setAL_Password(LPCSTR pass)		{ m_al_password = pass;		}
		inline void		setAL_World(LPCSTR world)		{ m_al_world	= world;	}
		inline void		setAL_Coordinates(LPCSTR coords){ m_al_coordinates = coords;}
		inline void		setAL_Global(bool global)		{ m_al_global	= global;	}
		inline void		setAL_Invisible(bool invis)		{ m_al_invisible= invis;	}
		inline void		setAL_Enabled(bool on)			{ m_al_enabled	= on;		}

	public:
		int				AL_Execute(const char* uni_host, int uni_port, const char* uni_name, int login_citizen, const char* login_password, const char* login_name, const char* world_name, const char* world_coordinates, bool world_global, bool world_invisible);

	protected:
		virtual void	OnALEvent_CallbackLogin			(int rc);
		virtual void	OnALEvent_CallbackEnter			(int rc);
		virtual void	OnALEvent_Complete				(void)								{}
		virtual void	OnALEvent_Fail					(int rc)							{}
	};

	int MakeConsoleStyle(bool Bold, bool Italic, bool Supressed);
};