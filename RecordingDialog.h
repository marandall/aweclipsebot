#pragma once
#include "listctrlgpx.h"


// RecordingDialog dialog

class RecordingDialog : public CDialog
{
	DECLARE_DYNAMIC(RecordingDialog)

public:
	RecordingDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~RecordingDialog();

// Dialog Data
	enum { IDD = IDD_RECORDING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	ListCtrlGpx c_events;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
