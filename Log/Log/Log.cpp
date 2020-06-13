// Log.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Log.h"
#include <fstream>
#include <string>

//#include <boost\format.hpp>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;


struct SimpleLog {
	SimpleLog(CString pfx) :flnm_(pfx) {

		CTime tm(CTime::GetCurrentTime());
		flnm_ += _T("%Y-%m-%d-%H-%M-%S.log");
		flnm_ = tm.Format(flnm_);
	}

	~SimpleLog() {
		if (os_.is_open()) {
			os_ << "\n";
			os_.close();
		}
	}

	void Init() {
		os_.open(flnm_, std::ios_base::ate);
		if (!os_.is_open()) {
			flnm_ = _T(" Не создаётся файл в текущей директории\n") + flnm_;
			//AfxMessageBox(flnm_);
			std::cerr << flnm_.GetString();
		}
	}

	std::ostream& Out(const char *fl, int ln, CString msg = _T("")) {
		if (os_.is_open()) {
			CTime tm(CTime::GetCurrentTime());
			os_ << "\n" << fl << "\t" << ln << "\t" << tm.Format(_T("%Y.%m.%d %H.%M.%S")).GetString();
			if (!msg.IsEmpty()) os_ << "\t" << msg.GetString();
		}
		return os_;
	}

	CString flnm_;
	std::ofstream os_;
};

#define LOGTXT(lg, txt) lg.Out(__FILE__, __LINE__, txt)
#define LOGLINE(lg) lg.Out(__FILE__, __LINE__)



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
			SimpleLog lg(_T("qwerty"));
			lg.Init();
			lg.Out(__FILE__, __LINE__, _T("qqqq"));
            // TODO: code your application's behavior here.
			lg.Out(__FILE__, __LINE__);
			lg.Out(__FILE__, __LINE__)<<_T(" всякая хуйня");
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
