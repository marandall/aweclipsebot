// UniverseOptionsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "UniverseOptionsDialog.h"
#include "AWSDK_Universe.h"

// UniverseOptionsDialog dialog

IMPLEMENT_DYNAMIC(UniverseOptionsDialog, CDialog)

UniverseOptionsDialog::UniverseOptionsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(UniverseOptionsDialog::IDD, pParent)
	, v_BrowserMin(20)
	, v_BrowserRelease(20)
	, v_BrowserBeta(20)
	, v_WorldMin(20)
	, v_WorldRelease(20)
	, v_WorldBeta(20)
	, v_Greeting(_T(""))
	, v_StartWorld(_T(""))
	, v_NotepadUrl(_T(""))
	, v_AllowTourists(FALSE)
	, v_SearchUrl(_T(""))
{

}

UniverseOptionsDialog::~UniverseOptionsDialog()
{
}

void UniverseOptionsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, ID_BROWSER_MIN,		v_BrowserMin);
	DDX_Text(pDX, ID_BROWSER_RELEASE,	v_BrowserRelease);
	DDX_Text(pDX, ID_BROWSER_BETA,		v_BrowserBeta);

	DDV_MinMaxInt(pDX, v_BrowserMin,	400, 1200);
	DDV_MinMaxInt(pDX, v_BrowserRelease,400, 1200);
	DDV_MinMaxInt(pDX, v_BrowserBeta,	400, 1200);

	DDX_Text(pDX, ID_WORLD_MIN,			v_WorldMin);
	DDX_Text(pDX, ID_WORLD_RELEASE,		v_WorldRelease);
	DDX_Text(pDX, ID_WORLD_BETA,		v_WorldBeta);

	DDX_Text(pDX, ID_GREETING,			v_Greeting);
	DDX_Text(pDX, ID_START_WORLD,		v_StartWorld);
	DDX_Text(pDX, ID_NOTEPAD_URL,		v_NotepadUrl);
	DDX_Check(pDX, ID_ALLOW_TOURISTS,	v_AllowTourists);
	DDX_Control(pDX, ID_BROWSER_MIN, c_BrowserMinimum);
	DDX_Control(pDX, ID_BROWSER_RELEASE, c_BrowserRelease);
	DDX_Control(pDX, ID_BROWSER_BETA, c_BrowserBeta);
	DDX_Control(pDX, ID_WORLD_MIN, c_WorldMinimum);
	DDX_Control(pDX, ID_WORLD_RELEASE, c_WorldRelease);
	DDX_Control(pDX, ID_WORLD_BETA, c_WorldBeta);
	DDX_Control(pDX, ID_GREETING, c_Greeting);
	DDX_Control(pDX, ID_START_WORLD, c_StartWorld);
	DDX_Control(pDX, ID_SEARCH_URL, c_SearchUrl);
	DDX_Control(pDX, ID_NOTEPAD_URL, c_NotepadUrl);
	DDX_Control(pDX, ID_ALLOW_TOURISTS, c_AllowTourists);
	DDX_Text(pDX, ID_SEARCH_URL, v_SearchUrl);
}


BEGIN_MESSAGE_MAP(UniverseOptionsDialog, CDialog)
	ON_WM_NCDESTROY()
	ON_BN_CLICKED(IDC_RELOAD, &UniverseOptionsDialog::OnBnClickedReload)
	ON_COMMAND(ID_DATAEXCHANGE_LOADFROMFILE, &UniverseOptionsDialog::OnDataExchange_LoadFile)
	ON_COMMAND(ID_DATAEXCHANGE_SAVETOFILE, &UniverseOptionsDialog::OnDataExchange_SaveFile)
	ON_BN_CLICKED(IDOK, &UniverseOptionsDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_MAKE_THIS, &UniverseOptionsDialog::OnBnClickedMakeThis)
END_MESSAGE_MAP()


// UniverseOptionsDialog message handlers
void UniverseOptionsDialog::Display(AW::UniverseOptions& Options)
{
	v_BrowserMin			= Options.getBrowserMinimum();
	v_BrowserRelease		= Options.getBrowserRelease();
	v_BrowserBeta			= Options.getBrowserBeta(); 
	v_WorldMin				= Options.getWorldMinimum(); 
	v_WorldRelease			= Options.getWorldRelease(); 
	v_WorldBeta				= Options.getWorldBeta(); 
	v_Greeting				= Options.GetGreeting(); 
	v_StartWorld			= Options.getStartWorld();
	v_NotepadUrl			= Options.getNotepadUrl();
	v_AllowTourists			= Options.getAllowTourists();
	UpdateData(FALSE);
}

