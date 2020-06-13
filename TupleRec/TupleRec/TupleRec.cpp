// TupleRec.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TupleRec.h"
#include "boost\any.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

CString create_test = _T("\
create table ##testtable(\
    _xml xml, \
    _uniqueidentifier uniqueidentifier not null,\
    _bit bit not null,\
    _tinyint tinyint not null,\
    _smallint smallint not null,\
    _int int not null,\
    _bigint bigint not null,\
    _float24 float(24) not null,\
    _float53 float(53) not null,\
    _real real not null,\
    _smallmoney smallmoney  not null,\
    _money money  not null,\
    _decimal20_4 decimal(20,4)  not null,\
    _decimal20 decimal(20)  not null,\
    _decimal35 decimal(35)  not null,\
    _decimal35_5 decimal(35,5)  not null,\
    _char char  not null,\
    _char20 char(20) not null,\
    _char20_half char(20) not null,\
    _varchar20 varchar(20) not null,\
    _varchar20_half varchar(20) not null,\
    _varchar_max varchar(max) not null,\
    _date date not null,\
    _datetime datetime not null,\
    _datetime2_0 datetime2(0) not null,\
    _datetime2_1 datetime2(1) not null,\
    _datetime2_3 datetime2(3) not null,\
    _datetime2_7 datetime2(7) not null,\
    _smalldatetime smalldatetime not null,\
    _time time null,\
    _char20_null char(20)  null\
);\n\
insert into ##testtable values(\
'<someshit/>',\
    newid(),\
    1,\
    250,\
    32000,\
    2000000000,\
    2000000000000,\
    123456.789,\
    1234567890.1234567890,\
    1234.0123456789,\
    45678.9012,\
    1234567890123.4567,\
    1234567890123456.7899,\
    12345678901234567890,\
    12345678901234567890123456789012345,\
    123456789012345678901234567890.12345,\
    'a',\
    'abcdeabcdeabcdeabcde',\
    'abcdeabcde',\
    'abcdeabcdeabcdeabcde',\
    'abcdeabcde',\
    '12345678901234567890123456789012345.123456789012345678901234567890.12345,',\
    getdate(),\
    getdate(),\
    getdate(),\
    getdate(),\
    getdate(),\
    getdate(),\
    getdate(),\
    null,\
    null\
);");








struct skip_type;

long GetInt(CRecordset& rs, short idx)
{
    CDBVariant val;
    rs.GetFieldValue(idx, val);
    return val.m_lVal;
}

double GetDouble(CRecordset& rs, short idx)
{
    CDBVariant val;
    rs.GetFieldValue(idx, val);
    return val.m_dblVal;
}


CTime GetTime(CRecordset& rs, short idx)
{
    CDBVariant val;
    rs.GetFieldValue(idx, val);
    return CTime(val.m_pdate->year,val.m_pdate->month,val.m_pdate->day,val.m_pdate->hour,val.m_pdate->minute,val.m_pdate->second);
}

CString GetCStr(CRecordset& rs, short idx)
{
    CDBVariant val;
    rs.GetFieldValue(idx, val);
    return *val.m_pstringA;
}

std::string GetStr(CRecordset& rs, short idx)
{
    CDBVariant val;
    rs.GetFieldValue(idx, val);
    return val.m_pstringA->GetString();
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

            
            CDatabase db;
            CString connectStr = _T("DRIVER=SQL Server;SERVER={10.0.18.3};DATABASE={ERBD_EGE_MAIN_17_Fresh_20170424};UID={ra};PWD={Njkmrjcdjb};ConnectionTimeout=1000000");
            BOOL b = db.OpenEx(connectStr, CDatabase::useCursorLib);
            try{
                db.ExecuteSQL(create_test);
            }catch(CDBException*ex){
                TCHAR buf[1000];
                ex->GetErrorMessage(buf, 1000);
                std::cout<<buf<<'***'<<std::endl;
                ex->Delete();
            }
            std::cout<<"no;name;m_nPrecision;m_nScale;m_nSQLType;m_dwType\n";
            CRecordset rs(&db);
            rs.Open(CRecordset::forwardOnly,_T("select cast(_xml as varchar(max)), cast( _uniqueidentifier as varchar(100)) as _ui_as_varchar100, _bit, _tinyint, _smallint, _int, _bigint, _float24, _float53, _real, _smallmoney, _money, _decimal20_4, _decimal20, _decimal35, _decimal35_5, _char, _char20, _char20_half, _varchar20, _varchar20_half, _varchar_max, _date, _datetime, _datetime2_0, _datetime2_1, _datetime2_3, _datetime2_7, _smalldatetime, _time, _char20_null from ##testtable"));
            if ( !rs.IsEOF())
                for(short i = 0; i <rs.GetODBCFieldCount(); ++i){
                    CDBVariant val;
                    try{
                    CODBCFieldInfo fieldinfo;
                    rs.GetODBCFieldInfo(i,fieldinfo);
                    std::cout<<i+1<<';'<<fieldinfo.m_strName<<';'<<fieldinfo.m_nPrecision<<';'<<fieldinfo.m_nScale<<';'<<fieldinfo.m_nSQLType<<';';
                    rs.GetFieldValue(i, val);
                    CString s = *val.m_pstring;
                    std::cout<<'\n'<<s<<'\n';
                    }catch(CDBException*ex){
                        TCHAR buf[1000];
                        ex->GetErrorMessage(buf, 1000);
                        std::cout<<buf<<'***'<<std::endl;
                        ex->Delete();
                        continue;
                    } 
                    std::cout<<val.m_dwType<<std::endl;
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
