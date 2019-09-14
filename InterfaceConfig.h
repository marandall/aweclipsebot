#pragma once

#include "InterfaceConfig_ChatDisplay_AG.h"
#include "InterfaceConfig_Logging_AG.h"

namespace InterfaceConfig
{
	// template classes derived from the data
	class ChatDisplay : public AutoTemplates::ChatDisplayData 
	{ 
	public:
		String getLogicalChatPrefix()
		{
			if (getPrefixChatWithTime() == false) return "";
			CTime tm = time64(0);
			return tm.Format("[%H:%M:%S] ");
		}

		String getLogicalConsolePrefix()
		{
			if (getPrefixConsolesWithTime() == false) return "";
			CTime tm = time64(0);
			return tm.Format("[%H:%M:%S] ");
		}
	};
	
	class Logging : public AutoTemplates::LoggingData 
	{ 
	};
	

	class AppConfig : 
		public Umbra::RemoteClass,
		public IConfigLoader
	{
	protected:
		ChatDisplay			chat_display;
		Logging				logging;
		
	public: // accessor methods for the display
		ChatDisplay&		getChatDisplay()		{ return chat_display;			}
		Logging&			getLogging()			{ return logging;				}
		
	public: // file IO
		inline void Load(Serializer& Sz, const char* IniKey = NULL)
		{
			chat_display.Load(Sz);
			logging.Load(Sz);
		}

		inline void Save(Serializer& Sz, const char* IniKey = NULL)
		{
			chat_display.Save(Sz);
			logging.Save(Sz);
		}
	};
}

// setup
extern InterfaceConfig::AppConfig g_InterfaceConfig;