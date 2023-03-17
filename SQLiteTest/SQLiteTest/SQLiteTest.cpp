// SQLiteTest.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "SQLiteTest.h"
#include "sqlite3pp.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/timer/timer.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using SQDb = sqlite3pp::database;
using SQQuery = sqlite3pp::query;
using SQRec = sqlite3pp::query::iterator;
using SQExeption = sqlite3pp::database_error;

CWinApp theApp;

using namespace std;

TCHAR values[] = _T( "0123456789abcdefghijklvyjpqrstuvwxyzABCDEFGHIJKLVYJPQRSTUVWXYZÀÁÂÃÄÅ¨ÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞßàáâãäå¸æçèéêëìíîïğñòóôõö÷øùúûüışÿ" );
vector<long> strdata( 1000000 );
vector<long> intdata( 1000000 );
vector<long> refs( 1000000 );


void GenTest()
{
	boost::minstd_rand gen( 37 );
	boost::timer::auto_cpu_timer t;
	boost::random::uniform_int_distribution<long> str( 10, _countof( values ) - 1 );
	boost::random::uniform_int_distribution<long> integer( 0, LONG_MAX );
	boost::random::uniform_int_distribution<long> refss( 1, 5);
	for ( int i = 0; i < 1000000; ++i ){
		intdata[i] = integer( gen );
		strdata[i] = str( gen );
		refs[i] = refss( gen );
	}
}

void TestInsert( sqlite3pp::database &db, int n )
{
	boost::timer::auto_cpu_timer t;
	sqlite3pp::transaction xct( db );
	sqlite3pp::command cmd( db, "INSERT INTO Test(TestInt, TestText) VALUES (?, ?)" );
	for ( int i = 0; i < n; ++i ){
		cmd.bind( 1, intdata[i] );
		TCHAR ch = values[strdata[i]];
		values[strdata[i]] = '\0';
		cmd.bind( 2, values, sqlite3pp::copy );
		values[strdata[i]] = ch;
		cmd.execute();
		cmd.reset();
		long long id = db.last_insert_rowid();
		sqlite3pp::command cmd_ref( db, "INSERT INTO TestRef(TestID, TestInt, TestLongLong, TestText, TestFloat, TestDouble) VALUES (?, ?, ?, ?, ?, ?)" );
		for ( int j = 0; j < refs[i]; ++j ){
			cmd_ref.bind( 1, id );
			cmd_ref.bind( 2, intdata[j] );
			long long ll = id * 100000000000ll;
			cmd_ref.bind( 3, ll+j );
			TCHAR ch = values[strdata[i]];
			values[strdata[i]] = '\0';
			cmd_ref.bind( 4, values, sqlite3pp::copy );
			float f = 1.0*id*id*id*id*id*id*id*id*id*id;
			double d = 1.0*id*id*id*id*id*id*id*id*id*id*id*id*id*id*id*id*id*id*id*id;
			cmd_ref.bind( 5, f );
			cmd_ref.bind( 6, d );
			values[strdata[i]] = ch;
			cmd_ref.execute();
			cmd_ref.reset();
		}
		if ( i && !(i % 10000) ) cout << i << '\r';
	}
	xct.commit();
	cout << "TestInsert(" << n << ")\n";
}

template<typename T>bool GetFieldVal( sqlite3pp::query::iterator &rec, int field_no, T& val )
{
	switch ( (*rec).column_type( field_no ) ){
	case SQLITE_INTEGER:
		val = boost::lexical_cast<T>((*rec).get<long long>( field_no ));
		break;

	case SQLITE_TEXT:
		val = boost::lexical_cast<T>((*rec).get<const char*>( field_no ));
		break;

	case SQLITE_FLOAT:
		val = boost::lexical_cast<T>((*rec).get<double>( field_no ));
		break;

	case SQLITE_BLOB:
		val = boost::lexical_cast<T>((*rec).get<const void *>( field_no ));
		break;

	case SQLITE_NULL:
		return false;
	}
	return true;
}

void ShowRecord( sqlite3pp::query::iterator r, int fields )
{
	for ( int f = 0; f < fields; ++f ){
		std::string s;
		if ( !GetFieldVal( r, f, s ) ) s = "NULL";
		cout << s << '\t';
	}
	cout << '\n';
}

