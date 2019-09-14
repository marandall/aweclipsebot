// UniverseEjectionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "UniverseEjectionDialog.h"
#include "IpToDNS.h"

UniverseEjectionDialog* UniverseEjectionDialog::m_pCurWnd;

void UniverseEjectionDialog_DNSResolved(Async::Header& Header)
{
	/* ignore all but this address */
	HOSTENT* pHostEnt = (HOSTENT*)Header.p_result;
	if (UniverseEjectionDialog::m_pCurWnd) {
		if (pHostEnt == NULL)
			UniverseEjectionDialog::m_pCurWnd->OnDnsResolved(Header.u_msg, "Not Available");
		else
			UniverseEjectionDialog::m_pCurWnd->OnDnsResolved(Header.u_msg, pHostEnt->h_name);
	}
}

// UniverseEjectionDialog dialog

IMPLEMENT_DYNAMIC(UniverseEjectionDialog, CDialog)

UniverseEjectionDialog::UniverseEjectionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(UniverseEjectionDialog::IDD, pParent)
{
	m_pCurWnd = this;
}

UniverseEjectionDialog::~UniverseEjectionDialog()
{
	m_pCurWnd = NULL;
}

void UniverseEjectionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IP_ADDRESS, c_IP);
	DDX_Control(pDX, IDC_DNS, c_DNS);
	DDX_Control(pDX, IDC_CREATED, c_Created);
	DDX_Control(pDX, IDC_EXPIRES, c_Expires);
	DDX_Control(pDX, IDC_COMMENT, c_Comment);
	DDX_Control(pDX, IDC_REASON, c_Reason);
}


BEGIN_MESSAGE_MAP(UniverseEjectionDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1,	&UniverseEjectionDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_NEXT,		&UniverseEjectionDialog::OnBnClickedNext)
	ON_BN_CLICKED(IDC_ADD,		&UniverseEjectionDialog::OnBnClickedAdd)
END_MESSAGE_MAP()


// UniverseEjectionDialog message handlers
void UniverseEjectionDialog::OnDnsResolved(unsigned int IP, String Dns)
{
	/* update the dns if we have our header */
	if (this_addr == IP) c_DNS.SetWindowTextA(Dns);
}

void UniverseEjectionDialog::Display(int ReasonCode)
{
	this_addr = bot._int(AW_EJECTION_ADDRESS);
	String ip_addr  = AW::_XConvert::long_to_ip_string(bot._int(AW_EJECTION_ADDRESS));
	String err_code = AW::Bot::GetReasonCode(ReasonCode);

	c_IP.		SetWindowTextA(ip_addr);
	c_Comment.	SetWindowTextA(bot._string(AW_EJECTION_COMMENT));
	c_Reason.	SetWindowTextA(err_code);

	/* carry our dns resolve */
	c_DNS.		SetWindowTextA("Searching...");
	IpToDNS::g_Host.LookupNow(this_addr, &UniverseEjectionDialog_DNSResolved, NULL);

}

void UniverseEjectionDialog::OnBnClickedButton1()
{
	bot.Lock();
	Display(bot._universe_ejection_previous());
	bot.Unlock();
}

void UniverseEjectionDialog::OnBnClickedNext()
{
	bot.Lock();
	Display(bot._universe_ejection_next());
	bot.Unlock();
}

void UniverseEjectionDialog::OnBnClickedAdd()
{
	// TODO: Add your control notification handler code here
}

