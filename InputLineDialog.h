#pragma once


// InputLineDialog dialog

class InputLineDialog : public CDialog
{
	DECLARE_DYNAMIC(InputLineDialog)

public:
	InputLineDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~InputLineDialog();

// Dialog Data
	enum { IDD = IDD_INPUT_LINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CStringA title;
	virtual BOOL OnInitDialog();
public:
	CString v_Text;
	CString v_about;
	afx_msg void OnBnClickedOk();
};
