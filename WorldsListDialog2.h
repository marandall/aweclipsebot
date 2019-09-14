#pragma once
#include "listctrlgpx.h"
#include "listener.h"

// WorldsListDialog dialog
namespace WorldList
{
	class World;
}

class WorldsListDialog : public CDialog, public Listener
{
	DECLARE_DYNAMIC(WorldsListDialog)

public: // event handling
	void onListener(EventBroadcaster* Broadcaster, int Id, EventMessage* EventInfo);
	void updateItem(WorldList::World* World, int Index = 0);

public:
	WorldsListDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~WorldsListDialog();

// Dialog Data
	enum { IDD = IDD_WORLDS_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	ListCtrlGpx c_list;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnNcDestroy();
	afx_msg void OnList_HeaderClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOnTop();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
