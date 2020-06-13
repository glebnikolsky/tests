// TestFolderDialog.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "direct.h"
#include "TestFolderDialog.h"
#include "boost\filesystem.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;
CString IOSlash(CString sDirectory,
                                 BOOL    bAddSlash=true)
{
    LONG nLength = sDirectory.GetLength(); 
    if(bAddSlash)
    {
        if(nLength > 0)
        {
            if(sDirectory.GetAt(nLength - 1) != _T('\\'))
                sDirectory += _T('\\');
        }
        else
        {
            sDirectory = _T(".\\");
        }
    }
    else
    {
        if(nLength > 0)
        {
            if(sDirectory.GetAt(nLength - 1) == _T('\\'))
                sDirectory = sDirectory.Left(nLength - 1);
        }
        else
        {
            sDirectory = _T(".");
        }
    }

    return sDirectory;
}


bool SelectDirectory(CString &dir, const TCHAR *title, bool showFiles)
{
    bool not_selected = false;
    do{
        BROWSEINFO cBrowseInfo = { 0 };
        cBrowseInfo.hwndOwner  = NULL;
        cBrowseInfo.ulFlags = BIF_NEWDIALOGSTYLE|BIF_RETURNONLYFSDIRS| (showFiles ? BIF_BROWSEINCLUDEFILES : 0);
        cBrowseInfo.lpszTitle  = title;
        LPITEMIDLIST lpSelected = ::SHBrowseForFolder(&cBrowseInfo);
        if(lpSelected == NULL)
            return false;
        LPTSTR pBuffer = dir.GetBufferSetLength(MAX_PATH*2+1);
        ::SHGetPathFromIDList(lpSelected, pBuffer);
        ::GlobalFree(lpSelected);
        dir = pBuffer;
        boost::filesystem::path p(dir.GetString());
        not_selected = !is_directory( p );
        if ( not_selected && ::AfxMessageBox(_T("Не выбрана директория. \r\nПовторить?"), MB_YESNO) != IDYES )
            return false;
    }while(not_selected);

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
            //CFolderPickerDialog  fFolder;
            //fFolder.m_ofn.lpstrTitle = _T("Выберите директорию с типографскими файлами");
            //fFolder.m_ofn.lpstrInitialDir = _T(".");
            //fFolder.m_ofn.lpstrFilter
            //fFolder.m_ofn.Flags = OFN_EXPLORER;
            //fFolder.DoModal();

            CString dir, d;
            if ( SelectDirectory(dir, _T("we rfihawofh  ehfljdhsg kljdhs gkljhdskljghdkjshf gkljd"), false))
            {
                d = dir;
            }
            if ( SelectDirectory(dir, _T("12312 52345 346546 56 3467347 546745754"), true))
            {
                d = dir;
            }
            //CString tmp = _T("C:\\");
            //BROWSEINFO cBrowseInfo = { 0 };
            //cBrowseInfo.hwndOwner  = NULL;
            //cBrowseInfo.ulFlags = BIF_NEWDIALOGSTYLE|BIF_BROWSEINCLUDEFILES|BIF_RETURNONLYFSDIRS;
            //cBrowseInfo.lpszTitle  = _T("Выбор директории");
            //LPITEMIDLIST lpSelected = ::SHBrowseForFolder(&cBrowseInfo);
            //if(lpSelected == NULL)
            //    return 0;
            //CString sSelectedPath;
            //LPTSTR pBuffer = sSelectedPath.GetBufferSetLength(MAX_PATH*2+1);
            //::SHGetPathFromIDList(lpSelected, pBuffer);
            //::GlobalFree(lpSelected);
            //sSelectedPath = pBuffer;
            //::IOSlash(sSelectedPath, TRUE);

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
