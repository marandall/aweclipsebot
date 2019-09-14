#pragma once

#include "umbra/umbra_remoteclass.h"
#include "umbra/umbra_variantmap.h"

class MessageInfo : public Umbra::RemoteClass
{
public:
	String		message;
	COLORREF	colour;
	bool		bold, italic, suppressed;
	int			mode;

public: // umbra interfaces
	bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
	{
		UMBRA_PROPERTY_RW(message,	  message);
		UMBRA_PROPERTY_RW(color,	  colour);
		UMBRA_PROPERTY_RW(bold,		  bold);
		UMBRA_PROPERTY_RW(italic,	  italic);
		UMBRA_PROPERTY_RW(suppressed, suppressed);
		UMBRA_PROPERTY_RW(moded,	  mode);

		// error
		return Umbra::PropertyRequest::NOT_HANDLED;
	}

	MessageInfo()
	{
		colour = 0x00;
		bold = italic = suppressed = false;
		mode = 0;
	}

	MessageInfo& operator=(const char* Data)
	{
		message = Data;
		return *this;
	}

public: /* read / write routines via ini*/
	void	Load(AW::EnhancedINI&, String Sect, String Prefix);
	void	Save(AW::EnhancedINI&, String Sect, String Prefix);

public: /* message dispatch */
	int		Execute(Avatar* TargetAvatar, Umbra::VariantMap& VMap);
};

bool MessageInfo_EditBox(MessageInfo&, CWnd*);
int  MessageInfo_Send(MessageInfo&, Avatar*, String Message);

inline bool umbra_property_exchangefunc(Umbra::Variant& V, MessageInfo& X, int Mode)
{
	if (Mode == Umbra::EXM_UPDATE_VARIANT)		V = X;
	else return false;
	return true;
}

namespace MessageMode
{
	enum 
	{
		Say,
		Whisper,
		Console,
		PlainConsole,
		Broadcast
	};
}