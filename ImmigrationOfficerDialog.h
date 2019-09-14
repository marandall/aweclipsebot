#pragma once
#include "afxwin.h"


// ImmigrationOfficerDialog dialog

class ImmigrationOfficerDialog : public CDialog
{
	DECLARE_DYNAMIC(ImmigrationOfficerDialog)

public:
	ImmigrationOfficerDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ImmigrationOfficerDialog();

// Dialog Data
	enum { IDD = IDD_ATTRIBUTE_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox c_History;
public:
	CString v_Message;
public:
	CEdit c_Message;
public:
	virtual BOOL OnInitDialog();
};
