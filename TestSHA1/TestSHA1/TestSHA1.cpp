// TestSHA1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SHA1.h"
#include "TestSHA1.h"
#include <string.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

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
            CString ts = "23.11.2016 ";
            CTime ut(1970, 1, 1, 6, 6, 6);
            CTime begin(2009,1,1,6,6,6);
            CTime end = CTime::GetCurrentTime() + CTimeSpan(2,0,0,0);
            SYSTEMTIME st;
            COleDateTime dt;
            if(dt.ParseDateTime(ts)){
                if(dt.GetAsSystemTime(st)){
                    CTime ct(st);
                }else
                    int i = 123;
                }
            }

/*            
            _TCHAR *aa = _T("FST3");
            LONG ll = *((LONG*)aa);

			CFile fl(_T("D:\\sht\\6.19_new_fmt.fst"), CFile::modeRead );
            long tmp, cnt;
            ULONGLONG full_len, body, flen;
            flen = fl.GetLength();
            CArchive ar( &fl, CArchive::load);
            CString crcf,crcc;
            ar >> tmp;
            ar >> cnt;
            for(LONG i = 0; i < cnt; i++){
                ar >> tmp;
            }
            ar >> tmp;
            ar >> tmp;
            ar >> body;
            ar >> full_len;
            ar >> crcf;
            fl.Seek(full_len - body, CFile::begin);
            sha1_cfile( fl, crcc );
            std::cout<<full_len<<'\t'<<flen<<'\t'<<body<<std::endl;
            std::cout<<crcc<<std::endl<<crcf<<std::endl;
            CFile fl1(_T("D:\\sht\\test.tst"), CFile::modeRead );
            sha1_cfile( fl1, crcc );
            std::cout<<crcc<<std::endl;
            std::cout<<"****";
            */
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
