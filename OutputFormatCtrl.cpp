// OutputFormatCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "OutputFormatCtrl.h"


// OutputFormatCtrl dialog

IMPLEMENT_DYNAMIC(OutputFormatCtrl, CDialog)

OutputFormatCtrl::OutputFormatCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(OutputFormatCtrl::IDD, pParent)
{

}

OutputFormatCtrl::~OutputFormatCtrl()
{
}

void OutputFormatCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INPUTBOX, c_Text);
	DDX_Control(pDX, IDC_EDIT, c_Edit);
}


BEGIN_MESSAGE_MAP(OutputFormatCtrl, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EDIT, &OutputFormatCtrl::OnBnClickedEdit)
END_MESSAGE_MAP()


// OutputFormatCtrl message handlers

void OutputFormatCtrl::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	const int BUTTON_WIDTH		= 30;
	const int BUTTON_SEPERATOR	= 2;

	// move controls
	if (IsWindow(c_Text))
	{
		c_Text.MoveWindow(0, 0, cx - BUTTON_WIDTH - BUTTON_SEPERATOR, cy);
		c_Edit.MoveWindow(cx - BUTTON_WIDTH, 0, BUTTON_WIDTH, cy);
	}
}

void OutputFormatCtrl::SetInfo(const MessageInfo& Info)
{
	info = Info;
	c_Text = info.message;
}

MessageInfo	OutputFormatCtrl::GetInfo()
{
	MessageInfo info = this->info;
	info.message = c_Text.GetText();
	return info;
}
void OutputFormatCtrl::OnBnClickedEdit()
{
	if (MessageInfo_EditBox(info, this) == true)
		c_Text = info.message;

}
