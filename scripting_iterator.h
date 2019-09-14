#pragma once
#include "umbra/umbra_remoteclass.h"
#include <vector>

namespace Umbra
{
	class IIteratorSignalReceiver;

	class IIteratorSignalReceiver
	{
	public: // on the notify
		virtual void onIteratorInvalidated() = 0;
		virtual void onIteratorSignalDestroyed() = 0;
	};


	class IteratorSignalAgent
	{
	public: // the iterators
		std::vector<IIteratorSignalReceiver*> links;

	public: // register
		void connect(IIteratorSignalReceiver* Isr)
		{
			links.push_back(Isr);
		}

		void disconnect(IIteratorSignalReceiver* Isr)
		{
			// remove the signal agent
			for (int i = (int)(links.size() - 1); i >= 0; i--)
			{
				if (links[i] == Isr)
					links.erase(links.begin() + i);
			}
		}

	public: // handling
		void signalInvalidated()
		{
			// remove the signal agent
			for (int i = (int)(links.size() - 1); i >= 0; i--)
			{
				links[i]->onIteratorInvalidated();
			}
		}

	public: // destroy
		virtual ~IteratorSignalAgent()
		{
			for (int i = (int)(links.size() - 1); i >= 0; i--)
			{
				links[i]->onIteratorSignalDestroyed();
				links.erase(links.begin() + i);
			}
		}
	};

	template<class C>
	class CodeIterator : public IIteratorSignalReceiver, public Umbra::RemoteClass
	{
	public: // the class to be monitored
		Umbra::Variant				container;
		C											eof_iterator;
		C											cur_iterator;
		C											active_iterator;
		IteratorSignalAgent*	p_isr;

	public: // constructor style
		CodeIterator(RemoteClass& Remote, IteratorSignalAgent& SigAgent, C Begin, C End)
		{
			container		= Remote.getUmbraRemoteClass();
			cur_iterator	= Begin;
			eof_iterator	= End;

			// connect to signal agent
			SigAgent.connect(this);
			p_isr = &SigAgent;
		}

		virtual ~CodeIterator()
		{
			if (p_isr != 0)
				p_isr->disconnect(this);
		}

		void onIteratorInvalidated()
		{
			cur_iterator = eof_iterator;
		}

		void onIteratorSignalDestroyed()
		{
			p_isr = 0;
		}

	public: // umbra interface
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
		{
			//
			// process the next iterator
			//

			UMBRA_FUNCTION_BEGIN(read) {
				// code information break
				UMBRA_FUNCTION_CS() {
					if (cur_iterator == eof_iterator || p_isr == 0)
					{
						UMBRA_FUNCTION_RET( Umbra::Variant() );
					}
					else
					{
						Umbra::Variant v = (*cur_iterator).getUmbraRemoteClass();
						active_iterator = cur_iterator;
						cur_iterator++;
						UMBRA_FUNCTION_RET( v );
					}
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();


			UMBRA_FUNCTION_BEGIN(next) {
				// code information break
				UMBRA_FUNCTION_CS() {
					if (cur_iterator == eof_iterator || p_isr == 0)
					{
						UMBRA_FUNCTION_RET( Umbra::Variant() );
					}
					else
					{
						Umbra::Variant v = (*cur_iterator).getUmbraRemoteClass();
						active_iterator = cur_iterator;
						cur_iterator++;
						UMBRA_FUNCTION_RET( v );
					}
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			return Umbra::FunctionRequest::NOT_HANDLED;
		}
	};
}