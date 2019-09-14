// TeleportDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "TeleportDialog.h"

#define TIMER_RECALCULATE	1

// TeleportDialog dialog

IMPLEMENT_DYNAMIC(TeleportDialog, CDialog)

TeleportDialog::TeleportDialog(CWnd* pParent /*=NULL*/)
	: CDialog(TeleportDialog::IDD, pParent)
	, v_Global(FALSE)
	, v_Invisible(FALSE)
	, v_String(_T(""))
	, v_World(_T(""))
{

}

TeleportDialog::~TeleportDialog()
{
}

void TeleportDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ID, c_Position);
	DDX_Control(pDX, IDC_COORDINATES, c_Coordinates);
	DDX_Check(pDX, IDC_GLOBAL_MODE, v_Global);
	DDX_Check(pDX, IDC_INVISIBLE, v_Invisible);
	DDX_Text(pDX, IDC_COORDINATES, v_String);
	DDX_Text(pDX, IDC_WORLD, v_World);
}


BEGIN_MESSAGE_MAP(TeleportDialog, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &TeleportDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// TeleportDialog message handlers

void TeleportDialog::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_RECALCULATE)
	{
		/* extract the string */
		CString pos_of;
		c_Coordinates.GetWindowTextA(pos_of);

		Location pos(pos_of);
		if (!pos.IsValid()) {
			c_Position.SetWindowTextA(_T("No Lock"));
		} else {
			String text;
			text.Format("(%d,%d,%d), %d", pos.getX(), pos.getY(), pos.getZ(), pos.getYAW() / 10);
			c_Position.SetWindowTextA(text);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL TeleportDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetTimer(TIMER_RECALCULATE, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void TeleportDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}