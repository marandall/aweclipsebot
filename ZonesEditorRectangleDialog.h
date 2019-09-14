#pragma once


// ZonesEditorRectangleDialog dialog

class ZonesEditorRectangleDialog : public CDialog
{
	DECLARE_DYNAMIC(ZonesEditorRectangleDialog)

public:
	ZonesEditorRectangleDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ZonesEditorRectangleDialog();

// Dialog Data
	enum { IDD = IDD_ZONES_EDITOR_RECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString v_Name;
	CString v_Top;
	CString v_Bottom;
	afx_msg void OnBnClickedDelete();
};
