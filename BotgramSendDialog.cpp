// BotgramSendDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "BotgramSendDialog.h"
#include "botgramprotocol.h"

// BotgramSendDialog dialog

IMPLEMENT_DYNAMIC(BotgramSendDialog, CDialog)

BotgramSendDialog::BotgramSendDialog(CWnd* pParent /*=NULL*/)
	: CDialog(BotgramSendDialog::IDD, pParent)
	, v_to(_T(""))
	, v_botname(_T(""))
{

}

BotgramSendDialog::~BotgramSendDialog()
{
}

void BotgramSendDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, c_Message);
	DDX_Control(pDX, IDC_SUBJECT, c_Subject);
	DDX_Control(pDX, IDC_TO, c_Target);
	DDX_Control(pDX, IDC_NAME, c_BotName);
	DDX_Text(pDX, IDC_TO, v_to);
	DDX_Text(pDX, IDC_NAME, v_botname);
}


BEGIN_MESSAGE_MAP(BotgramSendDialog, CDialog)
	ON_BN_CLICKED(IDC_SEND, &BotgramSendDialog::OnBnClickedSend)
END_MESSAGE_MAP()


// BotgramSendDialog message handlers

void BotgramSendDialog::OnBnClickedSend()
{
	// build the email message
	BotgramProtocol::SendingThread thread;
	int	to = atoi(c_Target.GetText());

	// body information
	String body;
	body.Format("To: %d\nFrom: %s (%d)\nSubject: %s\n\n%s", 
		atoi(c_Target.GetText()), bot._string(AW_LOGIN_PRIVILEGE_NAME), bot._int(AW_LOGIN_OWNER),
		c_Subject.GetText(),
		c_Message.GetText());

	// botgram send
	thread.content_type		= "botmail/text";
	thread.target_citizen	= to;
	thread.target_user		= c_BotName.GetText();

	// convert the body into packets
	thread.open(body);
	BotgramProtocol::SendingKernel::getInstance().add(thread);

	// send
	MessageBox("Message Sent!");
	EndDialog(IDOK);
}
