#pragma once

namespace AW
{
	class Bot;
	class EnhancedINI;

	class UniverseOptions
	{
	protected:
		int		browser_minimum;
		int		browser_release;
		int		browser_beta;
		int		world_minimum;
		int		world_release;
		int		world_beta;
		String	greeting;
		String	start_world;
		String	notepad_url;
		String	search_url;
		BOOL	allow_tourists;

	public: // reading methods for the universe options
		int		getBrowserMinimum()	const { return browser_minimum;		}
		int		getBrowserRelease() const { return browser_release;		}
		int		getBrowserBeta()	const { return browser_beta;		}
		int		getWorldMinimum()	const { return world_minimum;		}
		int		getWorldRelease()	const { return world_release;		}
		int		getWorldBeta()		const { return world_beta;			}
		String	GetGreeting()		const { return greeting;			}
		String	getStartWorld()		const { return start_world;			}
		String	getNotepadUrl()		const { return notepad_url;			}
		String	getSearchUrl()		const { return search_url;			}
		BOOL	getAllowTourists()	const { return allow_tourists;		}

	public: // writing methods for universe options
		void	setBrowserRange(int Minimum, int Release, int Beta) {
						browser_minimum = Minimum;
						browser_release = Release;
						browser_beta	= Beta;					    }
		void	setWorldRange(int Minimum, int Release, int Beta)   {
						world_minimum = Minimum;
						world_release = Release;
						world_beta	= Beta;						   }
		void	setUrls(const char* Search, const char* Notepad)   {
						search_url = Search;
						notepad_url = Notepad;					   }
		void	SetGreeting(const char* Text)	{ greeting = Text;			}
		void	setStartWorld(const char* Text)	{ start_world = Text;		}
		void	setNotepadUrl(const char* Text)	{ notepad_url = Text;		}
		void	setSearchUrl(const char* Text)	{ search_url  = Text;		}
		void	setAllowTourists(BOOL Value)	{ allow_tourists = Value;	}

	public: // data exchange methods
		int		FromSDK	(Bot& Source);
		int		ToSDK	(Bot& Source, bool Apply = false);
		bool	FromFile(const char* File);
		bool	ToFile	(const char* File);
		void	Exchange(EnhancedINI& INI, bool Read);
	};
};