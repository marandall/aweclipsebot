#pragma once


// JumpEditorDialog dialog

class JumpEditorDialog : public CDialog
{
	DECLARE_DYNAMIC(JumpEditorDialog)

public:
	JumpEditorDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~JumpEditorDialog();

// Dialog Data
	enum { IDD = IDD_JUMP_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString v_name;
	CString v_coordinates;
	int v_owner;
};
