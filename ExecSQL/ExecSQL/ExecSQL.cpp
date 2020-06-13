// ExecSQL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ExecSQL.h"

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
/*
			USE [test]
			GO

				SET ANSI_NULLS ON
				GO

				SET QUOTED_IDENTIFIER ON
				GO

				CREATE TABLE [dbo].[test](
				[idx] [int] IDENTITY(1,1) NOT NULL,
				[intval] [int] NOT NULL,
				[charval] [nchar](10) NOT NULL
				) ON [PRIMARY]

			GO
*/
			CString connectStr;
			connectStr.Format(_T("DRIVER=SQL Server;SERVER={(local)\\SQLEXPRESS};DATABASE={test};UID={gnn};PWD={Code123Pin};ConnectionTimeout=10000"));
			CDatabase db;
			BOOL b = db.OpenEx(connectStr, CDatabase::useCursorLib);
			if ( b ){
				{
					try{
						db.ExecuteSQL(_T("delete from test;"));
						db.ExecuteSQL(_T("insert into test(intval,charval) values(1,N'1');") // OK
						_T("insert into test(intval,charval) values(N'2',2);") // OK
						_T("insert into test(intval) values(3);")); // charval NOT NULL
					}catch(CDBException* pe){
						pe->Delete();
					}
					CRecordset rc(&db);
					rc.Open(CRecordset::snapshot ,_T("select count(*) from test"));
					CDBVariant val;
					rc.GetFieldValue((short)0, val);
					ASSERT(val.m_iVal == 2);
				}
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
