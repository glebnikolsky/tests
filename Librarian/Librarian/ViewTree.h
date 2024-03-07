
#pragma once

/////////////////////////////////////////////////////////////////////////////
// Îêíî CViewTree

class CViewTree : public CTreeCtrl
{
// Ñîçäàíèå
public:
	CViewTree() noexcept;

// Ïåðåîïðåäåëåíèå
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// Ðåàëèçàöèÿ
public:
	virtual ~CViewTree();

protected:
	DECLARE_MESSAGE_MAP()
};
