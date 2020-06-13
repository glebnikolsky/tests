// ConvertScheme.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ConvertScheme.h"
#include <boost/crc.hpp>
#include <Shlobj.h>
#include <comutil.h>
#include <vector>
#include <boost\crc.hpp>

#pragma comment(lib,"Shell32.lib")
#pragma comment(lib,"comsuppw.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;
// Структура заголовка
// SCM\0 - схема на основе stream
// SCQ\0 - схема на основе sqLite
// версия схемы. Рекомендуется старшим байтом дублировать тип
// контрольная сумма
// длина схемы
// Пример  SCHM 01000000 01020304 ffff0000
//         sign  version   crc32...length.
//struct SchemeContainer{
//    SchemeContainer(void *buf){
//        sign_[0] = static_cast<char*>(buf)[0];
//        sign_[1] = static_cast<char*>(buf)[1];
//        sign_[2] = static_cast<char*>(buf)[2];
//        sign_[3] = static_cast<char*>(buf)[3];
//        version_ = static_cast<long*>(buf)[1];
//        crc32_   = static_cast<long*>(buf)[2];
//        length_  = static_cast<size_t*>(buf)[3];
//    }
//
//    SchemeContainer(char *sign, long ver, size_t len, long crc, void *buf){
//        memcpy(sign_, sign, 4);
//        sign_[4] = 0;
//        version_ = ver;
//        length_  = len;
//        crc32_   = crc;
//    }
//
//    char sign_[5];
//    long version_;
//    long crc32_;
//    size_t length_;
//}; 

bool GetTemporaryPah(CString &path)
{
    TCHAR path_buff[MAX_PATH];
    BOOL ret = GetTempPath(MAX_PATH, path_buff);
    if ( ret){
#ifndef _UNICODE
        _bstr_t tmp = path_buff;
        path = (char*)tmp;
#else
        path = path_buff;
#endif;
    }
    return ret;
}

bool GetMyDocument(CString &path)
{
    TCHAR path_buff[MAX_PATH];

    if(! SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL|CSIDL_FLAG_CREATE, NULL, 0, path_buff))) 
        return false;
#ifndef _UNICODE
    _bstr_t tmp = path_buff;
    path = (char*)tmp;
#else
    path = path_buff;
#endif;
    return true;
}

const size_t HeaderLength = 16;


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
            CString tmp, mydoc;
            bool i1,i2;
            i1 = GetTemporaryPah(tmp);
            i2 = GetMyDocument(mydoc);

            CFile flin;
            CFileException e;
            if ( !flin.Open(_T("D:\\Work\\tests\\ConvertScheme\\Debug\\erbd_gia_reg_13_03 201303 4.159 11.06.2013.fst"), CFile::modeRead, &e) ){
                CString tmp;
                TCHAR   err_cause[255];
                e.GetErrorMessage(err_cause, 255);
                tmp = flin.GetFilePath() + _T(" ") +err_cause;
                AfxMessageBox(tmp);
                return 0;
            }
            ULONGLONG len = flin.GetLength();
            std::vector<char> buf(static_cast<size_t>(len) + HeaderLength);
            flin.Read(&buf[HeaderLength], static_cast<UINT>(len));

            if ( !strcmp(&buf[HeaderLength], "SCM" ) ){
                AfxMessageBox(_T("Уже новый формат!"));
                return 0;
            }

            CFile flout;
            CString outname = flin.GetFilePath() + _T(".fst");
            if ( !flout.Open(outname.GetString(), CFile::modeCreate|CFile::modeWrite, &e) ){
                CString tmp;
                TCHAR   err_cause[255];
                e.GetErrorMessage(err_cause, 255);
                tmp = flin.GetFilePath() + _T(" ") +err_cause;
                AfxMessageBox(tmp);
                return 0;
            }
            boost::crc_32_type  result;
            result.process_bytes( &buf[HeaderLength], buf.size()- HeaderLength);
            long l = 0;
            memmove(&buf[0],"SCM", 4);
            memmove(&buf[4],&l, 4);
            l = result.checksum();
            memmove(&buf[8],&l, 4);
            l = buf.size();
            memmove(&buf[12],&l, 4);
            flout.Write(&buf[0], buf.size());
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
