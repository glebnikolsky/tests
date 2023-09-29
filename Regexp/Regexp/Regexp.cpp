// Regexp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>

void FindGO(std::string &file, std::string &batch)
{
	using namespace std;
	using namespace boost;
	auto begin = file.begin();
	auto cbegin = file.begin();
	auto cend = file.begin();
	regex go( _T( "(?i)^\\s*go\\s*\\d*.*" ) );
	auto NextLine = [&](){
		if ( cend == file.end() ) return false;
		while ( cend != file.end() && *cend != '\n' ) ++cend;
		if ( cend != file.end() ) ++cend;
		return true;
	};
	while ( NextLine() && !regex_match( cbegin, cend, go, match_perl ) ) cbegin = cend;
	batch = string( begin, cend );
	file = string( cend, file.end() );
}



bool CheckSQL( const std::string &file, std::string &err )
{
	using namespace std;
	using namespace boost;
	bool checked{true};
	char_separator<TCHAR> field_sep( _T( "\n" ), _T( "" ), boost::keep_empty_tokens );
	tokenizer<char_separator<TCHAR> > line( file, field_sep );
	int line_no{1};
	regex dbtype( _T( "(?i).*values.*'DbType'.*" ) );
	regex dbver( _T( "(?i).*values.*'DbVersion'.*" ) );
	for ( auto i = line.begin(); i != line.end(); ++i, ++line_no ){
		if ( regex_match( *i, dbtype, match_perl ) ){
			checked = false;
			err += _T( "Строка " ) + to_string( line_no ) + _T( " содержит 'DbType'\n" );
		}
		if ( regex_match( *i, dbver, match_perl ) ){
			checked = false;
			err += _T( "Строка " ) + to_string( line_no ) + _T( " содержит 'DbVersion'\n" );
		}
	}
	return checked;
}



int main()
{
	std::string s = _T("a\nvalues'DbType'\nvalues(    'DbType' )\n\nnasdasdasdasdas values ( 'DbType')\nA\nVALUES'DBTYPE'\nVALUES(    'DBTYPE' )\n\nNASDASDASDASDAS VALUES ( 'DbVersion')");
	std::string s1 = _T("ago\nGO\n--go\n    go\n go 123\n go 1 --jdhfksldfklsdafkhsd\n        ");
	std::string err;
	if ( !CheckSQL( s1, err) ){
		std::cout << err;
	}
	std::string p;
	while ( !s1.empty() ){
		FindGO( s1, p );
		std::cout << p<<'|';
	}
	return 0;
}

