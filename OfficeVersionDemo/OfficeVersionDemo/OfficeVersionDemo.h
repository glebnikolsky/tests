
// OfficeVersionDemo.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// COfficeVersionDemoApp:
// � ���������� ������� ������ ��. OfficeVersionDemo.cpp
//

class COfficeVersionDemoApp : public CWinApp
{
public:
	COfficeVersionDemoApp();

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern COfficeVersionDemoApp theApp;