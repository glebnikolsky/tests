// TestAuth.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TestAuth.h"
#include <iostream>



#import "D:\u_use\Interfaces\TR55\IUSE55Authorizer.tlb" no_namespace, named_guids

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;

// The one and only application object

CWinApp theApp;

using namespace std;

bool release_auth = true;
bool release_check_db = true;

void TryAuth(void)
{
	CString app_name = _T("Typography");
	short nStationCode = 101;
	LONG nDBType = 2;
	CString comment;
	CString *pComment = &comment;

	TRACE(_T("CFTCClient::InitializeTroughABBYYAuth\r\n"));
	TRACE(_T("\tABBY Инициализация клиента для приложения '%s'\r\n"), app_name);
	CString msg(_T(""));
	TRACE(_T("\tИнициализация авторизатора ABBYY...\r\n"));

	// создаем объект авторизации
	HRESULT hr = CoInitialize(NULL);
	CComPtr<IAuthorization> auth = NULL;
	CLSID auth_id;
	_bstr_t instName = L"USE54AuthorizerModule.CAuthorization";
	//#if( LIB_VERSION != 201410 )
	instName = L"USE55AuthorizerModule.CAuthorization";
	//#endif
	TRACE(_T("Module name - %s\r\n"), CString(instName.GetBSTR()).GetString());
	CLSIDFromProgID(instName, &auth_id);
	HRESULT ahr = auth.CoCreateInstance(auth_id);
	if (FAILED(ahr)) {
		msg.Format(_T("Ошибка (%X) создания объекта %s"), hr, CString(instName.GetBSTR()).GetString());
		if (pComment != NULL)
			*pComment = msg;
		else
			AfxMessageBox(msg);
	}
	else {
		CComPtr<IAuthResult> result = auth->TryAuthorize((short)nDBType, (short)nStationCode);
		if (result->IsSuccess) {
			AfxMessageBox(_T("!!!"));
		}
		else if (result->IsCanceled) {
			msg = _T("Отменено пользователем!");
			AfxMessageBox(msg);
		}
		else {
			_bstr_t errmsg = L"";
			result->get_ErrDesc(errmsg.GetAddress());
			msg.Format(_T("Ошибка авторизации:\r\n:Код [%d]: %s"), result->ErrCode, CString(errmsg.GetBSTR()).GetString());
			if (pComment != NULL)
				*pComment = msg;
			else
				AfxMessageBox(msg);
		}
		result.Release();
		auth.Release();
	}
	::CoUninitialize();
}


void TryCheck()
{
	CString dsn = _T("10.0.18.3");
	TRACE(_T("Authorization::IsConnected\r\n"));
	VARIANT_BOOL bRet(FALSE);
	BSTR DSN = dsn.AllocSysString();
	TRACE(_T("Авторизация - 55\r\n"));
	HRESULT hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	CComPtr<IAuthorization> auth = NULL;
	CLSID auth_id;
	_bstr_t instName = L"USE55AuthorizerModule.CAuthorization";
	CLSIDFromProgID(instName, &auth_id);
	HRESULT ahr = auth.CoCreateInstance(auth_id);
	if (FAILED(ahr)) {
		if ( release_auth ) auth.Release();
		release_check_db = true;
	}
	else {
		CComPtr<IAuthResult> result = auth->CheckHeadDB(DSN, 1);
//		IAuthResultPtr result = auth->CheckHeadDB(DSN, 1);
		if (result) {
			bRet = result->DB1Exist;
			if (release_check_db) result.Release();
			if (release_auth) auth.Release();
		}
	}
	::SysFreeString(DSN);
	::CoUninitialize();
}


void TestCreate()
{
	TRACE(_T("Authorization::CreateDB\r\n"));
	BSTR ErrMsg = NULL;
	long ErrCode;
	CString saLogin = _T("ra");
	CString saPWD = _T("Njkmrjcdjb");
	BSTR Login = saLogin.AllocSysString();
	BSTR PWD = saPWD.AllocSysString();
	CString serverName = _T("10.0.18.3");
	BSTR SvrName = serverName.AllocSysString();
	VARIANT_BOOL bRet = FALSE;
	TRACE(_T("Авторизация - 55\r\n"));
	HRESULT hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	{
		CComPtr<IAuthorization> auth = NULL;
		CLSID auth_id;
		_bstr_t instName = L"USE55AuthorizerModule.CAuthorization";
		CLSIDFromProgID(instName, &auth_id);
		HRESULT ahr = auth.CoCreateInstance(auth_id);
		if (FAILED(ahr)) {
			CString msg;
			msg.Format(_T(" Ошибка (%X) создания объекта %s"), hr, CString(instName.GetBSTR()).GetString());
		}
		else {
			bRet = auth->CreateHeadDB(Login, PWD, SvrName, &ErrCode, &ErrMsg);
			//auth.Release();
		}
	}
	if (bRet == VARIANT_FALSE) {
	}
	::CoUninitialize();
	::SysFreeString(Login);
	::SysFreeString(PWD);
	::SysFreeString(SvrName);
}



int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // initialize MFC and print and error on failure
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: change error code to suit your needs
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
        {
			int no = 1;
			while (no) {
				cout<< _T("1. Authorize\n2. Check db\n3. Togglle release_auth. Now = ")<< release_auth<< _T("\n4.Togglle release_check_db. Now = ")<<release_check_db<<_T("\n5. Create TRDB\n0. Exit\n>");
				cin >> no;
				switch (no) {
				case 1:
					TryAuth();
					break;
				case 2:
					TryCheck();
					break;
				case 3:
					release_auth = !release_auth;
					break;
				case 4:
					release_check_db = !release_check_db;
					break;
				case 5:
					TestCreate();
					break;
				default:
					break;
				}
			}
        }
    }
    else
    {
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}
