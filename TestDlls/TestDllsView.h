
// TestDllsView.h : interface of the CTestDllsView class
//

#pragma once
#include "MyCUG.h"


class CTestDllsView : public CView
{
protected: // create from serialization only
	CTestDllsView();
	DECLARE_DYNCREATE(CTestDllsView)

// Attributes
public:
	CTestDllsDoc* GetDocument() const;
	MyCug grid_;
	bool first_time;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CTestDllsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSize( UINT nType, int cx, int cy );
};

#ifndef _DEBUG  // debug version in TestDllsView.cpp
inline CTestDllsDoc* CTestDllsView::GetDocument() const
   { return reinterpret_cast<CTestDllsDoc*>(m_pDocument); }
#endif

