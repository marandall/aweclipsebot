#pragma once


// ZonesEditorSphereDialog dialog

class ZonesEditorSphereDialog : public CDialog
{
	DECLARE_DYNAMIC(ZonesEditorSphereDialog)

public:
	ZonesEditorSphereDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ZonesEditorSphereDialog();

// Dialog Data
	enum { IDD = IDD_ZONES_EDITOR_SPHERE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString v_Name;
	CString v_Centre;
	float v_Radius;
	afx_msg void OnBnClickedDelete();
};
