// testLoad.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "testLoad.h"
#include "loadFileUtils.h"
#include <boost/format.hpp>
//#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <sstream>
#include <fstream>
#include <string>

//#include <boost/timer/timer.hpp>
//#include <boost/tokenizer.hpp>
//#include <boost/filesystem.hpp>
//#include <boost/crc.hpp>
#include <locale>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/// \typedef tokenizer вспомогательный, для токенизации строк
//typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

// The one and only application object

CWinApp theApp;

//using namespace std;


CDatabase db;


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		CString connectStr;
        connectStr.Format(_T("DRIVER=SQL Server;SERVER={10.0.18.3};DATABASE={ERBD_EGE_MAIN_16__2016_09_26};UID={ra};PWD={Njkmrjcdjb};ConnectionTimeout=1000000"));
		//connectStr.Format(_T("DRIVER=SQL Server;SERVER={10.0.18.1};DATABASE={ERBD_EGE_MAIN_13_APP};UID={gnn};PWD={Code123Pin};ConnectionTimeout=10000"));
 		BOOL b = db.OpenEx(connectStr, CDatabase::useCursorLib);
//		db.ExecuteSQL(_T("use fct"));
        CString sql =_T("select cast((\
SELECT r.REGION AS '@code', r.RegionName AS '@name', \n\
(SELECT r.REGION AS  '@Region', SubjectCode AS '@SubjectCode', ExamDate AS '@ExamDate',\n\
VariantCode AS '@VariantCode', ZoneCode AS '@ZoneCode', StuffCode AS '@StuffCode',\n\
Barcode_R AS '@Barcode_R'\n\
FROM (SELECT PP.RegionCode, PP.SubjectCode, PP.ExamDate, \n\
(                     \n\
select  pf.VariantCode\n\
from fct.PdfFiles PF       \n\
        where PF.SubjectCode =  PP.SubjectCode \n\
        AND PF.ExamDate = PP.ExamDate\n\
            AND pf.ZoneCode = PP.ZoneCode\n\
            AND pf.PdfName = PP.PdfName\n\
            AND pf.RowNumber = PP.PdfNumber               ) as VariantCode\n\
            ,PP.ZoneCode, PP.StuffCode, PP.Barcode_R\n\
            \n\
            FROM fct.PressPre PP inner join fct.LoadFile LF ON \n\
            pp.LoadFileID = LF.LoadFileID    \n\
            WHERE PP.ExamDate > '2016.06.20' \n\
            and PP.SubjectCode  in (29,30,31,33)\n\
            ) AS subjects\n\
            WHERE subjects.RegionCode = r.REGION\n\
            FOR XML PATH('KimDatabase'), TYPE\n\
            ) AS Subjects\n\
            FROM rbdc_Regions AS r\n\
            FOR XML PATH('Region'), ROOT('regions')) as varchar(MAX))");
        
        CRecordset rs(&db);

        try{
            rs.Open(CRecordset::forwardOnly,sql,CRecordset::readOnly);
            CDBVariant val;
            CString s;
            rs.GetFieldValue((short)0,s);
            std::ofstream ofs("qq");
            ofs<<s.GetString();
        }catch(CDBException* pe){
            TCHAR buf[1000];
            pe->GetErrorMessage(buf, 1000);
            pe->Delete();
            std::string msg(buf);
            std::cout<<msg;
        }
        CString tok, tmp = "Дата экзамена;Предмет;Зона;Итого";
        int pos = 0;
        tok = tmp.Tokenize(";",pos);
        while( pos >0){
            std::cout<<tok.GetString()<<std::endl;
            tok = tmp.Tokenize(";",pos);
        }


                boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
                std::string qqqqq = to_simple_string(now);
   				std::ostringstream err;
				err<<boost::format(_T("%1$10d|%2$5d|")) % 1% 2;
                std::string s = err.str();
                err.str(std::string());
                s = err.str();
		ProcFiles(db, _T("D:\\Work\\csv"),200);
	}

	return nRetCode;
}


