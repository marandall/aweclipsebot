#include "stdafx.h"
#include "IpToDNS.h"
#include "EclipseEvolutionDialog.h"

namespace IpToDNS
{
	Host g_Host;

	Lookup* Host::Search(HANDLE Handle)
	{
		for (size_t i = 0; i < list.size(); i++)
			if (list[i].handle = Handle)
				return &list[i];
		return NULL;
	}
	
	Lookup* Host::Search(unsigned int Ip)
	{
		for (size_t i = 0; i < list.size(); i++)
			if (list[i].ip_addr = Ip)
				return &list[i];
		return NULL;
	}

	void Host::CacheItem(unsigned int IP, String DNS)
	{
		/* find existing lookups */
		for (size_t i = 0; i < cache.size(); i++) {
			if (cache[i].ip_addr = IP) {
				cache[i].dns = DNS;
				return;
			} }
		Cached item = { IP, DNS };
		cache.push_back(item);
	}


	bool Host::LookupNow(unsigned int ip_addr, Async::DispatchFunc DispTo, void* OwnData)
	{
		return false;

		/* check if already being resolved */
		bool is_searching = (Search(ip_addr) != NULL);
		/* create new item and store it */
		Async::Header header(DispTo, OwnData);
		header.p_disp_to = DispTo;
		header.p_msg = OwnData;
		Lookup lookup;
		lookup.header = header;
		lookup.issued = time64(0);

		/* allocate a buffer bug enough to handle host structure */
		char* pBuffer = new char[MAXGETHOSTSTRUCT];
		memset(pBuffer, 0, MAXGETHOSTSTRUCT);			// clear buffer
		in_addr iaHost;									// input host
		iaHost.s_addr = ip_addr;						// set the ip to lookup

		/* fire handle */
		HANDLE handle = WSAAsyncGetHostByAddr(rt_diag->m_hWnd,
			UWM_GETHOSTADDR_RESULT, (const char *)&iaHost,		// reply result
			sizeof(struct in_addr), AF_INET,					// lookup INET
			pBuffer, MAXGETHOSTSTRUCT);							// giving our buffer

		if (WSAGetLastError() || handle == NULL)
		{
			WriteMsg("Winsock output error", 0, OM_TIME | OT_DEBUG);
			delete pBuffer;
			return false;
		}

		/* handles a new event */
		lookup.handle = handle;
		lookup.buffer = pBuffer;
		list.push_back(lookup);
		return list.size() > 0;
	}

	void Host::OnResult(HANDLE ResultHandle, int ErrorCode)
	{
		/* go through every item to check, backwards */
		for (int i = (int)list.size() - 1; i >= 0; i--) {
			/* if this is not our handle, ignore it */
			if (list[i].handle != ResultHandle)
				continue;

			Lookup& lui = list[i];
			Async::Header hdr = lui.header;
			
			/* on error code send back error */
			if (ErrorCode) {
				hdr.p_result = NULL;
				hdr.u_rc  = ErrorCode;
				hdr.u_msg = lui.ip_addr;
			} else {
				
				HOSTENT* pHostEnt = (HOSTENT*)lui.buffer;
				hdr.p_result = (void*)pHostEnt;			// passes back host struct
				hdr.u_rc = 0;							// success
				unsigned int ip_ulong;
					
				if (pHostEnt->h_addr_list[0] != NULL)
				{
					memcpy(&ip_ulong, pHostEnt->h_addr_list[0], 4);
					String str = UnsignedIpToString(ip_ulong);
					hdr.u_msg = ip_ulong;				// int message is lookup ip
					// do not use caching at this time
					// CacheItem(ip_ulong, pHostEnt->h_name);
				}
			}

			/* execute the local call and delete */
			(*hdr.p_disp_to)(hdr);
			delete lui.buffer;
			list.erase(list.begin() + i);
		}
	}
}