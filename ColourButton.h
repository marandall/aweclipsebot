#pragma once


// ColourButton

class ColourButton : public CButton
{
	DECLARE_DYNAMIC(ColourButton)

public:
	ColourButton();
	virtual ~ColourButton();

protected:
	DECLARE_MESSAGE_MAP()
	COLORREF	m_colour;

public:
	COLORREF getColour() const { return m_colour;}
	ColourButton& operator=(COLORREF C) { m_colour = C; if (IsWindow(m_hWnd)) { Invalidate(); } return *this; }

public:
	afx_msg void OnPaint();
public:
	afx_msg void OnBnClicked();
};