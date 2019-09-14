#pragma once
#include "afxwin.h"


// ZonesEditorPolygonDialog dialog

class ZonesEditorPolygonDialog : public CDialog
{
	DECLARE_DYNAMIC(ZonesEditorPolygonDialog)

public:
	ZonesEditorPolygonDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ZonesEditorPolygonDialog();

// Dialog Data
	enum { IDD = IDD_ZONES_EDITOR_POLYGON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString v_name;
	float v_base;
	float v_height;
	CString v_map;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedDelete();
};
