#pragma once
#include "web.h"
#include "Resource.h"

// WebClientDialog dialog

class WebClientDialog : public CDialog
{
	DECLARE_DYNAMIC(WebClientDialog)

public:
	WebClientDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~WebClientDialog();

// Dialog Data
	enum { IDD = IDD_WEB_HOLDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CWeb		c_WebCtrl;
	CStringA	v_Url;
public:
	virtual BOOL OnInitDialog();

	static int createUrlWindow(String Url, CWnd* Parent);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};