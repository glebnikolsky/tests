
// SingleDocView.cpp : implementation of the CSingleDocView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SingleDoc.h"
#endif

#include "SingleDocDoc.h"
#include "SingleDocView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSingleDocView

IMPLEMENT_DYNCREATE(CSingleDocView, CView)

BEGIN_MESSAGE_MAP(CSingleDocView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSingleDocView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CSingleDocView construction/destruction

CSingleDocView::CSingleDocView()
{
	// TODO: add construction code here

}

CSingleDocView::~CSingleDocView()
{
}

BOOL CSingleDocView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSingleDocView drawing

void CSingleDocView::OnDraw(CDC* /*pDC*/)
{
	CRepDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CSingleDocView printing


void CSingleDocView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSingleDocView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSingleDocView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSingleDocView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CSingleDocView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSingleDocView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSingleDocView diagnostics

#ifdef _DEBUG
void CSingleDocView::AssertValid() const
{
	CView::AssertValid();
}

void CSingleDocView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRepDoc* CSingleDocView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRepDoc)));
	return (CRepDoc*)m_pDocument;
}
#endif //_DEBUG


// CSingleDocView message handlers
