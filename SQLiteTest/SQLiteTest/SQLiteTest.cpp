// SQLiteTest.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "SQLiteTest.h"
#include "sqlite3pp.h"

#include "zip.h"
#include "unzip.h"

#include <io.h>
#include <fcntl.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <chrono>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/integer.hpp>
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

TCHAR values[] = _T( "0123456789abcdefghijklvyjpqrstuvwxyzABCDEFGHIJKLVYJPQRSTUVWXYZ¿¡¬√ƒ≈®∆«»… ÀÃÕŒœ–—“”‘’÷◊ÿŸ⁄€‹›ﬁﬂ‡·‚„‰Â∏ÊÁËÈÍÎÏÌÓÔÒÚÛÙıˆ˜¯˘˙˚¸˝˛ˇ" );
vector<long> strdata( 1000000 );
vector<long> intdata( 1000000 );
vector<long> refs( 1000000 );


struct Foo{
	struct Bar{
		int a;
		std::vector<long> b;
		std::string c;
	};
	Bar b;
	const int& GetA(){ return b.a; }
	const std::vector<long>& GetB(){ return b.b; }
	const size_t& GetBSize(){ return b.b.size(); }
};


string cp1251_to_utf8( const char *str ){
	string res;
	int result_u, result_c;
	result_u = MultiByteToWideChar( 1251, 0, str, -1, 0, 0 );
	if ( !result_u ){ return 0; }
	wchar_t *ures = new wchar_t[result_u];
	if ( !MultiByteToWideChar( 1251, 0, str, -1, ures, result_u ) ){
		delete[] ures;
		return 0;
	}
	result_c = WideCharToMultiByte( CP_UTF8, 0, ures, -1, 0, 0, 0, 0 );
	if ( !result_c ){
		delete[] ures;
		return 0;
	}
	char *cres = new char[result_c];
	if ( !WideCharToMultiByte( CP_UTF8, 0, ures, -1, cres, result_c, 0, 0 ) ){
		delete[] cres;
		return 0;
	}
	delete[] ures;
	res.append( cres );
	delete[] cres;
	return res;
}

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
		std::cout << std::distance(qry.begin(),qry.end()) << '\n';
		int n{0};
		for ( auto r = qry.begin(); r != qry.end(); ++r,++n ) ShowRecord( r, qry.column_count() );
		cout << "---------------------------------------------\n";
		std::cout << n << '\n';
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

bool TestZip()
{
	boost::timer::auto_cpu_timer t;
	HZIP hz = CreateZip( _T( "D:\\tmp\\test.zip" ), _T( "qwerty" ) );
	int fh;
	int err = _sopen_s( &fh, _T( "D:\\tmp\\ERBD_EGE_MAIN_22.db" ), _O_BINARY | _O_RDONLY, _SH_DENYRD, _S_IREAD );
	if ( err ) std::cout << "fuck";
	boost::filesystem::path p( _T( "D:\\tmp\\ERBD_EGE_MAIN_22.db" ) );
	auto fsize = file_size( p );

	const unsigned int BUFSIZE = 0x4000;
	char buf[BUFSIZE];

	for ( size_t bytes, part{0}; (bytes = _read( fh, buf, BUFSIZE )) != 0; ++part ){
		ZipAdd( hz, (_T( "ERBD_EGE_MAIN_22.db." ) + std::to_string( part )).c_str(), buf, bytes );
		fsize -= bytes;
		std::cout << '\t' << fsize << '\r';
	}
	std::cout << '\n';
	_close( fh );
	CloseZip( hz );
	return true;
}

