#pragma once

/* a forward reference to the zone */
namespace Zones
{
	class Zone;

	class ZoneConnector
	{
	public:
		ZoneConnector(void) { m_pThisZone = NULL; }
		~ZoneConnector(void);

	protected: 
		Zone*	m_pThisZone;

	public:
		bool	ZoneModifyPos	(const AW::Location& position);
		Zone*	getZone		(void) const	{ return m_pThisZone; }
		void	ZoneReset		(void)			{ m_pThisZone = NULL; }
		String	getZoneId		(void) const;
		String	getZoneDescriptor(const char* seps) const;
		void	ClearZone		(void);

	public:
		virtual void onEventZoneChanged(Zone* pOld, 
										 Zone* pNew) { ; }
		virtual void onEventZoneReset	(void)		 { ; }
	};
}