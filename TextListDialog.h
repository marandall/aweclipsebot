#pragma once


// TextListDialog dialog

class TextListDialog : public CDialog
{
	DECLARE_DYNAMIC(TextListDialog)

public:
	TextListDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~TextListDialog();

// Dialog Data
	enum { IDD = IDD_TEXT_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString v_List;
};
