// ChCwd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ChCwd.h"
#include <direct.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

// Вернёт cwd или пустую строку
CString GetCWD()
{
    TCHAR *buf;
    CString ret;
#ifndef _UNICODE
    if ( (buf = _getcwd(NULL, 0)) != NULL)
#else
    if ( (buf = _wgetcwd(NULL, 0)) != NULL)
#endif
        ret = buf;
    if ( buf )
        free(buf);
    return ret;
}


bool ChangeCWD(TCHAR *old_cwd, TCHAR* new_cwd, CString &err)
{
    int ndrive = 0;
#ifndef _UNICODE
    if ( tolower(*old_cwd) != tolower(*new_cwd) )
        ndrive = tolower(*new_cwd) - L'a' +1;
#else
    if ( towlower(*old_cwd) != towlower(*new_cwd) )
        ndrive = towlower(*new_cwd) - L'a' +1;
#endif
    if ( _chdrive(ndrive)){
        err = _T("Не могу переключиться на диск ");
        err += *new_cwd;
        return false;
    }
    if ( _chdir(new_cwd) ){
        err = _T("Не могу сменить директорию на ");
        err += new_cwd;
        return false;
    }
    return true;
}   


bool AdminRightNeeded()
{
    CFile fl;
    CFileException fex;
    if ( !fl.Open(_T("Check file to detect rights"), CFile::modeWrite|CFile::modeCreate, &fex) ){
        return false;
    }
    fl.Close();
    _unlink(_T("Check file to detect rights"));
    return true;
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
            if ( argc != 3){
                cout <<"Args!";
                exit(1);
            }
            TCHAR *buf;
			if ( (buf = _getcwd(NULL, 0)) == NULL){
                cout<<"Errrrrrroooooooooooorrrrrrrrrrrrrrrrrrrrrrrrrrr\n!";
            }
            else{
                CString olddir = buf, newdir = argv[1];
                olddir.MakeUpper();
                newdir.MakeUpper();

                if ( olddir[0] != newdir[0] ){
                    int ndrive = newdir[0]-L'A'+1;
                    if ( _chdrive(newdir[0]-L'A'+1)){
                        cout<<"Errrrrrroooooooooooorrrrrrrrrrrrrrrrrrrrrrrrrrr DDDiiiiiisssskkkk\n!"<<errno;
                        exit(1);
                    }
                }
                if ( _chdir(newdir)){
                    cout<<"Errrrrrroooooooooooorrrrrrrrrrrrrrrrrrrrrrrrrrr DDDir\n!";
                    exit(1);
                }
                CFile fl(_T("File in new cwd"), CFile::modeWrite|CFile::modeCreate);
                olddir += _T("\r\n");
                olddir += newdir;
                fl.Write(olddir.GetString(), olddir.GetLength());
                free(buf);
            }
            {
                CString tmp = GetCWD();
                CString err;
                if ( !ChangeCWD( (TCHAR*)tmp.GetString(), argv[2], err) )
                {
                    cout<<err.GetString();
                }
                CFile fl(_T("Another File in new cwd"), CFile::modeWrite|CFile::modeCreate);
                tmp += _T("\r\n");
                tmp += argv[2];
                fl.Write(tmp.GetString(), tmp.GetLength());
            }
            {
                CString err;
                cout<<AdminRightNeeded();
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
