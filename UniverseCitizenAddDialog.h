#pragma once
#include "afxwin.h"


// UniverseCitizenAddDialog dialog

class UniverseCitizenAddDialog : public CDialog
{
	DECLARE_DYNAMIC(UniverseCitizenAddDialog)

public:
	UniverseCitizenAddDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~UniverseCitizenAddDialog();

// Dialog Data
	enum { IDD = IDD_UNIVERSE_CITIZEN_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CString v_UserName;
	CString v_EMail;
	CString v_Password;
	virtual BOOL OnInitDialog();
	CEdit c_User;
};
