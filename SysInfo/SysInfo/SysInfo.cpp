// SysInfo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SysInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;
CString GetWindowsVersionName()
{
    typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
    typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);
    #define PRODUCT_PROFESSIONAL	0x00000030
    #define VER_SUITE_WH_SERVER	0x00008000
    CString res;
    OSVERSIONINFOEX osvi ={0};
    SYSTEM_INFO si = {0};
    BOOL bOsVersionInfoEx;
    DWORD dwType; 
    //ZeroMemory(&si, sizeof(SYSTEM_INFO));
    //ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX)); 
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*) &osvi);
    if(bOsVersionInfoEx == 0)
        return res; 
    PGNSI pGNSI = (PGNSI) GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetNativeSystemInfo");
    if(NULL != pGNSI)
        pGNSI(&si);
    else 
        GetSystemInfo(&si); // Check for unsupported OS
    if (VER_PLATFORM_WIN32_NT != osvi.dwPlatformId || osvi.dwMajorVersion <= 4 ) {
        return false;
    } 
    res = _T("Microsoft "); // Test for the specific product. 
    if ( osvi.dwMajorVersion == 6 )
    {
        if( osvi.dwMinorVersion == 0 )
        {
            if( osvi.wProductType == VER_NT_WORKSTATION )
                res += _T("Windows Vista ");
            else 
                res += _T("Windows Server 2008 ");
        }  
        if ( osvi.dwMinorVersion == 1 )
        {
            if( osvi.wProductType == VER_NT_WORKSTATION )
                res += _T("Windows 7 ");
            else 
                res += _T("Windows Server 2008 R2 ");
        }
        if( osvi.dwMinorVersion == 2)
        {
            if( osvi.wProductType == VER_NT_WORKSTATION )
                res += _T("Windows 8 ");
            else 
                res += _T("Windows Server 2012 ");
        }
        if ( osvi.dwMinorVersion == 3)
        {
            if( osvi.wProductType == VER_NT_WORKSTATION )
                res += _T("Windows 8.1 ");
            else 
                res += _T("Windows Server 2012 R2");
        }
        PGPI pGPI = (PGPI) GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetProductInfo");
        pGPI( osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType);  
        switch( dwType )
        {
        case PRODUCT_ULTIMATE:
            res += _T("Ultimate Edition");
            break;
        case PRODUCT_PROFESSIONAL:
            res += _T("Professional");
            break;
        case PRODUCT_HOME_PREMIUM:
            res += _T("Home Premium Edition");
            break;
        case PRODUCT_HOME_BASIC:
            res += _T("Home Basic Edition");
            break;
        case PRODUCT_ENTERPRISE:
            res += _T("Enterprise Edition");
            break;
        case PRODUCT_BUSINESS:
            res += _T("Business Edition");
            break;
        case PRODUCT_STARTER:
            res += _T("Starter Edition");
            break;
        case PRODUCT_CLUSTER_SERVER:
            res += _T("Cluster Server Edition");
            break;
        case PRODUCT_DATACENTER_SERVER:
            res += _T("Datacenter Edition");
            break;
        case PRODUCT_DATACENTER_SERVER_CORE:
            res += _T("Datacenter Edition (core installation)");
            break;
        case PRODUCT_ENTERPRISE_SERVER:
            res += _T("Enterprise Edition");
            break;
        case PRODUCT_ENTERPRISE_SERVER_CORE:
            res += _T("Enterprise Edition (core installation)");
            break;
        case PRODUCT_ENTERPRISE_SERVER_IA64:
            res += _T("Enterprise Edition for Itanium-based Systems");
            break;
        case PRODUCT_SMALLBUSINESS_SERVER:
            res += _T("Small Business Server");
            break;
        case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
            res += _T("Small Business Server Premium Edition");
            break;
        case PRODUCT_STANDARD_SERVER:
            res += _T("Standard Edition");
            break;
        case PRODUCT_STANDARD_SERVER_CORE:
            res += _T("Standard Edition (core installation)");
            break;
        case PRODUCT_WEB_SERVER:
            res += _T("Web Server Edition");
            break;
        }
    } 
    if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
    {
        if( GetSystemMetrics(SM_SERVERR2) )
            res += _T("Windows Server 2003 R2, ");
        else if ( osvi.wSuiteMask & VER_SUITE_STORAGE_SERVER )
            res += _T("Windows Storage Server 2003");
        else if ( osvi.wSuiteMask & VER_SUITE_WH_SERVER )
            res += _T("Windows Home Server");
        else if( osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
            res += _T("Windows XP Professional x64 Edition");
        else 
            res += _T("Windows Server 2003, ");  // Test for the server type.
        if ( osvi.wProductType != VER_NT_WORKSTATION )
        {
            if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_IA64 )
            {
                if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                    res += _T("Datacenter Edition for Itanium-based Systems");
                else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                    res += _T("Enterprise Edition for Itanium-based Systems");
            }   
            else if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
            {
                if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                    res += _T("Datacenter x64 Edition");
                else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                    res += _T("Enterprise x64 Edition");
                else res += _T("Standard x64 Edition");
            }   
            else
            {
                if ( osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER )
                    res += _T("Compute Cluster Edition");
                else if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                    res += _T("Datacenter Edition");
                else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                    res += _T("Enterprise Edition");
                else if ( osvi.wSuiteMask & VER_SUITE_BLADE )
                    res += _T("Web Edition");
                else 
                    res += _T("Standard Edition");
            }
        }
    }//if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 ) 
    if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
    {
        res += _T("Windows XP ");
        if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
            res += _T("Home Edition");
        else res += _T("Professional");
    } 
    if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
    {
        res += _T("Windows 2000 ");  
        if ( osvi.wProductType == VER_NT_WORKSTATION )
            res += _T("Professional");
        else 
        {
            if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                res += _T("Datacenter Server");
            else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                res += _T("Advanced Server");
            else res += _T("Server");
        }
    } // Include service pack (if any) and build number. if(wcslen(osvi.szCSDVersion) > 0) {
    res += _T(" ");
    res += osvi.szCSDVersion;

    res += _T(" (build ");
    CString tmp;
    tmp.Format(_T("%d)"), osvi.dwBuildNumber);
    res += tmp;
    if ( osvi.dwMajorVersion >= 6 ) 
    {
        if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
            res += _T(", 64-bit");
        else if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_INTEL )
            res += _T(", 32-bit");
    }
    return res; 
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
			CString tmp = GetWindowsVersionName();
            CString qqq = tmp + L'*';
            cout<<qqq.GetString();
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
