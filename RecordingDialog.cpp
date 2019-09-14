// RecordingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "RecordingDialog.h"
#include "Recorder.h"

// RecordingDialog dialog

IMPLEMENT_DYNAMIC(RecordingDialog, CDialog)

RecordingDialog::RecordingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(RecordingDialog::IDD, pParent)
{

}

RecordingDialog::~RecordingDialog()
{
}

void RecordingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, c_events);
}


BEGIN_MESSAGE_MAP(RecordingDialog, CDialog)
	ON_BN_CLICKED(IDOK, &RecordingDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// RecordingDialog message handlers

void RecordingDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL RecordingDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	using namespace Recorder;

	// events
	c_events.SetExtendedStyle(c_events.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	// insert item
	c_events.InsertColumn(0, _T("Time"),    0, 70);
	c_events.InsertColumn(1, _T("Event"),   0, 120);
	c_events.InsertColumn(2, _T("Avatar"),  0, 120);
	c_events.InsertColumn(3, _T("Details"), 0, 500);

	// reference
	Recording& recording = Recording::getInstance();

	// display each item
	for (Recording::iterator itr = recording.begin(); 
		itr != recording.end();
		itr++)
	{
		u_ptr<Recorder::Record> record = *itr;

		// add items
		int id = c_events.InsertItem(c_events.GetItemCount(), MakeGuiTime(record->time));
		c_events.EditItem(id, 1, record->getRecordName());
		c_events.EditItem(id, 2, record->avatar->getName());
		c_events.EditItem(id, 3, record->getText());
	}

	return TRUE;
}
