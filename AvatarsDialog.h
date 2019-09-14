#pragma once

#include "listctrlgpx.h"
#include "Resource.h"
#include "afxwin.h"

class Avatar;

// AvatarsDialog dialog

class AvatarsDialog : public CDialog
{
	DECLARE_DYNAMIC(AvatarsDialog)

public:
	AvatarsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~AvatarsDialog();

// Dialog Data
	enum { IDD = IDD_ECLIPSE_TECHLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

	/* tracking the height */
	int known_height;
	int max_height;
public:
	int getStdHeight() { return known_height; }

public:
	ListCtrlGpx c_Avatars;

public: // expanding and collapsing
	void	expand();
	void	collapse();

public:
	void	AvatarAdd	(Avatar& User);
	void	AvatarUpdate(Avatar& User);
	void	AvatarMove	(Avatar& User);
	void	AvatarDelete(Avatar& User);

public:
	afx_msg void OnBnClickedButtonExpand();
	afx_msg void OnBnClickedShowAvatars();
	afx_msg void OnBnClickedShowUniverse();
	afx_msg void OnBnClickedSplitVertical();
	
public:
	CButton c_Expand;
	CStatic c_ShowBar;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnAvatars_RightClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAvatars_HeaderClick(NMHDR *pNMHDR, LRESULT *pResult);
};