void ShowTable( sqlite3pp::database &db, const TCHAR* sql )
{
	cout << '\n' << sql << '\n'<<"---------------------------------------------\n";
	try{
		sqlite3pp::query qry( db, sql );
		std::cout << (begin(qry) != end(qry)) << '\n';
		for ( int i = 0; i < qry.column_count(); ++i ) cout << qry.column_name( i ) << "\t";
		cout << "\n---------------------------------------------\n";
		for ( auto r = qry.begin(); r != qry.end(); ++r ) ShowRecord( r, qry.column_count() );
		cout << "---------------------------------------------\n";
	}
	catch(sqlite3pp::database_error dberr){
		std::cout << "Error: "<<dberr.what()<<'\n';
	}

}

string LoadSQL( const TCHAR* filename )
{
	ifstream in( filename );
	string sql, tmp;
	while ( getline( in, tmp ) ) sql += tmp + "\n";
	return sql;
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
			setlocale( LC_ALL, "ru-RU" );
			sqlite3pp::database db1( _T( "D:\\tmp\\ERBD_EGE_MAIN_22.db" ) );
TCHAR *qq = "select * from pressPre where packnumber in('00212111',\n\
'00212617',\n\
'00006859',\n\
'00011901',\n\
'00052524',\n\
'00082423',\n\
'00139181',\n\
'00160606',\n\
'00161350',\n\
'00021287',\n\
'00100843',\n\
'00167312',\n\
'00200941',\n\
'00036744',\n\
'00051663',\n\
'00080272',\n\
'00107137',\n\
'00110179',\n\
'00174688',\n\
'00209268',\n\
'00009627',\n\
'00025567',\n\
'00033911',\n\
'00032601',\n\
'00075985',\n\
'00089213',\n\
'00095621',\n\
'00104593',\n\
'00170932',\n\
'00020646',\n\
'00024837',\n\
'00053648',\n\
'00107753',\n\
'00135003',\n\
'00185171',\n\
'00000353',\n\
'00155121',\n\
'00208088',\n\
'00035670',\n\
'00055374',\n\
'00059879',\n\
'00136550',\n\
'00149442',\n\
'00149943',\n\
'00157921',\n\
'00209675',\n\
'00023953',\n\
'00028456',\n\
'00161894',\n\
'00188571',\n\
'00202502',\n\
'00004706',\n\
'00014879',\n\
'00045485',\n\
'00101817',\n\
'00121511',\n\
'00149147',\n\
'00154010',\n\
'00163082',\n\
'00170834',\n\
'00184615',\n\
'00215028',\n\
'00027962',\n\
'00032953',\n\
'00045441',\n\
'00075340',\n\
'00097553',\n\
'00113612',\n\
'00119604',\n\
'00158675',\n\
'00185140',\n\
'00070215',\n\
'00084627',\n\
'00090719',\n\
'00101287',\n\
'00131171',\n\
'00140682',\n\
'00147153',\n\
'00212361',\n\
'00010832',\n\
'00011533',\n\
'00079917',\n\
'00100051',\n\
'00131639',\n\
'00005653',\n\
'00028857',\n\
'00147507',\n\
'00202170',\n\
'00024841',\n\
'00032272',\n\
'00032424',\n\
'00108838',\n\
'00129760',\n\
'00003898',\n\
'00095095',\n\
'00103947',\n\
'00122668',\n\
'00125955',\n\
'00133236',\n\
'00175734',\n\
'00212617',\n\
'00006859',\n\
'00011901',\n\
'00052524',\n\
'00082423',\n\
'00139181',\n\
'00160606',\n\
'00161350',\n\
'00021287',\n\
'00100843',\n\
'00167312',\n\
'00200941',\n\
'00036744',\n\
'00051663',\n\
'00080272',\n\
'00107137',\n\
'00110179',\n\
'00174688',\n\
'00209268',\n\
'00009627',\n\
'00025567',\n\
'00033911',\n\
'00032601',\n\
'00075985',\n\
'00089213',\n\
'00095621',\n\
'00104593',\n\
'00170932',\n\
'00020646',\n\
'00024837',\n\
'00053648',\n\
'00107753',\n\
'00135003',\n\
'00185171',\n\
'00000353',\n\
'00155121',\n\
'00208088',\n\
'00035670',\n\
'00055374',\n\
'00059879',\n\
'00136550',\n\
'00149442',\n\
'00149943',\n\
'00157921',\n\
'00209675',\n\
'00023953',\n\
'00028456',\n\
'00161894',\n\
'00188571',\n\
'00202502',\n\
'00004706',\n\
'00014879',\n\
'00045485',\n\
'00101817',\n\
'00121511',\n\
'00149147',\n\
'00154010',\n\
'00163082',\n\
'00170834',\n\
'00184615',\n\
'00215028',\n\
'00027962',\n\
'00032953',\n\
'00045441',\n\
'00075340',\n\
'00097553',\n\
'00113612',\n\
'00119604',\n\
'00158675',\n\
'00185140',\n\
'00070215',\n\
'00084627',\n\
'00090719',\n\
'00101287',\n\
'00131171',\n\
'00140682',\n\
'00147153',\n\
'00212361',\n\
'00010832',\n\
'00011533',\n\
'00079917',\n\
'00100051',\n\
'00131639',\n\
'00005653',\n\
'00028857',\n\
'00147507',\n\
'00202170',\n\
'00024841',\n\
'00032272',\n\
'00032424',\n\
'00108838',\n\
'00129760',\n\
'00003898',\n\
'00095095',\n\
'00103947',\n\
'00122668',\n\
'00125955',\n\
'00133236',\n\
'00175734',\n\
'00212617',\n\
'00006859',\n\
'00011901',\n\
'00052524',\n\
'00082423',\n\
'00139181',\n\
'00160606',\n\
'00161350',\n\
'00021287',\n\
'00100843',\n\
'00167312',\n\
'00200941',\n\
'00036744',\n\
'00051663',\n\
'00080272',\n\
'00107137',\n\
'00110179',\n\
'00174688',\n\
'00209268',\n\
'00009627',\n\
'00025567',\n\
'00033911',\n\
'00032601',\n\
'00075985',\n\
'00089213',\n\
'00095621',\n\
'00104593',\n\
'00170932',\n\
'00020646',\n\
'00024837',\n\
'00053648',\n\
'00107753',\n\
'00135003',\n\
'00185171',\n\
'00000353',\n\
'00155121',\n\
'00208088',\n\
'00035670',\n\
'00055374',\n\
'00059879',\n\
'00136550',\n\
'00149442',\n\
'00149943',\n\
'00157921',\n\
'00209675',\n\
'00023953',\n\
'00028456',\n\
'00161894',\n\
'00188571',\n\
'00202502',\n\
'00004706',\n\
'00014879',\n\
'00045485',\n\
'00101817',\n\
'00121511',\n\
'00149147',\n\
'00154010',\n\
'00163082',\n\
'00170834',\n\
'00184615',\n\
'00215028',\n\
'00027962',\n\
'00032953',\n\
'00045441',\n\
'00075340',\n\
'00097553',\n\
'00113612',\n\
'00119604',\n\
'00158675',\n\
'00185140',\n\
'00070215',\n\
'00084627',\n\
'00090719',\n\
'00101287',\n\
'00131171',\n\
'00140682',\n\
'00147153',\n\
'00212361',\n\
'00010832',\n\
'00011533',\n\
'00079917',\n\
'00100051',\n\
'00131639',\n\
'00005653',\n\
'00028857',\n\
'00147507',\n\
'00202170',\n\
'00024841',\n\
'00032272',\n\
'00032424',\n\
'00108838',\n\
'00129760',\n\
'00003898',\n\
'00095095',\n\
'00103947',\n\
'00122668',\n\
'00125955',\n\
'00133236',\n\
'00175734',\n\
'00212617',\n\
'00006859',\n\
'00011901',\n\
'00052524',\n\
'00082423',\n\
'00139181',\n\
'00160606',\n\
'00161350',\n\
'00021287',\n\
'00100843',\n\
'00167312',\n\
'00200941',\n\
'00036744',\n\
'00051663',\n\
'00080272',\n\
'00107137',\n\
'00110179',\n\
'00174688',\n\
'00209268',\n\
'00009627',\n\
'00025567',\n\
'00033911',\n\
'00032601',\n\
'00075985',\n\
'00089213',\n\
'00095621',\n\
'00104593',\n\
'00170932',\n\
'00020646',\n\
'00024837',\n\
'00053648',\n\
'00107753',\n\
'00135003',\n\
'00185171',\n\
'00000353',\n\
'00155121',\n\
'00208088',\n\
'00035670',\n\
'00055374',\n\
'00059879',\n\
'00136550',\n\
'00149442',\n\
'00149943',\n\
'00157921',\n\
'00209675',\n\
'00023953',\n\
'00028456',\n\
'00161894',\n\
'00188571',\n\
'00202502',\n\
'00004706',\n\
'00014879',\n\
'00045485',\n\
'00101817',\n\
'00121511',\n\
'00149147',\n\
'00154010',\n\
'00163082',\n\
'00170834',\n\
'00184615',\n\
'00215028',\n\
'00027962',\n\
'00032953',\n\
'00045441',\n\
'00075340',\n\
'00097553',\n\
'00113612',\n\
'00119604',\n\
'00158675',\n\
'00185140',\n\
'00070215',\n\
'00084627',\n\
'00090719',\n\
'00101287',\n\
'00131171',\n\
'00140682',\n\
'00147153',\n\
'00212361',\n\
'00010832',\n\
'00011533',\n\
'00079917',\n\
'00100051',\n\
'00131639',\n\
'00005653',\n\
'00028857',\n\
'00147507',\n\
'00202170',\n\
'00024841',\n\
'00032272',\n\
'00032424',\n\
'00108838',\n\
'00129760',\n\
'00003898',\n\
'00095095',\n\
'00103947',\n\
'00122668',\n\
'00125955',\n\
'00133236',\n\
'00175734',\n\
'00186336')";

ShowTable( db1, qq );





			try{
				boost::filesystem::path p( "C:\\Program Files (x86)\\Federal Test Center\\DDOffice2022-2023FL\\init\\Sheets.sqlt" );
				if ( boost::filesystem::exists( p ) ) 
					cout << "sadas dasdf asfasdf\n";
				sqlite3pp::database db;
				std::swap( db, db1 );
				string sql = LoadSQL( _T( "D:\\tmp\\BaseAdmin.sql" ) );
				if ( db.execute( sql.c_str() )) cout << db.error_msg()<<'\n';
				db.enable_foreign_keys( true );
//				TestInsert( db, 100 );
				ShowTable( db, _T( "\
SELECT WSheets.*, WMarks.* FROM (SELECT * FROM WSheets WHERE (SheetID IN (SELECT SheetID FROM WSheetSends WHERE (SendID = 1)))) \n\
WSheets LEFT OUTER JOIN WMarks ON WSheets.SheetID = WMarks.SheetID" ) );
				ShowTable( db, _T( "\
SELECT * FROM Files \n\
WHERE FileID IN (SELECT DISTINCT WMarks.FileID FROM (SELECT * FROM WSheets WHERE (FileID = 1) ) \n\
WSheets INNER JOIN WMarks ON WSheets.SheetID = WMarks.SheetID)" ) );

					boost::timer::auto_cpu_timer t;
					{
						cout << "DELETE FROM TestRef ";
						boost::timer::auto_cpu_timer t;
						sqlite3pp::transaction xct( db );
						if ( db.executef( _T( "DELETE FROM Testref WHERE (TestID %% %d) = 0; DELETE FROM Test WHERE (TestID %% %d) = 0" ), 19, 19 ) )  cout << db.error_msg() << '\n';
						cout << db.changes() << '\n';
						xct.commit();
					}
				//{
				//	cout << "DELETE FROM Test ";
				//	boost::timer::auto_cpu_timer t;
				//	sqlite3pp::transaction xct( db );
				//	if ( db.executef( _T( "DELETE FROM Test WHERE (TestID %% %d) == 0" ), 19 ) )  cout << db.error_msg() << '\n';
				//	cout << db.changes() << '\n';
				//	xct.commit();
				//}
				ShowTable( db, _T( "SELECT * FROM TestRef ORDER BY TestRefID DESC LIMIT 100 " ) );
				db.execute( _T( "UPDATE Sessions SET DisconnectTime = CURRENT_TIMESTAMP" ) );
				ShowTable( db, _T( "SELECT * FROM Sessions" ) );
			}
			catch ( exception &ex ){
				cout << ex.what();
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
