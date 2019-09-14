#include "stdafx.h"
#include "DesktopConnector.h"

bool DesktopConnector::connect()
{
	// register a desktop message
	_idMessage	= RegisterWindowMessage("ECLIPSEEVO2_IPC_CHANNEL");
	HWND hFound = FindWindowA(NULL, _T("Eclipse Desktop Monitor"));
	if (hFound)
	{
		_targetHwnd = hFound;
		return true;
	}
	else
	{
		_targetHwnd = 0;
		return false;
	}
}

void DesktopConnector::onMessage(COPYDATASTRUCT& Cds, HWND Source)
{
	// check is for this message we are handling
	if (Cds.dwData != _idMessage)
		return;

	// load vector check
	std::vector<char> bits(Cds.cbData, 0);
	memcpy(&bits[0], (void*)Cds.lpData, Cds.cbData);
	
	// unpack message
	ProtocolMessage pm;
	pm.unpack(bits, "EVOMSG");

	// type checking
	if (pm.getType() == MessageTypes::SERVER_ONLINE)
	{
		_targetHwnd = Source;
		broadcastAppLoad();
	}
}

bool DesktopConnector::send(int Type, ProtocolMessage& PM)
{
	COPYDATASTRUCT cd;
	std::vector<char> pd = PM.pack(Type, "EVOMSG");
	cd.cbData = pd.size();
	cd.lpData = &pd[0];
	cd.dwData	= _idMessage;

	if (_targetHwnd)
	{
		SendMessage(_targetHwnd, 
			WM_COPYDATA, 
			(WPARAM)(HWND)AfxGetApp()->GetMainWnd()->GetSafeHwnd(), 
			(LPARAM)(LPVOID)&cd);
		return true;
	}
	else
	{
		return false;
	}
}

bool DesktopConnector::broadcastAppLoad()
{
	ProtocolMessage pm;
	return send(MessageTypes::APPLICATION_STARTED, pm);
}

bool DesktopConnector::broadcastAppShutdown()
{
	ProtocolMessage pm;
	return send(MessageTypes::APPLICATION_SHUTTING_DOWN, pm);
}

bool DesktopConnector::broadcastUpdate()
{
	ProtocolMessage pm;
	pm.add(MessageTypes::UpdateData::PROFILE, "__PROFILE__");
	pm.add(MessageTypes::UpdateData::BOT_NAME, (const char*)bot.getName());
	pm.add(MessageTypes::UpdateData::WORLD,			(const char*)bot.getWorld());
	pm.add(MessageTypes::UpdateData::CONNECTION, (const char*)ToString(bot.getUniverseConnection()));
	return send(MessageTypes::UPDATES_INFO, pm);
}

