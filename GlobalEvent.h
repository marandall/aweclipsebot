#pragma once

/*
	Function callback async object
*/

namespace Async
{
	class Header;
	typedef void(*DispatchFunc)(Header&);

	class Header
	{
	public:
		unsigned int	u_msg;			// identifier of message
		void*			p_src;			// usually a EclipseBot
		int				u_rc;			// reason code
		void*			p_result;		// the result info, needs freeing
		void*			p_msg;			// extra data for it, needs freeing
		
		bool			svr_erase_msg;	// erase result?
		bool			b_emsg;			// erase message?
		DispatchFunc	p_disp_to;		// dispatch to location

	public:
		Header() { ; }
		Header(DispatchFunc DispFunc, void* pData = NULL)
		{
			p_disp_to = DispFunc;
			p_msg     = pData;
		}
	};
}