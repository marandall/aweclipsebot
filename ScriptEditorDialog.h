#pragma once
#include "afxwin.h"
#include "textbox.h"
#include "listctrlgpx.h"


// ScriptEditorDialog dialog
namespace Umbra { class Module; }

class ScriptEditorDialog : public CDialog
{
	DECLARE_DYNAMIC(ScriptEditorDialog)

public:
	ScriptEditorDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ScriptEditorDialog();

// Dialog Data
	enum { IDD = IDD_UMBRA_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	int current_module_id;

	DECLARE_MESSAGE_MAP()
public:
	CFont			codeFont;
	CComboBox		c_Module;
	TextBox			c_CodeWindow;
	afx_msg void	OnModuleList_SelChange();
	virtual BOOL	OnInitDialog();
	afx_msg void	OnBnClickedCompile();
protected:
	virtual void OnOK();
public:
	ListCtrlGpx c_moduleList;

public: // update functions
	void RefreshModules();
	void SelectModule(int ID);
	Umbra::Module* GetCurrentModule();

	afx_msg void OnModules_CreateNew();
	afx_msg void OnCompiler_RecompileModule();
	afx_msg void OnModuleList_Click(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnProject_EventBindings();
	afx_msg void OnNcDestroy();
	CButton c_compile;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
