#include "stdafx.h"
#include "cis_interfaces.h"
#include "CIS_Movers.h"					// for ejection / ejnt commands
#include "movers.h"						// mover driver control interface
#include "movers_autodriver.h"

using namespace Movers;

/*

namespace CIS 
{
	namespace Exec 
	{
	/*
		int DriverCreateProcessor::process()
		{
			String logText;

			// split into id and file
			SplitString args(getFlagText(), ",", 2);
			if (args.size() != 2)
			{
				write("Insufficent arguments; expect [id], [file]");
				return Reason::InvalidArgument;
			}

			// arguments
			String	argId	= args[0]; argId.Trim();
			String	argFile	= args[1]; argFile.Trim();

			// check if a driver exists
			AutoDriverKernel& driverKernel = AutoDriverKernel::getInstance();
			if (driverKernel.getEntryByName(argId))
			{
				logText.Format("Error creating driver; %s already exists", argId);
				write(logText);
				Reason::BadArgument;
			}

			// convert files
			argFile.Replace("%world%", getWorldPath());
			argFile.Replace("%world_wp%", getWorldPath() + "\\waypoints");
			argFile.Replace("%shared_wp%", GetAppPath() + "\\data\\shared_waypoints");
			argFile.Replace("..", "");

			// create a new element
			AutoDriver& wpE = *driverKernel.createNew(argId);
			/*wpE.loaded_file = argFile;
			if (wpE.waypoints.LoadWaypoints(argFile) == false)
			{
				logText.Format("Driver created, status paused as unable to read waypoint file");
				write(logText);
				return Reason::Success;
			}
			else
			{
				// do we want it to start immediately?
				if (getFlag("-run"))
					wpE.wpDriver.Continue();

				logText.Format("Driver created for %s, status %s @ waypoint %d", 
					wpE.id, wpE.wpDriver.IsPaused() ? "paused" : "running", wpE.wpDriver.getCurLine());
				write(logText);
			}

			return Reason::Success;
		}

		int DriverDeleteProcessor::process()
		{
			// reference the automatic mover driver
			AutoDriverKernel& driverKernel = AutoDriverKernel::getInstance();
			String logText;


			// arguments
			String argId = getFlagText();

			//// look up
			
			WaypointEntry* pWpE = g_MoverDriver.getEntryByName(argId);
			if (pWpE == NULL)
			{
				logText.Format("Unable to delete driver; %s could not be found", argId);
				return Reason::BadArgument;
			}
			else
			{
				// delete it
				g_MoverDriver.Delete(argId);
				write("Mover driver has been deleted");
			}

			return Reason::Success;
		}

		int DriverPauseProcessor::process()
		{
			// reference the automatic mover driver
			AutoDriverKernel& driverKernel = AutoDriverKernel::getInstance();
			String logText;

			//// arguments
			//String argId = getFlagText();
			//AutoDriver* pWpE = driverKernel.getEntryByName(argId);

			//// find it or not
			//if (pWpE == NULL)
			//{
			//	logText.Format("Unable to pause driver; driver cannot be found");
			//	write(logText);
			//	return Reason::BadArgument;
			//}
			//else
			//{
			//	pWpE->wpDriver.Pause();
			//	logText.Format("Mover driver '%s' state; paused @ waypoint %d", argId, pWpE->wpDriver.getCurLine());
			//	write(logText);
			//}

			return Reason::Success;
		}

		int DriverGoProcessor::process()
		{
			// reference the automatic mover driver
			AutoDriverKernel& driverKernel = AutoDriverKernel::getInstance();
			String logText;

			//// arguments
			//String argId = getFlagText();
			//AutoDriver* pWpE = driverKernel.getEntryByName(argId);

			//// find it or not
			//if (pWpE == NULL)
			//{
			//	logText.Format("Unable to resume driver; driver cannot be found");
			//	write(logText);
			//	return Reason::BadArgument;
			//}
			//else
			//{
			//	pWpE->wpDriver.Continue();
			//	logText.Format("Mover driver '%s' state; running @ waypoint %d", argId, pWpE->wpDriver.getCurLine());
			//	write(logText);
			//}

			return Reason::Success;
		}

		int DriverReloadProcessor::process()
		{
			// reference the automatic mover driver
			AutoDriverKernel& driverKernel = AutoDriverKernel::getInstance();
			String logText;

			// arguments
			String argId = getFlagText();
			AutoDriver* pWpE = driverKernel.getEntryByName(argId);

			//// find it or not
			//if (pWpE == NULL)
			//{
			//	logText.Format("Unable to reload driver; driver cannot be found");
			//	write(logText);
			//	return Reason::BadArgument;
			//}
			//else
			//{
			//	pWpE->wpDriver.LoadWaypoints(pWpE->loaded_file);
			//	pWpE->wpDriver.reset();
			//	pWpE->wpDriver.Pause();
			//	logText.Format("Mover driver '%s' reloaded; paused @ waypoint 0", argId);
			//	write(logText);
			//}

			return Reason::Success;
		}

		int DriverListProcessor::process()
		{
			// find the waypoint
			// WaypointEntry* pWpE = g_MoverDriver.getEntryByName(getFlagText());
			return Reason::Success;
		}
	}
}

*/