#pragma once
#include <vector>
#include "aw.h"

namespace AW
{
	class Bot;

	class AttributeMapping
	{
	public:
		int		attribute_id;
		String	name;
		int		var_type;
		String	text;
	};

	class AttributeMap
	{
	public:	
		AttributeMap(void);

		// tracking variables, m_pSourceBot is the reading source, m_Map contains the array
		Bot*				m_pSourceBot;
		AttributeMapping	m_items[AW_MAX_ATTRIBUTE];
		void				UpdateAll();

	public:
		String	AttributeToString(int ID);
	};



	class EventToStringMapping
	{
	public:
		int			event_id;
		String		name;
		UINT_PTR	uPtr;
	};

	class EventToStringMap
	{
	public:
		EventToStringMap();
		EventToStringMapping	m_items[AW_MAX_EVENT];
	};
}