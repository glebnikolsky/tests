// Ya.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <iostream>
//#include <sstream>
//#include <string>
//#include <set>
//using namespace std;
//
//int main()
//{
//	string input;
//	istringstream is;
//	getline( cin, input );
//	is.str( input );
//	set<string> line;
//	string tmp;
//	while ( !is.eof() ){
//		is >> tmp;
//		if ( line.count( tmp ) ) cout << "YES\n";
//		else {
//			line.insert( tmp );
//			cout << "NO\n";
//		}
//	}
//    return 0;
//}

#include <iostream>
#include <fstream>
#include <string>
using namespace std;
//
//int main( int argc, char **argv )
//{
//	ifstream is( "input.txt", std::ios::in );
//	ofstream os( "output.txt" );
//	char jewels[26] = {};
//	string ln;
//	getline( is, ln );
//	for ( auto &i : ln ) 
//		if ( isalpha( i ) ) jewels[tolower( i ) - 'a'] = 1;
//	int j{0};
//	getline( is, ln );
//	for ( auto &i : ln ) if ( isalpha( i ) ) j += jewels[tolower( i ) - 'a'];
//	os << j;
//	return 0;
//}
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main( int argc, char **argv )
{
	ifstream is( "input.txt", std::ios::in );
	ofstream os( "output.txt" );
	long am, ar, mr, pr;
	is >> am >> ar >> pr >> mr;
	long prm = pr / 1000000, prr = pr % 1000000;
	int turns{0};
	for (;;)
	{
		
	}
	
	return 0;
}