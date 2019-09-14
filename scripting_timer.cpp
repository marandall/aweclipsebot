#include "stdafx.h"
#include "scripting_timer.h"
#include "scripting_host.h"

namespace ScriptingHost_ClassLib
{
	int D_Timer::insert(int Miliseconds, String Function, Umbra::Variant& Arg)
	{
		time_t ticks = (_time64(0) * 1000) + (GetTickCount() % 1000) + Miliseconds;

		// add a new item
		Trigger trig;
		trig.ticks			= ticks;
		trig.function		= Function;
		trig.arg				= Arg;

		for (iterator t = triggers.begin(); t != triggers.end(); t++)
		{
			if (t->ticks >= ticks)
			{
				triggers.insert(t, trig);
				return 1;
			}
		}

		// insert at the back
		triggers.push_back(trig);
		return 1;
	}

	void D_Timer::process()
	{
		// any count?
		if (triggers.size() == 0)
			return;

		// tracking
		time_t ticks = (_time64(0) * 1000) + (GetTickCount() % 1000);		
		for (iterator t = triggers.begin(); t != triggers.end(); )
		{
			if (t->ticks <= ticks)
			{
				// call the scripting host
				ScriptingHost::getInst().doFunction(t->function, &Umbra::VariantArray(t->arg), 0, &bot);
				t = triggers.erase(t);
			}
			else if (t->ticks > ticks)
			{
				return;
			}
		}
	}

	void D_Timer::clear()
	{
		triggers.clear();
	}

	bool D_Timer::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R) 
	{ 
		// unsupported element
		return Umbra::PropertyRequest::NOT_HANDLED;  
	}

	bool D_Timer::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R) 
	{ 
		UMBRA_FUNCTION_BEGIN(add) {
			UMBRA_FUNCTION_DA(0, Function, Umbra::VT_STRING);
			UMBRA_FUNCTION_DA(1, Miliseconds, Umbra::VT_INTEGER);
			UMBRA_FUNCTION_OA(2, Argument, Umbra::VT_ANY, Umbra::Variant());

			// code information break
			UMBRA_FUNCTION_CS() {
				insert(vaMiliseconds.toInt32(), vaFunction.toString().c_str(), vaArgument);
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// unsupported element
		return Umbra::FunctionRequest::NOT_HANDLED;
	}
}