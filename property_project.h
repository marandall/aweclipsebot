#pragma once
#include "property_objects.h"
#include "property_project_ag.h"
#include "listener.h"

class QueryInterfaceDialog;

namespace Property
{
	class AsyncRequest;

	enum PROJECT_JOB_TYPES
	{
		PROJECT_LIVE,
		PROJECT_WORKING,
		PROJECT_BUILDING_ADD,
		PROJECT_BUILDING_LOAD,
		PROJECT_BUILDING_DELETE,
		PROJECT_RESULT
	};

	struct BuildStateCounts
	{
		int	queued;
		int	pending;
		int	failed;
		int	succeeded;

		BuildStateCounts()
		{
			queued		= 0;
			pending		= 0;
			failed		= 0;
			succeeded	= 0;
		}
	};


	class Project : public AutoTemplates::ProjectData, public EventBroadcaster
	{
	public: // protection from edits
		bool		m_protected;
		int			ref_count;
		bool		locked;
		void		protect()		{ m_protected = true;	}
		void		unprotect()		{ m_protected = false;	}
		bool		isProtected()	{ return m_protected;	}

	public: // event notices
		static const int LE_BUILD_STATE_CHANGE	= 1;
		static const int LE_BUILDING_COMPLETED	= 2;

	public: // additional information about the project
		Objects					objects;
		QueryInterfaceDialog*	editor_wnd;

	public: // struct counts
		BuildStateCounts state_counts;
		void		recount();

	public: // reference tracking
		Project()   
		{ 
			job_type	= PROJECT_WORKING; 
			m_protected = false;
			editor_wnd	= 0;
		}

	public: // job type checks
		bool		IsLive()			{ return job_type == PROJECT_LIVE;				}
		bool		IsWorking()			{ return job_type == PROJECT_WORKING;			}
		bool		IsBuilding()		{ return job_type == PROJECT_BUILDING_ADD;		}
		bool		IsBuildLoading()	{ return job_type == PROJECT_BUILDING_LOAD;		}
		bool		IsBuildDeleting()	{ return job_type == PROJECT_BUILDING_DELETE;	}
		bool		IsResult()			{ return job_type == PROJECT_RESULT;			}
		void		setJobType(int Assgn);


	public: // sanity checks
		bool		IsEditable() const	{ return false;			}

	public: // as a result of object actions
		void		useObjectResult(int ReasonCode, ObjectData& Object);
		bool		handle_available_slot	(EclipseBot& Client);
	};
}