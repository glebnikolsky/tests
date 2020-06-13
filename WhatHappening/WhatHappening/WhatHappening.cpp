// WhatHappening.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WhatHappening.h"
#include <iostream>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

BOOL String2Time( _TCHAR *sTime, CTime& tResult )
{
    SYSTEMTIME st;
    COleDateTime dt;
    CTime fake = CTime(1999,1,1,0,0,0,0);
    if(dt.ParseDateTime(sTime) )
        if(dt.GetYear() > 1970 && dt.GetYear() < 2038 && dt.GetAsSystemTime(st) ) tResult = CTime(st);
        else tResult = fake;  
    else tResult = fake;  
    return tResult != fake;
}


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
            CTime tResult;
            CString tmp;
            std::cout<<String2Time( _T("01.01.1969"), tResult )<<std::endl;
            std::string st = tResult.Format(_T("%Y.%m.%d %H:%M:%S")).GetString();
            std::cout<<st<<std::endl;

            std::cout<<String2Time( _T("31.12.1971"), tResult )<<std::endl;
            st = tResult.Format(_T("%Y.%m.%d %H:%M:%S")).GetString();
            std::cout<<st<<std::endl;

            std::cout<<String2Time( _T("31.12.1971 23:24:25"), tResult )<<std::endl;
            st = tResult.Format(_T("%Y.%m.%d %H:%M:%S")).GetString();
            std::cout<<st<<std::endl;

            std::cout<<String2Time( _T("31.12.1971 24:24:25"), tResult )<<std::endl;
            st = tResult.Format(_T("%Y.%m.%d %H:%M:%S")).GetString();
            std::cout<<st<<std::endl;

            std::cout<<String2Time( _T("01.12.2018 23:24:25"), tResult )<<std::endl;
            st = tResult.Format(_T("%Y.%m.%d %H:%M:%S")).GetString();
            std::cout<<st<<std::endl;

		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
