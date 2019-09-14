// QueryWorldProgressDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "QueryWorldProgressDialog.h"
#include "property_query_world.h"
#include "helpers_time.h"
#include "property_object.h"

// QueryWorldProgressDialog dialog

IMPLEMENT_DYNAMIC(QueryWorldProgressDialog, CDialog)

enum
{
	COL_INDEX,
	COL_RANGE,
	COL_DURATION,
	COL_OBJECTS,
	COL_STATUS
};

QueryWorldProgressDialog::QueryWorldProgressDialog(CWnd* pParent /*=NULL*/)
	: CDialog(QueryWorldProgressDialog::IDD, pParent)
{

}

QueryWorldProgressDialog::~QueryWorldProgressDialog()
{
}

void QueryWorldProgressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATUS, c_status);
	DDX_Control(pDX, IDC_PROGRESS, c_progress);
	DDX_Control(pDX, IDC_COUNT, c_objects);
	DDX_Control(pDX, IDC_COUNTOF, c_objectsof);
	DDX_Control(pDX, IDC_ESTIMATED_PERCENT, c_estimatedPercent);
	DDX_Control(pDX, IDC_TIME_ELAPSED, c_timeElapsed);
	DDX_Control(pDX, IDC_TIME_ETR, c_timeEstimatedDuration);
	DDX_Control(pDX, IDC_ERROR, c_error);
	DDX_Control(pDX, IDC_SECTORS, c_iterators);
}


BEGIN_MESSAGE_MAP(QueryWorldProgressDialog, CDialog)
	ON_BN_CLICKED(IDOK, &QueryWorldProgressDialog::OnBnClickedOk)
	ON_WM_TIMER()
	ON_COMMAND(ID_QUERY_BACKUPTOFILE, &QueryWorldProgressDialog::OnQuery_BackupToFile)
END_MESSAGE_MAP()


// QueryWorldProgressDialog message handlers


void QueryWorldProgressDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void QueryWorldProgressDialog::rebuild()
{
	// update process
	Querying::WorldQuery& wq = bot.getQueryWorld();

	// reset list
	c_iterators.DeleteAllItems();
	c_iterators.SetItemCount((int)wq.iterators.size());

	// update the building information
	for (size_t i = 0; i < wq.iterators.size(); i++)
	{
		Querying::IteratorRange& seg = wq.iterators[i];

		// iterator ranges
		String range;
		range.Format("%d%c - %d%c", 
			abs(seg.begin_x), seg.begin_x < 0 ? 'e' : 'w',
			abs(seg.end_x),   seg.end_x   < 0 ? 'e' : 'w');

		// update iterators
		int id = (int)c_iterators.InsertItem(i, ToString(i));
		c_iterators.EditItem(id, COL_RANGE, range);
		updateSegment(i);
	}
}

void QueryWorldProgressDialog::updateSegment(size_t Seg)
{
	// map for the world query status
	Querying::WorldQuery& wq		= bot.getQueryWorld();
	Querying::IteratorRange seg	= wq.iterators[Seg];
	int id = (int)Seg;

	// status
	String status, timer;
	if (seg.timer_start != 0)
	{
		if (seg.timer_end == 0)
		{
			status = "Working";
		}
		else
		{
			status = "Done";
		}
	}

	// timer text information
	if (seg.timer_start != 0)
	{
		if (seg.timer_end)
		{
			timer = Helpers::Time::SecondsToTimeStr(seg.timer_end - seg.timer_start);
		}
		else
		{
			timer = Helpers::Time::SecondsToTimeStr(_time64(0) - seg.timer_start);
		}
	}

	// update items
	c_iterators.EditItem(id, COL_DURATION, timer);
	c_iterators.EditItem(id, COL_OBJECTS, ToString(seg.total_objects));
	c_iterators.EditItem(id, COL_STATUS, status);
}

void QueryWorldProgressDialog::onListener(EventBroadcaster* Broadcaster, int Id, EventMessage* EventInfo)
{
	// update process
	Querying::WorldQuery& wq = bot.getQueryWorld();

	// broadcaster event
	Querying::IteratorUpdatedMessage* msg = dynamic_cast<Querying::IteratorUpdatedMessage*>(EventInfo);
	if (Broadcaster == &wq.events && msg)
	{
		if (Id == wq.EVID_ITERATOR_UPDATED)
		{
			updateSegment(msg->index);
		}
	}
}

void QueryWorldProgressDialog::OnQuery_BackupToFile()
{
	// open file dialog
	const TCHAR szFilter[] = _T("Activeworlds Propdump v4 (*.awpd)|*.awpd|Eclipse Property Project (*.e2pp)|*.e2pp||");
	CFileDialog file(TRUE, "awpd", 0, 4|2, szFilter, this);
	if (file.DoModal() != IDOK) 
		return;

	// begin the query
	String ext = file.GetFileExt();
	bot.queryBackup(
		ext == "awpd" ? Property::FileFormat::FORMAT_V4 : Property::FileFormat::FORMAT_PROJECT,
		file.GetPathName());

	// rebuild the list
	rebuild();
}

BOOL QueryWorldProgressDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// the progress range
	SetTimer(0, 1000, 0);
	c_progress.SetRange(0, 500);

	// create cols
	c_iterators.InsertColumn(COL_INDEX,			"#", 0, 30);
	c_iterators.InsertColumn(COL_RANGE,			"Range", 0, 120);
	c_iterators.InsertColumn(COL_DURATION,	"Time", 0, 80);
	c_iterators.InsertColumn(COL_OBJECTS,		"Found", 0, 80);
	c_iterators.InsertColumn(COL_STATUS,		"Status", 0, 80);

	// connect columns
	bot.getQueryWorld().events.acceptListener(this);
	rebuild();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void QueryWorldProgressDialog::OnTimer(UINT_PTR nIDEvent)
{
	// update process
	Querying::WorldQuery& wq = bot.getQueryWorld();

	// the status
	AW::Location lcell(wq.getCurCellX() * 1000, 0, wq.getCurCellZ() * 1000, 0);
	String status; 
	status.Format("Query @ %d o/m", wq.getAverage() * 60);
	c_status.SetWindowTextA(wq.isComplete() ? "Completed : Holding" :  status);
	
	// the progress
	int pos		 = 455;
	int expected = bot._int(AW_WORLD_OBJECT_COUNT);
	expected = expected ? expected : 1;

	// the object count
	if (pos < expected)
		pos = (int)((500.0F / expected) * wq.getCount());
	c_progress.SetPos(wq.isComplete() ? 500 : pos);

	// the number of objects queried
	c_objects.SetWindowTextA( ToString(wq.getCount()) );
	c_objectsof.SetWindowTextA( ToString(expected) );

	// estimated percentage
	String estimatedPerc; estimatedPerc.Format("%.6f%%", 100.0F / expected * wq.getCount()); 
	c_estimatedPercent.SetWindowTextA( wq.isComplete() ? "100%" : estimatedPerc );


	// get the duration
	c_timeElapsed.SetWindowText( Helpers::Time::SecondsToTimeStr((int)wq.getDuration()) );

	// seconds
	int objects_remain = expected - wq.getCount();
	int seconds_remain = objects_remain / (wq.getAverage() == 0 ? 1 : wq.getAverage());
	c_timeEstimatedDuration.SetWindowTextA( Helpers::Time::SecondsToTimeStr(seconds_remain) );

	// error
	c_error.SetWindowTextA( wq.getLastErrorString() );

	CDialog::OnTimer(nIDEvent);
}
