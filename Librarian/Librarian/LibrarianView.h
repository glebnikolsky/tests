
// LibrarianView.h: èíòåðôåéñ êëàññà CLibrarianView
//

#pragma once


class CLibrarianView : public CView
{
protected: // ñîçäàòü òîëüêî èç ñåðèàëèçàöèè
	CLibrarianView() noexcept;
	DECLARE_DYNCREATE(CLibrarianView)

// Àòðèáóòû
public:
	CLibrarianDoc* GetDocument() const;

// Îïåðàöèè
public:

// Ïåðåîïðåäåëåíèå
public:
	virtual void OnDraw(CDC* pDC);  // ïåðåîïðåäåëåíî äëÿ îòðèñîâêè ýòîãî ïðåäñòàâëåíèÿ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Ðåàëèçàöèÿ
public:
	virtual ~CLibrarianView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Ñîçäàííûå ôóíêöèè ñõåìû ñîîáùåíèé
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // âåðñèÿ îòëàäêè â LibrarianView.cpp
inline CLibrarianDoc* CLibrarianView::GetDocument() const
   { return reinterpret_cast<CLibrarianDoc*>(m_pDocument); }
#endif

