
// MFCApplication22View.cpp: ðåàëèçàöèÿ êëàññà CMFCApplication22View
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS ìîæíî îïðåäåëèòü â îáðàáîò÷èêàõ ôèëüòðîâ ïðîñìîòðà ðåàëèçàöèè ïðîåêòà ATL, ýñêèçîâ
// è ïîèñêà; ïîçâîëÿåò ñîâìåñòíî èñïîëüçîâàòü êîä äîêóìåíòà â äàííûì ïðîåêòå.
#ifndef SHARED_HANDLERS
#include "MFCApplication22.h"
#endif

#include "MFCApplication22Doc.h"
#include "MFCApplication22View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication22View

IMPLEMENT_DYNCREATE(CMFCApplication22View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication22View, CView)
	// Ñòàíäàðòíûå êîìàíäû ïå÷àòè
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// Ñîçäàíèå èëè óíè÷òîæåíèå CMFCApplication22View

CMFCApplication22View::CMFCApplication22View() noexcept
{
	// TODO: äîáàâüòå êîä ñîçäàíèÿ

}

CMFCApplication22View::~CMFCApplication22View()
{
}

BOOL CMFCApplication22View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: èçìåíèòü êëàññ Window èëè ñòèëè ïîñðåäñòâîì èçìåíåíèÿ
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Ðèñîâàíèå CMFCApplication22View

void CMFCApplication22View::OnDraw(CDC* /*pDC*/)
{
	CMFCApplication22Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: äîáàâüòå çäåñü êîä îòðèñîâêè äëÿ ñîáñòâåííûõ äàííûõ
}


// Ïå÷àòü CMFCApplication22View

BOOL CMFCApplication22View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// ïîäãîòîâêà ïî óìîë÷àíèþ
	return DoPreparePrinting(pInfo);
}

void CMFCApplication22View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: äîáàâüòå äîïîëíèòåëüíóþ èíèöèàëèçàöèþ ïåðåä ïå÷àòüþ
}

void CMFCApplication22View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: äîáàâüòå î÷èñòêó ïîñëå ïå÷àòè
}


// Äèàãíîñòèêà CMFCApplication22View

#ifdef _DEBUG
void CMFCApplication22View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication22View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication22Doc* CMFCApplication22View::GetDocument() const // âñòðîåíà íåîòëàæåííàÿ âåðñèÿ
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication22Doc)));
	return (CMFCApplication22Doc*)m_pDocument;
}
#endif //_DEBUG


// Îáðàáîò÷èêè ñîîáùåíèé CMFCApplication22View
