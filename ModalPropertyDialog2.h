#pragma once
#include "PropertyScriptCtrl.h"
#include "afxwin.h"

// ModalPropertyDialog2 dialog

class ModalPropertyDialog2 : public CDialog
{
	DECLARE_DYNAMIC(ModalPropertyDialog2)

public: // modal callback
	class ICallback
	{
	public:
		virtual ~ICallback()
		{
		}

		virtual void run() = 0;
	};

public:
	ModalPropertyDialog2(CWnd* pParent = NULL);   // standard constructor
	virtual ~ModalPropertyDialog2();

public: // property script builder
	PropertyBuilderHelper	c_PsHelper;
	PropertyBuilderBase*	m_builderBase;
	CString								title;
	ICallback*						init_callback;

// Dialog Data
	enum { IDD = IDD_MODAL_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic c_position;
	afx_msg void OnBnClickedOk();

	CButton c_ok;
};
