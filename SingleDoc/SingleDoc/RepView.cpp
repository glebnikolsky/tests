// CrepView.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "RepDoc.h"
#include "combo.h"
#include "RepView.h"


// CrepView

IMPLEMENT_DYNCREATE(CRepView, CFormView)

CRepView::CRepView()
	: CFormView(IDD_REPORT)
{

}

CRepView::~CRepView()
{
}

void CRepView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_COMBO1, cb1 );
}

BEGIN_MESSAGE_MAP(CRepView, CFormView)
	ON_BN_CLICKED( IDC_BUTTON1, &CRepView::OnBnClickedButton1 )
ON_WM_CREATE()
END_MESSAGE_MAP()


// CrepView diagnostics

#ifdef _DEBUG
void CRepView::AssertValid() const
{
	CFormView::AssertValid();
}
CRepDoc* CRepView::GetDocument() // non-debug version is inline
{
	ASSERT( m_pDocument->IsKindOf( RUNTIME_CLASS( CRepDoc ) ) );
	return (CRepDoc*)m_pDocument;
}

#ifndef _WIN32_WCE
void CRepView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CRepView::OnDraw( CDC* pDC )
{
	CRepDoc* pDoc = GetDocument();
	ASSERT_VALID( pDoc );
	pDoc->SetTitle( _T( "Всякая фигня" ) );

}

BOOL CRepView::PreCreateWindow( CREATESTRUCT& cs )
{
	return CFormView::PreCreateWindow( cs );
}

void CRepView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	grid_.AttachGrid( this, IDC_GRID );
	cb1.AddString( _T( "qwerty1" ) );
	cb1.AddString( _T( "qwerty2" ) );
	cb1.AddString( _T( "qwerty3" ) );
	cb1.AddString( _T( "qwerty4" ) );
	cb1.AddString( _T( "qwerty5" ) );
	cb1.AddString( _T( "qwerty6" ) );
	cb1.AddString( _T( "qwerty7" ) );
	cb1.AddString( _T( "qwerty8" ) );
}


void CRepView::OnBnClickedButton1()
{
	int j{0};
	for ( int i = 0; i < cb1.GetCount(); ++i ) if ( cb1.GetCheck( i ) )++j;
	int n = j;
}


int CRepView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CFormView::OnCreate( lpCreateStruct ) == -1 )
		return -1;

	return 0;
}
