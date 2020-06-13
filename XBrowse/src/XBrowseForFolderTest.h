// XBrowseForFolderTest.h : main header file for the XBROWSEFORFOLDERTEST application
//

#ifndef XBROWSEFORFOLDERTEST_H
#define XBROWSEFORFOLDERTEST_H

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXBrowseForFolderTestApp:
// See XBrowseForFolderTest.cpp for the implementation of this class
//

class CXBrowseForFolderTestApp : public CWinApp
{
public:
	CXBrowseForFolderTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXBrowseForFolderTestApp)
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXBrowseForFolderTestApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //XBROWSEFORFOLDERTEST_H
