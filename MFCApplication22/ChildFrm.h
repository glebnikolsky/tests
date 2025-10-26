
// ChildFrm.h: èíòåðôåéñ êëàññà CChildFrame
//

#pragma once

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame() noexcept;

// Àòðèáóòû
protected:
	CSplitterWnd m_wndSplitter;
public:

// Îïåðàöèè
public:

// Ïåðåîïðåäåëåíèå
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Ðåàëèçàöèÿ
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Ñîçäàííûå ôóíêöèè ñõåìû ñîîáùåíèé
protected:
	DECLARE_MESSAGE_MAP()
};
