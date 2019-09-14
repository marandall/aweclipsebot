#pragma once
#include "afxwin.h"
#include "EditExtended.h"

#define UWM_GETHOSTADDR_RESULT		(WM_USER + 1)

// EclipseInputDialog dialog

class EclipseInputDialog : public CDialog
{
	DECLARE_DYNAMIC(EclipseInputDialog)

public:
	EclipseInputDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~EclipseInputDialog();

// Dialog Data
	enum { IDD = IDD_ECLIPSE_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	int	getStdHeight() const { return 67; }		// precalculated height

public:
	void SwitchToCheck(CButton& Button);
	CButton c_Whisper;
	CButton c_Commands;
	CButton c_Console;
	CButton c_Speak;
	EditExtended c_InputBox;
	CFont	m_boxFont;

public:
	afx_msg void OnBnClickedSpeak();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnBnClickedWhisper();
public:
	afx_msg void OnBnClickedCommands();
public:
	afx_msg void OnBnClickedConsole();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	CButton c_ConsoleMode;
public:
	afx_msg void OnBnClickedConsoleMode();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnClose();
protected:
	virtual void OnOK();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
