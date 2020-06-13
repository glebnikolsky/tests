// XBrowseForFolderTest.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "XBrowseForFolderTest.h"
#include "XBrowseForFolderTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXBrowseForFolderTestApp

BEGIN_MESSAGE_MAP(CXBrowseForFolderTestApp, CWinApp)
	//{{AFX_MSG_MAP(CXBrowseForFolderTestApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXBrowseForFolderTestApp construction

CXBrowseForFolderTestApp::CXBrowseForFolderTestApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CXBrowseForFolderTestApp object

CXBrowseForFolderTestApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CXBrowseForFolderTestApp initialization

BOOL CXBrowseForFolderTestApp::InitInstance()
{
	TRACE(_T("in CXBrowseForFolderTestApp::InitInstance\n"));

#if _MFC_VER < 0x700
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif

	// must call CoInitialize() if new dialog style (BIF_NEWDIALOGSTYLE) is used
	CoInitialize(NULL);

	CXBrowseForFolderTestDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	return FALSE;
}

int CXBrowseForFolderTestApp::ExitInstance() 
{
	CoUninitialize();
	return CWinApp::ExitInstance();
}
