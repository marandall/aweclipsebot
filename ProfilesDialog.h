#pragma once
#include "listctrlgpx.h"
#include "afxwin.h"

namespace Profiles
{
	class Profile;
}

// ProfilesDialog dialog

class ProfilesDialog : public CDialog
{
	DECLARE_DYNAMIC(ProfilesDialog)

public:
	ProfilesDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ProfilesDialog();

// Dialog Data
	enum { IDD = IDD_PROFILES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	Profiles::Profile* m_pCurrentProfile;
	void	ShowProfile(Profiles::Profile& ActiveProfile);

public:
	int			v_Citizen;
	CString		v_Password;
	CString		v_BotName;
	CString		v_World;
	CString		v_Coords;
	BOOL		v_Global;
	BOOL		v_Invisible;
	bool		m_UseDirectly;

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CComboBox c_Style;
	virtual void OnOK();
public:
	CComboBox c_Universe;
public:
	afx_msg void OnBnClickedOk2();
public:
	CString v_Icon;
public:
	CString v_Universe;
};
