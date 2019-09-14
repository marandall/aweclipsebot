// UniverseCitizenDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "UniverseCitizenDialog.h"
#include "awsdk_citizen.h"
#include "CitizenLookup_Kernel.h"
#include "CallbackQueue.h"
#include "DateTimeDialog.h"

class UCDLookupByIdentifier : public CitizenLookup::LookupBase
{
public:
	Umbra::Variant uv_dialog;

	void Result(AW::Citizen& Info, int Reason) 
	{
		// attempt to check if the dialog is still open
		Umbra::RemoteClass* pRemote = uv_dialog.toRemoteClass();
		UniverseCitizenDialog* pDialog = static_cast<UniverseCitizenDialog*>(pRemote);
		if (pDialog == NULL) 
			return;

		// push to dialog
		pDialog->info_to_dialog(Info, Reason);
	}
};

class UCDLookupSequence : public CitizenLookup::LookupBase
{
public:
	Umbra::Variant uv_dialog;

	void Result(AW::Citizen& Info, int Reason) 
	{
		// attempt to check if the dialog is still open
		Umbra::RemoteClass* pRemote = uv_dialog.toRemoteClass();
		UniverseCitizenDialog* pDialog = static_cast<UniverseCitizenDialog*>(pRemote);
		if (pDialog == NULL) 
			return;

		// push to dialog
		pDialog->info_to_dialog(Info, Reason);
	}
};

class UCD_ActionBase : public CallbackQueue::Target
{
public: // the umbra variable for this dialog
	Umbra::Variant uv_dialog;

public: // tracking info
	UniverseCitizenDialog* getDialog()
	{
		Umbra::RemoteClass* pRemote = uv_dialog.toRemoteClass();
		UniverseCitizenDialog* pDialog = static_cast<UniverseCitizenDialog*>(pRemote);
		return pDialog;
	}
};

class UCD_CitizenAdd : public UCD_ActionBase
{
public:
	void handle_result(EclipseBot& Client, int Result)
	{
		// lookup the dialog
		UniverseCitizenDialog* pDialog = getDialog();
		if (pDialog == NULL) return;

		// query the bot
		AW::Citizen citizen;
		citizen.from_sdk(Client);

		// display on screen
		pDialog->info_to_dialog(citizen, Result);
		if (Result)
		{
			String log;
			log.Format("Unable to add %s; rc %d (%s)", 
				citizen.getName(), Result, AW::Bot::GetReasonCode(Result));
			pDialog->Log(log);
		}
	}
};

class UCD_CitizenChange : public UCD_ActionBase
{
public:
	void handle_result(EclipseBot& Client, int Result)
	{
		// lookup the dialog
		UniverseCitizenDialog* pDialog = getDialog();
		if (pDialog == NULL) return;

		// query the bot
		AW::Citizen citizen;
		citizen.from_sdk(Client);

		// display on screen
		pDialog->info_to_dialog(citizen, Result);
		if (Result)
		{
			String log;
			log.Format("Unable to change %d; rc %d (%s)", 
				citizen.getCitizen(), Result, AW::Bot::GetReasonCode(Result));
			pDialog->Log(log);
		}
	}
};

class UCD_CitizenDelete : public UCD_ActionBase
{
public:
	void handle_result(EclipseBot& Client, int Result)
	{
		// lookup the dialog
		UniverseCitizenDialog* pDialog = getDialog();
		if (pDialog == NULL) return;

		// query the bot
		AW::Citizen citizen;
		citizen.from_sdk(Client);

		// display on screen
		if (Result)
		{
			String log;
			log.Format("Unable to delete %d; rc %d (%s)", 
				citizen.getCitizen(), Result, AW::Bot::GetReasonCode(Result));
			pDialog->Log(log);
		}
		else
		{
			String log;
			log.Format("Deleted user #%d", 
				citizen.getCitizen(), Result, AW::Bot::GetReasonCode(Result));
			pDialog->Log(log);
		}
	}
};

// UniverseCitizenDialog dialog
IMPLEMENT_DYNAMIC(UniverseCitizenDialog, CDialog)

UniverseCitizenDialog::UniverseCitizenDialog(CWnd* pParent /*=NULL*/)
	: CDialog(UniverseCitizenDialog::IDD, pParent)
{
	auto_query = 0;
}

UniverseCitizenDialog::~UniverseCitizenDialog()
{
}

void UniverseCitizenDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NUMBER, c_Number);
	DDX_Control(pDX, IDC_EDIT6, c_Name);
	DDX_Control(pDX, IDC_EMAIL1, c_Email);
	DDX_Control(pDX, IDC_PASSWORD1, c_Password);
	DDX_Control(pDX, IDC_BOTLIMIT, c_BotLimit);
	DDX_Control(pDX, IDC_PRIVPASS, c_PrivilegePassword);
	DDX_Control(pDX, IDC_HOMEPAGE, c_Homepage);
	DDX_Control(pDX, IDC_COMMENTS, c_Comments);
	DDX_Control(pDX, IDC_IMMIGRATED, c_Immigrated);
	DDX_Control(pDX, IDC_EXPIRES, c_Expires);
	DDX_Control(pDX, IDC_TOTAL_TIME, c_TotalTime);
	DDX_Control(pDX, IDC_LAST_IP, c_LastAddress);
	DDX_Control(pDX, IDC_ENABLED, c_Enabled);
	DDX_Control(pDX, IDC_BETA, c_Beta);
	DDX_Control(pDX, IDC_TRIAL, c_Trial);
	DDX_Control(pDX, IDC_PAV, c_PAV);
	DDX_Control(pDX, IDC_DISABLE_TELEGRAMS, c_DisableTelegrams);
	DDX_Control(pDX, IDC_EDIT2, c_Log);
	DDX_Control(pDX, IDC_LAST_LOGIN, c_LastLogin);
}


BEGIN_MESSAGE_MAP(UniverseCitizenDialog, CDialog)
	ON_BN_CLICKED(IDC_LOOKUP_NUMBER, &UniverseCitizenDialog::OnBnClickedLookupNumber)
	ON_BN_CLICKED(IDC_LOOKUP_NAME, &UniverseCitizenDialog::OnBnClickedLookupName)
	ON_BN_CLICKED(IDC_LOOKUP_NEXT, &UniverseCitizenDialog::OnBnClickedNext)
	ON_WM_NCDESTROY()
	ON_BN_CLICKED(IDC_LOOKUP_PREVIOUS, &UniverseCitizenDialog::OnBnClickedLookupPrevious)
	ON_BN_CLICKED(IDC_ADD_CITIZEN, &UniverseCitizenDialog::OnBnClickedAddCitizen)
	ON_BN_CLICKED(IDC_UPDATE_CITIZEN, &UniverseCitizenDialog::OnBnClickedUpdateCitizen)
	ON_BN_CLICKED(IDC_DELETE_CITIZEN, &UniverseCitizenDialog::OnBnClickedDeleteCitizen)
	ON_BN_CLICKED(IDC_EDIT_EXPIRY, &UniverseCitizenDialog::OnBnClickedEditExpiry)
END_MESSAGE_MAP()


// UniverseCitizenDialog message handlers
String genericTimeToDate(int Timestamp)
{
	const char* basic_representation = "%H:%M:%S";
	CTime tm = Timestamp;
	
	// epoch equals never
	if (Timestamp == 0) 
		return "Never";

	// formt as basic rep
	return tm.Format(basic_representation);
}

void UniverseCitizenDialog::info_to_dialog(const AW::Citizen& Info, int Reason)
{
	// lookup error
	if (Reason)
	{
		String log;
		log.Format("Unable to lookup citizen; rc %d (%s)", Reason, AW::Bot::GetReasonCode(Reason));
		Log(log);
	}
	else
	{
		// time modules for sdk
		String immigrated = genericTimeToDate(Info.getImmigrationTime());
		String expires	 = genericTimeToDate(Info.getExpirationTime());
		String last_login = genericTimeToDate(Info.getLastLogin());

		m_current_citizen = Info.getNumber();
		c_Number			.OverloadedSet	(Info.getNumber());
		c_Name				.OverloadedSet	(Info.getName());
		c_Email				.OverloadedSet	(Info.getEmail());
		c_Password			.OverloadedSet	(Info.getPassword());
		c_BotLimit			.OverloadedSet	(Info.getBotLimit());
		c_PrivilegePassword	.OverloadedSet	(Info.getPrivilegePassword());
		c_Homepage			.OverloadedSet	(Info.getUrl());
		c_Comments			.OverloadedSet	(Info.getComment());
		c_Immigrated		.OverloadedSet	(immigrated);
		c_Expires			.OverloadedSet	(expires);
		c_TotalTime			.OverloadedSet	(Info.getTotalTime());
		c_LastAddress		.OverloadedSet	(AW::_XConvert::long_to_ip_string(Info.getLastAddress()));
		c_Enabled			.SetCheck		(Info.getEnabled());
		c_Beta				.SetCheck		(Info.getBeta());
		c_Trial				.SetCheck		(Info.getTrial());
		c_PAV				.SetCheck		(Info.getCavEnabled());
		c_DisableTelegrams	.SetCheck		(ConvBool(Info.getDisableTelegrams()));
		m_last_good	= Info;

		// log response
		String log;
		log.Format("Lookup Successful of #%d", Info.getCitizen());
		Log(log);
	}
}

