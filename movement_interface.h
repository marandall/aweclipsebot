#pragma once

namespace AW 
{ 
	class Location; 
	class LocationEx; 
	class Bot; 
}

namespace Movement
{
	class Controller;

	class IMovement
	{
	public: // interfaces
		virtual int getNextChange(Controller* Control, AW::Location Current, AW::Location& Result, float Resolution) = 0;
	
	public: // enumeration types for movement
		static const int DO_NOTHING	= 0;
		static const int DO_PAUSE	= 1;
		static const int DO_MOVE	= 2;
	};
}