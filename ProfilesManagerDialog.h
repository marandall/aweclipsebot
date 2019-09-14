#pragma once
#include "afxcmn.h"
#include "ListCtrlGpx.h"
#include "afxwin.h"

// ProfilesManagerDialog dialog
namespace Profiles
{
	class Profile;
}

class ProfilesManagerDialog : public CDialog
{
	DECLARE_DYNAMIC(ProfilesManagerDialog)

public:
	ProfilesManagerDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ProfilesManagerDialog();

// Dialog Data
	enum { IDD = IDD_PROFILES_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	ListCtrlGpx c_List;
	afx_msg void OnList_DoubleClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnList_DoubleHit(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic c_ProfileId;
	afx_msg void OnList_DoubleRightClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();

public:
	void	UpdateGrid(int Id, Profiles::Profile& profile);
	String	selected_profile;
};
