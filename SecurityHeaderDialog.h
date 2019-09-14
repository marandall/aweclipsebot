#pragma once


// SecurityHeaderDialog dialog

class SecurityHeaderDialog : public CDialog
{
	DECLARE_DYNAMIC(SecurityHeaderDialog)

public:
	SecurityHeaderDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~SecurityHeaderDialog();

// Dialog Data
	enum { IDD = IDD_SECURITY_HEADER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString v_identifier;
	CString v_name;
	CString v_description;
};
