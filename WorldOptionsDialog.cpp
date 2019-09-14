// WorldOptionsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "WorldOptionsDialog.h"
#include "SerializationEditorDialog.h"

// WorldOptionsDialog dialog

IMPLEMENT_DYNAMIC(WorldOptionsDialog, CDialog)

WorldOptionsDialog::WorldOptionsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(WorldOptionsDialog::IDD, pParent)
{

}

WorldOptionsDialog::~WorldOptionsDialog()
{
	c_pProperties->DestroyWindow();
	delete c_pProperties;
}

void WorldOptionsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRACKING, c_TrackingRect);
	DDX_Control(pDX, IDC_TAB1, c_Tabs);
}


BEGIN_MESSAGE_MAP(WorldOptionsDialog, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &WorldOptionsDialog::OnTabs_SwitchTab)
	ON_COMMAND(ID_FILE_SAVEATTRIBUTES, &WorldOptionsDialog::OnFile_SaveAttributes)
	ON_COMMAND(ID_FILE_OPENATTRIBUTES, &WorldOptionsDialog::OnFile_OpenAttributes)
	ON_COMMAND(ID_SDK_RELOADVIASDK, &WorldOptionsDialog::OnSdk_Reload)
	ON_BN_CLICKED(IDOK, &WorldOptionsDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// WorldOptionsDialog message handlers
void WorldOptionsDialog::create_group(int ID, CStringA Title)
{
	c_Tabs.InsertItem(ID, CString(Title));
	c_pProperties->create_group(ID, Title);
}

BOOL WorldOptionsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// create the window
	c_pProperties = new PropertyScriptCtrl(this);
	c_pProperties->Create(PropertyScriptCtrl::IDD, this);

	PropertyScriptCtrl& pcs = *c_pProperties;
	create_group(WAG_GENERAL,			"General");
	create_group(WAG_RIGHT,				"Rights");
	create_group(WAG_GROUND,			"Ground");
	create_group(WAG_OBJECT_PATH,		"Object Path");
	create_group(WAG_BUILDING,			"Building");
	create_group(WAG_FOG,				"Fog");
	create_group(WAG_LIGHTING,			"Lighting");
	create_group(WAG_MOVEMENT,			"Movement");
	create_group(WAG_SKY,				"Sky");
	create_group(WAG_CLOUDS_1,			"Clouds 1");
	create_group(WAG_CLOUDS_2,			"Clouds 2");
	create_group(WAG_CLOUDS_3,			"Clouds 3");
	create_group(WAG_COMMUNICATIONS,	"Comms");
	create_group(WAG_AVATARS,			"Avatars");
	create_group(WAG_WATER,				"Water");
	create_group(WAG_SOUND,				"Sounds");

	// rect
	RECT rt;
	c_TrackingRect.GetWindowRect(&rt);
	ScreenToClient(&rt);
	c_pProperties->MoveWindow(&rt);
	CString buffer;

	// create the objects we need
	create_objects();

	// create max visibility
	for (int i = 30; i < 200; i+= 10)
	{
		buffer.Format(_T("%d meters"), i);
		controls.c_pMinimumVisibility->InsertWithData(buffer, i);
	}

	// create rating
	controls.c_pRating->InsertWithData(_T("G Rated"),		0);
	controls.c_pRating->InsertWithData(_T("PG Rated"),		1);
	controls.c_pRating->InsertWithData(_T("PG-13 Rated"),	2);
	controls.c_pRating->InsertWithData(_T("R Rated"),		3);
	controls.c_pRating->InsertWithData(_T("X Rated"),		4);

	// create object density / cell data limits
	controls.c_pCellLimit->InsertWithData(_T("Normal (1650 bytes)"),	1650);
	controls.c_pCellLimit->InsertWithData(_T("Large (2750 bytes)"),		2750);
	controls.c_pCellLimit->InsertWithData(_T("Huge (4400 bytes)"),		4400);
	controls.c_pCellLimit->InsertWithData(_T("Mega (5500 bytes)"),		5500);
	controls.c_pCellLimit->InsertWithData(_T("Ultra (11000 bytes)"),	11000);


	// copy initial attributes
	WorldAttributes attr;
	attr.from_sdk(bot);
	attributes_to_screen(attr);

	c_pProperties->ShowWindow(SW_SHOW);
	pcs.show_group(WAG_GENERAL);
	pcs.Invalidate();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void WorldOptionsDialog::OnBnClickedGeneral()
{
	c_pProperties->show_group(WAG_GENERAL);
}

void WorldOptionsDialog::OnTabs_SwitchTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	c_pProperties->show_group(c_Tabs.GetCurSel());
	*pResult = 0;
}

void WorldOptionsDialog::OnFile_SaveAttributes()
{
	// copy everything from this dialog
	WorldAttributes config;
	screen_to_attributes(config);

	// create INI
	AW::EnhancedINI ini = config.getEnhancedINI();

	// display exporter
	SerializationEditorDialog dlg;
	dlg.setup(ini, &WorldAttributes::StaticGroupMap());
	if (dlg.DoModal() != IDOK)
		return;

	// attribute load
	const char szFilter[] = "World Attributes (*.awatt)|*.awatt||";
	CFileDialog file(FALSE, "awatt", 0, 4|2, szFilter, this);
	if (file.DoModal() != IDOK) 
		return;

	// save the INI here
	dlg.Result().Save(file.GetFileName());
}

void WorldOptionsDialog::OnFile_OpenAttributes()
{
	// open the file attributes file
	const char szFilter[] = "World Attributes (*.awatt)|*.awatt||";
	CFileDialog file(TRUE, "awatt", 0, 4|2, szFilter, this);
	if (file.DoModal() != IDOK) 
		return;

	// copy everything from this dialog
	WorldAttributes attr;
	screen_to_attributes(attr);
	
	// read the file
	errno_t err = attr.read_file(file.GetFileName());
	if (err != 0)
	{
		MessageBox(_T("Unable to open the attributes file, or invalid format"), NULL, MB_ICONEXCLAMATION);
		return;
	}

	// push to screen
	attributes_to_screen(attr);
}

void WorldOptionsDialog::OnSdk_Reload()
{
	// copy everything from this dialog
	WorldAttributes attr;
	attr.from_sdk(bot);

	// push to screen
	attributes_to_screen(attr);
}

void WorldOptionsDialog::OnOK()
{
	screen_to_attributes(attributes);
	CDialog::OnOK();
}

void WorldOptionsDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
