#pragma once
#include "listctrlgpx.h"
#include "afxwin.h"

namespace History
{
	class AvatarArticle;
};

// AvatarHistoryDialog dialog

class AvatarHistoryDialog : public CDialog
{
	DECLARE_DYNAMIC(AvatarHistoryDialog)

public:
	AvatarHistoryDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~AvatarHistoryDialog();

// Dialog Data
	enum { IDD = IDD_HISTORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	int m_si_name;		
	int m_si_session;
	int m_si_entered;
	int m_si_entered_pos;
	int m_si_citizen;
	int m_si_privilege;
	int m_si_privname;
	int m_si_ip;
	int m_si_dns;
	int m_si_exited;
	int m_si_exited_pos;

	void DisplayItem(History::AvatarArticle& Item);

public:
	ListCtrlGpx c_DataList;
	ListCtrlGpx c_History;
	afx_msg void OnHistoryList_ItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnNcDestroy();
public:
	CEdit c_SelectedText;
};
