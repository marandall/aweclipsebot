#include "stdafx.h"				// for precompiled header
#include "EclipseBot.h"			// for EclipseBot
#include "awsdk_query.h"		// querying
#include "awsdk_objects_v3.h"	// object data

// property project information
#include "property_objects.h"
#include "property_projects.h"
#include "property_async_command.h"
#include "property_query_5x5.h"
#include "property_query_queue.h"
#include "property_query_world.h"
#include "MiniParser.h"
#include "InterfaceConfig.h"
#include "callbackqueue.h"
#include "scripting_host.h"

// for editing
#include "avataroptions.h"
#include "property_editors.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

Querying::IQuery* EclipseBot::getCurrentQuery()
{
	if (m_query_mode == CommonBot::QUERY_LARGE_AREA)
		return query_multiqueue;
	else if (m_query_mode == CommonBot::QUERY_BACKUP)
		return query_world;
	else
		return &getQuery5x5();
}

Querying::Query_5x5& EclipseBot::getQuery5x5()
{
	if (m_query_5x5) return *m_query_5x5;
	else 
	{
		m_query_5x5 = new Querying::Query_5x5();
		m_query_5x5->SetBot(*this);
		m_query_5x5->setObjects(getLiveCache());
		return *m_query_5x5;
	}
}

Property::Objects& EclipseBot::getLiveCache()
{
	if (m_live_cache) return *m_live_cache;

	// create a new project
	m_live_cache = &Property::g_Projects.CreateProject("__live")->objects;
	if (m_live_cache == NULL)
		throw std::exception();
	return *m_live_cache;
}

int EclipseBot::Query5x5(int X, int Z)
{
	// change to given query mode
	setQueryMode(CommonBot::QUERY_5x5);

	// ensure cache and data is valid
	getLiveCache();
	getQuery5x5().SetBot(*this);
	return getQuery5x5().Query(X, Z);
}

int EclipseBot::QueryWorld()
{
	// change to given query mode
	setQueryMode(CommonBot::QUERY_WORLD);

	// ensure cache and data is valid
	getLiveCache().clear_trackers();
	getLiveCache().empty_project();
	
	// set first cell and use iterator mode
	_int_set(AW_CELL_ITERATOR, 0);
	_bool_set(AW_CELL_COMBINE, TRUE);
	return _cell_next();
}

int	EclipseBot::queryLargeArea(AW::Coordinates NW, AW::Coordinates SE)
{
	setQueryMode(CommonBot::QUERY_LARGE_AREA);

	// set the query to begin
	return query_multiqueue->queryBetween(NW, SE);
}

int EclipseBot::queryLargeAreaAround(AW::Coordinates Mid, int Width, int Height)
{
	setQueryMode(CommonBot::QUERY_LARGE_AREA);
	return query_multiqueue->queryAround(Mid, Width, Height);
}

int	EclipseBot::queryBackup(int Format, CString File)
{
	setQueryMode(CommonBot::QUERY_BACKUP);

	// query the range
	query_world->save_to		= File;
	query_world->save_format	= Format;
	return query_world->start();

}

int EclipseBot::QueryEnd()
{
	return 0;
}

/* events triggered by objects and queries */
void EclipseBot::onEventCellBegin(void)
{
	getCurrentQuery()->handleCellBegin();
}

void EclipseBot::onEventCellEnd(void)
{
	getCurrentQuery()->handleCellEnd();
}

void EclipseBot::onEventCellObject(void)
{
	// read the object data
	ObjectData objData;
	objData.from_sdk(*this);

	// cache handlign
	getCurrentQuery()->handleCellObject(objData);
}

void EclipseBot::onCallbackQuery(int ReasonCode)
{
	getCurrentQuery()->handleCallbackQuery(ReasonCode);
}

void EclipseBot::onCallbackCellResult(int ReasonCode)
{
	getCurrentQuery()->handleCallbackCell(ReasonCode);
}

void EclipseBot::onCallbackObjectResult(int ReasonCode)
{
	bot.getCQObjectResult().handle_result(ReasonCode);
}

