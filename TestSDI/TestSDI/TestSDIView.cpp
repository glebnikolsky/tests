
// TestSDIView.cpp : implementation of the CTestSDIView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TestSDI.h"
#endif

#include "TestSDIDoc.h"
#include "TestSDIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestSDIView

IMPLEMENT_DYNCREATE(CTestSDIView, CView)

BEGIN_MESSAGE_MAP(CTestSDIView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CTestSDIView construction/destruction

CTestSDIView::CTestSDIView()
{
	// TODO: add construction code here

}

CTestSDIView::~CTestSDIView()
{
}

BOOL CTestSDIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTestSDIView drawing

void CTestSDIView::OnDraw(CDC* /*pDC*/)
{
	CTestSDIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CTestSDIView printing

BOOL CTestSDIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestSDIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestSDIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CTestSDIView diagnostics

#ifdef _DEBUG
void CTestSDIView::AssertValid() const
{
	CView::AssertValid();
}

void CTestSDIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestSDIDoc* CTestSDIView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestSDIDoc)));
	return (CTestSDIDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestSDIView message handlers
