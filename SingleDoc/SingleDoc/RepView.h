#pragma once
#include "RepGrid.h"
#include "afxwin.h"

// CrepView form view

class CRepView : public CFormView
{
	DECLARE_DYNCREATE(CRepView)

protected:
	CRepView();           // protected constructor used by dynamic creation
	virtual ~CRepView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REPORT };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
public:

public:
	CRepDoc* GetDocument();
	virtual void OnDraw( CDC* pDC );  // overridden to draw this view
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
	virtual void OnInitialUpdate();

protected:
	RepGrid grid_;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	ChkCombo cb1;
	afx_msg void OnBnClickedButton1();
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
};

#ifndef _DEBUG  
inline CRepDoc* CRepView::GetDocument()
{
	return (CEx14Doc*)m_pDocument;
}
#endif

