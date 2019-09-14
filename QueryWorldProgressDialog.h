#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "listener.h"
#include "listctrlgpx.h"


// QueryWorldProgressDialog dialog

class QueryWorldProgressDialog : 
	public CDialog, 
	public Listener
{
	DECLARE_DYNAMIC(QueryWorldProgressDialog)

public:
	QueryWorldProgressDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~QueryWorldProgressDialog();

// Dialog Data
	enum { IDD = IDD_QUERY_WORLD_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public: // event information
	void onListener(EventBroadcaster* Broadcaster, int Id, EventMessage* EventInfo);

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnQuery_BackupToFile();
	CEdit c_status;
	CProgressCtrl c_progress;
	CEdit c_objects;
	CEdit c_objectsof;
	CEdit c_estimatedPercent;
	CEdit c_timeElapsed;
	CEdit c_timeEstimatedDuration;
	virtual BOOL OnInitDialog();
	CStatic c_error;

public: // update dialogs
	void rebuild();
	void updateSegment(size_t Seg);

	ListCtrlGpx c_iterators;
};
