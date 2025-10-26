
// MainFrm.cpp: ðåàëèçàöèÿ êëàññà CMainFrame
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication22.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // èíäèêàòîð ñòðîêè ñîñòîÿíèÿ
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// Ñîçäàíèå èëè óíè÷òîæåíèå CMainFrame

CMainFrame::CMainFrame() noexcept
{
	// TODO: äîáàâüòå êîä èíèöèàëèçàöèè ÷ëåíà
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Íå óäàëîñü ñîçäàòü ñòðîêó ñîñòîÿíèÿ\n");
		return -1;      // íå óäàëîñü ñîçäàòü
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: èçìåíèòü êëàññ Window èëè ñòèëè ïîñðåäñòâîì èçìåíåíèÿ
	//  CREATESTRUCT cs

	return TRUE;
}

// Äèàãíîñòèêà CMainFrame

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG


// Îáðàáîò÷èêè ñîîáùåíèé CMainFrame

