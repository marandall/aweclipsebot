#pragma once
#include "citizenlookup_kernel.h"
#include "cis_exec.h"

namespace CIS
{
	class CitizenLookupCompleter : public CitizenLookup::LookupBase
	{
	public: // the delayed mechanism
		Exec::IExec*	pExec;

	public: // completion
		CitizenLookupCompleter(Exec::IExec* E)
		{
			pExec = E;
		}

	public: // accessor result
		void Result(AW::Citizen& Info, int Reason)
		{
			// set the data upon it
			pExec->resumption_args["citinfo"]	= Info.getUmbraRemoteClass();
			pExec->resumption_args["rc"]		= Reason;
		
			// continue
			pExec->runResumption();
			delete pExec;
		}
	};

	class CitizenGroupLookupCompleter : public CitizenLookup::GroupLookupBase
	{
	public: // the delayed mechanism
		Exec::IExec*	pExec;
		
	public: // completion
		CitizenGroupLookupCompleter(Exec::IExec* E, CitizenLookup::Kernel* K) : GroupLookupBase(K)
		{
			pExec = E;
		}

	public: // accessor result
		void onComplete()
		{
			// set the data upon it
			pExec->resumption_args["results"]	= getUmbraRemoteClass();
		
			// continue
			pExec->runResumption();
			if (delete_children)
				delete pExec;
		}
	};
}