#include "stdafx.h"				// for precompiled header
#include "Avatar.h"				// for avatar class
#include "helpers_xml.h"			// xml writing

// memory allocation checking
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* string lookup abilities */
String Avatar::getCID(void) const
{
	/* DEFINE: variables for in function */
	String name_extension, text;
	if (getPrivLUS() == LookupState::Complete)
		name_extension.Format(" - %s", m_priv_name);	
	
	/* SITUATION: User is a tourist */
	if (m_citizen == 0 && m_privilege == 0)	
		return "@tourist";
	else if (m_citizen == m_privilege) 
		text.Format("Citizen: %d", getCitizen());
	else if (m_citizen == 0) 
		text.Format("Privilege: %d%s", m_privilege, name_extension);	
	else 
		text.Format("Citizen: %d, Privilege: %d%s", m_citizen, m_privilege, name_extension);
	return text;
}

String Avatar::getAddressInfo(void) const
{
	/* variables used */
	if (getAddressLUS() == LookupState::Complete)
	{
		String text;
		text.Format("addr: %s", m_address);
		/* do we also need to include the dns? */
		if (getDnsLUS() == LookupState::Complete)
			text.AppendFormat(", dns: %s", getDns());
		return text;
	} 
	else
		return "Unavailable";
}

String Avatar::GetBestLock(void) const
{
	String format;
	if (IsBot())
	{
		String bot_name = getName();
		bot_name.Replace("[", "");
		bot_name.Replace("]", "");
		format.Format("%d-bot-%s", getPrivilege(), bot_name);
	}
	else if (IsCitizen())
	{
		format.Format("%d", getCitizen());
	}
	else
	{
		String tourist_name = getName();
		tourist_name.Replace("\"", "");
		tourist_name.Replace("\"", "");
		format.Format("tourist-%s", tourist_name);
	}
	return format;
}

String Avatar::UserStrings(String str)
{
	return "";
}

String Avatar::getFileKey(void) const
{
	String format;
	if (IsBot())
	{
		String bot_name = getName();
		bot_name.Replace("[", "");
		bot_name.Replace("]", "");
		format.Format("%d-bot-%s", getPrivilege(), bot_name);
	}
	else if (IsCitizen())
	{
		format.Format("%d", getCitizen());
	}
	else
	{
		String tourist_name = getName();
		tourist_name.Replace("\"", "");
		tourist_name.Replace("\"", "");
		format.Format("tourist-%s", tourist_name);
	}

	return getProfilePath() + CStringA("\\user_profiles\\") + format + ".eni";
}

String Avatar::getOutputReport(bool full, bool eject, bool self) const
{
	/* variable used for formatting */
	String result, local_name, local_pers ;
	
	/* we modify the code slightly depending on who says it */
	if (self) 
	{
		local_name	= "You";
		local_pers	= "are";
	}
	else 
	{
		local_name	= m_name;
		local_pers	= "is";
	}

	/* we test each possible condition */
	if (full && eject) 
	{
		result.Format("%s #%d with [%s] (%s) build: %d (%s) avatar (%d, %d, %s) %s located at %s",
			local_name,	getSession(), getCID(),	getAddressInfo(),
			getBuild(), getBuildDescription(), getAvatar(), getGesture(), getStateName(),
			local_pers, getCoordinates());
	} 
	else if (full) 
	{
		result.Format("%s #%d with [%s] build: %d (%s) avatar (%d, %d, %s) %s located at %s",
			local_name,	getSession(), getCID(),
			getBuild(),	getBuildDescription(), getAvatar(), getGesture(), getStateName(),
			local_pers, getCoordinates());
	} 
	else 
	{
		result.Format("%s #%d %s located at %s",
			local_name,	getSession(), local_pers, getCoordinates());
	}

	/* return our result object */
	return result;
}

String Avatar::getStateName() const
{
	return "";
}

bool Avatar::IsPattern(String Pattern, bool IpAuthorised) const
{
	// first check the default patterns
	bool b_std = AvatarData::IsPattern(Pattern, IpAuthorised);
	if (b_std == true) 
		return true;

	// is one of the group
	if (Pattern.Left(6) == "group:")
	{
		return GetRight(String("groups/") + Pattern.Mid(6));
	}

	// check against rights
	if (Pattern.Left(4) == "drs:") return GetRight(Pattern.Mid(4));

	// check against world rights
	if (Pattern == "eject()")		return HasEject();
	else if (Pattern == "ps()")		return HasPS();
	else if (Pattern == "build()")	return HasBuild();

	// no joy
	return false;
}

String Avatar::getXML_Type1(void) const
{
	Helpers::XML::DataKeyset xml_writer;
	
	xml_writer.InsertKey("session",		m_session);
	xml_writer.InsertKey("name",		m_name);
	xml_writer.InsertKey("citizen",		m_citizen);
	xml_writer.InsertKey("privilege",	m_privilege);
	xml_writer.InsertKey("privname",	m_priv_name);
	xml_writer.InsertKey("ip",			getAddress());
	xml_writer.InsertKey("dns",			getDns());
	xml_writer.InsertKey("coordinates",	getCoordinates());
	xml_writer.InsertKey("avatar",		getAvatar());
	xml_writer.InsertKey("cells",		getCellCoordinates());


	return xml_writer.ToSingleXML("avatar");
}

BOOL Avatar::db_WriteEnter()
{
	ADODB::SQLBuilder sql_b;
	sql_b.Insert("user_name",		m_name);
	sql_b.Insert("session_id",		m_session);
	sql_b.Insert("enter_time",		(int)time64(0));
	sql_b.Insert("enter_coords",	getCellCoordinates());
	sql_b.Insert("citizen",			m_citizen);
	sql_b.Insert("privilege",		m_privilege);
	sql_b.Insert("privilege_name",	m_priv_name);
	sql_b.Insert("browser_build",	m_build);
	sql_b.Insert("world",			plc_bot->getWorld());
	sql_b.Insert("world_instance",	plc_bot->_string(AW_WORLD_NAME));

	// execute sql
	String sql = sql_b.CreateInsert("local_users");
	BOOL state = g_Database.Execute(sql);

	// query single data
	ADODB::Recordset rs;
	g_Database.Query(rs, "SELECT @@IDENTITY as IDENT");
	uuid_l = atoi(rs["IDENT"]);
	rs.Close();
	return TRUE;
}

BOOL Avatar::db_WriteUpdate()
{
	ADODB::SQLBuilder sql_b;
	sql_b.Insert("privilege_name",	m_priv_name);
	sql_b.Insert("ip",				m_address);
	sql_b.Insert("dns",				m_dns);

	// execute sql
	String cmdText;
	cmdText.Format("WHERE user_id=%d", uuid_l);
	return g_Database.Execute(sql_b.CreateUpdate("local_users", cmdText));
}

BOOL Avatar::db_WriteExit()
{
	ADODB::SQLBuilder sql_b;		
	sql_b.Insert("privilege_name",	m_priv_name);
	sql_b.Insert("ip",				m_address);
	sql_b.Insert("dns",				m_dns);
	sql_b.Insert("active_ratio",	1);
	sql_b.Insert("exit_time",		(int)time64(0));;
	sql_b.Insert("exit_coords",		getCellCoordinates());

	// execute sql
	String cmdText;
	cmdText.Format("WHERE user_id=%d", uuid_l);
	return g_Database.Execute(sql_b.CreateUpdate("local_users", cmdText));
}