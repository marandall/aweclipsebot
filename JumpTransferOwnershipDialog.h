#pragma once


// JumpTransferOwnershipDialog dialog

class JumpTransferOwnershipDialog : public CDialog
{
	DECLARE_DYNAMIC(JumpTransferOwnershipDialog)

public:
	JumpTransferOwnershipDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~JumpTransferOwnershipDialog();

// Dialog Data
	enum { IDD = IDD_JUMP_TRANSFER_OWNERSHIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int v_citizen;
};
