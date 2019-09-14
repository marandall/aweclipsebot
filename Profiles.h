#pragma once
#include <vector>
using namespace std;

void CloseProfile(String ProfileId);

namespace Profiles
{
	class Profile;
	class Profiles;

	class Universe
	{
	public:
		String	name, host;
		int		port;
		int		auth_cit;
	};

	class Universes
	{
	public: // the internal storage
		vector<Universe>	m_list;

	public: // generic accessor methods
		Universe operator[](size_t Index) { return m_list[Index]; }
		size_t size() const { return m_list.size(); }
		Universe*	getUniverse(String Name) { for (size_t i = 0; i < size(); i++) if (!Name.CompareNoCase(m_list[i].name)) return &m_list[i]; return NULL; }
	
	public: // reading and writing
		bool	Load(String FilePath);
	};

	class ProfileIcons
	{
	public:
		struct Icon {
			String name;
			int	  resource_id;
		};

	public:
		vector<Icon> list;
		void InsertIcon(String Name, int ResourceId);
		int	 GetResource(String Name);
	};

	class Profile
	{
		class Profiles;
	protected:
		/* profile id */
		String	profile_id;
		String	uni_name;
		String	icon_id;
		int		unique_index;

		/* how to login */
		int		login_citizen;
		String	login_password;
		String	login_name;

		/* target locations */
		String	targetworld;
		bool	targetglobal;
		bool	targetinvisible;
		String	targetcoords;
		int		targetavatar;
	
	public:
		bool	OpenProfile(String Directory);
		Profile	(String ProfileId);
		~Profile();
		bool	Save();
	
	public:
		int		getIndex	() const { return unique_index; }
		void	setIdent	(String ProfileId);
		void	SetIconId	(String ResourceId) { icon_id = ResourceId; }
		void	setUniverse	(String Universe);
		void	setLogin	(int Citizen, String Password, String Name);
		void	setTarget	(String World, bool Global, bool Invisible, String Coords, int Avatar);

	public:
		String	getUniName()		const;
		String	getId()				const;
		String	getIconId()			const;
		int		getLoginCitizen()	const;
		String	getLoginPassword()	const;
		String	getLoginName()		const;
		String	getTargetWorld()	const;
		bool	getTargetGlobal()	const;
		bool	getTargetInvisible()const;
		int		getTargetAvatar()	const;
		String	getTargetCoords()	const;
	};

	class Profiles
	{
	protected:
		vector<Profile>	list;

	protected:
		Profile*	m_Profile;

	public:
		Profile*	GetByIndex		(int ID);
		Profile*	getProfile		(String Id);
		Profile*	getCurrent		(void);
		bool		OpenDirectory	(String Directory);
		Profile&	operator[]		(size_t Index);
		size_t		size			(void);
		bool		SelectProfile	(String Id);
	};

	extern Profiles		g_Profiles;
	extern ProfileIcons g_Icons;
	extern Universes	g_Universes;
}