void UniverseOptionsDialog::Extract(AW::UniverseOptions& Options)
{
	Options.setBrowserRange	(v_BrowserMin, v_BrowserRelease, v_BrowserBeta);
	Options.setWorldRange	(v_WorldMin, v_WorldRelease, v_WorldBeta);
	Options.setUrls			(v_SearchUrl, v_NotepadUrl);
	Options.SetGreeting		(v_Greeting);
	Options.setStartWorld	(v_StartWorld);
	Options.setAllowTourists(v_AllowTourists);
}

void UniverseOptionsDialog::ReloadFromSdk()
{
	/* load a set of options */
	AW::UniverseOptions options;
	options.FromSDK(bot);
	Display(options);
	CWnd* pWnd[] = { &c_BrowserMinimum, &c_BrowserRelease,
		&c_BrowserBeta, &c_WorldMinimum, &c_WorldRelease, &c_WorldBeta,
		&c_Greeting, &c_StartWorld, &c_SearchUrl, &c_NotepadUrl, &c_AllowTourists };
	for (size_t i = 0; i < sizeof(pWnd) / sizeof(pWnd[0]); i++)
		pWnd[i]->EnableWindow(bot.HasRoot() ? TRUE : FALSE);
}

void UniverseOptionsDialog::OnOK()
{
	/* check validators */
	if (UpdateData(TRUE) == FALSE)
		return;

	/* only allow updates if we have universe auth rights */
	if (bot.HasRoot() == false)
		return;

	/* update the universe options */
	bot.Lock();
	bot._int_set(AW_UNIVERSE_BROWSER_MINIMUM,	v_BrowserMin);
	bot._int_set(AW_UNIVERSE_BROWSER_RELEASE,	v_BrowserRelease);
	bot._int_set(AW_UNIVERSE_BROWSER_BETA,		v_BrowserBeta);
	
	/* world section */
	bot._int_set(AW_UNIVERSE_WORLD_MINIMUM,		v_WorldMin);
	bot._int_set(AW_UNIVERSE_WORLD_RELEASE,		v_WorldRelease);
	bot._int_set(AW_UNIVERSE_WORLD_BETA,		v_WorldBeta);

	/* strings section */
	bot._string_set(AW_UNIVERSE_WELCOME_MESSAGE,v_Greeting.Left(AW_MAX_ATTRIBUTE_LENGTH));
	bot._string_set(AW_UNIVERSE_WORLD_START,	v_StartWorld.Left(16));
#if AW_BUILD > 50
	bot._string_set(AW_UNIVERSE_NOTEPAD_URL,	v_NotepadUrl.Left(AW_MAX_ATTRIBUTE_LENGTH));
#endif
	bot._bool_set(AW_UNIVERSE_ALLOW_TOURISTS,	v_AllowTourists);
	
	/* make changes and apply */
	int rc = bot._universe_attributes_change();
	if ( rc ) {
		CStringA logText;
		logText.Format("Could not apply universe attributes:\n\n Reason %d\n%s",
			rc, AW::Bot::GetReasonCode(rc));
		MessageBox(CString(logText), "Universe", MB_ICONWARNING);
	} else {
		MessageBox(_T("Universe Options have been applied"));
	}
	bot.Unlock();

	CDialog::OnOK();
}

BOOL UniverseOptionsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	ReloadFromSdk();
	return TRUE;
}

void UniverseOptionsDialog::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	delete this;
}

void UniverseOptionsDialog::OnBnClickedReload()
{
	ReloadFromSdk();
}

void UniverseOptionsDialog::OnDataExchange_LoadFile()
{
}

void UniverseOptionsDialog::OnDataExchange_SaveFile()
{
	CFileDialog file(FALSE, "ini");
	if (file.DoModal() != IDOK) return;
	
	/* transfer the data from the sdk */
	AW::UniverseOptions uni_opts;
	uni_opts.FromSDK(bot);
	uni_opts.ToFile(file.GetFileName());
}

void UniverseOptionsDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void UniverseOptionsDialog::OnBnClickedMakeThis()
{
	if (c_StartWorld.IsWindowEnabled() == TRUE)
		c_StartWorld.SetWindowTextA(bot._string(AW_WORLD_NAME));
}
