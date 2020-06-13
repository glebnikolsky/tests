
// TestSDI.h : main header file for the TestSDI application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTestSDIApp:
// See TestSDI.cpp for the implementation of this class
//

class CTestSDIApp : public CWinApp
{
public:
	CTestSDIApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTestSDIApp theApp;
