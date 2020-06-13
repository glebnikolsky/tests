#include "stdafx.h"
#include "sinstance.h"
#include "MyFrameWnd.h"

class CMyApp : public CWinApp
{
  virtual BOOL InitInstance();
};

//the one and only app
CMyApp theApp;

BOOL CMyApp::InitInstance()
{
  //Check for the previous instance as soon as possible
  CInstanceChecker instanceChecker(_T("CInstanceChecker Example FrameWnd App"));
  instanceChecker.ActivateChecker();
  if (instanceChecker.PreviousInstanceRunning())
  {
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

	  AfxMessageBox(_T("Previous version detected, will now restore it"), MB_OK);
    instanceChecker.ActivatePreviousInstance(cmdInfo.m_strFileName);
	  return FALSE;
  }

  //Standard MFC stuff, create the mainframe
  CFrameWnd* pMainFrm = new CMyFrameWnd();
  if (!pMainFrm->Create(NULL, m_pszAppName))
    return FALSE;
  pMainFrm->ShowWindow(SW_SHOW);
  m_pMainWnd = pMainFrm;

  // If this is the first instance of our App then track it so any other instances can find us
  instanceChecker.TrackFirstInstanceRunning();

  //For testing purposes check the functions again to make sure they work when calling later on
  //in an apps lifetime
#ifdef _DEBUG
  BOOL bPrevInstance = instanceChecker.PreviousInstanceRunning();
  bPrevInstance;
  BOOL bTrack = instanceChecker.TrackFirstInstanceRunning();
  bTrack;
#endif

  return TRUE;
}



