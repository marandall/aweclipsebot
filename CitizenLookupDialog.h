#pragma once


// CitizenLookupDialog dialog

class CitizenLookupDialog : public CDialog
{
	DECLARE_DYNAMIC(CitizenLookupDialog)

public:
	CitizenLookupDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CitizenLookupDialog();

// Dialog Data
	enum { IDD = IDD_CITIZEN_LOOKUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString v_Query;
	CString v_Format;
	BOOL v_ShowFailures;
};
