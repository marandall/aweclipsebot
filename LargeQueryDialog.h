#pragma once


// LargeQueryDialog dialog

class LargeQueryDialog : public CDialog
{
	DECLARE_DYNAMIC(LargeQueryDialog)

public:
	LargeQueryDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~LargeQueryDialog();

// Dialog Data
	enum { IDD = IDD_QUERY_LARGE_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString v_top;
	CString v_bottom;
	CString v_aroundCenter;
	int v_aroundWidth;
	int v_aroundHeight;
};
