// DLL.h : main header file for the DLL DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CDLLApp
// See DLL.cpp for the implementation of this class
//

class CDLLApp : public CWinApp
{
public:
	CDLLApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
