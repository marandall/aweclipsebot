#pragma once
#include "textbox.h"
#include "afxwin.h"
#include "umbra/umbra_remoteclass.h"
#include "awsdk_citizen.h"

// UniverseCitizenDialog dialog
namespace AW
{
	class Citizen;
}

class UniverseCitizenDialog : public CDialog, Umbra::RemoteClass
{
	DECLARE_DYNAMIC(UniverseCitizenDialog)

public:
	UniverseCitizenDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~UniverseCitizenDialog();

// Dialog Data
	enum { IDD = IDD_UNIVERSE_CITIZEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public: // citizen information
	int			m_current_citizen;
	AW::Citizen	m_last_good; 
	int		auto_query;
	void	info_to_dialog(const AW::Citizen& Info, int Reason);
	void	dialog_to_info(AW::Citizen& Info);
	void	Log(String Msg);

public:
	TextBox c_Number;
	TextBox c_Name;
	TextBox c_Email;
	TextBox c_Password;
	TextBox c_BotLimit;
	TextBox c_PrivilegePassword;
	TextBox c_Homepage;
	TextBox c_Comments;
	TextBox c_Immigrated;
	TextBox c_Expires;
	TextBox c_TotalTime;
	TextBox c_LastAddress;
	CButton c_Enabled;
	CButton c_Beta;
	CButton c_Trial;
	CButton c_PAV;
	CButton c_DisableTelegrams;
	TextBox c_Log;

	afx_msg void OnBnClickedLookupNumber();
	afx_msg void OnBnClickedLookupName();
	afx_msg void OnBnClickedNext();
	afx_msg void OnNcDestroy();
	afx_msg void OnBnClickedLookupPrevious();
	afx_msg void OnBnClickedAddCitizen();
	afx_msg void OnBnClickedUpdateCitizen();
	TextBox c_LastLogin;
	afx_msg void OnBnClickedDeleteCitizen();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedEditExpiry();

};
