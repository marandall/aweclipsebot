#pragma once
#include "textbox.h"


// SelectUrlDialog dialog

class SelectUrlDialog : public CDialog
{
	DECLARE_DYNAMIC(SelectUrlDialog)

public:
	SelectUrlDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~SelectUrlDialog();

// Dialog Data
	enum { IDD = IDD_SEND_URL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	TextBox c_Url;
public:
	CString v_Target;
public:
	CString v_Url;
public:
	CString v_Post;
public:
	BOOL v_Target3D;
};
