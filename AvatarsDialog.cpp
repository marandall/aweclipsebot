// AvatarsDialog.cpp : implementation file
//

#include "stdafx.h"						// for standard headers
#include "AvatarsDialog.h"				// for this dialog
#include "Avatar.h"						// for avatar class
#include "GlobalConversions.h"			// for the ToString
#include "EclipseEvolutionDialog.h"		// for the main window
#include "EclipseDialog_MainIcons.h"	// a list of main icons 

/* variables for the avatar */
#define ISC_NAME		0
#define ISC_SESSION		1
#define ISC_CITIZEN		2
#define ISC_PRIVILEGE	3
#define ISC_PRIVNAME	4
#define ISC_ADDRESS		5
#define ISC_DNS			6
#define ISC_LOCATION	7
#define ISC_BUILD		8

// AvatarsDialog dialog
namespace Data {
	EclipseDialog_MainIcons av_icons;
}

IMPLEMENT_DYNAMIC(AvatarsDialog, CDialog)

AvatarsDialog::AvatarsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(AvatarsDialog::IDD, pParent)
{

}

AvatarsDialog::~AvatarsDialog()
{
}

void AvatarsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AVATARS, c_Avatars);
	DDX_Control(pDX, IDC_BUTTON_EXPAND, c_Expand);
	DDX_Control(pDX, IDC_SELECTION_BAR, c_ShowBar);
}


BEGIN_MESSAGE_MAP(AvatarsDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_EXPAND, &AvatarsDialog::OnBnClickedButtonExpand)
	ON_BN_CLICKED(IDC_SHOW_AVATARS, &AvatarsDialog::OnBnClickedShowAvatars)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_AVATARS, &AvatarsDialog::OnAvatars_RightClick)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &AvatarsDialog::OnAvatars_HeaderClick)
END_MESSAGE_MAP()


// AvatarsDialog message handlers
void AvatarsDialog::AvatarAdd(Avatar& User) 
{
	/* insert a new item for the avatar */
	int place_id = c_Avatars.InsertItem(c_Avatars.GetItemCount(), User.getName());
	c_Avatars.SetItemData(place_id, (DWORD_PTR)User.getSession());

	/* only required to set the name here */
	c_Avatars.EditItem(place_id, ISC_SESSION, ToStringT(User.getSession()), 0);
	AvatarUpdate(User);
}

void AvatarsDialog::AvatarUpdate(Avatar& User)
{
	/* we expect a single avatar - if we cannot find it then we have some problem */
	int place_id = c_Avatars.GetByItemData((UINT_PTR)User.getSession());
	if (place_id == -1)
		return;	

	/* update the relevant sections */
	c_Avatars.EditItem(place_id, ISC_NAME, User.getName(), 0);

	/* citizen */
	c_Avatars.EditItem(place_id, ISC_CITIZEN,	ToString(User.getCitizen()),  User.getIconCitizen());
	c_Avatars.EditItem(place_id, ISC_PRIVILEGE, ToString(User.getPrivilege()),User.getIconPrivilege());
	c_Avatars.EditItem(place_id, ISC_PRIVNAME,  User.getPrivName(),			  User.getIconPrivName());

	/* ip and DNS */
	c_Avatars.EditItem(place_id, ISC_ADDRESS,	User.getAddress(),			  User.getIconIP());
	c_Avatars.EditItem(place_id, ISC_DNS,		User.getDns(),				  User.getIconDNS());
		
	/* update build */
	CStringA text; text.Format("%d (%s)", User.getBuild(), User.getBuildDescription());
	c_Avatars.EditItem(place_id, ISC_BUILD,		text,	   User.getIconBuild());

	/* update coordinates */
	AvatarMove(User);
}

void AvatarsDialog::AvatarMove(Avatar& User)
{
	/* we expect a single avatar - if we cannot find it then we have some problem */
	int place_id = c_Avatars.GetByItemData((UINT_PTR)User.getSession());
	if (place_id == -1)
		return;	

	/* update the avatars position only */
	c_Avatars.EditItem(place_id, ISC_LOCATION, User.getCoordinates(), User.getIconState());
}

void AvatarsDialog::AvatarDelete(Avatar& User) 
{
	/* we expect a single avatar - if we cannot find it then we have some problem */
	int place_id = c_Avatars.GetByItemData((UINT_PTR)User.getSession());
	if (place_id == -1)
		return;	
	c_Avatars.DeleteItem(place_id);
}

void AvatarsDialog::OnBnClickedButtonExpand()
{
	/* is it shown? */
	if (c_Expand.GetCheck() == TRUE) 
	{
		c_ShowBar.ShowWindow(SW_HIDE);
		c_Avatars.ShowWindow(SW_SHOW);
		known_height = 150;
	} 
	else 
	{
		c_Avatars.ShowWindow(SW_HIDE);
		c_ShowBar.ShowWindow(SW_SHOW);
	
		/* find button size */
		RECT rtButton;
		c_Expand.GetWindowRect(&rtButton);
		known_height = rtButton.bottom - rtButton.top; 		
	}

	/* force resize */
	EclipseEvolutionDialog* pWnd = (EclipseEvolutionDialog*)AfxGetApp()->GetMainWnd();
	pWnd->ForceResize();
}

