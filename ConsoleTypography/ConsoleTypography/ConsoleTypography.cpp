// ConsoleTypography.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ConsoleTypography.h"
#include <iostream>

#include "pugixml.hpp"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWinApp theApp;

using namespace std;




int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;
    setlocale(LC_ALL, "ru-RU");
	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
            CDatabase db;

            BOOL b = db.OpenEx(_T("DRIVER=SQL Server;SERVER={10.0.18.3};DATABASE={[ERBD_EGE_MAIN_19_20190430};UID={ra};PWD={Njkmrjcdjb};ConnectionTimeout=10000"), CDatabase::useCursorLib);

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
