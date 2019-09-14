#pragma once
#include "richeditctrl_ext.h"
#include <vector>
#include "afxwin.h"

// EclipseLogDialog dialog

class EclipseLogDialog : public CDialog
{
	DECLARE_DYNAMIC(EclipseLogDialog)

public:
	EclipseLogDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~EclipseLogDialog();

// Dialog Data
	enum { IDD = IDD_ECLIPSE_LOG_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	struct WindowPair { CButton* button; CRichEditCtrl_Ext* window; } ;
	std::vector<WindowPair>	windows;
	void	SelectWindow(CButton& Button);

public:
	CRichEditCtrl_Ext c_TechLog;
	CRichEditCtrl_Ext c_SecurityLog;
	CRichEditCtrl_Ext c_UserLog;
	CRichEditCtrl_Ext c_DebugLog;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	CButton c_Tech;
	CButton c_Security;
	CButton c_UserList;
	CButton c_Debug;

public:
	afx_msg void OnBnClickedTech();
public:
	afx_msg void OnBnClickedSecurity();
public:
	afx_msg void OnBnClickedUul();
public:
	afx_msg void OnBnClickedDebug();
protected:
	virtual void OnCancel();
};
