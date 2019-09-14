#pragma once
#include "afxwin.h"


// TeleportDialog dialog

class TeleportDialog : public CDialog
{
	DECLARE_DYNAMIC(TeleportDialog)

public:
	TeleportDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~TeleportDialog();

// Dialog Data
	enum { IDD = IDD_ECLIPSE_TELEPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	CStatic c_Position;
public:
	CEdit c_Coordinates;
public:
	virtual BOOL OnInitDialog();
public:
	BOOL v_Global;
public:
	BOOL v_Invisible;
public:
	CString v_String;
public:
	CString v_World;
	afx_msg void OnBnClickedOk();
};
