// RichEditCtrl_Ext.cpp : implementation file
//
#include "stdafx.h"
#include "RichEditCtrl_Ext.h"
#include "Richedit.h"
#include "InterfaceConfig.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif


// CRichEditCtrl_Ext

IMPLEMENT_DYNAMIC(CRichEditCtrl_Ext, CRichEditCtrl)
CRichEditCtrl_Ext::CRichEditCtrl_Ext()
{
	overflow_offset = 0;
}

CRichEditCtrl_Ext::~CRichEditCtrl_Ext()
{
}


BEGIN_MESSAGE_MAP(CRichEditCtrl_Ext, CRichEditCtrl)
END_MESSAGE_MAP()


void CRichEditCtrl_Ext::setFirstTab(int Width)
{
	PARAFORMAT pf;
	SetSel(0, -1);
	GetParaFormat(pf);
	pf.cTabCount	= 1;
	pf.dwMask		= PFM_TABSTOPS|PFM_OFFSET;
	pf.cbSize		= sizeof pf;
	pf.rgxTabs[0]	= Width;
	pf.rgxTabs[1]	= Width;
	pf.dxOffset		= Width; 
	SetParaFormat(pf);
}
// CRichEditCtrl_Ext message handlers

void CRichEditCtrl_Ext::EraseContents()
{
	SetSel(0, GetTextLength());
	ReplaceSel("");
}

// -----------------------------------------------------------------------------------------
// Writes a pure RTF section
// -----------------------------------------------------------------------------------------
void CRichEditCtrl_Ext::WriteRichText(LPCSTR msg, long col, int opts)
{
	has_changed = true;

	// we set the information
	CHARFORMAT cf;
	cf.crTextColor	= (COLORREF)col;						// set the text colour
	cf.cbSize		= sizeof(cf);							// the size of the character format
	cf.dwMask		= CFM_COLOR | CFM_BOLD | CFM_ITALIC;	// associated flags each time
	cf.dwEffects	= (opts & MF_BOLD ? CFE_BOLD : 0) |		// Bold modifier
					  (opts & MF_ITALIC ? CFE_ITALIC : 0);	// Italic modifier

	/* get our current selection lengths */
	long	start_char, end_char;							// Variables for application
	GetSel	(start_char, end_char);							// Retrieve our lengths

	/* our selection is turned off to automatically scroll */
	HideSelection(TRUE, FALSE);								// Hide, Not Perminant

	/* the lengths of our relevant strings here */
	const long len	= GetTextLength();
	const long len2 = (long)(len + strlen(msg));

	/* set character range as the null byte */
	SetSel(len, len);										// set null byte
	SetSelectionCharFormat(cf);								// apply formatting
	ReplaceSel(msg);										// replace null selection
	SetSel(len2, len2);										// set new selection length at the end

	/* change hide selection so we can find our info again */
	HideSelection(FALSE, TRUE);								// Switch back visible selection
	
	/* do we need to select a different range yet or not? */
	if (start_char == end_char)
		SetSel(GetTextLength(), GetTextLength());			// End of the line
	else
		SetSel(start_char, end_char);						// What it was before
}

// -----------------------------------------------------------------------------------------
// Writes an appropriatly formatted line of data to the given CRichEditCtrl
// -----------------------------------------------------------------------------------------
void CRichEditCtrl_Ext::WriteText(LPCSTR msg, long col, int opts)
{
	has_changed = true;

	/* variables*/
	CTime	time = CTime::GetCurrentTime();
	String true_data;

	/* do we need to add the time? */
	if (opts & OM_TIME)
		true_data.Format("%s\t%s\r\n", time.Format(g_InterfaceConfig.getLogging().getDateFormat()), msg);
	else
		true_data.Format("%s\r\n", msg);

	// does require splitting up if using forebold
	if (opts & MF_FBOLD)
	{
		// does a tabe already exist		
		int	tab_at	= true_data.Find("\t");

		// split across the tab
		String pt1 = true_data.Mid(0,tab_at);			// copy the pretab
		String pt2 = true_data.Mid(tab_at++);			// final string has the new line appended

		// store the previous length before ading information
		int prev_len = GetTextLength();
		WriteRichText(pt1, col, opts | MF_BOLD);
		WriteRichText(pt2, col, opts | opts);

		// add the message information
		MessageInfo mi;
		mi.timestamp	= _time64(0);
		mi.length			= pt1.GetLength() + pt2.GetLength();
		mi.text				= pt1 + pt2; 
		overflow.push_back(mi);
	}
	else
	{
		WriteRichText(true_data, col, opts);

		int len = GetTextLength();

		// add the message information
		MessageInfo mi;
		mi.timestamp	= _time64(0);
		mi.length			= true_data.GetLength();
		mi.text				= true_data; 
		overflow.push_back(mi);
	}
}

// the static stream
DWORD 
CRichEditCtrl_Ext::EditStreamCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	String buffer = (char*)(pbBuff);
	((CRichEditCtrl_Ext*)(dwCookie))->AllocBufferEx(buffer);
	return NULL;
}

void 
CRichEditCtrl_Ext::AllocBufferEx(LPCSTR Extra)
{
	m_stored_rtf.Append(Extra);
}

// -----------------------------------------------------------------------------------------
// Triggers file save
// -----------------------------------------------------------------------------------------
long 
CRichEditCtrl_Ext::SaveToFile(LPCSTR FileName)
{
	// set up our stream
	EDITSTREAM	es;
	es.dwCookie		= (DWORD_PTR)this;
	es.dwError		= 0;
	es.pfnCallback	= (EDITSTREAMCALLBACK)CRichEditCtrl_Ext::EditStreamCallback;
	StreamOut(SF_RTF, es);
	return NULL;
}

int	CRichEditCtrl_Ext::getOverflowLength()
{
	int bytes = 0;
	for (OverflowList::iterator itr = overflow.begin(); itr != overflow.end(); itr++)
		bytes+= itr->length;
	return bytes;
}

void CRichEditCtrl_Ext::cropOverflow(int Count)
{
	// how many lines are visible in the range
	if (Count > (int)overflow.size())
		return;

	// get the current selection
	long curSelFrom, curSelTo;
	GetSel(curSelFrom, curSelTo);

	// reverse until we have those required
	int limit = (int)overflow.size();
	int count = 0;
	int bytes = 0;

	for (OverflowList::iterator itr = overflow.begin(); itr != overflow.end(); itr++)
	{
		count++;
		bytes+= itr->length - 1;
		if ((count + Count) == limit)
		{
			// disable selection
			HideSelection(TRUE, TRUE);

			// select the new items upto this range
			SetSel(0, bytes);
			ReplaceSel("", 0);

			// kill begin here and there
			itr++;
			overflow.erase(overflow.begin(), itr);
			overflow_offset--;

			// update selection
			HideSelection(FALSE, TRUE);
			SetSel(curSelFrom - bytes, curSelTo - bytes);
			return;
		}
	}
}





