#include "stdafx.h"				// for precompiled header
#include "Avatar.h"				// for avatar class
#include "FileLoader.h"
#include "Helpers_Time.h"

// memory allocation checking
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

long Avatar::Eject(const long Duration, CStringA MessageText, CStringA LogText, Avatar *Caller)
{
	/* variables - the mesage */
	String	err_msg, logText;
	int		rc	= 0;

	/* there are several conditions that could cause this event to fail
		these include not having eject (which should be checked by the calling function)
		or the target being invunerable		*/
	if (plc_bot->HasEject() == false) 
	{
		logText.Format("Ejection of %s [%s] has been rejected - Insufficent Rights", getName(), getCID(), Duration);   	
		WriteMsg(logText, RGB_EJECT, OT_DEBUG | OT_BOTH | OM_TIME);
		return -1;
	}
	else if (getPrivilege() == 1) 
	{
		logText.Format("Ejection of %s [%s] has been rejected - Target Invunerable due to Root", getName(), getCID(), Duration);   	
		WriteMsg(logText, RGB_EJECT, OT_DEBUG | OT_BOTH | OM_TIME);
		return -1;
	}

	/* do we have any data with which to send the target an alert */
	if (MessageText.GetLength() > 0) { 
		logText.Format("You have been ejected for: %s", MessageText);
		ConsoleWhisper(logText, RGB_RED, CM_BOLD);
	}

	/* ejects a user from the world - this is only valid if the user does not have AWLD */
	/* Universe CT, World CT or any other superprivileged access within the world		*/
	AW::Avatar::Eject(Duration);
		
	/* write a log to the main window */
	logText.Format("I have ejected %s [%s] for %s", getName(), getCID(), Helpers::Time::SecondsToTimeStr(Duration));   	
	WriteMsg(logText, RGB_EJECT, OT_BOTH | OM_TIME);

	/* save the ejection parameters */
	String log_path;
	log_path.Format("%s\\ejects.txt", getProfilePath());

	/* create extensive log to write to file */
	logText.Format	    ("------------- Eject Report -------------\n");
	logText.AppendFormat("Date:      %s\n",				getTime("%d %m %Y, %H:%M:%S"));
	logText.AppendFormat("Target Information [\n");
	logText.AppendFormat("  Target:    %s #%i\n",		getName(), getSession());
	logText.AppendFormat("  Citizen:   %s\n",			getCID());
	logText.AppendFormat("  Address:   %s\n",			getAddressInfo()); 
	logText.AppendFormat("  Location:  %s, %s\n",		plc_bot->getWorld(), getCoordinates());
	logText.AppendFormat("]\n");
	logText.AppendFormat("Details Given:\n");
	logText.AppendFormat("  Duration:  %s\n",			Helpers::Time::SecondsToTimeStr(Duration));
	logText.AppendFormat("  Reason:    %s\n",			LogText);
	logText.AppendFormat("  Msg Given: %s\n",			MessageText);
	logText.AppendFormat("]\n");
	
	/* add extra information if ejector known */
	if (Caller != NULL) {
		logText.AppendFormat("Ejector Identification [\n");
		logText.AppendFormat("  Name:      %s #%i\n",		Caller->getName(), Caller->getSession());
		logText.AppendFormat("  Citizen:   %s\n",			Caller->getCID());
		logText.AppendFormat("  Address:   %s\n",			Caller->getAddressInfo()); 
	} else {
		logText.AppendFormat("Ejector Identification:\n");
		logText.AppendFormat("  Name:      [%s] #%i (self)\n",	bot.getName(), plc_bot->_session());
	}
	logText.AppendFormat("]\n\n");

	/*  write output */
	FileWriter writer(log_path, "a");
	if (writer.IsOK())
		writer.WriteLine(logText);
	else {
		logText.Format("@Error:\tUnable to write eject log: Reason: %d", (int)errno);
		WriteMsg(logText, RGB_RED, OT_DEBUG | OT_ERROR | OT_TECH | MF_FBOLD); 
	}

	/* post the eject message to users */
	logText.Format("EJECT_REPORT: %s %s (%s) has been ejected for %s by %s (%s)", 
		getName(), getCID(), getAddress(), Helpers::Time::SecondsToTimeStr(Duration), 
		Caller ? Caller->getName() : "sys()", LogText);

	// for each user
	for (size_t i = 0; i < avatars.size(); i++)
		if (avatars[i].GetRight("eject/info"))
			avatars[i].ConsoleWhisper(logText, 0xFF, CM_SUPPRESSED);

	/* return the reason code */
	return rc;
}