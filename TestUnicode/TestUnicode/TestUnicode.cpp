// TestUnicode.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TestUnicode.h"
#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <comdef.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

void Test()
{

    CFile fl;
    if ( fl.Open(_T("D:\\tmp\\09_171111_1_kim_3140.csv "), CFile::modeRead) ){
        _TCHAR *buf = new char[fl.GetLength()+2];
        buf[fl.GetLength()] = 0;
        size_t len = fl.GetLength();
        fl.Read( buf, fl.GetLength() );
        fl.Close();
        std::string fbody = buf;
        //setlocale(LC_ALL, "ru-RU");
        if ( fbody.substr(0,3) == std::string(_T("\xef" "\xbb" "\xbf")) || 
            fbody.substr(0,2) == std::string(_T("\xfe" "\xef"))  || 
            fbody.substr(0,2) == std::string(_T("\xff" "\xfe")) ){
            wchar_t *w = (wchar_t *)buf;
            w[len/2] = 0;
            ++w;
            _bstr_t tmp(w);
            std::string str;
            str =  tmp;
            cout<<str;
        }
    }
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
			Test();
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
