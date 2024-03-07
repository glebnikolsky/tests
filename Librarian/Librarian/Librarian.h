
// Librarian.h: îñíîâíîé ôàéë çàãîëîâêà äëÿ ïðèëîæåíèÿ Librarian
//
#pragma once

#ifndef __AFXWIN_H__
	#error "âêëþ÷èòü pch.h äî âêëþ÷åíèÿ ýòîãî ôàéëà â PCH"
#endif

#include "resource.h"       // îñíîâíûå ñèìâîëû


// CLibrarianApp:
// Ñâåäåíèÿ î ðåàëèçàöèè ýòîãî êëàññà: Librarian.cpp
//

class CLibrarianApp : public CWinAppEx
{
public:
	CLibrarianApp() noexcept;


// Ïåðåîïðåäåëåíèå
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Ðåàëèçàöèÿ
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLibrarianApp theApp;
