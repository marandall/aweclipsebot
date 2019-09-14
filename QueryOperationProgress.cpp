// QueryOperationProgress.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "QueryOperationProgress.h"


// QueryOperationProgress dialog

IMPLEMENT_DYNAMIC(QueryOperationProgress, CDialog)

QueryOperationProgress::QueryOperationProgress(CWnd* pParent /*=NULL*/)
	: CDialog(QueryOperationProgress::IDD, pParent)
{

}

QueryOperationProgress::~QueryOperationProgress()
{
}

void QueryOperationProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(QueryOperationProgress, CDialog)
END_MESSAGE_MAP()


// QueryOperationProgress message handlers
