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
#include <sstream>


#include <boost/timer/timer.hpp>
#include <boost/filesystem.hpp>
#include <boost\format.hpp>
#include <boost/regex.hpp>

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
			rs.GetFieldValue( (short)2, val );
			xml += val.m_pstring->GetString();
		}
	}
	catch ( CDBException *ex ) {
		std::cout << ex->m_strError << '\n' << sql.GetString();
		ex->Delete();
		return false;
	}
	return true;
}

bool DB2fileSingle( CDatabase &db, long loadno, long type, string &xml, string &flnm )
{
	CfctPressOrders rs( &db );
	rs.m_nLoadNo = loadno;
	rs.m_nType = type;
	try {
		rs.Open( CRecordset::forwardOnly, NULL, CRecordset::readOnly );
		xml = rs.m_sXML;
		flnm = rs.m_sFilename;
	}
	catch ( CDBException *ex ) {
		std::cout << ex->m_strError;
		ex->Delete();
		return false;
	}
	return true;
}

bool DB2file( CDatabase &db, long loadno, long type, const string &outdir )
{
	CfctPressOrders rs( &db );
	rs.m_nLoadNo = loadno;
	rs.m_nType = type;
	try {
		rs.Open( CRecordset::forwardOnly, NULL, CRecordset::readOnly );
	}
	catch ( CDBException *ex ) {
		std::cout << ex->m_strError;
		ex->Delete();
		return false;
	}
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_string( rs.m_sXML.GetString() );
	if ( !result ) {
		cout << result.description();
		return false;
	}
	namespace fs = std::experimental::filesystem::v1;
	fs::path p( outdir );
	p /= rs.m_sFilename.GetString();
	bool b = doc.save_file( p.string().c_str(), "  " );
	return true;
}



bool FormatXML( const string &xml, const string &outdir, const string &outfile )
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_string( xml.c_str() );
	if ( !result ) {
		cout << result.description();
		return false;
	}
	namespace fs = std::experimental::filesystem::v1;
	fs::path p( outdir );
	p /= outfile;
	bool b = doc.save_file( p.string().c_str(), "  " );

	return true;
}

using namespace boost::filesystem;

struct FileInfo{
	FileInfo( const char *fn, bool fromfile ):filename( fn ), from_file( fromfile ), taskid( -1 ), tt( -1 ), subj( -1 ), variant( -1 ), type( -1 ), no_of_changes( -1 )
		, op_date( _T( "" ) ), sha1_from_base( _T( "" ) ), sha1_from_file( _T( "" ) ), username( _T( "" ) ), ip( _T( "" ) ){}
	bool operator<( const FileInfo &rt ){
		return filename < rt.filename && type <= rt.type;
	}
	CString filename;
	bool from_file;
	int taskid;
	int tt;
	int subj;
	int variant;
	int type;
	int no_of_changes;
	CString op_date;
	CString sha1_from_base;
	CString sha1_from_file;
	CString username;
	CString ip;
};

CString Filename( const TCHAR * fl )
{
	CString flnm = fl;
	if ( !flnm.IsEmpty() ){
		int pos = flnm.ReverseFind( '\\' );
		if ( pos >= 0 ) flnm = flnm.Mid( pos + 1 );
	}
	return flnm;
}

void SetXMLAttribute( const char *fpath , const TCHAR * fl1, const TCHAR * fl2 )
{
	pugi::xml_document doc;
	if ( Filename( fpath ) == Filename( fl2 ) ) return;
	pugi::xml_parse_result result = doc.load_file( fpath );
	pugi::xpath_query query( "/items/item" );
	pugi::xpath_node_set res = query.evaluate_node_set( doc );
	for ( size_t i = 0; i < res.size(); ++i ){
		res[i].node().append_attribute( "sort" );
		int old_mark = res[i].node().attribute( "mark" ).as_int(),
			max_mark = res[i].node().attribute( "max_mark" ).as_int();
		if ( Filename(fpath) != Filename(fl1) ){
			pugi::xml_node node = res[i].node().append_child( "new_mark");
			if ( old_mark < max_mark ) node.append_child( pugi::node_pcdata ).set_value( std::to_string( old_mark + 1 ).c_str() );
			else node.append_child( pugi::node_pcdata ).set_value( std::to_string( max_mark ? max_mark - 1 : 0 ).c_str() );
		}
	}
	doc.save_file( fpath );
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
			//test t{ 1 };
			//t.(1);
			//t.(2);

			boost::timer::auto_cpu_timer t;
			setlocale(LC_ALL, "ru-RU");
			if ( argc < 2 ){
				std::cout << _T("gentest путь [имя пустого файла] [имя неизменённого файла]\n\
путь - директория выгрузки\n\
имя пустого файла - для создания файла выгрузки из ФИПИ без изменений\n\
имя неизменённого файла - останется файл выгрузки без изменений\n\
будут преобразованы файлы выгрузки в файлы для загрузки");
				return 0;
			}
			//if (argc < 3) {
			//	cout << "Usage: ExecSQL loadno type";
			//	return 1;
			//}
	//		CString connectStr = 
	//_T( "DRIVER=SQL Server;SERVER={10.0.18.3};DATABASE={ERBD_EGE_MAIN_21_20210331};UID={ra};PWD={Njkmrjcdjb};Connect Timeout=300;" );
	//		CDatabase db;
	//		try{
	//		BOOL b = db.OpenEx( _T( "DRIVER=SQL Server;SERVER={10.0.18.3};DATABASE={ERBD_EGE_MAIN_21_20210331};UID={ra};PWD={Njkmrjcdjb}" ),
	//			CDatabase::noOdbcDialog | CDatabase::useCursorLib);
	//		}
	//		catch ( CDBException *ex ){
	//			CString s = ex->m_strError;
	//		}
	//		db.SetQueryTimeout(6000);
			path p( argv[1] );
			boost::regex e( "(?i)(.*\\\\)(.*)( - КИМ )\\d{6}\\.xml" );
			std::vector<FileInfo> fileinfo;
			for ( directory_entry const& x : directory_iterator( p ) ){
				boost::smatch what;
				if ( is_regular_file( x ) && boost::regex_match( x.path().string(), what, e, boost::match_perl ) ){
					SetXMLAttribute( x.path().string().c_str(), argc > 2 ? argv[2]:"", argc > 3 ? argv[3] : "" );
	
				}
			}
		}
	}
	else
	{
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
