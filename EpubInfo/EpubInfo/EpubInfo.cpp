// EpubInfo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "pugixml.hpp"
#include "unzip.h"
#include "zip.h"
#include <string>
#include <tuple>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

using BookInfo = std::tuple<fs::path, std::vector<std::string>, std::vector<std::string>>;

std::vector<BookInfo> bi;

std::wstring CodePageToUnicode( int codePage, const char *src )
{
	if ( !src ) return std::wstring();
	int srcLen = strlen( src );

	int requiredSize = MultiByteToWideChar( codePage,
		0,
		src, srcLen, 0, 0 );

	if ( !requiredSize )
	{
		return std::wstring(); 
	}
	std::wstring w(requiredSize + 1,L'\0');

	int retval = MultiByteToWideChar( codePage,
		0,
		src, srcLen, (LPWSTR)w.c_str(), requiredSize );
	if ( !retval ) return std::wstring();

	return w;
}

std::string UnicodeToCodePage( int codePage, const wchar_t *src )
{
	if ( !src ) return 0;
	int srcLen = wcslen( src );
	if ( !srcLen ) return std::string();

	int requiredSize = WideCharToMultiByte( codePage,
		0,
		src, srcLen, 0, 0, 0, 0 );

	if ( !requiredSize ) return std::string();

	std::string x( requiredSize + 1, '\0');

	int retval = WideCharToMultiByte( codePage,
		0,
		src, srcLen, (LPSTR)x.c_str(), requiredSize, 0, 0 );
	if ( !retval ) return std::string();

	return x;
}

void ProcZip( const fs::path &file )
{
	std::cout << file << '\n';
	HZIP hz = OpenZip( file.string().c_str(), nullptr);
	if ( !hz ) {
		std::cerr << "OOps!\n";
		return;
	}
	ZIPENTRY ze;
	GetZipItem( hz, -1, &ze );
	int numitems = ze.index;
	for ( int i = 0; i < numitems; ++i ){
		GetZipItem( hz, i, &ze );
		boost::to_lower( ze.name );
		if ( !boost::ends_with( ze.name, "content.opf" ) ) continue;
		std::cout << std::string( ze.name ) << '\t' << ze.unc_size << '\n';
		std::vector<TCHAR> buf( ze.unc_size + 1, '\0' );
		UnzipItem( hz, i, &buf[0], ze.unc_size );
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_buffer( &buf[0], buf.size() );
		pugi::xpath_query query( "/package/metadata/dc:title" );
		pugi::xpath_query query1( "/package/metadata/dc:creator" );
		pugi::xpath_node_set res = query.evaluate_node_set( doc );
		std::vector<std::string> title, author;
		for ( auto k : res ){
			pugi::xml_node nd = k.node().first_child();
			title.push_back(UnicodeToCodePage( 1251, CodePageToUnicode( 65001, nd.value()).c_str()));
			std::cout << *title.rbegin() << '\n';
		}
		pugi::xpath_node_set res1 = query1.evaluate_node_set( doc );
		for ( auto k : res1 ){
			pugi::xml_node nd = k.node().first_child();
			author.push_back(UnicodeToCodePage( 1251, CodePageToUnicode( 65001, nd.value() ).c_str() ));
			std::cout << *author.rbegin() << '\n';
		}
		bi.emplace_back(file, author, title);
	}
}


int main(int argc, TCHAR *argv[])
{
	setlocale( LC_ALL, "ru-RU" );

	for ( fs::recursive_directory_iterator it( argv[1] ); it != fs::recursive_directory_iterator(); ++it ){
		if ( !is_regular_file( it->status() ) ) continue;
		if ( it->path().extension() != _T( ".epub" ) ) continue;
		ProcZip( it->path() );
	}
    return 0;
}

