#include "stdafx.h"
#include "Profiles.h"
#include "Resource.h"
#include "EclipseEvolutionDialog.h"
#include "FileLoader.h"
#include "helpers_xml.h"

namespace Profiles
{
	Profiles		g_Profiles;
	ProfileIcons	g_Icons;
	Universes		g_Universes;

	void ProfileIcons::InsertIcon(String Name, int ResourceId)
	{
		Icon ico = { Name, ResourceId } ;
		list.push_back(ico);
	}
	
	int	ProfileIcons::GetResource(String Name)
	{
		for (size_t i = 0; i < list.size(); i++)
			if (list[i].name == Name)
				return list[i].resource_id;
		return IDI_RED_1;
	}

	String	Profile::getId()			 const { return profile_id;       }
	String	Profile::getIconId()		 const { return icon_id;		  }
	String	Profile::getUniName()		 const { return uni_name;		  }
	int		Profile::getLoginCitizen()	 const { return login_citizen;	  }
	String	Profile::getLoginPassword()	 const { return login_password;	  }
	String	Profile::getLoginName()		 const { return login_name;  	  }
	String	Profile::getTargetWorld()	 const { return targetworld;  	  }
	bool	Profile::getTargetGlobal()	 const { return targetglobal;    }
	bool	Profile::getTargetInvisible()const { return targetinvisible; }
	int		Profile::getTargetAvatar()	 const { return targetavatar;	  }
	String	Profile::getTargetCoords()	 const { return targetcoords;	  }

	void Profile::setIdent(String ProfileId)
	{
		profile_id			= ProfileId;
	}
	
	void Profile::setUniverse(String Universe)
	{
		uni_name			= Universe;
	}
	
	void Profile::setLogin(int Citizen, String Password, String Name)
	{
		/* how to login */
		login_citizen		= Citizen;
		login_password		= Password;
		login_name			= Name;
	}
	
	void Profile::setTarget(String World, bool Global, bool Invisible, String Coords, int Avatar)
	{
		targetworld		= World;
		targetglobal		= Global;
		targetinvisible	= Invisible;
		targetavatar		= Avatar;
		targetcoords		= Coords;
	}

	
	bool Profile::OpenProfile(String Directory)
	{
		/* load the enhanced INI file */
		AW::EnhancedINI	ini;
		if (!ini.Load(Directory + "\\login.ini"))
			return false;

		bool b_Read = true;
		ini.Exchange(b_Read, "login",	"uni_name",		uni_name);
		ini.Exchange(b_Read, "login",	"icon",			icon_id);

		/* citizen options */
		ini.Exchange(b_Read, "login",	"login_citizen",login_citizen);
		ini.Exchange(b_Read, "login",	"login_pass",	login_password);
		ini.Exchange(b_Read, "login",	"login_name",	login_name);

		/* world login options */
		ini.Exchange(b_Read, "login",	"tgt_world",	targetworld);
		ini.Exchange(b_Read, "login",	"tgt_global",	targetglobal);
		ini.Exchange(b_Read, "login",	"tgt_invisible",targetinvisible);
		ini.Exchange(b_Read, "login",	"tgt_avatar",	targetavatar);
		ini.Exchange(b_Read, "login",	"tgt_coords",	targetcoords);
		return true;
	}

	bool Profile::Save()
	{
		/* load the enhanced INI file */
		AW::EnhancedINI	ini;

		bool b_Read = false;
		ini.Exchange(b_Read, "login",	"uni_name",		uni_name);
		ini.Exchange(b_Read, "login",	"icon",			icon_id);

		/* citizen options */
		ini.Exchange(b_Read, "login",	"login_citizen",login_citizen);
		ini.Exchange(b_Read, "login",	"login_pass",	login_password);
		ini.Exchange(b_Read, "login",	"login_name",	login_name);

		/* world login options */
		ini.Exchange(b_Read, "login",	"tgt_world",	targetworld);
		ini.Exchange(b_Read, "login",	"tgt_global",	targetglobal);
		ini.Exchange(b_Read, "login",	"tgt_invisible",targetinvisible);
		ini.Exchange(b_Read, "login",	"tgt_avatar",	targetavatar);
		ini.Exchange(b_Read, "login",	"tgt_coords",	targetcoords);
	
		String Directory = GetAppPath() + "\\data\\profiles\\" + profile_id;
		if (!ini.Save(Directory + "\\login.ini"))
			return false;

		return true;
	}

	Profile::Profile(String ProfileId)
	{
		profile_id	= ProfileId;
		static int uid = 0;
		unique_index = ++uid;
	}
	
	Profile::~Profile()
	{
	}

	Profile& Profiles::operator[](size_t Index)
	{
		return list[Index];
	}

	size_t Profiles::size(void)
	{
		return list.size();
	}

	Profile* Profiles::getProfile(String Id)
	{
		for (size_t i = 0; i < list.size(); i++)
			if (list[i].getId() == Id)
				return &list[i];
		return NULL;
	}

	bool Profiles::OpenDirectory(String Directory)
	{
		WIN32_FIND_DATA file_attr;
		String DirectorySearch = Directory + "\\*";
		HANDLE hFind = FindFirstFile(DirectorySearch, &file_attr);
		if (hFind == INVALID_HANDLE_VALUE) {
			String logText;
			return false;
		} else {
			BOOL b_OK = TRUE;
			String DirPath, file_name;
			while (b_OK == TRUE) {
				if (file_attr.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					file_name = file_attr.cFileName;
					DirPath.Format("%s\\%s", Directory, file_attr.cFileName);
					if (DirPath.Right(1) != ".") {
						Profile profile(file_attr.cFileName);
						profile.OpenProfile(DirPath);
						list.push_back(profile);
					}
				}
				b_OK = FindNextFile(hFind, &file_attr);
			}
			FindClose(hFind);
		}
		return true;
	}

	bool Profiles::SelectProfile(String Id)
	{
		Profile* pSelect = getProfile(Id);
		if (pSelect == NULL)
			return false;
		m_Profile = pSelect;

		/* load the profile icon information */
		rt_diag->SetIcon(AfxGetApp()->LoadIconA(
			g_Icons.GetResource(pSelect->getIconId())), FALSE);
		return true;
	}

	Profile* Profiles::getCurrent()
	{
		return m_Profile;
	}

	Profile* Profiles::GetByIndex(int ID)
	{
		for (size_t i = 0; i < list.size(); i++)
			if (list[i].getIndex() == ID)
				return &list[i];
		return NULL;
	}

	bool Universes::Load(String FilePath)
	{
		// open file, check error
		FileReader file(FilePath);
		if (!file.IsOK())
		{
			WriteMsg(String("Error opening universe list: ") + FilePath, RGB_RED, OT_ERROR | OT_TECH);
			return false;
		}
		else
		{
			// upload universe information
			m_list.clear();

			// enum each item
			for (size_t i = 0; i < file.size(); i++)
			{
				Helpers::XML::DataKeyset dsKeys;
				Helpers::XML::ParseSingleLine((LPCSTR)file[i], dsKeys);
		
				// only accept universe tags at this time
				if (dsKeys.type != "universe")
					continue;

				// read the keys
				Universe uInfo;
				uInfo.name = dsKeys["name"];
				uInfo.host = dsKeys["host"];
				uInfo.port = atoi(dsKeys["port"]);
				uInfo.auth_cit = atoi(dsKeys["usvr"]);

				// pop onto the list
				m_list.push_back(uInfo);
			}

			// all is well
			return true;
		}
	}
}