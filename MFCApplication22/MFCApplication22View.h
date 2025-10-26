
// MFCApplication22View.h: èíòåðôåéñ êëàññà CMFCApplication22View
//

#pragma once


class CMFCApplication22View : public CView
{
protected: // ñîçäàòü òîëüêî èç ñåðèàëèçàöèè
	CMFCApplication22View() noexcept;
	DECLARE_DYNCREATE(CMFCApplication22View)

// Àòðèáóòû
public:
	CMFCApplication22Doc* GetDocument() const;

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
	virtual ~CMFCApplication22View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Ñîçäàííûå ôóíêöèè ñõåìû ñîîáùåíèé
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // âåðñèÿ îòëàäêè â MFCApplication22View.cpp
inline CMFCApplication22Doc* CMFCApplication22View::GetDocument() const
   { return reinterpret_cast<CMFCApplication22Doc*>(m_pDocument); }
#endif

