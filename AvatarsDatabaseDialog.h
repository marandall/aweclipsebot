#pragma once
#include "listctrlgpx.h"
#include "afxwin.h"


// AvatarsDatabaseDialog dialog

class AvatarsDatabaseDialog : public CDialog
{
	DECLARE_DYNAMIC(AvatarsDatabaseDialog)

public:
	AvatarsDatabaseDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~AvatarsDatabaseDialog();

// Dialog Data
	enum { IDD = IDD_AVATARS_DATABASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	ListCtrlGpx c_Rows;
	virtual BOOL OnInitDialog();

public:
	void		Query(CString SQL);
	CStringA	m_LastSQL;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	CEdit c_SQL;
	afx_msg void OnQuery_ExecuteNew();
	afx_msg void OnEnChangeQuery();
};
