#pragma once

#ifdef _AWSDK_USE_UMBRA
#include "umbra/umbra_remoteclass.h"
#endif

/*****************************************************************************
**                                                                          **
**                                                                          **
**                              AW::Location                               **
**             Is the base representation of a point in 3D space            **
**                                                                          **
** The base location class acts as the base for every other class that      **
**   is explicitly existant within Active Worlds, this includes such things **
**   as objects, avatars and the very bot itself. All derrive from the      **
**   base location.                                                         **
**																			**
** The base location itself contains everything needed for the most         **
**   detailed position of all; the object. However to save space the        **
**   avatar pitch is aliased to the the object tilt.                        **
**                                                                          **
** ASCII and Wide versions are supported via CString conversion             **
**                                                                          **
** get properties are all declared as const to allow easy passsing and      **
**   to ensure that the objects are not modified, as are helper functions   **
**                                                                          **
** Design History:                                                          **
**    Initial Design:	Mark Randall, August 20, 2005                       **
**                                                                          **
**                                                                          **
*****************************************************************************/
namespace AW 
{
	class Bot;
	class Location

#ifdef _AWSDK_USE_UMBRA
		: public Umbra::RemoteClass
#endif

	{
	public:
		Location							(void)	{ m_x = m_y = m_z = m_yaw = m_pitch = m_roll = 0; };
		Location							(int x, int y, int z, int yaw);
		Location							(const Location& src);
		Location&	operator=				(const Location& src);
		bool		operator==				(const Location& src) const;
		virtual ~Location					(void){};

	public: // format styles
		static const int COORDS_FORMAT_FULL	= 0;
		static const int COORDS_FORMAT_XZ	= 1;

	// *****************************************************************************************
	//							  Support functions to get data 
	// *****************************************************************************************
	public:
		inline int		getX					(void)	const		{ return m_x;	  }
		inline int		getY					(void)	const		{ return m_y;	  }
		inline int		getZ					(void)	const		{ return m_z;	  }
		inline int		getYAW					(void)	const		{ return m_yaw;   }
		inline int		getYaw					(void) const		{ return m_yaw;	  }
		inline int		getPitch				(void)	const		{ return m_pitch; }
		inline int		getTilt					(void)	const		{ return m_pitch; }
		inline int		getRoll					(void)	const		{ return m_roll;  }
		float			GetRadianYAW			(void)	const;

		String getCoordinates			(int F = COORDS_FORMAT_FULL)	const;
		String getCellCoordinates		(void)							const;
		int getCellX(void) const;
		int getCellZ(void) const;
		int getCellIndex(void) const;
		int getSectorX(void) const;
		int getSectorZ(void) const;
		int getSectorIndex(void) const;
		double distanceTo(int x, int y, int z) const;
		double distanceTo(const Location& pt) const;
		double distanceToGD(int x, int z) const ;
		double distanceToGD(const Location& pt) const ;
		bool PointInRect(const Location& hi, const Location& lo) const;
		bool PointInRect_2D(const Location& hi, const Location& lo) const;
	
		Location		CalculateOffset			(int x, int y, int z, int yaw)	const;
		Location		CalculateOffset			(const Location& pt)			const;
		bool			InRect					(int ox, int oy, int oz, 
												int distance)					const;
		bool			InRect					(const Location& pt, 
												int distance)					const;
		int				AngleBetween			(int x, int z)					const;
		int				AngleBetween			(const Location& pt)			const;
		double			AngleBetweenR			(int x, int z)					const;
		double			AngleBetweenR			(const Location& pt)			const;
	
	protected:
		void			setPosition				(int x, int y, int z, int yaw);
	
	public:	
		Location		getThisLocation			() { return *this; }
		Location		getOffsetPosition		(int Distance, int Elevation, int Rel = 0);
		Location		toHighPoint					(Location& Other);
		Location		toLowPoint					(Location& Other);

	protected:
		int				m_x, m_y, m_z, m_yaw, m_pitch, m_roll;

	public:
		static int		CellFromPosition		(int position);

	public: // umbra interfaces
		virtual bool getAllowScriptedLocationChanges() { return true; }
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest&);
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest&);
	};

	/*****************************************************************************
	**                                                                          **
	**                                                                          **
	**                             AW::Coordinates                             **
	**        The editable coordinates resolving class used for translating		**
	**                 a string into a locational value in units                **
	**                                                                          **
	**                                                                          **
	** The base coordinates is an extension to the base location with the       **
	**   sole purpose of allowing coordinates to be resolved and translated     **
	**   easily from string or other formats.                                   **
	**                                                                          **
	** Typically the FromString function will be overridden in a derrived       **
	**   class to allow other formats to be entered, for example looking        **
	**   up an avatars position 'av:Mark Randall'                               **
	**                                                                          **
	** Design History:                                                          **
	**    Initial Design:	Mark Randall, August 20, 2005                       **
	**                                                                          **
	**                                                                          **
	*****************************************************************************/
	class Coordinates
		: public Location
	{
	public:
		Coordinates();
		virtual ~Coordinates();

	public: // additional world information
		String		m_world;

	public: // access the world only
		String		getWorld() const { return m_world; }

	public:
		bool		FromString(LPCSTR source);
		bool		FromStringW(LPCTSTR source);
		bool		FromLocation(AW::Location& location);
	};


	/* extended class provided since 5th september that 
	the 
	ability to set a location */
	class LocationEx :
		public Location
	{
	public:
		void		set							(int x, int y, int z, int yaw) { setPosition(x, y, z, yaw); }
		void		setX						(int x)	{ m_x = x; }
		void		setY						(int y)	{ m_y = y; }
		void		setZ						(int z)	{ m_z = z; }
		void		setYAW						(int yaw)	{ m_yaw = yaw; }
		void		setTilt						(int tilt)	{ m_pitch = tilt; }
		void		SetRoll						(int roll)	{ m_roll = roll; }
	};

	/*****************************************************************************
	**                                                                          **
	**                                                                          **
	**                                                                          **
	**                                                                          **
	**                                                                          **
	** ======================================================================== **
	**                                                                          **
	**                                                                          **
	**                    QUICK HELPER CLASS FOR CONVERSIONS					**
	**                                                                          **
	**                                                                          **
	** ======================================================================== a**
	**                                                                          **
	**                                                                          **
	**                                                                          **
	**                                                                          **
	**                                                                          **
	*****************************************************************************/
	#define AW_INT_TO_RADIAN_CONVERSION			0.00174532925
	class _XConvert {
	public:
		static CString long_to_ip_string(int ip) {
			String textString;
			textString.Format("%d.%d.%d.%d",ip & 0xFF, (BYTE)(ip >> 8),(BYTE)(ip>> 16),(BYTE)(ip>> 24));
			/* return relevant */
			return textString;
		}
		static float aw_angle_to_radians(int yaw) {
			return (float)(AW_INT_TO_RADIAN_CONVERSION * yaw);
		}
		static double aw_radians_to_angle(double rads) {
			return (rads / AW_INT_TO_RADIAN_CONVERSION);
		}
		static CString aw_universe_build_to_description(int build){
			return "4.1";
		}
	};
};

