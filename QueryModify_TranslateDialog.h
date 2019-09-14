#pragma once
#include "textbox.h"


// QueryModify_TranslateDialog dialog

class QueryModify_TranslateDialog : public CDialog
{
	DECLARE_DYNAMIC(QueryModify_TranslateDialog)

public:
	QueryModify_TranslateDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~QueryModify_TranslateDialog();

// Dialog Data
	enum { IDD = IDD_QUERY_MODIFY_TRANSLATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	TextBox c_Source;
public:
	TextBox c_Destination;
public:
	TextBox c_Operation;
public:
	afx_msg void OnBnClickedUnselect4();
public:
	afx_msg void OnBnClickedTranslate();
public:
	afx_msg void OnBnClickedTranslate2();
};
