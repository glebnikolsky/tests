
// TestDllsView.cpp : implementation of the CTestDllsView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TestDlls.h"
#endif

#include "TestDllsDoc.h"
#include "TestDllsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestDllsView

IMPLEMENT_DYNCREATE(CTestDllsView, CView)

BEGIN_MESSAGE_MAP(CTestDllsView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTestDllsView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CTestDllsView construction/destruction

CTestDllsView::CTestDllsView():first_time(true)
{
	// TODO: add construction code here

}

CTestDllsView::~CTestDllsView()
{
}

BOOL CTestDllsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTestDllsView drawing

void CTestDllsView::OnDraw(CDC* /*pDC*/)
{
	CTestDllsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CTestDllsView printing


void CTestDllsView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTestDllsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestDllsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestDllsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTestDllsView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTestDllsView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTestDllsView diagnostics

#ifdef _DEBUG
void CTestDllsView::AssertValid() const
{
	CView::AssertValid();
}

void CTestDllsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestDllsDoc* CTestDllsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestDllsDoc)));
	return (CTestDllsDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestDllsView message handlers


int CTestDllsView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CView::OnCreate( lpCreateStruct ) == -1 )
		return -1;

	RECT	rect;
	GetClientRect( &rect );
	grid_.CreateGrid( WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rect, this, 1234 );
	CenterWindow();
	if ( first_time ){
		first_time = false;
	}

	return 0;
}


void CTestDllsView::OnSize( UINT nType, int cx, int cy )
{
	CView::OnSize( nType, cx, cy );

	RECT rect;
	GetClientRect( &rect );
	grid_.MoveWindow( &rect );

}
