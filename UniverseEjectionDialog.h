#pragma once
#include "afxwin.h"


// UniverseEjectionDialog dialog

class UniverseEjectionDialog : public CDialog
{
	DECLARE_DYNAMIC(UniverseEjectionDialog)

public:
	UniverseEjectionDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~UniverseEjectionDialog();

// Dialog Data
	enum { IDD = IDD_UNIVERSE_EJECTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void Display(int ReasonCode);

	DECLARE_MESSAGE_MAP()

public:
	static UniverseEjectionDialog* m_pCurWnd;
	void OnDnsResolved(unsigned int IP, String Dns); 
	unsigned int this_addr;

public:
	CEdit c_IP;
	CEdit c_DNS;
	CEdit c_Created;
	CEdit c_Expires;
	CEdit c_Comment;
	CEdit c_Reason;

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedAdd();
};
