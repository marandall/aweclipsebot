#pragma once
#include "afxwin.h"

// UniverseOptionsDialog dialog
namespace AW { class UniverseOptions; }

class UniverseOptionsDialog : public CDialog
{
	DECLARE_DYNAMIC(UniverseOptionsDialog)

public:
	UniverseOptionsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~UniverseOptionsDialog();

// Dialog Data
	enum { IDD = IDD_UNIVERSE_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void		ReloadFromSdk();
	void		Display(AW::UniverseOptions& Options);
	void		Extract(AW::UniverseOptions& Options);

	DECLARE_MESSAGE_MAP()
public:
	int			v_BrowserMin;
	int			v_BrowserRelease;
	int			v_BrowserBeta;
	int			v_WorldMin;
	int			v_WorldRelease;
	int			v_WorldBeta;
	CString		v_Greeting;
	CString		v_StartWorld;
	CString		v_NotepadUrl;
	BOOL		v_AllowTourists;
protected:
	virtual void OnOK();
public:
	CEdit c_BrowserMinimum;
	CEdit c_BrowserRelease;
	CEdit c_BrowserBeta;
	CEdit c_WorldMinimum;
	CEdit c_WorldRelease;
	CEdit c_WorldBeta;
	CEdit c_Greeting;
	CEdit c_StartWorld;
	CEdit c_SearchUrl;
	CEdit c_NotepadUrl;
	CButton c_AllowTourists;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnNcDestroy();
	afx_msg void OnBnClickedReload();
	afx_msg void OnDataExchange_LoadFile();
	afx_msg void OnDataExchange_SaveFile();
public:
	CString v_SearchUrl;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedMakeThis();
};
