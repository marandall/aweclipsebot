#pragma once
#include "afxwin.h"
#include "colourbutton.h"


// ConsoleMessageDialog dialog

class ConsoleMessageDialog : public CDialog
{
	DECLARE_DYNAMIC(ConsoleMessageDialog)

public:
	ConsoleMessageDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ConsoleMessageDialog();

// Dialog Data
	enum { IDD = IDD_COMMS_BROADCAST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CString		v_Prefix;
	CString		v_Message;
	CButton		c_Bold;
	CButton		c_Italics;

public:
	ColourButton c_Colour;
public:
	BOOL v_Bold;
public:
	BOOL v_Italic;
};
