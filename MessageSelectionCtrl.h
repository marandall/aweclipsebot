#pragma once
#include "afxwin.h"
#include "messages.h"

// MessageSelectionCtrl dialog

class MessageSelectionCtrl : public CDialog
{
	DECLARE_DYNAMIC(MessageSelectionCtrl)

public:
	MessageSelectionCtrl(CWnd* pParent = NULL);   // standard constructor
	virtual ~MessageSelectionCtrl();

// Dialog Data
	enum { IDD = IDD_MESSAGE_FORMAT_CHILD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	MessageInfo* m_pInfo;

public:
	CButton c_Edit;
	afx_msg void OnBnClickedEdit();
};
