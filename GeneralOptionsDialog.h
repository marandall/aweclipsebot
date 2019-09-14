#pragma once
#include "afxwin.h"
#include "CommonFeatures.h"
#include "PropertyScriptCtrl.h"
#include "afxcmn.h"

// GeneralOptionsDialog dialog

class GeneralOptionsDialog : public CDialog
{
public:
	static const int GOG_GREETINGS				= 0;
	static const int GOG_AUTO_RESPONDING	= 1;

private:
	DECLARE_DYNAMIC(GeneralOptionsDialog)

public:
	GeneralOptionsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~GeneralOptionsDialog();

// Dialog Data
	enum { IDD = IDD_GENERAL_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

protected: // common dialog interfaces
	CommonFeatures::AutoTemplates::GreetingsPropertyBuilder			m_dsGreetings;
	CommonFeatures::AutoTemplates::AutoRespondingPropertyBuilder	m_dsAutoResponding;

public: // our own controls
	PropertyScriptCtrl*		c_pProperties;
	CStatic					c_TrackingRect;

public:
	virtual BOOL OnInitDialog();
	CTabCtrl c_Tabs;
	afx_msg void OnTabs_Switch(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual void OnOK();
};