bool TestUnZip()
{
	boost::timer::auto_cpu_timer t;
	HZIP hz = OpenZip( _T( "D:\\tmp\\test.zip" ), _T( "qwerty" ) );
	int fh;
	int err = _sopen_s( &fh, _T( "D:\\tmp\\ERBD_EGE_MAIN_22.dbz" ), _O_CREAT | _O_BINARY | _O_WRONLY, _SH_DENYWR, _S_IWRITE );
	ZIPENTRY ze;
	SetUnzipBaseDir( hz, _T( "D:\\tmp\\" ) );
	GetZipItem( hz, -1, &ze );
	const unsigned int BUFSIZE = 0x4000;
	char buf[BUFSIZE];
	long fsize{0};
	for ( int fl = 0, no_files = ze.index; fl < no_files; ++fl ){
		GetZipItem( hz, fl, &ze );
		UnzipItem( hz, fl, buf, BUFSIZE );
		_write( fh, buf, ze.unc_size );
		fsize += ze.unc_size;
		std::cout << fsize << '\r';
	}
	std::cout << '\n';
	_close( fh );
	CloseZip( hz );
	return true;
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

			{
				using namespace std;
				ios state( nullptr );
				state.copyfmt( cout );
				cout << std::uppercase
					<< std::hex;
				cout << setfill( '0' )
					<< setw( 2 ) << 1 << setfill( '0' )
					<< setw( 2 ) << 2 << setfill( '0' )
					<< setw( 2 ) << 10 << setfill( '0' )
					<< setw( 2 ) << 12;
				cout.copyfmt( state );
				cout << '\t'<<10;
				ostringstream tmp;
				tmp << std::uppercase
					<< std::hex;
				tmp << setfill( '0' ) << setw( 2 ) << 1 << setfill( '0' ) << setw( 2 ) << 2 << setfill( '0' ) << setw( 2 ) << 3 << setfill( '0' ) << setw( 2 ) << 10;
				cout << '\n' << tmp.str();


			}



			auto t1= std::chrono::high_resolution_clock::now();
			setlocale( LC_ALL, "ru-RU" );
			boost::int_t<48>::fast val;
			std::string ptn( _T( "1234_5_6" ) ), a, b;
			a = boost::replace_all_copy( ptn, "_", "9" );
			b = boost::replace_all_copy( ptn, "_", "0" );
			long long l{0};
			for ( long i = 0; i < 1000000; i += 7 )l += i;
			auto t2 = std::chrono::high_resolution_clock::now();

			std::cout << std::chrono::duration<double, std::milli>( t2 - t1 ).count() << ' ' << l << '\n';



			{
			
				const char* s = " ‡Î Û ÍÎ‡˚ ÒÔËÁ‰ËÎ ÍÓ‡ÎÎ˚. ŸÛÙ\n";
				wofstream w("d:\\tmp\\wfile.txt");
				w << L"Ù˚\n";
				w.close();
				ofstream o( "d:\\tmp\\wfile.txt",ios_base::app  );
				for ( int i = 0; i < 100; ++i ){
					o<<cp1251_to_utf8( s );
				}


			}


			sqlite3pp::database db1( _T( "D:\\tmp\\ERBD_EGE_MAIN_22.db" ) );
TCHAR *qq = "select * from pressPre where packnumber in('00212111','00212617','00186336')";

//ShowTable( db1, qq );
if ( db1.execute( _T( "\
DROP TABLE IF EXISTS Stations;\n\
CREATE TABLE IF NOT EXISTS Stations(\n\
	ID_Station INT PRIMARY KEY,\n\
	RegionCode INT,\n\
	StationCode INT,\n\
	StationName TEXT NOT NULL,\n\
	uStationName TEXT NOT NULL\n\
);\n\
\n\
DROP TABLE IF EXISTS Auditories;\n\
CREATE TABLE IF NOT EXISTS Auditories(\n\
	ID_Station INT KEY,\n\
	AuditoriumCode INT NOT NULL,\n\
	AuditoriumName TEXT NOT NULL,\n\
	uAuditoriumName TEXT NOT NULL,\n\
	PRIMARY KEY (ID_Station, AuditoriumCode),\n\
	FOREIGN KEY  (ID_Station) REFERENCES Stations(ID_Station)\n\
);\n\
\n\
DROP TABLE IF EXISTS KIM_Auditories;\n\
CREATE TABLE IF NOT EXISTS KIM_Auditories(\n\
	ID_Station INT KEY,\n\
	AuditoriumCode INT NOT NULL,\n\
	StuffCode TEXT NOT NULL,\n\
	PRIMARY KEY (StuffCode)\n\
);\n\
DROP INDEX IF EXISTS KIM_AuditoriesStuffCode;\n\
CREATE INDEX IF NOT EXISTS KIM_AuditoriesStationAu ON KIM_Auditories( ID_Station, AuditoriumCode);\n" ) ))  cout << db1.error_msg() << '\n';

CDatabase dbsql;
try{
	BOOL b = dbsql.OpenEx( _T( "DRIVER=SQL Server;SERVER={85.143.100.115};DATABASE={FP_EGE_2023};UID={KeyService};PWD={KeyService};ConnectionTimeout=10000" ), CDatabase::useCursorLib );
}
catch ( CDBException *ex ){
	cout << ex->m_strError.GetString();
}
CRecordset rs( &dbsql );
rs.Open( CRecordset::forwardOnly, "\
SELECT distinct ID, RegionCode, StationCode, cast(StationName as nvarchar(1000)) FROM[FP_EGE_2023].[dbo].[Stations]" );
sqlite3pp::command stations( db1, "INSERT INTO Stations(ID_Station, RegionCode, StationCode, StationName, uStationName) VALUES (?, ?, ?, ?,'')" );
sqlite3pp::transaction xct( db1 );
for ( ; !rs.IsEOF(); rs.MoveNext()){
	CDBVariant val;
	CString sval;
	rs.GetFieldValue( (short)0, val );
	stations.bind( 1, val.m_iVal );
	rs.GetFieldValue( (short)1, val );
	stations.bind( 2, val.m_iVal);
	rs.GetFieldValue( (short)2, val );
	stations.bind( 3, val.m_iVal);
	rs.GetFieldValue( (short)3, sval );
	stations.bind( 4, sval.GetString(), sqlite3pp::copy );
	stations.execute()<<'\n';
	stations.reset();
}
xct.commit();
rs.Close();
rs.Open( CRecordset::forwardOnly, "\
SELECT distinct s.ID, e.AuditoriumCode, cast(e.AuditoriumName as nvarchar(1000))\n\
FROM[FP_EGE_2023].[dbo].[Stations] AS s\n\
JOIN[FP_EGE_2023].[dbo].[AuditoriumExams] AS e\n\
ON( s.Id = e.StationId )" );
sqlite3pp::transaction xct1( db1 );
sqlite3pp::command auditories( db1, "INSERT INTO Auditories(AuditoriumID, ID_Station, AuditoriumCode, AuditoriumName, uAuditoriumName) VALUES (?, ?, ?, ?,'')" );
int auditoriumID{1};
for ( ; !rs.IsEOF(); rs.MoveNext(), ++auditoriumID ){
	CDBVariant val;
	CString sval;
	auditories.bind( 1, auditoriumID );
	rs.GetFieldValue( (short)0, val );
	auditories.bind( 2, val.m_lVal );
	rs.GetFieldValue( (short)1, val );
	auditories.bind( 3, val.m_iVal );
	rs.GetFieldValue( (short)2, sval );
	auditories.bind( 4, sval.GetString(), sqlite3pp::copy );
	auditories.execute()<<'\n';
	auditories.reset();
}
xct1.commit();
rs.Close();
rs.Open( CRecordset::forwardOnly, "\
SELECT KimNumber, s.Id, n.AuditoryCode\n\
FROM dbo.FullPackageNumbers AS n\n\
JOIN dbo.Keys AS k ON( n.KeyId = k.Id )\n\
JOIN dbo.Stations AS s ON( k.RegionCode = s.RegionCode AND k.StationCode = s.StationCode )" );
sqlite3pp::transaction xct2( db1 );
sqlite3pp::command stuff_codes( db1, "INSERT INTO KIM_Auditories(StuffCode, ID_Station, AuditoriumCode) VALUES (?, ?, ?)" );
for ( ; !rs.IsEOF(); rs.MoveNext(), ++auditoriumID ){
	CDBVariant val;
	CString sval;
	rs.GetFieldValue( (short)0, sval );
	stuff_codes.bind( 1, sval.GetString(), sqlite3pp::copy );
	rs.GetFieldValue( (short)1, val );
	stuff_codes.bind( 2, val.m_lVal );
	rs.GetFieldValue( (short)2, val );
	stuff_codes.bind( 3, val.m_lVal );
	int res = stuff_codes.execute();
	if ( res){
		cout << "!!!!!!!!!!!!!!!!!!!!" << res;
		break;
	}
	stuff_codes.reset();
}
xct2.commit();
rs.Close();

ShowTable( db1, "select count(*) from stations" );
ShowTable( db1, "select count(*) from Auditories" );
ShowTable( db1, "select count(*) from KIM_Auditories" );

			try{
				boost::filesystem::path p( "C:\\Program Files (x86)\\Federal Test Center\\DDOffice2022-2023FL\\init\\Sheets.sqlt" );
				if ( boost::filesystem::exists( p ) ) 
					cout << "sadas dasdf asfasdf\n";
				sqlite3pp::database db;
				std::swap( db, db1 );
				string sql = LoadSQL( _T( "D:\\tmp\\BaseAdmin.sql" ) );
				if ( db.execute( sql.c_str() )) cout << db.error_msg()<<'\n';
				db.enable_foreign_keys( true );
				TestInsert( db, 100 );
//				ShowTable( db, _T( "\
//SELECT WSheets.*, WMarks.* FROM (SELECT * FROM WSheets WHERE (SheetID IN (SELECT SheetID FROM WSheetSends WHERE (SendID = 1)))) \n\
//WSheets LEFT OUTER JOIN WMarks ON WSheets.SheetID = WMarks.SheetID" ) );
//				ShowTable( db, _T( "\
//SELECT * FROM Files \n\
//WHERE FileID IN (SELECT DISTINCT WMarks.FileID FROM (SELECT * FROM WSheets WHERE (FileID = 1) ) \n\
//WSheets INNER JOIN WMarks ON WSheets.SheetID = WMarks.SheetID)" ) );
				ShowTable( db, _T( "SELECT * FROM Test" ) );

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
