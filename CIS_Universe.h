#pragma once

namespace CIS 
{
	class Request;
	class Response;

	namespace Exec 
	{
		namespace Universe
		{
			int LookupNumber (const CIS::Request& _Req, CIS::Response& _Resp);
			int LookupName   (const CIS::Request& _Req, CIS::Response& _Resp);
		}
	}
}