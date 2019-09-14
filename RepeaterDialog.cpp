// RepeaterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "RepeaterDialog.h"
#include "CIS_Dispatch.h"
#include "GlobalGui.h"

// RepeaterDialog dialog

IMPLEMENT_DYNAMIC(RepeaterDialog, CDialog)

RepeaterDialog::RepeaterDialog(CWnd* pParent /*=NULL*/)
	: CDialog(RepeaterDialog::IDD, pParent)
	, v_InputBox(_T(""))
	, v_Output(_T(""))
{

}

RepeaterDialog::~RepeaterDialog()
{
}

void RepeaterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INPUT, v_InputBox);
	DDX_Control(pDX, IDC_QUEUE, c_Queue);
	DDX_Control(pDX, IDC_MPS, c_Mps);
	DDX_Text(pDX, IDC_OUTPUT, v_Output);
	DDX_Control(pDX, IDC_OUTPUT, c_OutputBox);
	DDX_Control(pDX, IDC_PAUSE, c_Paused);
}


BEGIN_MESSAGE_MAP(RepeaterDialog, CDialog)
	ON_BN_CLICKED(IDC_BEGIN, &RepeaterDialog::OnBnClickedBegin)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SAY, &RepeaterDialog::OnBnClickedSay)
	ON_BN_CLICKED(IDC_SAY2, &RepeaterDialog::OnBnClickedSay2)
	ON_BN_CLICKED(IDC_SAY3, &RepeaterDialog::OnBnClickedSay3)
	ON_WM_NCDESTROY()
	ON_BN_CLICKED(IDC_CLEAR, &RepeaterDialog::OnBnClickedClear)
END_MESSAGE_MAP()


// RepeaterDialog message handlers

void RepeaterDialog::OnBnClickedBegin()
{
	UpdateData(TRUE);

	/* split items up for output */
	SplitString lines(v_InputBox, "\r\n", -1);
	for (size_t i = 0; i < lines.size(); i++)
		c_Queue.InsertString(c_Queue.GetCount(), String(lines[i]) + " mps");
	c_Queue.SelectString(-1, "1 mps");
	RESIZE_LISTBOX(c_Mps);
}

void RepeaterDialog::OnTimer(UINT_PTR nIDEvent)
{
	if (c_Queue.GetCount() == 0 || c_Paused.GetCheck() == TRUE)
		return;

	String cmd;

	/* post off our next message */
	int ips = (int)c_Mps.GetItemData(c_Mps.GetCurSel());
	for (int i = 0; i < ips; i++) {
		if (c_Queue.GetCount() > 0) {
			CString strText;
			c_Queue.GetText(0, strText);

			/* dispatch this item */
			cmd.Format("%s %s", (const char*)v_Output, strText);
			CIS::CommandInterpreter::beginConsole(cmd, true);

			/* pop the item */
			c_Queue.DeleteString(0);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL RepeaterDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	OnBnClickedSay();

	for (int i = 1; i < 10; i++)
		c_Mps.SetItemData(
			c_Mps.InsertString(c_Mps.GetCount(), 
			ToString(i)), (DWORD_PTR)i);

	SetTimer(0, 1000, 0);
	return TRUE;
}

void RepeaterDialog::OnBnClickedSay()
{
	c_OutputBox.SetWindowTextA(_T("say {arg}"));
}

void RepeaterDialog::OnBnClickedSay2()
{
	c_OutputBox.SetWindowTextA(_T("cout/whisptgt {arg}"));
}

void RepeaterDialog::OnBnClickedSay3()
{
	c_OutputBox.SetWindowTextA(_T("cmsg -b black, {arg}"));
}

void RepeaterDialog::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	delete this;
}

void RepeaterDialog::OnBnClickedClear()
{
	while (c_Queue.GetCount())
		c_Queue.DeleteString(0);
}
