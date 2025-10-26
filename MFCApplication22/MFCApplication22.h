
// MFCApplication22.h: îñíîâíîé ôàéë çàãîëîâêà äëÿ ïðèëîæåíèÿ MFCApplication22
//
#pragma once

#ifndef __AFXWIN_H__
	#error "âêëþ÷èòü pch.h äî âêëþ÷åíèÿ ýòîãî ôàéëà â PCH"
#endif

#include "resource.h"       // îñíîâíûå ñèìâîëû


// CMFCApplication22App:
// Ñâåäåíèÿ î ðåàëèçàöèè ýòîãî êëàññà: MFCApplication22.cpp
//

class CMFCApplication22App : public CWinApp
{
public:
	CMFCApplication22App() noexcept;


// Ïåðåîïðåäåëåíèå
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Ðåàëèçàöèÿ
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCApplication22App theApp;
