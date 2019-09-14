#pragma once
#include "textbox.h"
#include "afxwin.h"
#include "messages.h"
#include "resource.h"

// OutputFormatCtrl dialog

class OutputFormatCtrl : public CDialog
{
	DECLARE_DYNAMIC(OutputFormatCtrl)

public:
	OutputFormatCtrl(CWnd* pParent = NULL);   // standard constructor
	virtual ~OutputFormatCtrl();

// Dialog Data
	enum { IDD = IDD_OUTPUT_FORMAT_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	void		SetInfo(const MessageInfo& Info);
	MessageInfo	GetInfo();

public:
	MessageInfo	info;
	TextBox		c_Text;
	CButton		c_Edit;
	afx_msg		void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedEdit();
};
