
// MainFrm.h: èíòåðôåéñ êëàññà CMainFrame
//

#pragma once

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame() noexcept;

// Àòðèáóòû
public:

// Îïåðàöèè
public:

// Ïåðåîïðåäåëåíèå
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Ðåàëèçàöèÿ
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // âñòðîåííûå ÷ëåíû ïàíåëè ýëåìåíòîâ óïðàâëåíèÿ
	CStatusBar        m_wndStatusBar;

// Ñîçäàííûå ôóíêöèè ñõåìû ñîîáùåíèé
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

};


