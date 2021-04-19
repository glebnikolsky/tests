// ExecSQL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ExecSQL.h"
#include "fctPressOrders.h"
#include "pugixml.hpp"

#include <algorithm>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

#include <boost/timer/timer.hpp>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

bool DB2fileMulti(CDatabase &db, long loadno, long type, string &xml, string &flnm)
{
	CRecordset rs(&db);
	CString sql;
	sql.Format(_T("{call fct.ccp_GeneratePressXmlMultiRow( %d, %d) }"), loadno, type);
	try {
		rs.Open(CRecordset::snapshot, sql, CRecordset::forwardOnly);
		for (; !rs.IsEOF(); rs.MoveNext()) {
			CDBVariant val;
			if (flnm.empty()) {
				rs.GetFieldValue((short)0, val);
				flnm = val.m_pstring->GetString();
			}
			rs.GetFieldValue((short)2, val);
			xml += val.m_pstring->GetString();
		}
	}
	catch (CDBException *ex) {
		std::cout << ex->m_strError<<'\n'<<sql.GetString();
		ex->Delete();
		return false;
	}
	return true;
}

bool DB2fileSingle(CDatabase &db, long loadno, long type, string &xml, string &flnm)
{
	CfctPressOrders rs(&db);
	rs.m_nLoadNo = loadno;
	rs.m_nType = type;
	try {
		rs.Open(CRecordset::forwardOnly, NULL, CRecordset::readOnly);
		xml = rs.m_sXML;
		flnm = rs.m_sFilename;
	}
	catch (CDBException *ex) {
		std::cout << ex->m_strError;
		ex->Delete();
		return false;
	}
	return true;
}

bool DB2file(CDatabase &db, long loadno, long type, const string &outdir)
{
	CfctPressOrders rs(&db);
	rs.m_nLoadNo = loadno;
	rs.m_nType = type;
	try {
		rs.Open(CRecordset::forwardOnly, NULL, CRecordset::readOnly);
	}
	catch (CDBException *ex) {
		std::cout << ex->m_strError;
		ex->Delete();
		return false;
	}
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_string(rs.m_sXML.GetString());
	if (!result) {
		cout << result.description();
		return false;
	}
	namespace fs = std::experimental::filesystem::v1;
	fs::path p(outdir);
	p /= rs.m_sFilename.GetString();
	bool b = doc.save_file(p.string().c_str(), "  ");
	return true;
}



bool FormatXML(const string &xml, const string &outdir, const string &outfile)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_string(xml.c_str());
	if (!result) {
		cout << result.description();
		return false;
	}
	namespace fs = std::experimental::filesystem::v1;
	fs::path p(outdir);
	p /= outfile;
	bool b = doc.save_file(p.string().c_str(), "  ");

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
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			boost::timer::auto_cpu_timer t;
			setlocale(LC_ALL, "ru-RU");
			if (argc < 3) {
				cout << "Usage: ExecSQL loadno type";
				return 1;
			}
			CString connectStr;
			connectStr.Format(_T("DRIVER=SQL Server;SERVER={10.0.18.3};DATABASE={ERBD_EGE_MAIN_19_20190430};UID={ra};PWD={Njkmrjcdjb};Connect Timeout=300;"), CDatabase::useCursorLib);
			CDatabase db;
			BOOL b = db.OpenEx(connectStr, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
			db.SetQueryTimeout(6000);
			b = DB2file(db, atol(argv[1]), atol(argv[2]), "d:\\tmp");
			//string xml, outfile;
			//{
			//	boost::timer::auto_cpu_timer t;
			//	if (!DB2fileSingle(db, atol(argv[1]), atol(argv[2]), xml, outfile) || !FormatXML(xml, "d:\\tmp\\single", outfile)) return 1;
			//}
			//xml.clear();
			//{
			//	boost::timer::auto_cpu_timer t;
			//	if (!DB2fileMulti(db, atol(argv[1]), atol(argv[2]), xml, outfile) || !FormatXML(xml, "d:\\tmp\\multi", outfile)) return 1;
			//}
		}
	}
	else
	{
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
