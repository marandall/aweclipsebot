#pragma once

// CRichEditCtrl_Ext
#define RTF_COLOUR		1
#define RTF_BOLD		2
#define RTF_ITALIC		4

class CRichEditCtrl_Ext : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CRichEditCtrl_Ext)

public:
	CRichEditCtrl_Ext();
	virtual ~CRichEditCtrl_Ext();

	void		WriteRichText(LPCSTR msg, long col, int opts);
	void		WriteText(LPCSTR msg, long col, int opts);
	long		SaveToFile(LPCSTR FileName);

	static		DWORD EditStreamCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
	void		AllocBufferEx(LPCSTR Extra);

	void		setFirstTab(int X = 1300);
	void		EraseContents();

private:
	String	m_stored_rtf;
	bool	has_changed;

public: // change tracking; limits to a certain number of rows
	struct MessageInfo
	{
		time_t	timestamp;
		int			length;
		String	text;
	};

public: // track each message that is posted; use it as overflow
	typedef std::list<MessageInfo> OverflowList;
	OverflowList	overflow;
	int						overflow_offset;
	int						overflow_deleted;
	int						getOverflowLength();

public: // erase all except the last x lines
	void cropOverflow(int Lines);

public:
	bool	ReadChangedState()
	{
		bool cs = has_changed;
		has_changed = false;
		return cs;
	}

protected:
	DECLARE_MESSAGE_MAP()
};


