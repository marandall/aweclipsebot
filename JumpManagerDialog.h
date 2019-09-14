#pragma once
#include "textbox.h"
#include "listctrlgpx.h"
#include "Beacons.h"

// JumpManagerDialog dialog

class JumpManagerDialog : public CDialog
{
	DECLARE_DYNAMIC(JumpManagerDialog)

public:
	JumpManagerDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~JumpManagerDialog();

// Dialog Data
	enum { IDD = IDD_JUMP_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public: // handling lists
	Beacons::ProcessingList search_results;

public:
	TextBox c_name;
	TextBox c_owner;
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnBnClickedSearch();
	ListCtrlGpx c_list;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnList_DoubleRightClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnList_DoubleClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnJumps_Import();
	afx_msg void OnJumps_ImportOld();
	afx_msg void OnJumps_Export();
	afx_msg void OnJumps_InsertNew();
	afx_msg void OnJumps_Reset();
	afx_msg void OnSearch_ExportResults();
	afx_msg void OnSearch_EmptyResults();
	afx_msg void OnSearch_TransferOwner();
	afx_msg void OnSearch_EraseAll();
	afx_msg void OnNcDestroy();
	afx_msg void OnJumps_ConfigOptions();
};
