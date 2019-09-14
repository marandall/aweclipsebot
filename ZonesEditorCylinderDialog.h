#pragma once


// ZonesEditorCylinderDialog dialog

class ZonesEditorCylinderDialog : public CDialog
{
	DECLARE_DYNAMIC(ZonesEditorCylinderDialog)

public:
	ZonesEditorCylinderDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ZonesEditorCylinderDialog();

// Dialog Data
	enum { IDD = IDD_ZONES_EDITOR_CYLINDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString v_Name;
	CString v_Base;
	float v_Radius;
	float v_Height;
	afx_msg void OnBnClickedDelete();
};
