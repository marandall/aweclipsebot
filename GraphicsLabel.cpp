// GraphicsLabel.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "GraphicsLabel.h"
#include "GraphicsNet_Base.h"
#include <gdiplus.h>



namespace GraphicsNet
{
	void BaseObject::post_handler_msg(const char* Msg, void* Data)
	{
		if (m_pMsgTarget) m_pMsgTarget->OnGLH_Event(*m_pWindow, Msg, Data);
	}
}


// GraphicsLabel dialog

IMPLEMENT_DYNAMIC(GraphicsLabel, CDialog)

GraphicsLabel::GraphicsLabel(CWnd* pParent /*=NULL*/)
	: CDialog(GraphicsLabel::IDD, pParent)
{
	m_Surface = 0;
}

GraphicsLabel::~GraphicsLabel()
{
}

void GraphicsLabel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GraphicsLabel, CDialog)
	ON_WM_PAINT()
//	ON_WM_RBUTTONUP()
//	ON_WM_NCLBUTTONDBLCLK()
//	ON_WM_NCLBUTTONDOWN()
	ON_WM_PARENTNOTIFY()
	ON_WM_NCHITTEST()
//	ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MBUTTONDOWN()
ON_WM_MBUTTONUP()
ON_WM_RBUTTONDOWN()
ON_WM_RBUTTONUP()
ON_WM_MBUTTONDBLCLK()
ON_WM_MOUSEWHEEL()
ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


#include "performance.h"

void GraphicsLabel::OnPaint()
{
	RECT rt;
	GetWindowRect(&rt);
	CRect rtx(rt);

	/* create graphics dispatch */
	PerformanceCounter pc(PerformanceCounter::AUTO_START);
	Gdiplus::Bitmap		bmp(rtx.Width(), rtx.Height());
	Gdiplus::Graphics	g(&bmp);
	pc.stop();

	// abort if no surface
	if (m_Surface == NULL)
		return;

	/* set the height and width elements */
	m_Surface->m_Width		= rtx.Width();
	m_Surface->m_Height		= rtx.Height();
	m_Surface->m_Gdi		= &g;
	m_Surface->m_pWindow	= this;
	m_Surface->m_pMsgTarget = dynamic_cast<GraphicsLabelHandler*>(GetParent());

	
	PerformanceCounter pc2(PerformanceCounter::AUTO_START);
	m_Surface->OnDraw();
	pc2.stop();

	PerformanceCounter pc3(PerformanceCounter::AUTO_START);
	/* copy to window */
	CPaintDC dc(this);
	Gdiplus::Graphics gThis(dc.m_hDC);
	gThis.DrawImage(&bmp, Gdiplus::PointF(0, 0));
	pc3.stop();
}


void GraphicsLabel::OnParentNotify(UINT message, LPARAM lParam)
{
	CDialog::OnParentNotify(message, lParam);
	DWORD ev_msg = LOWORD(message);

	// get the point and translate it
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	if (ev_msg == WM_LBUTTONDOWN)
	{
		
	}
	else if (ev_msg == WM_MBUTTONDOWN)
	{
	}	
	else if (ev_msg == WM_RBUTTONDOWN)
	{
	}
}

BOOL GraphicsLabel::OnInitDialog()
{
	CDialog::OnInitDialog();
	ModifyStyleEx(WS_EX_NOPARENTNOTIFY , 0, 0);
	return TRUE;
}

LRESULT GraphicsLabel::OnNcHitTest(CPoint point)
{
	return HTCLIENT;
}

void GraphicsLabel::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_Surface) m_Surface->OnLButtonDown(point.x, point.y);
	CDialog::OnLButtonDown(nFlags, point);
}

void GraphicsLabel::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_Surface) m_Surface->OnLButtonUp(point.x, point.y);
	CDialog::OnLButtonUp(nFlags, point);
}

void GraphicsLabel::OnMButtonDown(UINT nFlags, CPoint point)
{
	if (m_Surface) m_Surface->OnMButtonDown(point.x, point.y);
	CDialog::OnMButtonDown(nFlags, point);
}

void GraphicsLabel::OnMButtonUp(UINT nFlags, CPoint point)
{
	if (m_Surface) m_Surface->OnMButtonUp(point.x, point.y);
	CDialog::OnMButtonUp(nFlags, point);
}

void GraphicsLabel::OnMButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_Surface) m_Surface->OnMButtonDoubleClick(point.x, point.y);
	CDialog::OnMButtonDblClk(nFlags, point);
}

void GraphicsLabel::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_Surface) m_Surface->OnRButtonDown(point.x, point.y);
	CDialog::OnRButtonDown(nFlags, point);
}

void GraphicsLabel::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (m_Surface) m_Surface->OnRButtonUp(point.x, point.y);
	CDialog::OnRButtonUp(nFlags, point);
}



BOOL GraphicsLabel::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


void GraphicsLabel::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_Surface) m_Surface->OnMouseMove(point.x, point.y, nFlags);
	CDialog::OnMouseMove(nFlags, point);
}
