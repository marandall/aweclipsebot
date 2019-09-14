#pragma once
#include "textbox.h"
#include "afxwin.h"
#include "afxcmn.h"

namespace Property
{
	class Project;

}

// QueryAction_BuildDialog dialog

class QueryAction_BuildDialog : public CDialog
{
	DECLARE_DYNAMIC(QueryAction_BuildDialog)

public:
	QueryAction_BuildDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~QueryAction_BuildDialog();

// Dialog Data
	enum { IDD = IDD_QUERY_ACTION_BUILD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	Property::Project* m_pProject;

public:
	TextBox			c_SuccessPath;
	TextBox			c_FailurePath;
	CStatic			c_ObjectsRemaining;
	CStatic			c_Successful;
	CStatic			c_Failed;
	CProgressCtrl	c_Progress;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	CStatic c_Status;
	CButton c_Paused;
public:
	virtual BOOL OnInitDialog();
public:
	
public:
	afx_msg void OnBnClickedPause();
public:
	afx_msg void OnBnClickedBuild();
	CButton c_MaintainED;
};
