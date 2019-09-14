#pragma once
#include "awsdk_location.h"

namespace AW
{
	class Bot;

	class Rider
	{
	public:
		int		session;
		int		distance;
		int		angle;
		int		y_delta;
		int		yaw_delta;
		int		pitch_delta;
	};

	class Entity : public AW::Location
	{
	protected: // attributed just to the mover
		int		m_type;
		int		m_id;
		int		m_state;
		int		m_flags;
		int		m_owner_session;
		int		m_model_num;
		int		m_owner_citizen;
		String	m_name;
		LocationEx		m_origin;

	public: // read acces
		int		getType			() { return m_type;				}
		int		getId			() { return m_id;				}
		int		getState		() { return m_state;			}
		int		getFlags		() { return m_flags;			}
		int		getOwnerSession	() { return m_owner_session;	}
		int		getModelNum		() { return m_model_num;		}
		int		getOwnerCitizen	() { return m_owner_citizen;	}
		String	getName			() { return m_name;				}
		LocationEx getOrigin	() { return m_origin;			}

	public: // sdk connection
		Bot*	m_pBot;
	
	public: // control methods
		int		rider_add(int id, int session, int dist, int angle,
                        int y_delta, int yaw_delta, int pitch_delta);
		int		rider_change (int id, int session, int dist, int angle,
						int y_delta, int yaw_delta, int pitch_delta);
		int		rider_delete (int id, int session);

	protected: // sdk read support
		void	on_update			(Bot& Client);

	public: // sdk read mechanisms
		void	on_entity_add		(Bot& Client);
		void	on_entity_change	(Bot& Client);
		void	on_entity_delete	(Bot& Client);

	public: // flag checks
		bool    isLoop              () const { int bit_f = AW_MOVER_FLAG_LOOP                       ; return (m_flags & bit_f) == bit_f; }
		bool    isUseObjtag         () const { int bit_f = AW_MOVER_FLAG_USE_OBJTAG                 ; return (m_flags & bit_f) == bit_f; }
		bool    isBumpAdd           () const { int bit_f = AW_MOVER_FLAG_BUMP_ADD                   ; return (m_flags & bit_f) == bit_f; }
		bool    isClickStart        () const { int bit_f = AW_MOVER_FLAG_CLICK_START                ; return (m_flags & bit_f) == bit_f; }
		bool    isClickPause        () const { int bit_f = AW_MOVER_FLAG_CLICK_PAUSE                ; return (m_flags & bit_f) == bit_f; }
		bool    isLockPosition      () const { int bit_f = AW_MOVER_FLAG_LOCK_POSITION              ; return (m_flags & bit_f) == bit_f; }
		bool    isLockYaw           () const { int bit_f = AW_MOVER_FLAG_LOCK_YAW                   ; return (m_flags & bit_f) == bit_f; }
		bool    isLockPitch         () const { int bit_f = AW_MOVER_FLAG_LOCK_PITCH                 ; return (m_flags & bit_f) == bit_f; }
		bool    isSingleRider       () const { int bit_f = AW_MOVER_FLAG_SINGLE_RIDER               ; return (m_flags & bit_f) == bit_f; }
		bool    isNoAutoYaw         () const { int bit_f = AW_MOVER_FLAG_NO_AUTO_YAW                ; return (m_flags & bit_f) == bit_f; }
		bool    isEmptyReset        () const { int bit_f = AW_MOVER_FLAG_EMPTY_RESET                ; return (m_flags & bit_f) == bit_f; }
		bool    isDisableFlying     () const { int bit_f = AW_MOVER_FLAG_DISABLE_FLYING             ; return (m_flags & bit_f) == bit_f; }
		bool    isKeepOnWater       () const { int bit_f = AW_MOVER_FLAG_KEEP_ON_WATER              ; return (m_flags & bit_f) == bit_f; }
		bool    isDisableUpwards    () const { int bit_f = AW_MOVER_FLAG_DISABLE_UPWARDS            ; return (m_flags & bit_f) == bit_f; }
		bool    isInvisibleAv       () const { int bit_f = AW_MOVER_FLAG_INVISIBLE_AV               ; return (m_flags & bit_f) == bit_f; }
		bool    isExitEjectUp       () const { int bit_f = AW_MOVER_FLAG_EXIT_EJECT_UP              ; return (m_flags & bit_f) == bit_f; }
		bool    isExitNonSolid      () const { int bit_f = AW_MOVER_FLAG_EXIT_NON_SOLID             ; return (m_flags & bit_f) == bit_f; }
		bool    isAvApplyTiltX      () const { int bit_f = AW_MOVER_FLAG_AV_APPLY_TILT_X            ; return (m_flags & bit_f) == bit_f; }
		bool    isAvApplyTiltZ      () const { int bit_f = AW_MOVER_FLAG_AV_APPLY_TILT_Z            ; return (m_flags & bit_f) == bit_f; }
		bool    isObjectLink        () const { int bit_f = AW_MOVER_FLAG_OBJECT_LINK                ; return (m_flags & bit_f) == bit_f; }
		bool    isDisableMove       () const { int bit_f = AW_MOVER_FLAG_DISABLE_MOVE               ; return (m_flags & bit_f) == bit_f; }
		bool    isTiltByTerrain     () const { int bit_f = AW_MOVER_FLAG_TILT_BY_TERRAIN            ; return (m_flags & bit_f) == bit_f; }
		bool    isDisableExplSeqs   () const { int bit_f = AW_MOVER_FLAG_DISABLE_EXPL_SEQS          ; return (m_flags & bit_f) == bit_f; }
		bool    isDisableTurnOnPlace() const { int bit_f = AW_MOVER_FLAG_DISABLE_TURN_ON_PLACE      ; return (m_flags & bit_f) == bit_f; }
		bool    isDisableTeleports  () const { int bit_f = AW_MOVER_FLAG_DISABLE_TELEPORTS          ; return (m_flags & bit_f) == bit_f; }
		bool    isDetachOnTeleport  () const { int bit_f = AW_MOVER_FLAG_DETACH_ON_TELEPORT         ; return (m_flags & bit_f) == bit_f; }

	public: // get set
		bool    isStateIdle         () const { int bit_f = AW_MOVER_STATE_IDLE                      ; return (m_state & bit_f) == bit_f; }
		bool    isStateStart        () const { int bit_f = AW_MOVER_STATE_START                     ; return (m_state & bit_f) == bit_f; }
		bool    isStateMove         () const { int bit_f = AW_MOVER_STATE_MOVE                      ; return (m_state & bit_f) == bit_f; }
		bool    isStatePause        () const { int bit_f = AW_MOVER_STATE_PAUSE                     ; return (m_state & bit_f) == bit_f; }
		bool    isStateContinue     () const { int bit_f = AW_MOVER_STATE_CONTINUE                  ; return (m_state & bit_f) == bit_f; }
		bool    isStateStop         () const { int bit_f = AW_MOVER_STATE_STOP                      ; return (m_state & bit_f) == bit_f; }
		bool    isStateReset        () const { int bit_f = AW_MOVER_STATE_RESET                     ; return (m_state & bit_f) == bit_f; }

	public: // set the state
		int	    setStateIdle        ();
		int	    setStateStart       ();
		int	    setStateMove        ();
		int	    setStatePause       ();
		int	    setStateContinue    ();
		int	    setStateStop        ();
		int	    setStateReset       ();

	public: // umbra interfaces
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);
	};
}