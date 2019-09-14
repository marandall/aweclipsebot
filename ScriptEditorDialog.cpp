// ScriptEditorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EclipseEvolution.h"
#include "ScriptEditorDialog.h"
#include "scripting_host.h"
#include "GlobalGui.h"

// ScriptEditorDialog dialog

IMPLEMENT_DYNAMIC(ScriptEditorDialog, CDialog)

ScriptEditorDialog::ScriptEditorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ScriptEditorDialog::IDD, pParent)
{

}

ScriptEditorDialog::~ScriptEditorDialog()
{
}

void ScriptEditorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CODEWINDOW, c_CodeWindow);
	DDX_Control(pDX, IDC_LIST1, c_moduleList);
	DDX_Control(pDX, IDC_COMPILE, c_compile);
}


BEGIN_MESSAGE_MAP(ScriptEditorDialog, CDialog)
	ON_BN_CLICKED(IDC_COMPILE, &ScriptEditorDialog::OnBnClickedCompile)
	ON_COMMAND(ID_MODULES_CREATENEW, &ScriptEditorDialog::OnModules_CreateNew)
	ON_COMMAND(ID_COMPILER_RECOMPILEMODULE, &ScriptEditorDialog::OnCompiler_RecompileModule)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &ScriptEditorDialog::OnModuleList_Click)
	ON_COMMAND(ID_PROJECT_EVENTBINDINGS, &ScriptEditorDialog::OnProject_EventBindings)
	ON_WM_NCDESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// ScriptEditorDialog message handlers

void ScriptEditorDialog::OnModuleList_SelChange()
{
	// the window
	String wnd;
	c_Module.GetWindowText(wnd);

}

// c_moduleList
void ScriptEditorDialog::RefreshModules()
{
	ScriptingHost& sh = ScriptingHost::getInst();

	// clear module list
	c_moduleList.DeleteAllItems();

	// the list
	for (Umbra::Host::ModuleIterator itr = sh.modules.begin(); itr != sh.modules.end(); itr++)
	{
		int nId = c_moduleList.InsertItem(c_moduleList.GetItemCount(), itr->first.c_str());
		c_moduleList.SetItemData(nId, (DWORD_PTR)itr->second.getAutoId());
	}
}

void ScriptEditorDialog::SelectModule(int ID)
{
	// lookup this module
	Umbra::Module* pModule = ScriptingHost::getInst().getModuleById(ID);
	if (pModule == 0)
	{
		MessageBox("Unable to load that module", 0, MB_ICONEXCLAMATION);
		return;
	}

	// display its source code
	c_CodeWindow.SetWindowTextA(pModule->getSourceCode().c_str());
	current_module_id = ID;
}

Umbra::Module* ScriptEditorDialog::GetCurrentModule()
{
	return ScriptingHost::getInst().getModuleById(current_module_id);
}

BOOL ScriptEditorDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	current_module_id = -1;

	// set code font
	codeFont.CreatePointFont(80, "Courier New");
	c_CodeWindow.SetFont(&codeFont);

	// assign headers
	c_moduleList.InsertColumn(0, "Module", 0, 100);

	// refresh module list
	RefreshModules();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void ScriptEditorDialog::OnBnClickedCompile()
{
	OnCompiler_RecompileModule();	
}

void ScriptEditorDialog::OnOK()
{
}

#include "SingleLineDialog.h"

void ScriptEditorDialog::OnModules_CreateNew()
{
	// use single dialog
	::SingleLineDialog sl(this);
	sl.v_title = "Enter Module Name";
	sl.v_text  = String("newmod") + ToString(rand());
	
	// did it return ok, otherwise quit out
	if (sl.DoModal() != IDOK)
		return;

	// attempt to allocate new module
	Umbra::Module* pNewModule = ScriptingHost::getInst().getNewModule((LPCSTR)sl.v_text);
	if (pNewModule == 0)
	{
		MessageBox(_T("The module name that you have selected is not valid or is already taken"));
	}
	else
	{
		RefreshModules();
	}
}

void ScriptEditorDialog::OnCompiler_RecompileModule()
{
	Umbra::Module* pMod = GetCurrentModule();
	String code = c_CodeWindow.GetText();
	
	if (pMod && pMod->compile(code) == false)
	{
		CString err;
		err.Format("There was an error compiling the script: \n\n%s", pMod->getCompileError().c_str());
		MessageBox(err, 0, MB_ICONEXCLAMATION);
	}
	else
	{
		MessageBox("Module has been compiled");
	}
}

void ScriptEditorDialog::OnModuleList_Click(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	// get the item data
	int id = c_moduleList.GetByHitTest();
	if (id != -1)
	SelectModule((int)c_moduleList.GetItemData(id));

}

void ScriptEditorDialog::OnProject_EventBindings()
{
}

void ScriptEditorDialog::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	delete this;
}

void ScriptEditorDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	const int spacer = 10;
	const int button_height = 25;
	const int button_width = 200;

	// divisional
	if (IsWindow(c_moduleList) == TRUE)
	{
		c_moduleList.MoveWindow(spacer, spacer, 120, cy - spacer * 2);
		c_CodeWindow.MoveWindow(120 + spacer * 2, spacer, cx - spacer * 3 - 120, cy - spacer * 3 - button_height); 
		c_compile.MoveWindow(cx - spacer - button_width, cy - spacer - button_height, button_width, button_height);
	}
}
