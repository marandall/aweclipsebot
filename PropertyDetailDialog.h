#pragma once
#include "listctrlgpx.h"


// PropertyDetailDialog dialog
namespace Property
{
	class Objects;
}

class PropertyDetailDialog : public CDialog
{
	DECLARE_DYNAMIC(PropertyDetailDialog)

public:
	PropertyDetailDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~PropertyDetailDialog();

// Dialog Data
	enum { IDD = IDD_PROPERTY_DETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	ListCtrlGpx c_list;
	afx_msg void OnBnClickedOk();

public: // handling methods
	virtual BOOL OnInitDialog();
	void	viewList(Property::Objects& ObjList);
};