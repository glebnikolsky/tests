// ParseSelectorGlue.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GSelector.h"

#include <filesystem>
#include <iostream>
#include <boost\optional\optional_io.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/find.hpp>

#include <regex>
#include <string>


boost::iterator_range<char*> FindLastWord( char* s, char* what )
{

	boost::iterator_range<char*> found = boost::ifind_last( s, what);
	while ( *found.begin() ){
		char *b = found.begin() - 1;
		char *e = found.end();
		if ( !isalnum( *(found.begin() - 1) ) && !isalnum( *found.end()) 
			&& *(found.begin() - 1) != '_' && *found.end() != '_'  
			&& *(found.begin() - 1) != '#' && *found.end() != '#' 
			&& *(found.begin() - 1) != '$' && *found.end() != '$' 
			&& *(found.begin() - 1) != '@' && *found.end() != '@' ) break;
		found = boost::ifind_last( boost::iterator_range<char*>(s,found.begin()-1), what );
	}
	return found;
}

int GetOrderByPos( char *s )
{
	boost::iterator_range<char*> order = FindLastWord( s, "order " );
	if ( *order.begin() ){
		boost::iterator_range<char*> over = FindLastWord( s, " over" );
		if ( *over.begin() ){
			char*	begin_srch = std::find( over.end(), order.begin(), '(' );
			int parenth{1};
			for ( ++begin_srch; begin_srch < order.begin() && parenth; ++begin_srch ){
				if ( *begin_srch == '(' ) parenth++;
				if ( *begin_srch == ')' ) parenth--;
			}
			if ( parenth ) 
				return -1;
			else 
				return std::distance( s, order.begin() );
		}
		else {
			return std::distance(s, order.begin());
		}
	}
	return -1;
}


int main()
{
	setlocale( LC_ALL, "ru-RU" );

	GSelector test;
	auto res = test.Init( "D:\\u_use\\Bin\\v140\\Typography_2015\\AllDebug\\glue.xml" );
	if ( res ){
		std::cout << *res;
		return 1;
	}
	auto snames = test.GetGSelectors();
	for ( auto i : snames ) std::cout << i << '\n';
	auto tt = test.FillGSelectorData( 0 );
	auto ttt = test.GetGSelectorMenu();
	for ( auto &i : ttt ) std::cout << i << '\n';
	auto aa = test.GetFieldNames();
	for ( auto &i : aa ) std::cout << i << '\n';
	auto bb = test.GetVars();
	for ( auto &i : bb ) std::cout << i.first << '=' << i.second << '\n';

	//std::regex r( R"([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?)" );
	//std::string s = "3 5 8 2.14 33 / * + = e pi";
	/*std::regex r( "(!![:alnum:]!!)" );
	std::string s = "dsf sadfsadfs !!aaaAa!! df sadflas ~Bbbb~sf";
	for ( std::sregex_iterator i = std::sregex_iterator( s.begin(), s.end(), r );
		i != std::sregex_iterator();
		++i )
	{
		if ( i->size() )
			std::cout << (*i)[0] << std::endl;
	}
	std::string q = "dsf sadfsadfs ~aaaAa~ df sadflas ~Bbbb~sf";
	boost::regex e( "(~[^~]+~)");
	boost::smatch what;
	bool b = boost::regex_match( q, what, e, boost::match_default | boost::match_all);
	if ( what[0].matched ){
		for ( int i = 0; i < what.size(); ++i )
			std::cout <<what[i].str();
	}*/
	char s1[] = "  JOIN (SELECT ht.REGION, ht.ParticipantFK, ht.SubjectCode, ht.HumanTestID,\
		ROW_NUMBER() OVER( PARTITION BY ht._over_ParticipantFK, ht.SubjectCode ORDER BY ht.ExamDate DESC ) AS RowNum\
		FROM dbo.res_HumanTests AS ht ORDER BY ht.ordered DESC";
	char s2[] = "JOIN (SELECT ht.REGION, ht.ParticipantFK, ht.SubjectCode, ht.HumanTestID,\
		ROW_NUMBER() over( PARTITION BY ht.$over#ParticipantFK, ht.SubjectCode ORDER BY ht.ExamDate DESC ) AS RowNum";
	boost::iterator_range<char*> order = FindLastWord( s1, "order" );
	std::cout << order.begin() << "\n\n";
	order = FindLastWord( s2, "order" );
	std::cout << order.begin() << "\n\n";
	order = FindLastWord( s1, "over" );
	std::cout << order.begin() << "\n\n";
	order = FindLastWord( s2, "over" );
	std::cout << order.begin() << "\n\n";

	int pos = GetOrderByPos( s1 );
	if ( pos != -1 ){
		std::cout << s1+pos << "\n\n";
		s1[pos] = '\0';
		std::cout << s1 << "\n\n";
	}
	pos = GetOrderByPos( s2 );
	if ( pos != -1 ){
		std::cout << s2 + pos << "\n\n";
		s2[pos] = '\0';
		std::cout << s2 << "\n\n";
	}
	return 0;
}

