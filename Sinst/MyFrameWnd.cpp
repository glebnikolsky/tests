#include "stdafx.h"
#include "MyFrameWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMyFrameWnd, CFrameWnd)

CMyFrameWnd::CMyFrameWnd()
{
}

CMyFrameWnd::~CMyFrameWnd()
{
}

BEGIN_MESSAGE_MAP(CMyFrameWnd, CFrameWnd)
  ON_MESSAGE(WM_COPYDATA, OnCopyData)
END_MESSAGE_MAP()

LRESULT CMyFrameWnd::OnCopyData(WPARAM /*wParam*/, LPARAM lParam)
{
  COPYDATASTRUCT* pCDS = reinterpret_cast<COPYDATASTRUCT*>(lParam);
  TCHAR* pszCmdLine = static_cast<TCHAR*>(pCDS->lpData);
  if (pszCmdLine)
  {
    CString sMsg;
    sMsg.Format(_T("Another instance passed us the command line: %s"), pszCmdLine);
    AfxMessageBox(sMsg);
  }
  return TRUE;
}
