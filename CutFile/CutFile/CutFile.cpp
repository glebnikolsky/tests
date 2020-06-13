// CutFile.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CutFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

const int BUFSIZE = 1024*100;

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
            if (argc == 4 ){
                LONGLONG new_len = _wtoi64(argv[3]);
                if ( new_len){
                    CFile fin(argv[1],CFile::modeRead), fout(argv[2], CFile::modeCreate|CFile::modeWrite);
                    if ( fin.m_hFile == CFile::hFileNull ){
                        cout<<"Can't open "<<argv[1];
                        return 0;
                    }
                    if ( fout.m_hFile == CFile::hFileNull ){
                        cout<<"Can't create "<<argv[2];
                        return 0;
                    }
                    if ( new_len > fin.GetLength()){

                        cout<<"New length more than old!";
                        return 0;
                    }
                    unsigned char buf[BUFSIZE];

                    while( new_len){
                        UINT len = min(BUFSIZE, new_len);
                        fin.Read(buf, len);
                        fout.Write(buf,len);
                        new_len -= len;
                    }
                    return 0;
                }
            }
            cout<<"CutFile old_name new_name new_size";
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
