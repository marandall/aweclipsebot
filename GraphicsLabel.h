#pragma once
#include "resource.h"

namespace GraphicsNet
{
	class BaseObject;
}

class GraphicsLabel;

// GraphicsLabel dialog
class GraphicsLabelHandler
{
public:
	virtual void OnGLH_Event(GraphicsLabel& Control, const char*, void* Data) { }; 
	virtual ~GraphicsLabelHandler() { }
};

class GraphicsLabel : public CDialog
{
public:
	DECLARE_DYNAMIC(GraphicsLabel)

public:
	GraphicsNet::BaseObject* m_Surface;

	GraphicsLabel(CWnd* pParent = NULL);   // standard constructor
	virtual ~GraphicsLabel();

// Dialog Data
	enum { IDD = IDD_GRAPHICS_WINDOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	void send_parent_msg(const char* MsgId);

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
