// QueryModify_TranslateDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "QueryModify_TranslateDialog.h"
#include "QueryDialog.h"

// QueryModify_TranslateDialog dialog

IMPLEMENT_DYNAMIC(QueryModify_TranslateDialog, CDialog)

QueryModify_TranslateDialog::QueryModify_TranslateDialog(CWnd* pParent /*=NULL*/)
	: CDialog(QueryModify_TranslateDialog::IDD, pParent)
{

}

QueryModify_TranslateDialog::~QueryModify_TranslateDialog()
{
}

void QueryModify_TranslateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NORTH, c_Source);
	DDX_Control(pDX, IDC_DEST, c_Destination);
	DDX_Control(pDX, IDC_OP, c_Operation);
}


BEGIN_MESSAGE_MAP(QueryModify_TranslateDialog, CDialog)
	ON_BN_CLICKED(IDC_UNSELECT4, &QueryModify_TranslateDialog::OnBnClickedUnselect4)
	ON_BN_CLICKED(IDC_TRANSLATE, &QueryModify_TranslateDialog::OnBnClickedTranslate)
	ON_BN_CLICKED(IDC_TRANSLATE2, &QueryModify_TranslateDialog::OnBnClickedTranslate2)
END_MESSAGE_MAP()


// QueryModify_TranslateDialog message handlers

void QueryModify_TranslateDialog::OnBnClickedUnselect4()
{
	// get from location to destination
	Location lc_from(c_Source.GetText());
	Location lc_dest(c_Destination.GetText());

	// subtract from each other
	AW::Location translation(lc_dest.get_X() - lc_from.get_X(),
							 lc_dest.get_Y() - lc_from.get_Y(),
							 lc_dest.get_Z() - lc_from.get_Z(),
							 0);
	c_Operation.SetWindowTextA(translation.get_Coordinates());
}

void QueryModify_TranslateDialog::OnBnClickedTranslate()
{
	Location lc_offset(c_Operation.GetText());

	// build options
	Property::MethodControls::Translate	options;
	options.selected_only	= false;
	options.direction		= lc_offset; 
	((QueryDialog*)GetParent())->Modify_Translate(options);
}

void QueryModify_TranslateDialog::OnBnClickedTranslate2()
{
	Location lc_offset(c_Operation.GetText());

	// build options
	Property::MethodControls::Translate	options;
	options.selected_only	= true;
	options.direction		= lc_offset; 
	((QueryDialog*)GetParent())->Modify_Translate(options);
}

