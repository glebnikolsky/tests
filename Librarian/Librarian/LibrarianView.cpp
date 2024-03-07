
// LibrarianView.cpp: ðåàëèçàöèÿ êëàññà CLibrarianView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS ìîæíî îïðåäåëèòü â îáðàáîò÷èêàõ ôèëüòðîâ ïðîñìîòðà ðåàëèçàöèè ïðîåêòà ATL, ýñêèçîâ
// è ïîèñêà; ïîçâîëÿåò ñîâìåñòíî èñïîëüçîâàòü êîä äîêóìåíòà â äàííûì ïðîåêòå.
#ifndef SHARED_HANDLERS
#include "Librarian.h"
#endif

#include "LibrarianDoc.h"
#include "LibrarianView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLibrarianView

IMPLEMENT_DYNCREATE(CLibrarianView, CView)

BEGIN_MESSAGE_MAP(CLibrarianView, CView)
	// Ñòàíäàðòíûå êîìàíäû ïå÷àòè
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLibrarianView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// Ñîçäàíèå èëè óíè÷òîæåíèå CLibrarianView

CLibrarianView::CLibrarianView() noexcept
{
	// TODO: äîáàâüòå êîä ñîçäàíèÿ

}

CLibrarianView::~CLibrarianView()
{
}

BOOL CLibrarianView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: èçìåíèòü êëàññ Window èëè ñòèëè ïîñðåäñòâîì èçìåíåíèÿ
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Ðèñîâàíèå CLibrarianView

void CLibrarianView::OnDraw(CDC* /*pDC*/)
{
	CLibrarianDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: äîáàâüòå çäåñü êîä îòðèñîâêè äëÿ ñîáñòâåííûõ äàííûõ
}


// Ïå÷àòü CLibrarianView


void CLibrarianView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CLibrarianView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// ïîäãîòîâêà ïî óìîë÷àíèþ
	return DoPreparePrinting(pInfo);
}

void CLibrarianView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: äîáàâüòå äîïîëíèòåëüíóþ èíèöèàëèçàöèþ ïåðåä ïå÷àòüþ
}

void CLibrarianView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: äîáàâüòå î÷èñòêó ïîñëå ïå÷àòè
}

void CLibrarianView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CLibrarianView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Äèàãíîñòèêà CLibrarianView

#ifdef _DEBUG
void CLibrarianView::AssertValid() const
{
	CView::AssertValid();
}

void CLibrarianView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLibrarianDoc* CLibrarianView::GetDocument() const // âñòðîåíà íåîòëàæåííàÿ âåðñèÿ
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLibrarianDoc)));
	return (CLibrarianDoc*)m_pDocument;
}
#endif //_DEBUG


// Îáðàáîò÷èêè ñîîáùåíèé CLibrarianView
