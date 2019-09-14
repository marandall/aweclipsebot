#pragma once
#include "awsdk_objects_v3.h"
#include "umbra/umbra_remoteclass.h"

namespace Property
{
	namespace FileFormat
	{
		enum PROPERTY_PROJECT_VERSION_1_ALIGNMENT
		{
			// identification
			PP1_ID					= 0,			// object id
			PP1_NUMBER,								// object number
			PP1_ORIGIN,								// origional object groups
			PP1_OWNER,								// citizen number
			PP1_TIMESTAMP,							// unix timestamp
			
			// position
			PP1_X,
			PP1_Y,
			PP1_Z,
			PP1_YAW,
			PP1_TILT,
			PP1_ROLL,
			PP1_TYPE,
			
			// the string data
			PP1_MODEL,								// model string
			PP1_DESCRIPTION,						// description string
			PP1_ACTION,								// action string
			PP1_BINARY,								// binary hex string

			// additional data
			PP1_SELECTED,
			PP1_LAYER,

			// cap on the individual tokens
			PP1_MAX_ALIGNMENT
		};

		enum FILE_FORMAT_TYPE
		{
			FORMAT_UNDEFINED = 0,
			FORMAT_PROJECT,
			FORMAT_V2,
			FORMAT_V3,
			FORMAT_V4
		};
	}

	class PropertyProject1_FormatData
	{

	};
}

// defenition of an Eclipse Type object data
class ObjectData :
	public AW::ObjectData
{
public:
	void ShiftXYZ(int X, int Y, int Z)
	{
		m_x+= X;  m_y+= Y; m_z+= Z;
	}

public: // umbra functions
	bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
	bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);
	int  getUmbraFlags() { return UMBRA_FLAGS_ALLOCATE_ONCE; }

public: // static values
	static const int BS_NONE		= 0;		// has not been allocated
	static const int BS_QUEUED		= 1;		// is expected to be built
	static const int BS_PENDING		= 2;		// is currently being proceed
	static const int BS_SUCCEEDED	= 3;		// has completed successfully
	static const int BS_FAILED		= 4;		// has completed with an error
	static const int BS_MAX			= 5;

protected: // building progress
	int		build_state;

public: // build sttae access
	int		getBuildState() const { return build_state; }
	void	changeBuildState(int State) { build_state = State;}

public: // boolean checks
	bool	isIdle()					{ return build_state == BS_NONE;		}
	bool	isQueued()					{ return build_state == BS_QUEUED;		}
	bool	isPending()					{ return build_state == BS_PENDING;		}
	bool	isSucceeded()				{ return build_state == BS_SUCCEEDED;	}
	bool	isFailed()					{ return build_state == BS_FAILED;		}

protected: // selected
	bool	selected;
public: // selected
	bool	getSelected()	{ return selected;			}
	void	select()		{ changeSelected(true);		}
	void	unselect()		{ changeSelected(false);	}
	void	changeSelected(bool Selected) { selected = Selected; }

public: // property project writing functions
	String	FileIO_getPropertyProjectLine_v1();
	bool	FileIO_UsePropertyProjectLine_v1(String& Line, Property::PropertyProject1_FormatData* = 0);

	// loading from propdump
	bool	FileIO_UsePropdump_v2			(String Line);
	bool	FileIO_UsePropdump_v3			(String Line);
	bool	FileIO_UsePropdump_v4			(String Line);
	String	FileIO_getPropdump_v2			(void);
	String	FileIO_getPropdump_v3			(void);
	String	FileIO_getPropdump_v4			(void);
	String	FileIO_getPropdump				(int Version);
	String	getVariantPropdump				(int Version);


public: // additional status flags
	bool	build_limbo;

public: // constructor
	ObjectData() 
	{ 
		build_limbo = false; 
		build_state = BS_NONE;
		selected	= false;
	}

public: // debug information
	String	getModelDescriptor();
	String	getDetails();
};