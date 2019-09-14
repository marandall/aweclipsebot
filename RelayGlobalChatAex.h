#pragma once
#include "RelayGlobalChatAex_AG.h"		// auto generated data file

namespace ChatControllers
{
	// kernel class
	class GlobalChatAexPlugin : public AutoTemplates::GlobalChatAexData
	{
	protected: // this is our tracking user
		Avatar* m_pUser;
	public:
		GlobalChatAexPlugin(Avatar& Av) { m_pUser = &Av; }

	public: // overrides
		tsb_t		getEffectiveBold()		{ return session_bold	== B_EMPTY ? bold		: session_bold;				}
		tsb_t		getEffectiveItalic()	{ return session_italic == B_EMPTY ? italic	: session_italic;			}
		AW::Colour	getEffectiveColour()	{ return session_color.getColour() == 0xFFFFFF ? colour : session_color;	} 

	public: // debug
		String getDebugString() const;
	};
}