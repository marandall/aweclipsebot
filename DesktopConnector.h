#pragma once
#include "ProtocolMessage_Bot.h"

class DesktopConnector
{
protected: // target information
	HWND _targetHwnd;
	UINT _idMessage;

public: // targets
	HWND getTargetHwnd() { return _targetHwnd; }
	UINT getIdMessage() { return _idMessage; }

public: // register for items
	bool connect();
	bool send(int Type, ProtocolMessage& PM);
	bool broadcastAppLoad();
	bool broadcastAppShutdown();
	bool broadcastUpdate();

public: // processing
	void onMessage(COPYDATASTRUCT& Cds, HWND Source);

public: // instancing
	static DesktopConnector& getInst() { static DesktopConnector s; return s; }
};