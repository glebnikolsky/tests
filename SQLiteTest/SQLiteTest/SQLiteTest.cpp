// SQLiteTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SQLiteTest.h"
#include "sqlite3pp.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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

// The one and only application object

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
	
	//for ( sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i ) {
	//	cout << (*i).get<int>( 0 ) << '\t' << (*i).get<const TCHAR*>( 1 ) << '\t'
	//		<< (*i).get<int>( 2 ) << '\t' << (*i).get<int>( 3 ) << '\t'
	//		<< (*i).get<const TCHAR*>( 4 ) << '\t' << (*i).get<const TCHAR*>( 5 ) << '\t' << (*i).get<int>( 6 ) << '\n';
	//	for ( int j = 0; j < qry.column_count(); ++j ) {
	//		switch ( (*i).column_type( j ) ){
	//		case SQLITE_INTEGER:cout << (*i).get<long long>( j ) << "\t"; break;
	//		case SQLITE_FLOAT:cout << (*i).get<double>( j ) << "\t"; break;
	//		case SQLITE_TEXT:cout << (*i).get<char const*>( j ) << "\t"; break;
	////		case SQLITE_NULL:cout << _T( "NULL\t" ); break;
	//		}
		//}
		//cout << '\n';
	//}

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

			std::set_terminate( [](){std::cout << "\nTerminate\n"; std::abort(); } );
			cout << "GenTest()\n";
			GenTest();
			try{
				sqlite3pp::database db( _T( "test.db" ) );
				sqlite3pp::database db1;
				std::swap(db,db1);
				string sql = LoadSQL( _T( "D:\\tmp\\BaseAdmin.sql" ) );
				if ( db.execute( sql.c_str() )) cout << db.error_msg()<<'\n';
				db.enable_foreign_keys( true );
				TestInsert( db, 100 );
				ShowTable( db, _T( "SELECT WSheets.*, WMarks.*\n"
					"  FROM (SELECT * FROM WSheets\n"
					" WHERE (SheetID IN (SELECT SheetID\n"
					"                      FROM WSheetSends\n"
					"       )) )WSheets\n"
					"  LEFT OUTER JOIN WMarks\n"
					"    ON WSheets.SheetID = WMarks.SheetID" ) );

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
