#include "stdafx.h"
#include "CitResolver_Host.h"
#include "CitResolver_Client.h"
#include "CitResolver_Lookup.h"
#include "AWSDK_Citizen.h"

namespace CitizenResolver
{

	/*
		is the host being looked up currently
	*/
	bool Host::IsQueryName(const char* CitizenName) const
	{
		for (size_t i = 0; i < list.size(); i++)
			if (list[i].name.CompareNoCase(CitizenName) == 0 && list[i].lookup_name == true)
				return true;
		return false;
	}

	bool Host::IsQueryNumber(int CitizenNumber) const 
	{
		for (size_t i = 0; i < list.size(); i++)
			if (list[i].citizen == CitizenNumber && list[i].lookup_name == false)
				return true;
		return false;
	}

	bool Host::LookupByNumber(int Citizen)
	{
		if (IsQueryNumber(Citizen))
			return false;
		Tracker tr;
		tr.citizen		= Citizen;
		tr.lookup_name	= false;
		tr.issued		= _time64(NULL);
		list.push_back(tr);
		bot._citizen_attributes_by_number(Citizen);
		return true;
	}
			
	bool Host::LookupByName(const char* CitizenName)
	{
		if (IsQueryName(CitizenName))
			return false;
		Tracker tr;
		tr.name			= CitizenName;
		tr.lookup_name	= true;
		tr.issued		= _time64(NULL);
		list.push_back(tr);
		bot._citizen_attributes_by_name(CitizenName);
		return true;
	}

	void Host::OnResult(AW::Bot& bot, int ReasonCode)
	{
		/* create a new citizenship based on this information */
		AW::Citizen citizen;
		citizen.FromSdk(bot);
		/* distribute out to every client */
		for (size_t i = 0; i < clients.size(); i++)
			clients[i]->LookupResult(ReasonCode, citizen);
		
		/* remove it from our list */
		for (int i = (int)list.size() - 1; i >= 0; i--) {
			if ((list[i].citizen == citizen.get_Citizen() && list[i].lookup_name == false) ||
				(list[i].name.CompareNoCase(citizen.get_Name()) == 0 && list[i].lookup_name == true))
			{
				list.erase(list.begin() + i);
			}
		}
	}

	void Host::Join(Client& Cli)
	{
		clients.push_back(&Cli);
		Cli.m_pPrimaryHost = this;
	}
	
	void Host::Leave(Client& Cli)
	{
		for (size_t i = 0; i < clients.size(); i++)
			if (clients[i] == &Cli){
				clients.erase(clients.begin() + i);
				return;
			}
	}

	void Host::OnDisconnect()
	{
		AW::Citizen citizen;
		for (size_t i = 0; i < clients.size(); i++)
			clients[i]->LookupResult(-1, citizen);
	}


	/* 
		Lookup by the various types - this accepts as many as fed to it
	*/
	bool Client::LookupByNumber(int Citizen, Async::DispatchFunc DispTo, void* OwnData)
	{
		/* cretae a lookup for a citizenship */
		Lookup lookup;
		Async::Header asyn; // and configure its return
		asyn.p_msg = OwnData;
		asyn.p_disp_to = DispTo;
		lookup.lookup_name = false;
		lookup.citizen = Citizen;
		lookup.hdr = asyn;
		list.push_back(lookup);
		m_pPrimaryHost->LookupByNumber(Citizen);
		return true;
	}

	bool Client::LookupByName(const char* CitizenName, Async::DispatchFunc DispTo, void* OwnData)
	{
		/* cretae a lookup for a citizenship */
		Lookup lookup;
		Async::Header asyn; // and configure its return
		asyn.p_msg = OwnData;
		asyn.p_disp_to = DispTo;
		lookup.lookup_name = true;
		lookup.name = CitizenName;
		lookup.hdr = asyn;
		list.push_back(lookup);
		m_pPrimaryHost->LookupByName(CitizenName);
		return true;
	}

	/*
		Handles the lookup of a user (if has been looked up) dispatches
	*/
	void Client::LookupResult(int ReasonCode, AW::Citizen& Citizen)
	{
		/* special case reason code - disconnected */
		if (ReasonCode == -1) {
			for (int i = (int)list.size() - 1; i >= 0; i--) {
				/* dispatch via function pointer */
				Async::Header asyHeader = list[i].hdr;
				asyHeader.p_result = (void*)&Citizen;
				asyHeader.u_rc = -1;
				(*asyHeader.p_disp_to)(asyHeader);
				list.erase(list.begin() + i);
			}
			return;
		}

		for (int i = (int)list.size() - 1; i >= 0; i--) {
			if ((list[i].citizen == Citizen.get_Citizen() && list[i].lookup_name == false) ||
				(list[i].name.CompareNoCase(Citizen.get_Name()) == 0 && list[i].lookup_name == true))
			{
				/* dispatch via function pointer */
				Async::Header asyHeader = list[i].hdr;
				asyHeader.p_result = (void*)&Citizen;
				asyHeader.u_rc = ReasonCode;
				(*asyHeader.p_disp_to)(asyHeader);
				list.erase(list.begin() + i);
			}
		}
	}
}