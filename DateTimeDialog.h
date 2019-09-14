#pragma once
#include "afxdtctl.h"


// DateTimeDialog dialog

class DateTimeDialog : public CDialog
{
	DECLARE_DYNAMIC(DateTimeDialog)

public:
	DateTimeDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~DateTimeDialog();

// Dialog Data
	enum { IDD = IDD_DATE_TIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTime	timestamp;
	afx_msg void OnBnClickedOk();
	CDateTimeCtrl c_Date;
	CDateTimeCtrl c_Time;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedNever();
	CMonthCalCtrl c_month;
};
