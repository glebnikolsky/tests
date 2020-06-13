#include "stdafx.h"
#include "sinstancedlg.h"
#include "sinstancedlgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CSinstancedlgApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CSinstancedlgApp::CSinstancedlgApp()
{
}

CSinstancedlgApp theApp;

BOOL CSinstancedlgApp::InitInstance()
{
	CSinstancedlgDlg dlg;
	m_pMainWnd = &dlg;
  dlg.m_InstanceChecker.ActivateChecker();
  if (dlg.m_InstanceChecker.PreviousInstanceRunning())
  {
    AfxMessageBox(_T("Previous version detected, will now restore it"), MB_OK);
    dlg.m_InstanceChecker.ActivatePreviousInstance();
    m_pMainWnd = NULL;
    return FALSE;
  }
	dlg.DoModal();

	return FALSE;
}
