// TestScheme.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TestScheme.h"

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
            CRegKey key;
            if ( key.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Federal Test Center\\Gia14 - Office\\Global Settings")) ==  ERROR_SUCCESS){
                _TCHAR buf[1000];
                ULONG sz;
                if ( key.QueryStringValue(_T("Код представительства"),buf,&sz) ==  ERROR_SUCCESS)
                    AfxMessageBox(buf);
                else 
                    AfxMessageBox(_T("Блин!"));
            }else AfxMessageBox(_T("Жопа!"));

            CFile fl(_T("D:\\tmp\\6.19_fmt_2016.fst"), CFile::modeRead);
            LONGLONG fsiz = fl.GetLength();
            _TCHAR buf[4];
            UINT rd = fl.Read(buf,4);
            if ( !memcmp(buf,_T("FST3"),4)){
                fl.Close();
                fl.Open(_T("D:\\tmp\\6.19_fmt_2016.fst"), CFile::modeRead);
                CArchive ar(&fl, CArchive::load);
                _TCHAR dummy;
                long sz;
                long tmp;
                ar>>dummy>>dummy>>dummy>>dummy>>sz;
                fl.Seek(0,CFile::begin);
                ar>>dummy>>dummy>>dummy>>dummy>>sz;
                while(sz--)
                    ar>>tmp;
                LONGLONG dat, tot;
                long gen, typ;
                CString crc;
                ar>>gen>>typ>>dat>>tot>>crc;

                //int *tmp = static_cast<int*>(&buf[4]);
                //int skip = ((*tmp) + 2)*4;
                //tmp = (long*)&buf[skip];
                ////Gen
                //++tmp;
                ////ProjType;
                //++tmp;
                ////datasize
                //++tmp;
                ////totalsize
                //++tmp;
                //_TCHAR *cp = (_TCHAR *)tmp;
                //++cp;
                //++cp;
                //++tmp;
                CString c = crc;


            }
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