void EclipseBot::onEventObjectAdd(void)
{
	// read the object data
	ObjectData objData;
	objData.from_sdk(*this);

	if (getQueryMode() == CommonBot::QUERY_5x5 || getQueryMode() == CommonBot::QUERY_WORLD)
	{
		// post it to cache
		getLiveCache().insert_object(objData);
	}

	// object class
	Avatar* pUser = avatars.getSession(_int(AW_OBJECT_SESSION));
	if (pUser == 0) return;

	// hit check
	pUser->updateLastObjectTime();

	// scripting host
	ScriptingHost::getInst().doFunction("EventObjectAdd", 
		&Umbra::VariantArray(objData, *pUser), 0, this);
}

void EclipseBot::onEventObjectDelete(void)
{
	ObjectData* pPrevObject = getLiveCache().find_object_ptr(_int(AW_OBJECT_NUMBER), _int(AW_CELL_X), _int(AW_CELL_Z));

	// object class
	Avatar* pUser = avatars.getSession(_int(AW_OBJECT_SESSION));
	if (pUser == 0) return;

	// hit check
	pUser->updateLastObjectTime();

	// deleting cached
	if (pPrevObject)
	{
		// scripting host
		ScriptingHost::getInst().doFunction("EventObjectDelete", 
			&Umbra::VariantArray(*pPrevObject, *pUser), 0, this);
	}

	// if we are in live update mode, remove it
	if (getQueryMode() == CommonBot::QUERY_5x5)
		getLiveCache().remove_object(_int(AW_OBJECT_NUMBER), _int(AW_CELL_X), _int(AW_CELL_Z));
}

String getBotSection(String Input)
{
	SplitString sp(Input, ";", -1);
	for (size_t i = 0; i < sp.size(); i++)
	{
		String t1 = sp[i]; t1.Trim();
		if (t1.Left(4) == "bot ")
			return t1.Mid(4);
	}
	return "";
}

class ClickmodeDeleteResult : public CallbackQueue::Target
{
public: // umbra class
	Umbra::Variant avatar;
	ObjectData	   objData;	

public: // create object
	ClickmodeDeleteResult(Avatar* A, ObjectData& D)
	{
		avatar	= A->getUmbraRemoteClass();
		objData	= D;
	}

	void handle_result(EclipseBot& Client, int Result)
	{
		if (Result != 0)
		{
			String temp;
			Avatar* pAv = dynamic_cast<Avatar*>(avatar.toRemoteClass());
			if (pAv)
			{
				temp.Format("Unable to edit '%s', reason %d", objData.getModel(), Result);
				pAv->ConsoleWhisper(temp, 0xFF, CM_SUPPRESSED);
			}
		}
	}
};

void EclipseBot::onEventObjectClick(void)
{
	String userName = _string(AW_AVATAR_NAME);

	/* find avatar */
	Avatar* pUser = avatars.getSession(_int(AW_AVATAR_SESSION));
	if (pUser == NULL)
		return;

	/* give pos */
	ObjectData objData;
	objData.from_sdk(*this);
	
	// clickmode
	AvatarOptions& avOpts = pUser->getOptions();
	if (avOpts.getClickMode() == AvatarOptions::CLICKMODE_DELETE)
	{
		// this privilege or ed
		if (pUser->HasEminent() || objData.getOwner() == pUser->getPrivilege())
		{
			object_delete( objData, new ClickmodeDeleteResult(pUser, objData) );
		}
	}
	else if (avOpts.getClickMode() == AvatarOptions::CLICKMODE_TELLPOS)
	{
		String s;
		s.Format("The object is located at %s", objData.getCoordinates());
		pUser->WhisperEx(s, true);
	}
	else if (avOpts.getClickMode() == AvatarOptions::CLICKMODE_EDITOR)
	{
		if (pUser->HasEminent() || objData.getOwner() == pUser->getPrivilege())
		{
			ObjectData od = objData;
			if (avOpts.getClickModeEditor()->run(od) == false)
				return;

			// copy
			object_change2(objData, od, new ClickmodeDeleteResult(pUser, objData));
		}
	}

	// scripting host
	ScriptingHost::getInst().doFunction("EventObjectClick", 
		&Umbra::VariantArray(*pUser, objData), 0, this);

}