// Adventofcode10.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>  
#include <set>
#include <string>
#include <vector>

using namespace std;


int main()
{
	//vector<int> v;
	//setlocale(LC_ALL, "ru-RU");
	//ifstream ifs("in3.txt");
	//ofstream ofs("out3.csv");
	//copy(istream_iterator<int>(ifs), istream_iterator<int>(), inserter(v, end(v)));
	//sort(begin(v), end(v));
	//v.push_back(v[v.size() - 1] + 3);
	//copy(begin(v), end(v), ostream_iterator<int>(ofs, ";"));
	//ofs << '\n';
	//vector<int> d;
	//for (int i = 0; i < v.size()-1; ++i) d.push_back( v[i+1] - v[i]);
	//copy(begin(d), end(d), ostream_iterator<int>(ofs, ";"));
	//ofs << '\n';

	/*std::vector<int> a, b, c;
	for ( int i = 0; i < 20; ++i ) a.push_back( i );
	for ( int i = 0; i < 20; i += 2 ) b.push_back( i );
	std::set_intersection( begin( a ), end( a ), begin( b ), end( b ), std::back_inserter( c ) );
	for ( auto &i : a )
		std::cout << i << ' ';
	std::cout << '\n';
	for ( auto &i : b )
		std::cout << i << ' ';
	std::cout << '\n'; 
	for ( auto &i : c )
		std::cout << i << ' ';
	std::cout << '\n';
*/
	std::ifstream is( "d:\\tmp\\Test.txt" );
	if ( !is.is_open() )
		return 0;
		struct tt{
		int t;
		int subj;
		std::string s;
		int z;
		int var;
	};
	std::vector<int> v;
	std::vector<tt> tv;

	while( !is.eof() ){
		tt tmp;
		is >> tmp.t >> tmp.subj >> tmp.s >> tmp.z >> tmp.var;
		tv.push_back( tmp );
	};
	std::set<int> res;
	std::string exam_date;
	int zone = -1;
	for ( auto &i : tv ){
		if ( i.t == 4 && i.subj == 1 && (i.s == exam_date || exam_date.empty()) && (i.z == zone || zone == -1) )
			res.insert( i.var );
	}

    return 0;
}

