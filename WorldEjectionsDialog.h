#pragma once
#include "world_ejections.h"
#include "listctrlgpx.h"

// WorldEjectionsDialog dialog

class WorldEjectionsDialog : public CDialog, public Listener
{
	DECLARE_DYNAMIC(WorldEjectionsDialog)

public:
	WorldEjectionsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~WorldEjectionsDialog();

// Dialog Data
	enum { IDD = IDD_WORLD_EJECTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public: // messaging
	void onListener(EventBroadcaster* Broadcaster, int Id, EventMessage* EventInfo);
	void updateList(Ejections::Ejection& Ej, bool First = false);
		
public:
	afx_msg void OnBnClickedQuery();
	ListCtrlGpx	_list;
	CStatusBar	_statusBar;

	virtual BOOL OnInitDialog();

public: // sorting mechanisms
	struct OnAddress : public ListCtrlGpx::Sorter
	{
		int sort(LPARAM Item1, LPARAM Item2, ListCtrlGpx::SortingParam* Options)
		{
			// lookup world items
			Ejections::Ejection* pW1 = (Ejections::Ejection*)Item1;
			Ejections::Ejection* pW2 = (Ejections::Ejection*)Item2;

			// comparisons
			return _stricmp(pW1->toString(), pW2->toString());
		}
	};

	struct OnType : public ListCtrlGpx::Sorter
	{
		int sort(LPARAM Item1, LPARAM Item2, ListCtrlGpx::SortingParam* Options)
		{
			// lookup world items
			Ejections::Ejection* pW1 = (Ejections::Ejection*)Item1;
			Ejections::Ejection* pW2 = (Ejections::Ejection*)Item2;

			// comparisons
			return _stricmp(pW1->getTypeName(), pW2->getTypeName());
		}
	};

	struct OnCreated : public ListCtrlGpx::Sorter
	{
		int sort(LPARAM Item1, LPARAM Item2, ListCtrlGpx::SortingParam* Options)
		{
			// lookup world items
			Ejections::Ejection* pW1 = (Ejections::Ejection*)Item1;
			Ejections::Ejection* pW2 = (Ejections::Ejection*)Item2;
			return ListCtrlGpx::Sorter::CommonInt(pW1->created, pW2->created); 
		}
	};

	struct OnExpired : public ListCtrlGpx::Sorter
	{
		int sort(LPARAM Item1, LPARAM Item2, ListCtrlGpx::SortingParam* Options)
		{
			// lookup world items
			Ejections::Ejection* pW1 = (Ejections::Ejection*)Item1;
			Ejections::Ejection* pW2 = (Ejections::Ejection*)Item2;
			return ListCtrlGpx::Sorter::CommonInt(pW1->expires, pW2->expires); 
		}
	};

	struct OnCommentName : public ListCtrlGpx::Sorter
	{
		int sort(LPARAM Item1, LPARAM Item2, ListCtrlGpx::SortingParam* Options)
		{
			// lookup world items
			Ejections::Ejection* pW1 = (Ejections::Ejection*)Item1;
			Ejections::Ejection* pW2 = (Ejections::Ejection*)Item2;

			// comparisons
			return _stricmp(pW1->getCommentName(), pW2->getCommentName());
		}
	};

	struct OnCommentEjector : public ListCtrlGpx::Sorter
	{
		int sort(LPARAM Item1, LPARAM Item2, ListCtrlGpx::SortingParam* Options)
		{
			// lookup world items
			Ejections::Ejection* pW1 = (Ejections::Ejection*)Item1;
			Ejections::Ejection* pW2 = (Ejections::Ejection*)Item2;

			// comparisons
			return _stricmp(pW1->getCommentEjector(), pW2->getCommentEjector());
		}
	};

private: // sorting algorithms
	OnAddress					_sortAddress;
	OnType						_sortType;
	OnCreated					_sortCreated;
	OnExpired					_sortExpired;
	OnCommentName			_sortCommentName;
	OnCommentEjector	_sortCommentEjector;

public:
	afx_msg void OnList_ToggleHeader(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNcDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