void AvatarsDialog::OnBnClickedShowAvatars()
{
	OnBnClickedButtonExpand();
}

void AvatarsDialog::OnBnClickedShowUniverse()
{
	OnBnClickedButtonExpand();
}

void AvatarsDialog::OnBnClickedSplitVertical()
{
	OnBnClickedButtonExpand();
}

void AvatarsDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	/* abort if we dont have a window */
	if (!IsWindow(c_Avatars))
		return;

	/* show each window */
	RECT rtButton;
	c_Expand.GetWindowRect(&rtButton);
	int button_width  = rtButton.right - rtButton.left;
	int button_height = rtButton.bottom - rtButton.top; 
	
	/* space seperators */
	int seperator	= 1;
	int main_left	= button_width + seperator;
	int main_width	= cx - main_left;

	if (c_Expand.GetCheck() == TRUE) 
	{
		c_Avatars.MoveWindow(main_left, 0, main_width, cy);
	} 
	else 
	{
		c_ShowBar.MoveWindow(main_left, 0, cx - main_left, button_height); 
	}

}

void AvatarsDialog::expand()
{
	c_Expand.SetCheck(TRUE);
	//OnBnClickedButtonExpand();
}

void AvatarsDialog::collapse()
{
	c_Expand.SetCheck(FALSE);

	c_Avatars.ShowWindow(SW_HIDE);
	c_ShowBar.ShowWindow(SW_SHOW);
	
	/* find button size */
	RECT rtButton;
	c_Expand.GetWindowRect(&rtButton);
	known_height = rtButton.bottom - rtButton.top; 	
}

