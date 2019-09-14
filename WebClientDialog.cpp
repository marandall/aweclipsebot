// WebClientDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "WebClientDialog.h"


// WebClientDialog dialog

IMPLEMENT_DYNAMIC(WebClientDialog, CDialog)

WebClientDialog::WebClientDialog(CWnd* pParent /*=NULL*/)
	: CDialog(WebClientDialog::IDD, pParent)
{

}

WebClientDialog::~WebClientDialog()
{
}

void WebClientDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WEB_WINDOW, c_WebCtrl);
}


BEGIN_MESSAGE_MAP(WebClientDialog, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// WebClientDialog message handlers

BOOL WebClientDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}

int WebClientDialog::createUrlWindow(String Url, CWnd* Parent)
{
	WebClientDialog* pWnd = new WebClientDialog(Parent);
	pWnd->Create(WebClientDialog::IDD, Parent);
	pWnd->ShowWindow(SW_SHOW);
	pWnd->SetWindowTextA(Url);

	
	COleVariant sLoc(Url);
	pWnd->c_WebCtrl.Navigate2(sLoc, NULL, NULL, NULL, NULL);
	return 0;
}

void WebClientDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (IsWindow(c_WebCtrl)) {
		c_WebCtrl.MoveWindow(0, 0, cx, cy);
	}
}
