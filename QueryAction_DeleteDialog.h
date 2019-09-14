#pragma once
#include "afxwin.h"


// QueryAction_DeleteDialog dialog

class QueryAction_DeleteDialog : public CDialog
{
	DECLARE_DYNAMIC(QueryAction_DeleteDialog)

public:
	QueryAction_DeleteDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~QueryAction_DeleteDialog();

// Dialog Data
	enum { IDD = IDD_QUERY_ACTION_DELETE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CStatic			c_ObjectsRemaining;
	CStatic			c_Successful;
	CStatic			c_Failed;
	CProgressCtrl	c_Progress;
	CStatic			c_Status;
	CButton			c_Paused;

	afx_msg void OnBnClickedBuild();
	afx_msg void OnBnClickedCheck4();
	CButton c_TrackSuccess;
	CButton c_TrackFailed;
};
