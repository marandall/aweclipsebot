#pragma once
#include <vector>

namespace Umbra
{
	class CallStackMonitor;
	class CallStackMonitorFrame;

	//
	// CallStackMonitor
	// -----------------------------
	//
	// Used by function calls to indicate the current state of the stack
	// and linked through every property etc
	//

	class CallStackMonitor
	{
	public: // type defenition for the frame list
		typedef std::vector<CallStackMonitorFrame*> FrameList;
		typedef FrameList::iterator					iterator;
	
	protected: // the frames list
		friend class CallStackMonitorFrame;
		friend class NativeRuntimeErrorException;
		FrameList _frames;
	};

	//
	// CallStackMonitorFrame
	// -----------------------------
	//
	// Calls operations upon call stack monitor referencing itself
	// in the list of frames alongside all the other junk
	//

	class CallStackMonitorFrame
	{
	protected: // identified by function name
		std::string			_func_id;		// resolved function name
		CallStackMonitor*	_monitor;		// the monitor that will be followed
		int					_line;			// which line of the previous function was it called upon

	public: // constructor destructor
		CallStackMonitorFrame(CallStackMonitor* Monitor, std::string FunctionId, int Line)
		{
			// assign reference
			_func_id	= FunctionId;
			_monitor	= Monitor;
			_line			= Line;

			// add self onto the monitor
			_monitor->_frames.push_back(this);
		}

		~CallStackMonitorFrame()
		{
			// erase the last element (ourself) from the list
			_monitor->_frames.pop_back();
		}

	public: // get the string data
		std::string info()
		{
			char trace[1024] = { 0 };
			sprintf_s(trace, "%d : %s", _line, _func_id.c_str());
			return trace;
		}
	};
}