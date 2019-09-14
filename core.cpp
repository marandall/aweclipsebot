#include "stdafx.h"
#include "messages.h"
#include "messageinfodialog.h"
#include "globaldataexchange.h"

int MessageInfo_Send(MessageInfo& MI, Avatar* pAv, String Message)
{
	if (MI.mode == MessageMode::Say)
	{
		return bot.say(Message);
	}
	else if (MI.mode == MessageMode::Whisper)
	{
		return pAv->WhisperEx(Message);
	}
	else if (MI.mode == MessageMode::PlainConsole)
	{
		return pAv->ConsoleMessageEx(Message, MI.colour, Make_CMF(MI.bold, MI.italic, false)); 
	}
	else if (MI.mode == MessageMode::Console)
	{
		return pAv->ConsoleMessagePrefixed(bot.getName(), Message, MI.colour,  Make_CMF(MI.bold, MI.italic, false));
	}
	else if (MI.mode == MessageMode::Broadcast)
	{
		return bot.consoleBroadcast(Message, MI.colour, Make_CMF(MI.bold, MI.italic, false)); 
	}
	else
	{
		return -1;
	}
}

bool MessageInfo_EditBox(MessageInfo& Info, CWnd* Window)
{
	MessageInfoDialog dialog(Window);

	bool read = false;
	DEX_Exchange(read, Info.message,	dialog.v_Message);
	DEX_Exchange(read, Info.mode,		dialog.v_Style);
	DEX_Exchange(read, Info.colour,		dialog.c_Colour);
	DEX_Exchange(read, Info.bold,		dialog.v_Bold);
	DEX_Exchange(read, Info.italic,		dialog.v_Italic);
	DEX_Exchange(read, Info.suppressed, dialog.v_Suppress);
	if (dialog.DoModal() != IDOK)
		return false;
	
	/* copy data back */
	read = true;
	DEX_Exchange(read, Info.message,	dialog.v_Message);
	DEX_Exchange(read, Info.mode,		dialog.v_Style);
	DEX_Exchange(read, Info.colour,		dialog.c_Colour);
	DEX_Exchange(read, Info.bold,		dialog.v_Bold);
	DEX_Exchange(read, Info.italic,		dialog.v_Italic);
	DEX_Exchange(read, Info.suppressed, dialog.v_Suppress);
	return true;
}

void MessageInfo::Load(AW::EnhancedINI& IS, String Sect, String Prefix)
{
	bool loading = true;
	IS.Exchange(loading, Sect, Prefix + "_msg",		message);
	IS.Exchange(loading, Sect, Prefix + "_mode",	mode);
	IS.Exchange(loading, Sect, Prefix + "_colour",	colour);
	IS.Exchange(loading, Sect, Prefix + "_bold",	bold);
	IS.Exchange(loading, Sect, Prefix + "_italic",	italic);
	IS.Exchange(loading, Sect, Prefix + "_suppressed",	suppressed);
}

void MessageInfo::Save(AW::EnhancedINI& IS, String Sect, String Prefix)
{
	bool loading = false;
	IS.Exchange(loading, Sect, Prefix + "_msg",		message);
	IS.Exchange(loading, Sect, Prefix + "_mode",	mode);
	IS.Exchange(loading, Sect, Prefix + "_colour",	colour);
	IS.Exchange(loading, Sect, Prefix + "_bold",	bold);
	IS.Exchange(loading, Sect, Prefix + "_italic",	italic);
	IS.Exchange(loading, Sect, Prefix + "_suppressed",	suppressed);
}

int	MessageInfo::Execute(Avatar* pAv, Umbra::VariantMap& vMap)
{
	const char* pBuffer = (LPCSTR)message;
	String Message = vMap.parse_string(pBuffer).c_str();
	
	MessageInfo& MI = *this;
	if (MI.mode == MessageMode::Say)
	{
		return bot.say(Message);
	}
	else if (MI.mode == MessageMode::Whisper && pAv)
	{
		return pAv->WhisperEx(Message, MI.suppressed);
	}
	else if (MI.mode == MessageMode::PlainConsole && pAv && bot.HasCT())
	{
		return pAv->ConsoleMessageEx(Message, MI.colour, Make_CMF(MI.bold, MI.italic, MI.suppressed)); 
	}
	else if (MI.mode == MessageMode::Console && pAv && bot.HasCT())
	{
		return pAv->ConsoleMessagePrefixed(bot.getName(), Message, MI.colour,  Make_CMF(MI.bold, MI.italic, MI.suppressed));
	}
	else if (MI.mode == MessageMode::Broadcast && bot.HasCT())
	{
		return bot.consoleBroadcast(Message, MI.colour, Make_CMF(MI.bold, MI.italic, MI.suppressed)); 
	}
	else
	{
		return -1;
	}
}
