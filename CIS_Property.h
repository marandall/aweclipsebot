#pragma once

namespace CIS 
{
	class Request;
	class Response;

	namespace Exec 
	{
		namespace Property
		{
			int Query5x5	(const CIS::Request& _Req, CIS::Response& _Resp);		// ^query5x5 <location>

			// seeding abilities
			int Seed		(const CIS::Request& _Req, CIS::Response& _Resp);
		}
	}
}