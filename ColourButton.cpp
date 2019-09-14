// olourButton.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "ColourButton.h"


// ColourButton

IMPLEMENT_DYNAMIC(ColourButton, CButton)

ColourButton::ColourButton()
{

}

ColourButton::~ColourButton()
{
}


BEGIN_MESSAGE_MAP(ColourButton, CButton)
	ON_WM_PAINT()
	ON_CONTROL_REFLECT(BN_CLICKED, &ColourButton::OnBnClicked)
END_MESSAGE_MAP()



// ColourButton message handlers



void ColourButton::OnPaint()
{
	RECT wnd;
	GetWindowRect(&wnd);
	ScreenToClient(&wnd);

	CPaintDC dc(this); // device context for painting

	CBrush br(m_colour);
	dc.FillRect(&wnd, &br);

	CPen pn(PS_SOLID, 1, (COLORREF)0);
	dc.DrawEdge(&wnd, EDGE_ETCHED, BF_RECT);
}

void ColourButton::OnBnClicked()
{
	CColorDialog dialog(m_colour, 0, this);
	if (dialog.DoModal() == IDOK)
		m_colour = dialog.GetColor();
	Invalidate();
}

