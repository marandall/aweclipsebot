#pragma once
#include "graphicslabel.h"
#include "GraphicsNet_2DMap.h"

// MapDialog dialog

class MapDialog : public CDialog
{
	DECLARE_DYNAMIC(MapDialog)

public:
	MapDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~MapDialog();

// Dialog Data
	enum { IDD = IDD_MAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	GraphicsLabel			c_DrawSurface;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnNcDestroy();
};
