#pragma once


// FloodPreventionDialog dialog

class FloodPreventionDialog : public CDialog
{
	DECLARE_DYNAMIC(FloodPreventionDialog)

public:
	FloodPreventionDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~FloodPreventionDialog();

// Dialog Data
	enum { IDD = IDD_CHAT_FLOOD_PREVENTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int v_MaxPer5;
	int v_MaxIdentical;
	int v_AutoMute;
	int v_AutoMuteDuration;
	BOOL v_Enabled;
};