BOOL AvatarsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	known_height = 150;			// default to shown

	/* change list styles */
	c_Avatars.ModifyStyle(0, WS_BORDER, 0);
	c_ShowBar.ModifyStyle(0, WS_BORDER, 0);

	/* create the columns for the avatars list */
	c_Avatars.InsertColumn(0, _T("Name"),			LVCFMT_LEFT,	110);
	c_Avatars.InsertColumn(1, _T("Session"),		LVCFMT_RIGHT,	70);
	c_Avatars.InsertColumn(2, _T("Citizen"),		LVCFMT_RIGHT,	70);
	c_Avatars.InsertColumn(3, _T("Privilege"),		LVCFMT_RIGHT,	70);
	c_Avatars.InsertColumn(4, _T("Privilege Name"),	LVCFMT_LEFT,	110);
	c_Avatars.InsertColumn(5, _T("IP Address"),		LVCFMT_LEFT,	80);
	c_Avatars.InsertColumn(6, _T("DNS Address"),	LVCFMT_LEFT,	80);
	c_Avatars.InsertColumn(7, _T("Location"),		LVCFMT_LEFT,	150);
	c_Avatars.InsertColumn(8, _T("Build"),			LVCFMT_LEFT,	60);

	c_Avatars.SetExtendedStyle(c_Avatars.GetExtendedStyle() | LVS_EX_FLATSB | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);

	/* force alignment */
	c_Expand.SetCheck(FALSE);
	c_Avatars.InitIcons();

	/* load the special icons */
	String BaseDir = GetAppPath() + "\\graphics\\symbolic\\";
	Data::EclipseDialog_MainIcons& icons = Data::av_icons;
	icons.main		= c_Avatars.InsertIcon(BaseDir + "dot.bmp",			"dot");
	icons.eject		= c_Avatars.InsertIcon(BaseDir + "eject.bmp",		"eject");
	icons.ps		= c_Avatars.InsertIcon(BaseDir + "ps.bmp",			"ps");
	icons.ps_eject	= c_Avatars.InsertIcon(BaseDir + "ps_eject.bmp",	"ps_eject");
	icons.ps_root	= c_Avatars.InsertIcon(BaseDir + "ps_root.bmp",		"ps_root");
	icons.equal		= c_Avatars.InsertIcon(BaseDir + "eq_same.bmp",		"eq_same");
	icons.equal_b	= c_Avatars.InsertIcon(BaseDir + "eq_tourist.bmp",	"eq_tourist");
	icons.warning	= c_Avatars.InsertIcon(BaseDir + "warning.bmp",		"warning");
	icons.alert		= c_Avatars.InsertIcon(BaseDir + "alert.bmp",		"alert");
	icons.ticked	= c_Avatars.InsertIcon(BaseDir + "tick.bmp",		"tick");
	icons.whisper	= c_Avatars.InsertIcon(BaseDir + "whisper.bmp",		"whisper");
	icons.blank		= c_Avatars.InsertIcon(BaseDir + "blank.bmp",		"blank");
	icons.blank		= c_Avatars.InsertIcon(BaseDir + "blank.bmp",		"blank");

	/* load the movement state icons*/
	BaseDir = GetAppPath() + "\\graphics\\movestate\\"; 
	icons.move_t[AW_AVATAR_STATE_WALKING]	= c_Avatars.InsertIcon(BaseDir + "walking.bmp");
	icons.move_t[AW_AVATAR_STATE_RUNNING]	= c_Avatars.InsertIcon(BaseDir + "running.bmp");
	icons.move_t[AW_AVATAR_STATE_JUMPING]	= c_Avatars.InsertIcon(BaseDir + "jumping.bmp");
	icons.move_t[AW_AVATAR_STATE_FALLING]	= c_Avatars.InsertIcon(BaseDir + "falling.bmp");
	icons.move_t[AW_AVATAR_STATE_SWIMMING]	= c_Avatars.InsertIcon(BaseDir + "swimming.bmp");
	icons.move_t[AW_AVATAR_STATE_WARPING]	= c_Avatars.InsertIcon(BaseDir + "warping.bmp");
	icons.move_t[AW_AVATAR_STATE_FLYING]	= c_Avatars.InsertIcon(BaseDir + "flying.bmp");
	icons.move_t[AW_AVATAR_STATE_RIDING]	= c_Avatars.InsertIcon(BaseDir + "riding.bmp");
	icons.move_t[AW_AVATAR_STATE_CLIMBING]	= c_Avatars.InsertIcon(BaseDir + "climbing.bmp");
	icons.move_t[AW_AVATAR_STATE_SLIDING1]	= c_Avatars.InsertIcon(BaseDir + "sliding1.bmp");
	icons.move_t[AW_AVATAR_STATE_SLIDING2]	= c_Avatars.InsertIcon(BaseDir + "sliding2.bmp");
	icons.move_t[AW_AVATAR_STATE_SLIDING3]	= c_Avatars.InsertIcon(BaseDir + "sliding3.bmp");

	/* load build states */
	BaseDir = GetAppPath() + "\\graphics\\builds\\"; 
	icons.build_t[BuildType::Anchient]	= c_Avatars.InsertIcon(BaseDir + "expired.bmp");
	icons.build_t[BuildType::Old]		= c_Avatars.InsertIcon(BaseDir + "old.bmp");
	icons.build_t[BuildType::Current]	= c_Avatars.InsertIcon(BaseDir + "std.bmp");
	icons.build_t[BuildType::Beta]		= c_Avatars.InsertIcon(BaseDir + "beta.bmp");
	icons.build_t[BuildType::BetaPlus]	= c_Avatars.InsertIcon(BaseDir + "advbeta.bmp");
	icons.build_t[BuildType::Bot]		= c_Avatars.InsertIcon(BaseDir + "bot.bmp");

	collapse();
	return TRUE;
}

void AvatarsDialog::OnAvatars_RightClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	POINT point, screen;
    GetCursorPos (&point);
	GetCursorPos (&screen);
    ScreenToClient(&point);
    HDHITTESTINFO HitTest;
    /* set up hit-test*/  
    HitTest.pt.x = point.x;
    HitTest.pt.y = point.y;

	int id = c_Avatars.HitTest(point);
	if (id != -1)
		rt_diag->AvatarContextMenu(*avatars.getSession((int)c_Avatars.GetItemData(id)), screen.x, screen.y);

	*pResult = 0;
}

int CALLBACK SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// sorting by lists
	Avatar* pAv1 = avatars.getSession((int)lParam1);
	Avatar* pAv2 = avatars.getSession((int)lParam2);

	switch (lParamSort)
	{
	case ISC_NAME:
		return strcmp(pAv1->getName(), pAv2->getName());
	case ISC_SESSION:
		return pAv1->getSession() <= pAv2->getSession();
	case ISC_CITIZEN:
		return pAv1->getCitizen() <= pAv2->getCitizen();
	case ISC_PRIVILEGE:
		return pAv1->getPrivilege() <= pAv2->getPrivilege();
	case ISC_PRIVNAME:
		return strcmp(pAv1->getPrivName(), pAv2->getPrivName());
	case ISC_ADDRESS:
		return pAv1->getAddressLng() <= pAv2->getAddressLng();
	case ISC_DNS:
		return strcmp(pAv1->getDns(), pAv2->getDns());
	case ISC_LOCATION:
		return strcmp(pAv1->getCoordinates(), pAv2->getCoordinates());
	case ISC_BUILD:
		return pAv1->getBuild() <= pAv2->getBuild();
	};

	return 0;
}

void AvatarsDialog::OnAvatars_HeaderClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	*pResult = 0;

	c_Avatars.SortItems(SortFunc, phdr->iItem);
}
