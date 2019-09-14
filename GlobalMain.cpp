#include "stdafx.h"
#include "EclipseEvolutionDialog.h"		// for class and rt_diag
#include "Profiles.h"
#include "InterfaceConfig.h"

bool IsRootMenuChecked(int MenuItem) {
	/* get the menu*/
	CMenu* menu = rt_diag->GetMenu();
	if (menu == false) return false;
	return (menu->GetMenuState(MenuItem, MF_BYCOMMAND) == MF_CHECKED);
}

void SetRootMenuChecked(int MenuItem, bool Checked) {
	/* get the menu*/
	CMenu* menu = rt_diag->GetMenu();
	if (menu == false) 
		return;
	menu->CheckMenuItem(MenuItem, Checked ? MF_CHECKED : 0);
}

void SetRootMenuEnabled(int MenuItem, bool Enabled)
{
	CMenu* mmenu = rt_diag->GetMenu();
	mmenu->EnableMenuItem(MenuItem, Enabled ? MF_ENABLED : MF_GRAYED);
}

void WriteMsg(char const * Message, long Colour, int Style)
{
	if ((Style & OT_MAIN) != 0)
	{	
		rt_diag->c_Log.WriteText(Message, Colour, Style);
	
		// log to file anything written to the primary display
		if (Profiles::g_Profiles.getCurrent())
		{
			// formt time
			CTime  t = CTime::GetCurrentTime();
			String logDate = t.Format("%d/%b/%Y %H:%M:%S");
			String logFile = t.Format("%b_%Y");
			String filePath;
			filePath.Format("%s\\logs\\%s_display.txt", getProfilePath(), logFile);

			FILE* fptr = NULL;
			errno_t err = fopen_s(&fptr, filePath, "a");
			if (err == 0)
			{
				fprintf_s(fptr, "%s %s\n", logDate, Message);
				fclose(fptr);
			}
		}
	}

	else if ((Style & OT_TECH) != 0)
		rt_diag->c_TechLog.c_TechLog.WriteText(Message, Colour, Style);
	
	else if ((Style & OT_DEBUG) != 0)
		rt_diag->c_TechLog.c_DebugLog.WriteText(Message, Colour, Style);
	
	else if ((Style & OT_USERLIST) != 0)
		rt_diag->c_TechLog.c_UserLog.WriteText(Message, Colour, Style);

	else if ((Style & OT_SECURITY) != 0)
		rt_diag->c_TechLog.c_SecurityLog.WriteText(Message, Colour, Style);
}

String getTime(const char* Format)
{
	CTime time = CTime::GetCurrentTime();
	return time.Format(Format == NULL ? g_InterfaceConfig.getLogging().getTimeFormat() : "%H:%M:%S");
}

String MakeGuiTime(__time64_t Timestamp, const char* Format)
{
	CTime time = Timestamp;
	return time.Format(Format == NULL ? g_InterfaceConfig.getLogging().getTimeFormat() : "%H:%M:%S");
}

CStringA GetAppPath(void) 
{
	/* the variables */
	char	path[_MAX_PATH * 2] = { 0 };

	/* get the module file name and strip final portion */
	GetModuleFileName(AfxGetInstanceHandle(), path, _MAX_PATH * 2);
	*strrchr(path, '\\') = '\0';

	/* get the string we need */
	return path;
}

String getProfilePath()
{
	String path;
	path.Format("%s\\data\\profiles\\%s", GetAppPath(), Profiles::g_Profiles.getCurrent()->getId());
	return path;
}

String getWorldPath()
{
	String path;
	path.Format("%s\\data\\universes\\%s\\worlds\\%s", GetAppPath(),
				Profiles::g_Profiles.getCurrent()->getUniName(), 
				bot.getWorld().GetLength() ? bot.getWorld() : "__nulworld");
	return path;
}

String getUserProfilesPath()
{
	String path;
	path.Format("%s\\data\\profiles\\%s\\u_setup", GetAppPath(), Profiles::g_Profiles.getCurrent()->getId());
	return path;
}

bool FileExists(String Path)
{
	return GetFileAttributes(Path) != INVALID_FILE_ATTRIBUTES;
}

CStringA ExtractLength(const char* p, size_t len) {
	CStringA ex = p;
	return ex.Mid(0, (int)len);
}

bool CharInStr(char Byte, const char* Range) {
	size_t length = strlen(Range);
	for (size_t i = 0; i < length; i++)
		if (Range[i] == Byte)
			return true;
	return false;
}

char* getnext_breaker(char* Text, const char* Breakers)
{
	char* p		= const_cast<char*>(Text);
	size_t bl	= strlen(Breakers);

	// loop while not at the end of the string
	while (*p != '\0')
	{
		for (size_t i = 0; i < bl; i++)
			if (Breakers[i] == *p)
				return p;
		p++;
	}

	// return final
	return p;
}

size_t SplitToMultipart(std::vector<CStringA>& lines, 
		const char* Input, size_t MAX_SIZE, const char* breakers)
{
	char* input = const_cast<char*>(Input);
	char* p = input;

	// check breakers
	// (strlen(breakers) == 0)
	breakers = ", .\n\t";
	
	// if its less than the max size just return it whole
	if (strlen(Input) < (size_t)MAX_SIZE)
	{
		lines.push_back(CStringA(Input));
		return lines.size();
	}

	// split it up at the words
	size_t length = 0;
	String tracker = "";
	while (true)
	{
		// if end of line
		if (*p == '\0')
			break;

		// if split it greater
		char* next_breaker = getnext_breaker(p, breakers);
		String word(p, static_cast<int>(next_breaker - p + 1));
		
		// is the single word too long?
		int word_len = word.GetLength();
		if ((word_len > (int)MAX_SIZE) && length == 0)
		{
			while (word.GetLength() > (int)MAX_SIZE)
			{
				String w2 = word.Mid(0, (int)MAX_SIZE);
				lines.push_back(w2);
				word = word.Mid((int)MAX_SIZE);
			}

			// last bit onto list
			tracker = word;
			length  = word.GetLength();
			p		= (next_breaker + 1); 
			continue;
		}

		// is the word still too short
		if ((length + word_len) < MAX_SIZE)
		{
			tracker += word;
			length  += word.GetLength(); 
			p = (next_breaker + 1);
			continue;
		}
		else
		{
			lines.push_back(tracker);
			
			// word length shorter
			size_t len_p = strlen(p);
			if (len_p < MAX_SIZE)
			{
				lines.push_back(CStringA(p));
				break;
			}

			length  = word.GetLength();
			tracker = word;
		}
	}


	return lines.size();
}