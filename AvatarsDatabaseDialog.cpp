#include "stdafx.h"
#include "EclipseEvolution.h"
#include "AvatarsDatabaseDialog.h"
#include "inputboxdialog.h"


// AvatarsDatabaseDialog dialog

IMPLEMENT_DYNAMIC(AvatarsDatabaseDialog, CDialog)

AvatarsDatabaseDialog::AvatarsDatabaseDialog(CWnd* pParent /*=NULL*/)
	: CDialog(AvatarsDatabaseDialog::IDD, pParent)
{

}

AvatarsDatabaseDialog::~AvatarsDatabaseDialog()
{
}

void AvatarsDatabaseDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, c_Rows);
	DDX_Control(pDX, IDC_QUERY, c_SQL);
}


BEGIN_MESSAGE_MAP(AvatarsDatabaseDialog, CDialog)
	ON_WM_SIZE()
	ON_COMMAND(ID_QUERY_EXECUTENEW, &AvatarsDatabaseDialog::OnQuery_ExecuteNew)
	ON_EN_CHANGE(IDC_QUERY, &AvatarsDatabaseDialog::OnEnChangeQuery)
END_MESSAGE_MAP()


// AvatarsDatabaseDialog message handlers
enum 
{
	C_ENTER_TIME,
	C_NAME,
	C_SESSION,
	C_CITIZEN,
	C_PRIVILEGE,
	C_PRIVILEGE_NAME,
	C_IP,
	C_DNS,
	C_ENTER_COORDS,
	C_WORLD,
	C_EXIT_TIME,
	C_EXIT_COORDS
};

BOOL AvatarsDatabaseDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	c_Rows.InsertColumn(C_ENTER_TIME,		_T("Timestamp"),		0, 120);
	c_Rows.InsertColumn(C_NAME,				_T("Name"),				0, 100);
	c_Rows.InsertColumn(C_SESSION,			_T("Session"),			0, 100);	
	c_Rows.InsertColumn(C_CITIZEN,			_T("Citizen"),			0, 70);
	c_Rows.InsertColumn(C_PRIVILEGE,		_T("Privilege"),		0, 70);
	c_Rows.InsertColumn(C_PRIVILEGE_NAME,	_T("Priv/Name"),		0, 100);
	c_Rows.InsertColumn(C_IP,				_T("Address"),			0, 100);
	c_Rows.InsertColumn(C_DNS,				_T("DNS"),				0, 150);
	c_Rows.InsertColumn(C_ENTER_COORDS,		_T("Coordinates"),		0, 120);
	c_Rows.InsertColumn(C_WORLD,			_T("World"),			0, 100);
	c_Rows.InsertColumn(C_EXIT_TIME,		_T("Exit Time"),		0, 100);
	c_Rows.InsertColumn(C_EXIT_COORDS,		_T("Exit Coordinates"),	0, 120);

	c_Rows.SetExtendedStyle(
		c_Rows.GetExtendedStyle() | LVS_EX_FLATSB | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void AvatarsDatabaseDialog::Query(CString SQL)
{
	// empty the table rows
	c_Rows.DeleteAllItems();

	ADODB::Recordset rs;
	BOOL ok = g_Database.Query(rs, SQL);
	c_Rows.SetItemCount(500);
	if (ok == TRUE)
	{
		c_SQL.SetWindowTextA(SQL);
		m_LastSQL = SQL;
		
		// loop through while not eof
		while (!rs.IsEOF())
		{
			int id = c_Rows.InsertItem(c_Rows.GetItemCount(), 
						MakeGuiTime(atoi(rs["enter_time"]), "%d %b, %Y %H:%M:%S"));
			c_Rows.EditItem(id, C_NAME,				rs["user_name"]);
			c_Rows.EditItem(id, C_SESSION,			rs["session_id"]);
			c_Rows.EditItem(id, C_CITIZEN,			rs["enter_time"]);
			c_Rows.EditItem(id, C_ENTER_COORDS,		rs["enter_coords"]);
			c_Rows.EditItem(id, C_CITIZEN,			rs["citizen"]);
			c_Rows.EditItem(id, C_PRIVILEGE,		rs["privilege"]);
			c_Rows.EditItem(id, C_PRIVILEGE_NAME,	rs["privilege_name"]);
			c_Rows.EditItem(id, C_WORLD,			rs["world"]);
			c_Rows.EditItem(id, C_IP,				rs["ip"]);
			c_Rows.EditItem(id, C_DNS,				rs["dns"]);

			// special handling for if we dont know exit time
			int enter_time = atoi(rs["exit_time"]);
			if (enter_time == 0)
				c_Rows.EditItem(id, C_EXIT_TIME,	_T("Unknown"));
			else
				c_Rows.EditItem(id, C_EXIT_TIME,	MakeGuiTime(atoi(rs["exit_time"]), "%d %b, %H:%M:%S"));
			// c_Rows.EditItem(id, C_EXIT_TIME,		ToString(enter_time));

			c_Rows.EditItem(id, C_EXIT_COORDS,		rs["exit_coords"] == "" ? "Unknown" : rs["exit_coords"]);
			rs.MoveNext();
		}
	}
	else
	{
		String log;
		log.Format("There was an error in the query");
		MessageBox(log, NULL, MB_ICONEXCLAMATION);
	}
}
void AvatarsDatabaseDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// sanity check, only valid sql
	if (IsWindow(c_SQL)) 
	{
		c_SQL.MoveWindow(0, 0, cx, 25);
		c_Rows.MoveWindow(0, 25, cx, cy - 25); 
	}
}


void AvatarsDatabaseDialog::OnQuery_ExecuteNew()
{
	InputBoxDialog dlg(this);
	dlg.v_Title		= "Enter SQL";
	dlg.v_String	= m_LastSQL;
	
	// open the dialog for input
	if (dlg.DoModal() == IDOK)
		Query(dlg.v_String);
}

void AvatarsDatabaseDialog::OnEnChangeQuery()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
