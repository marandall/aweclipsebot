// ModalPropertyDialog2.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "ModalPropertyDialog2.h"


// ModalPropertyDialog2 dialog

IMPLEMENT_DYNAMIC(ModalPropertyDialog2, CDialog)

ModalPropertyDialog2::ModalPropertyDialog2(CWnd* pParent /*=NULL*/)
	: CDialog(ModalPropertyDialog2::IDD, pParent)
{
	init_callback = 0;
}

ModalPropertyDialog2::~ModalPropertyDialog2()
{
}

void ModalPropertyDialog2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POSITION, c_position);
	DDX_Control(pDX, IDOK, c_ok);
}


BEGIN_MESSAGE_MAP(ModalPropertyDialog2, CDialog)
	ON_BN_CLICKED(IDOK, &ModalPropertyDialog2::OnBnClickedOk)
END_MESSAGE_MAP()


// ModalPropertyDialog2 message handlers

BOOL ModalPropertyDialog2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// window
	SetWindowText(title);

	// add the property script control
	c_PsHelper.create_control(*this, c_position);
	c_PsHelper.create_group(0, title, *m_builderBase);

	// call back handler
	if (init_callback) 
		init_callback->run();

	c_PsHelper.attributes_to_dialog();
	c_PsHelper.window->show_group(0);

	// get the height
	int height = c_PsHelper.window->getGroupHeight(0);
	
	// move window to position
	RECT r;
	GetWindowRect(&r);

	// change location
	c_PsHelper.window->MoveWindow(10, 10, r.right - r.left - 20, height + 5);

	// resize height
	r.bottom = r.top + height + 80;
	MoveWindow(&r);

	// update the button
	c_ok.MoveWindow(r.right - r.left - 10 - 100, height + 20, 100, 30);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void ModalPropertyDialog2::OnBnClickedOk()
{
	c_PsHelper.list.front()->dialog_to_remote_attributes();
	OnOK();
}
