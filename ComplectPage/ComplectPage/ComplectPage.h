
// ComplectPage.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CComplectPageApp:
// � ���������� ������� ������ ��. ComplectPage.cpp
//

class CComplectPageApp : public CWinApp
{
public:
	CComplectPageApp();

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CComplectPageApp theApp;