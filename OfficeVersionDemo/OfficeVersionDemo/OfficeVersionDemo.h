
// OfficeVersionDemo.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// COfficeVersionDemoApp:
// О реализации данного класса см. OfficeVersionDemo.cpp
//

class COfficeVersionDemoApp : public CWinApp
{
public:
	COfficeVersionDemoApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern COfficeVersionDemoApp theApp;