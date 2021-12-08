
// RecordsetView.cpp : implementation of the CRecordsetView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Recordset.h"
#endif

#include "RecordsetDoc.h"
#include "RecordsetView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRecordsetView

IMPLEMENT_DYNCREATE(CRecordsetView, CView)

BEGIN_MESSAGE_MAP(CRecordsetView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CRecordsetView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CRecordsetView construction/destruction

CRecordsetView::CRecordsetView()
{
	// TODO: add construction code here

}

CRecordsetView::~CRecordsetView()
{
}

BOOL CRecordsetView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CRecordsetView drawing

void CRecordsetView::OnDraw(CDC* /*pDC*/)
{
	CRecordsetDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CRecordsetView printing


void CRecordsetView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CRecordsetView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRecordsetView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRecordsetView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CRecordsetView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CRecordsetView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CRecordsetView diagnostics

#ifdef _DEBUG
void CRecordsetView::AssertValid() const
{
	CView::AssertValid();
}

void CRecordsetView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRecordsetDoc* CRecordsetView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRecordsetDoc)));
	return (CRecordsetDoc*)m_pDocument;
}
#endif //_DEBUG


// CRecordsetView message handlers
