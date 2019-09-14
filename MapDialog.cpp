// MapDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "MapDialog.h"
#include "GNet_QueryMap.h"
#include "awsdk_query.h"
#include "property_query_5x5.h"

// MapDialog dialog
GraphicsDisplay::QueryMap qMap;


IMPLEMENT_DYNAMIC(MapDialog, CDialog)

MapDialog::MapDialog(CWnd* pParent /*=NULL*/)
	: CDialog(MapDialog::IDD, pParent)
{

}

MapDialog::~MapDialog()
{
}

void MapDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, c_DrawSurface);
}


BEGIN_MESSAGE_MAP(MapDialog, CDialog)
	ON_WM_TIMER()
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()


BOOL MapDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	qMap.changeCenter(0, 0);
	qMap.m_PPU		= 0.005F;
	qMap.m_pQuery	= &bot.getQuery5x5();
	qMap.m_pCache	= &bot.getLiveCache();

	c_DrawSurface.m_Surface = &qMap;

	SetTimer(1, 1000, NULL);
	return TRUE;
}

void MapDialog::OnTimer(UINT_PTR nIDEvent)
{
	c_DrawSurface.Invalidate();

	CDialog::OnTimer(nIDEvent);
}

void MapDialog::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	delete this;
}
