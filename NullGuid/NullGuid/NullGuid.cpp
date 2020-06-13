// NullGuid.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "NullGuid.h"
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;


void Test()
{
    CDatabase db;
    BOOL b = db.OpenEx(_T("DRIVER=SQL Server;SERVER={10.0.18.3};DATABASE={ERBD_EGE_MAIN_17_Fresh_20170424};UID={ra};PWD={Njkmrjcdjb};ConnectionTimeout=1000000"), 
        CDatabase::useCursorLib);
    CRecordset rs(&db);

    rs.Open(CRecordset::forwardOnly, 
        _T("SELECT cast(RecheckList as varchar(max)) FROM [dbo].[ac_RecheckGroups]"), 
        CRecordset::readOnly);
        for(short fld = 0; fld < rs.GetODBCFieldCount(); ++fld){
            //CString s;
            //rs.GetFieldValue(fld, s);
            //cout<<s.GetString()<<endl;
            CODBCFieldInfo fi;
            rs.GetODBCFieldInfo(fld, fi);
            cout<<fi.m_strName<<": "<<fi.m_nSQLType<<endl;

            CDBVariant val;
			rs.GetFieldValue(fld, val);
			if (val.m_dwType == 0x0b)
			{
				BSTR tmp = val.m_pstring->AllocSysString();
//				cout << tmp-> << endl;
			}

            //if ( val.m_dwType)
            //    cout<<val.m_pstring->GetString()<<endl;
            //else cout<<"NULL: "<<val.m_dwType<<endl;
        }

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
			Test();
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