void UniverseCitizenDialog::dialog_to_info(AW::Citizen& Info)
{
	Info.setNumber				(c_Number			.getInt()		);
	Info.setName				(c_Name				.getString()	);
	Info.setEmail				(c_Email			.getString()	);
	Info.setPassword			(c_Password			.getString()	);
	Info.setBotLimit			(c_BotLimit			.getInt()		);
	Info.setPrivilegePassword	(c_PrivilegePassword.getString()	);
	Info.setUrl				(c_Homepage			.getString()	);
	Info.setComment			(c_Comments			.getString()	);
	Info.setEnabled			(c_Enabled			.GetCheck()		);
	Info.setBeta				(c_Beta				.GetCheck()		);
	Info.setTrial				(c_Trial			.GetCheck()		);
	Info.setCavEnabled			(c_PAV				.GetCheck()		);
	
	// build privacy bitmask
	int privacy = 0;
	if (c_DisableTelegrams.GetCheck()) privacy|= AW_PRIVACY_BLOCK_TELEGRAMS;
	Info.setPrivacy			(privacy);
}

void UniverseCitizenDialog::Log(String Msg)
{
	// replace item
	int len = c_Log.GetText().GetLength();
	c_Log.SetSel(len, len);

	// work out string and append it
	String msg;
	msg.Format("[%s] %s\r\n\r\n", getTime(), Msg);
	c_Log.ReplaceSel(msg);

	// select the bottom section again
	len+= Msg.GetLength();
	c_Log.SetSel(len, len);
}

void UniverseCitizenDialog::OnBnClickedLookupNumber()
{
	// lookup the avatar information
	UCDLookupByIdentifier* pLookup = new UCDLookupByIdentifier();
	pLookup->uv_dialog = *this;

	// query the citizen
	int citizen = atoi(c_Number.GetText());
	CitizenLookup::g_Kernel.lookup_by_number(citizen, pLookup);
	Log("Looking up citizen...");
}

void UniverseCitizenDialog::OnBnClickedLookupName()
{
	// lookup the avatar information
	UCDLookupByIdentifier* pLookup = new UCDLookupByIdentifier();
	pLookup->uv_dialog = *this;

	// query the citizen
	String citizen = c_Name.GetText();
	CitizenLookup::g_Kernel.lookup_by_name(citizen, pLookup);
	Log("Looking up citizen name...");
}

void UniverseCitizenDialog::OnBnClickedNext()
{
	// lookup the avatar information
	UCDLookupSequence* pLookup = new UCDLookupSequence();
	pLookup->uv_dialog = *this;

	// query the citizen
	String citizen = c_Name.GetText();
	CitizenLookup::g_Kernel.lookup_citizen_next(m_current_citizen, pLookup);
	Log("Looking up next citizen...");
}

void UniverseCitizenDialog::OnBnClickedLookupPrevious()
{
	// lookup the avatar information
	UCDLookupSequence* pLookup = new UCDLookupSequence();
	pLookup->uv_dialog = *this;

	// query the citizen
	String citizen = c_Name.GetText();
	CitizenLookup::g_Kernel.lookup_citizen_previous(m_current_citizen, pLookup);
	Log("Looking up previous citizen...");
}

void UniverseCitizenDialog::OnNcDestroy()
{
	__super::OnNcDestroy();
	delete this;
}


void UniverseCitizenDialog::OnBnClickedAddCitizen()
{
	// lookup the user
	UCD_CitizenAdd* pLookup = new UCD_CitizenAdd();
	pLookup->uv_dialog = *this;

	// read citizen information from screen
	AW::Citizen citizen;
	dialog_to_info(citizen);

	// log
	String log;
	log.Format("Attempting to create citizen '%s'", citizen.getName());
	Log(log);

	// trigger the execution
	bot.citizen_add(citizen, pLookup);
}

void UniverseCitizenDialog::OnBnClickedUpdateCitizen()
{
	// lookup the user
	UCD_CitizenChange* pLookup = new UCD_CitizenChange();
	pLookup->uv_dialog = *dynamic_cast<Umbra::RemoteClass*>(this);

	// read citizen information from screen
	AW::Citizen citizen = m_last_good;
	dialog_to_info(citizen);

	// log
	String log;
	log.Format("Changing citizen #%d - %s", m_current_citizen, m_last_good.getName());
	Log(log);

	// trigger the execution
	bot.citizen_change(citizen, pLookup);
}

void UniverseCitizenDialog::OnBnClickedDeleteCitizen()
{
	// lookup the user
	UCD_CitizenDelete* pLookup = new UCD_CitizenDelete();
	pLookup->uv_dialog = *dynamic_cast<Umbra::RemoteClass*>(this);

	// read citizen information from screen
	AW::Citizen citizen = m_last_good;
	dialog_to_info(citizen);

	// log
	String log;
	log.Format("Deleting citizen #%d - %s", m_current_citizen, m_last_good.getName());
	Log(log);

	// trigger the execution
	bot.citizen_delete(m_current_citizen, pLookup);
}

BOOL UniverseCitizenDialog::OnInitDialog()
{
	__super::OnInitDialog();

	// align
	CenterWindow();

	// a citizen number to look up
	if (auto_query)
	{
		c_Number.OverloadedSet(auto_query);
		OnBnClickedLookupNumber();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void UniverseCitizenDialog::OnBnClickedEditExpiry()
{
	DateTimeDialog dlg;
	dlg.timestamp = m_last_good.getExpirationTime();
	if (dlg.DoModal())
	{
	}
}
