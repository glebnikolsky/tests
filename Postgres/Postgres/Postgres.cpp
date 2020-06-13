// Postgres.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Postgres.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

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
			CDatabase db;
			CDatabase dbms;
			dbms.OpenEx(_T("DRIVER=SQL Server;SERVER={10.0.18.3};DATABASE={ERBD_EGE_MAIN_18_20180606};UID={ra};PWD={Njkmrjcdjb};ConnectionTimeout=10000"), CDatabase::useCursorLib);
			if (db.OpenEx(_T("DSN=TestPostgre;ConnectionTimeout=10000"), CDatabase::useCursorLib)) {
				std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
				CRecordset rs(&db);
				rs.Open(CRecordset::forwardOnly, _T("SELECT * FROM dbc.\"TestTable\""), CRecordset::none);
				for (; !rs.IsEOF(); rs.MoveNext()) {
					CString f1, f2;
					rs.GetFieldValue((short)0, f1);
					rs.GetFieldValue((short)1, f2);
					cout << '\t' << f1.GetString() << '\t' << f2.GetString() << '\n';
				}
				cout << _T("--------------------------------------ADD SOME-------------------------------------------------------------------------------------\n");
				rs.Close();
				for (int i = 2; i < 100; i++) {
					CString tmp;
					tmp.Format("insert into dbc.\"TestTable\"(\"someData\") values('%d')", i);
					db.ExecuteSQL(tmp);
				}
				rs.Open(CRecordset::forwardOnly, _T("SELECT * FROM dbc.\"TestTable\""), CRecordset::none);
				for (; !rs.IsEOF(); rs.MoveNext()) {
					CString f1, f2;
					rs.GetFieldValue((short)0, f1);
					rs.GetFieldValue((short)1, f2);
					cout << '\t' << f1.GetString() << '\t' << f2.GetString() << '\n';
				}
				cout << _T("--------------------------------------Delete SOME-------------------------------------------------------------------------------------\n");
				rs.Close();
				db.ExecuteSQL("delete from dbc.\"TestTable\" where \"someData\" like '1%'");
				rs.Open(CRecordset::forwardOnly, _T("SELECT * FROM dbc.\"TestTable\""), CRecordset::none);
				for (; !rs.IsEOF(); rs.MoveNext()) {
					CString f1, f2;
					rs.GetFieldValue((short)0, f1);
					rs.GetFieldValue((short)1, f2);
					cout << '\t' << f1.GetString() << '\t' << f2.GetString() << '\n';
				}
				cout << _T("--------------------------------------From mssql-------------------------------------------------------------------------------------\n");
				CRecordset rsm(&dbms);
				rsm.Open(CRecordset::forwardOnly, _T("SELECT * FROM [ERBD_EGE_MAIN_18_20180606].[dbo].[rbdc_ExamTechnologies]"), CRecordset::none);
				for (; !rsm.IsEOF(); rsm.MoveNext()) {
					CString f1, f2;
					rsm.GetFieldValue((short)1, f1);
					rsm.GetFieldValue((short)2, f2);
					cout << '\t' << f1.GetString() << '\t' << f2.GetString() << '\n';
				}

				rs.Close();
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
