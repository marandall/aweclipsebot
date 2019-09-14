#pragma once

namespace CIS 
{
	class Request;
	class Response;

	namespace Exec 
	{
		// uul commands
		// ------------------------------
		// 
		// uul/query 
		// uul/inworld
		// 
		// 
		namespace UUL
		{
			int LookupHost	(const CIS::Request& _Req, CIS::Response& _Resp);
			int InWorld		(const CIS::Request& _Req, CIS::Response& _Resp);
			int RipEmails	(const CIS::Request& _Req, CIS::Response& _Resp);
			int Ratios		(const CIS::Request& _Req, CIS::Response& _Resp);
		}
	}
}