#pragma once

namespace Data 
{
	class EclipseDialog_MainIcons
	{
	public:
		/* pure abilities*/
		int	main;				// dot icon
		int	eject;				// has eject
		int root;				// has root
		int ps;					// has public speaker
		int ps_eject;			// has ps and eject
		int ps_root;			// has ps and root
		int ps_ct;				// has ps and ct

		/* styles for states */
		int locked;				// unable to acces
		int ticked;				// is ticked and ok
		int working;			// is doing something
		int warning;			// warning - user error
		int alert;				// major problem
		int equal;				// equal are same
		int equal_b;			// equal is tourist

		int move_t[15];			// an array of move types
		int build_t[10];		// an array of build types

		/* styles for whispers */
		int whisper;
		int blank;
	};

	extern EclipseDialog_MainIcons av_icons;
}