#pragma once

#define UWM_CTRLKEY_RETURN	(WM_APP + 0x01)

class EditExtended : public CEdit
{
public:
	EditExtended(void);
	~EditExtended(void);
	DECLARE_MESSAGE_MAP()
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

protected:
	COLORREF	m_font_colour;
	COLORREF	m_bg_colour;
	CBrush*		m_br_back;

public:
	void		setEditColours(COLORREF Font, COLORREF Back);

private:
	CFont	courier;

public:
	void	setCourier();
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
};;