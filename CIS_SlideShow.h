#pragma once

namespace CIS 
{
	class Request;
	class Response;

	namespace Exec 
	{
		namespace Slides
		{		
			/* Managing Slide Shows */
			int Create	(const CIS::Request& _Req, CIS::Response& _Resp);
			int Reset	(const CIS::Request& _Req, CIS::Response& _Resp);
			int Empty	(const CIS::Request& _Req, CIS::Response& _Resp);
			int Edit	(const CIS::Request& _Req, CIS::Response& _Resp);
			int Delete	(const CIS::Request& _Req, CIS::Response& _Resp);
			int Goto	(const CIS::Request& _Req, CIS::Response& _Resp);
			int List	(const CIS::Request& _Req, CIS::Response& _Resp);
		}
	}
}