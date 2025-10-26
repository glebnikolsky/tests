
// ChildFrm.cpp: ðåàëèçàöèÿ êëàññà CChildFrame
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication22.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
END_MESSAGE_MAP()

// Ñîçäàíèå èëè óíè÷òîæåíèå CChildFrame

CChildFrame::CChildFrame() noexcept
{
	// TODO: äîáàâüòå êîä èíèöèàëèçàöèè ÷ëåíà
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: èçìåíèòå êëàññ Window èëè ñòèëè ïîñðåäñòâîì èçìåíåíèÿ CREATESTRUCT cs
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// Äèàãíîñòèêà CChildFrame

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}
#endif //_DEBUG

// Îáðàáîò÷èêè ñîîáùåíèé CChildFrame
