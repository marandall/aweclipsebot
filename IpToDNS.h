#pragma once
#include <vector>
#include "GlobalEvent.h"

namespace IpToDNS
{
	class Lookup
	{
	public:
		HANDLE		  handle;
		unsigned int  ip_addr;
		__time64_t	  issued;
		Async::Header header;
		char*		  buffer;
	};

	class Cached
	{
	public:
		unsigned int	ip_addr;
		String			dns;
	};

	class Host
	{
	protected:
		std::vector<Lookup>	list;
		std::vector<Cached> cache;

		void	CacheItem(unsigned int IP, String DNS);
		String	getCached(unsigned int IP);
		Lookup* Search(HANDLE Handle);
		Lookup* Search(unsigned int Ip);
	
	public:
		bool LookupNow(unsigned int ip_addr, Async::DispatchFunc DispTo, void* OwnData = NULL);
		void OnResult(HANDLE ResultHandle, int ErrorCode);
	};

	extern Host g_Host;
}