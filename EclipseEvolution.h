// EclipseEvolution.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// EclipseEvolutionApp:
// See EclipseEvolution.cpp for the implementation of this class
//

class EclipseEvolutionApp : public CWinApp
{
public:
	EclipseEvolutionApp();

// Overrides
public:
	virtual BOOL InitInstance();
	HACCEL  m_haccel;

// Implementation

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAvatar_RelayLeave();
	virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);
};

extern EclipseEvolutionApp theApp;