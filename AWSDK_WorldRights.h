#pragma once

namespace AW
{
	class Bot;

	/*
		The rights of a user within a world
	*/
	class WorldRights
	{
	protected:
		bool enter;
		bool build;
		bool ed;
		bool spobjs;
		bool spcmds;
		bool ps;
		bool eject;
		bool bots;
		bool speak;
		bool talk;
		bool v4;

	public:
		bool HasEnter			(void) const { return enter; }
		bool HasBuild			(void) const { return build; }
		bool HasED				(void) const { return ed; }
		bool HasSpecialObjects	(void) const { return spobjs; }
		bool HasSpecialCommands	(void) const { return spcmds; }
		bool HasPublicSpeaker	(void) const { return ps; }
		bool HasEject			(void) const { return eject; }
		bool HasBots			(void) const { return bots; }
		bool HasSpeak			(void) const { return speak; }
		bool HasTalk			(void) const { return talk; }
		bool HasV4				(void) const { return v4; }
	
	public:
		int FromBot(Bot& Source, int Citizen); 
	};
};