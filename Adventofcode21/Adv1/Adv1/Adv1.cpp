// Adv1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <algorithm>
#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

//void adv1( istream &is )
//{
//	int n{0};
//	int f, l;
//	is >> f;
//	while ( is >> l ){
//		if ( l > f ) ++n;
//		f = l;
//	}
//	cout << n;
//}
//
//void adv11( istream &is )
//{
//	int n{0};
//	vector<int> v;
//	copy( istream_iterator<int>( is ),
//		istream_iterator<int>(),
//		back_inserter( v ) );	
//	auto sum3 = [&v]( int i ){ return v[i] + v[i + 1] + v[i + 2]; };
//	int f = sum3( 0 );
//	for ( size_t i = 1; i < v.size()-2; ++i ){
//		int l = sum3( i );
//		n += l > f ? 1 : 0;
//		f = l;
//	}
//	cout << n;
//}
//
//void adv2(istream &is)
//{
//	int d{0}, h{0}, a{0};
//	string s;
//	while ( is >> s ){
//		int x;
//		is >> x;
//		switch ( s[0] ){
//		case 'f': h += x; d += x*a;  break;
//		case 'u': a -= x; break;
//		case 'd': a += x; break;
//		}
//	}
//	cout << (long long)h*d;
//}
//
//void adv3( istream &is )
//{
//	string b;
//	int n{0}, g{0},e{0};
//	vector<int>v;
//	while ( is >> b ){
//		if ( v.empty() ) v = vector<int>( b.length(), 0 );
//		for ( size_t i = 0; i < b.length(); ++i ) v[i] += b[i] - '0';
//		++n;
//	}
//	int p2{1};
//	for ( int i = v.size() - 1; i >= 0; --i ){
//		if ( v[i] > n / 2 ) e += p2;
//		else g += p2;
//		p2 *= 2;
//	}
//	cout << (long long)g*e;
//}
//
//void divide31( int pos, TCHAR bit, vector<string>&v, vector<string>&vm, vector<string>&vl )
//{
//	for ( auto &i : v )
//		if ( i[pos] == bit ) vm.push_back( i );
//		else vl.push_back( i );
//		if ( vm.size() < vl.size() || (vm.size() == vl.size() && vl[0][pos] == bit) ) vm.swap( vl );
//}
//
//void divide30( int pos, TCHAR bit, vector<string>&v, vector<string>&vm, vector<string>&vl )
//{
//	for ( auto &i : v )
//		if ( i[pos] == bit ) vm.push_back( i );
//		else vl.push_back( i );
//		if ( vm.size() > vl.size() || (vm.size() == vl.size() && vl[0][pos] == bit) ) vm.swap( vl );
//}
//
//int b2d( const string &s )
//{
//	int p2{1}, res{0};
//	for ( int i = s.size() - 1; i >= 0; --i ){
//		res += (s[i] - '0')*p2;
//		p2 *= 2;
//	}
//	return res;
//}
//
//void adv31( istream &is )
//{
//	vector<string> v;
//	copy( istream_iterator<string>( is ),
//		istream_iterator<string>(),
//		back_inserter( v ) );
//	vector<string> vc(v);
//	for ( size_t pos = 0; pos < v[0].size(); ++pos ){
//		vector<string> vm, vl;
//		divide31( pos, '1', vc, vm, vl );
//		vc.swap( vm );
//		if ( vc.size() == 1 ) break;
//	}
//	int og = b2d( vc[0] );
//	vc = v;
//	for ( size_t pos = 0; pos < v[0].size(); ++pos ){
//		vector<string> vm, vl;
//		divide30( pos, '0', vc, vm, vl );
//		vc.swap( vm );
//		if ( vc.size() == 1 ) break;
//	}
//	int co2 = b2d( vc[0] );
//	cout <<co2*og;
//}
//
//bool check_rows4( vector<int> &v )
//{
//	for ( int row = 0; row < 25; row += 5 ){
//		bool bingo{true};
//		for ( int col = 0; bingo && col < 5; ++col ) bingo &= v[row + col] < 0;
//		if ( bingo ) return true;
//	}
//	return false;
//}
//
//bool check_cols4( vector<int> &v )
//{
//	for ( int col = 0; col < 5; ++col ){
//		bool bingo{true};
//		for ( int row = 0; bingo && row < 25; row += 5 ) bingo &= v[row + col] < 0;
//		if ( bingo ) return true;
//	}
//	return false;
//}
//
//int move4( int probe, vector < vector<int>> &cards )
//{
//	int card{0};
//	for ( auto &i : cards ){
//		for ( auto &j : i )
//			if ( j == probe ){
//				j = -1;
//				break;
//			}
//		if ( check_rows4( i ) || check_cols4( i ) ) return card;
//		card++;
//	}
//	return -1;
//}
//
//int move41( int probe, vector < vector<int>> &cards, set<int> &closed )
//{
//	int card{0};
//	int card_found{-1};
//	for ( auto &i : cards ){
//		if ( !closed.count( card ) ){
//			for ( auto &j : i )
//				if ( j == probe ){
//					j = -1;
//					break;
//				}
//			if ( check_rows4( i ) || check_cols4( i ) ){
//				closed.insert( card );
//				card_found = card;
//			}
//		}
//		card++;
//	}
//	return card_found;
//}
//
//void adv4( istream &is )
//{
//	string s;
//	getline( is, s );
//	vector<int>moves;
//	const TCHAR *ch = s.c_str();
//	while ( *ch ){
//		moves.push_back( atoi( ch ) );
//		ch++;
//		if ( *ch ){
//			while ( *ch != ',' ) ++ch;
//			++ch;
//		}
//	}
//	vector < vector<int>> cards;
//	vector<int> vtmp;
//	int tmp;
//	while ( is >> tmp ){
//		vtmp.push_back( tmp );
//		if ( vtmp.size() == 25 ){
//			cards.push_back( vtmp );
//			vtmp.clear();
//		}
//	}
//	for ( auto &i : moves ){
//		int card = move4( i, cards );
//		if ( card >= 0 ){
//			int sum{0};
//			for ( auto &j : cards[card] ) sum += j > 0 ? j : 0;
//			cout<< sum*i;
//			return;
//		}
//	}
//}
//void print41( vector < vector<int>> &cards )
//{
//	for ( int row = 0; row < 25; row += 5 ){
//		for ( size_t card = 0; card < cards.size(); ++card ){
//			for ( int col = 0; col < 5; col++ ) cout << setw( 3 ) << cards[card][row + col];
//			cout << "|  |";
//		}
//		cout << '\n';
//	}
//}
//
//void adv41( istream &is )
//{
//	string s;
//	getline( is, s );
//	vector<int>moves;
//	const TCHAR *ch = s.c_str();
//	while ( *ch ){
//		moves.push_back( atoi( ch ) );
//		ch++;
//		if ( *ch ){
//			while ( *ch != ',' ) ++ch;
//			++ch;
//		}
//	}
//	vector < vector<int>> cards;
//	vector<int> vtmp;
//	int tmp;
//	while ( is >> tmp ){
//		vtmp.push_back( tmp );
//		if ( vtmp.size() == 25 ){
//			cards.push_back( vtmp );
//			vtmp.clear();
//		}
//	}
//	int last_win_number{0}, last_sum{0};
//	set<int> closed;
//	for ( auto &i : moves ){
//		int card = move41( i, cards, closed );
//		//cout << '\t' << i << '\n';
//		//print41( cards );
//		if ( card >= 0 ){
//			int sum{0};
//			for ( auto &j : cards[card] ) sum += j > 0 ? j : 0;
//			//cout << i<< '\t' << sum<<'\t'<<sum*i <<'\t'<<card<< '\n';
//			//print41( cards );
//			last_win_number = i;
//			last_sum = sum;
//		}
//	}
//	cout << last_win_number*last_sum;
//}
//
//void adv5( istream &is )
//{
//	using line = tuple<int, int, int, int>;
//	using dot = tuple<int, int>;
//	vector<line> lines;
//	string s;
//	while ( getline( is, s ) ){
//		int x1, y1, x2, y2;
//		const char *ch = s.c_str();
//		x1 = atoi( ch );
//		while ( *ch++ != ',' );
//		y1 = atoi( ch );
//		while ( *ch++ != '>' );
//		++ch;
//		x2 = atoi( ch );
//		while ( *ch++ != ',' );
//		y2 = atoi( ch );
//		if ( x1 == x2 || y1 == y2 ){
//			if ( x1 == x2 && y1 > y2 ) swap( y1, y2 );
//			if ( y1 == y2 && x1 > x2 ) swap( x1, x2 );
//			lines.push_back( make_tuple( x1, y1, x2, y2 ) );
//		}
//	}
//	int res{0};
//	map<dot,int> field;
//	for ( auto &i : lines ){
//		if ( get<0>( i ) == get<2>( i ) ) {
//			for ( int j = get<1>( i ); j <= get<3>( i ); ++j ){
//				dot tmp = make_tuple( get<0>( i ), j );
//				field[tmp]++;
//			}
//		}
//		else{
//			for ( int j = get<0>( i ); j <= get<2>( i ); ++j ){
//				dot tmp = make_tuple( j, get<1>( i ));
//				field[tmp]++;
//			}
//		}
//	}
//	for ( auto &i : field ) if ( i.second > 1 ) res++;
//	cout << res;
//}
//// 0 - гориз y1->y2 ++y  0,1 - 0,3
//// 1 - верт  x1->x2 ++x  0,2 - 4,2
//// 2  xy-xy ++x ++y      1,1 - 3,3
//// 3  ++x --y			 0,8 - 8,0 (x1<x2 && y1>y2)
//// 4  --x ++y            8,0 - 0,8 (x1>x2 && y1<y2)
//// 5  --x --y						(x1>x2 && y1>y2)
//// 7 точка               7,1 - 7,1
///*
//
//+0123456789
//0.......1.
//1......7..
//2.........
//3.........
//4.........
//5.........
//6.........
//7.........
//8.........
//9.........
//*/
//
//
//int classify51( int &x1, int &y1, int &x2, int &y2 )
//{
//	if ( x1 == x2 )
//		if ( y1 == y2 ) return 5;
//		else {
//			if ( y1 > y2 ) swap( y1, y2 );
//			return 0;
//	}
//	if ( y1 == y2 && x1 != x2 ){
//		if ( x1 > x2 ) swap( x1, x2 );
//		return 1;
//	}
//	if ( x1 == y1 && x2 == y2 ){
//		if ( x1 > x2 ){
//			swap( x1, x2 );
//			swap( y1, y2 );
//		}
//		return 2;
//	}
//	if ( abs(x1 - x2) == abs(y1 - y2 )){
//		if ( x1 < x2 && y1 < y2 ) return 2;
//		if ( x1<x2 && y1>y2 ) return 3;
//		if ( x1>x2 && y1<y2) return 4;
//		if ( x1 > x2 && y1 > y2 ) return 5;
//	}
//	return -1;
//}
//
//
//void adv51( istream &is )
//{
//	using line = tuple<int, int, int, int, int>;
//	using dot = tuple<int, int>;
//	vector<line> lines;
//	string s;
//	while ( getline( is, s ) ){
//		int x1, y1, x2, y2, type{-1};
//		const char *ch = s.c_str();
//		x1 = atoi( ch );
//		while ( *ch++ != ',' );
//		y1 = atoi( ch );
//		while ( *ch++ != '>' );
//		++ch;
//		x2 = atoi( ch );
//		while ( *ch++ != ',' );
//		y2 = atoi( ch );
//		type = classify51( x1, y1, x2, y2 );
//		if ( type != -1 )
//			lines.push_back( make_tuple( x1, y1, x2, y2,type ) );
//	}
//	int res{0};
//	map<dot, int> field;
//	for ( auto &i : lines ){
//		//cout << "(" << get<0>( i ) << "," << get<1>( i ) << ")->(" << get<2>( i ) << "," << get<3>( i ) << ") type= " << get<4>( i ) << "\n";
//		switch ( get<4>( i ) ){
//		case 0: 
//		for ( int y = get<1>( i ); y <= get<3>( i ); ++y ){
//			//cout << "(" << get<0>( i ) << "," << y << ") ";
//			field[make_tuple( get<0>( i ), y )]++;
//		}
//		//cout << '\n';
//		break;
//
//		case 1: 
//		for ( int x = get<0>( i ); x <= get<2>( i ); ++x ) {
//			//cout << "(" << x << "," << get<1>( i ) << ") ";
//			field[make_tuple( x, get<1>( i ) )]++;
//		}
//		//cout << '\n';
//		break;
//
//		case 2:	
//		for ( int x = get<0>( i ), y = get<1>( i ); x <= get<2>( i ); ++x, ++y ){
//			//cout << "(" << x << "," << y << ") ";
//			field[make_tuple( x, y )]++;
//		}
//		//cout << '\n';
//		break;
//
//		case 3:	
//		for ( int x = get<0>( i ), y = get<1>( i ); x <= get<2>( i ); ++x, --y ){
//			//cout << "(" << x << "," << y << ") ";
//			field[make_tuple( x, y )]++;
//		}
//		//cout << '\n';
//		break;
//
//		case 4:	
//		for ( int x = get<0>( i ), y = get<1>( i ); x >= get<2>( i ); --x, ++y ) {
//			//cout << "(" << x << "," << y << ") ";
//			field[make_tuple( x, y )]++;
//		}
//		//cout << '\n';
//		break;
//
//		case 5:
//		for ( int x = get<0>( i ), y = get<1>( i ); x >= get<2>( i ); --x, --y ) {
//			//cout << "(" << x << "," << y << ") ";
//			field[make_tuple( x, y )]++;
//		}
//		//cout << '\n';
//		break;
//
//		case 6: 
//			//cout << "(" << get<0>( i ) << "," << get<1>( i ) << ")\n";
//			field[make_tuple( get<0>( i ), get<1>( i ) )]++;
//			break;
//		}
//	}
//	for ( auto &i : field ) if ( i.second > 1 ) res++;
//	cout << res;//21466
//}
//
//long long turn6( map<int, long long> &fish )
//{
//	map<int, long long> tmp;
//	for ( auto &i : fish ){
//		if ( !i.first ){
//			tmp[8] = i.second;
//			tmp[6] = i.second;
//		}
//		else tmp[i.first - 1] += i.second;
//	}
//	fish.swap( tmp );
//	long long fishes{0};
//	for ( auto &i : fish )fishes += i.second;
//	return fishes;
//
//}
//
//void adv6( istream &is )
//{
//	string s;
//	getline( is, s );
//	const TCHAR *ch = s.c_str();
//	map<int,long long> fish;
//	while ( *ch ){
//		int tmp = atoi( ch );
//		fish[tmp]++;
//		while ( *ch && *ch++ != ',' );
//	}
//	long long fishes;
//	for ( int i = 0; i < 256; ++i ){
//		fishes = turn6( fish );
//		cout << (i + 1) << ' ' << fishes << '\n';
//	}
//}
//
//void adv7( istream &is )
//{
//	string s;
//	getline( is, s );
//	const TCHAR *ch = s.c_str();
//	vector<int> crab;
//	while ( *ch ){
//		int tmp = atoi( ch );
//		crab.push_back( tmp );
//		while ( *ch && *ch++ != ',' );
//	}
//	sort( begin( crab ), end( crab ) );
//	int median[11];
//	for ( int i = -5; i < 6; ++i ) median[i + 5] = crab[(crab.size() / 2) + i];
//	int fuel[_countof( median )] = {};
//	for ( auto &i : crab ) for ( int j = 0; j < _countof(median); ++j ) fuel[j] += abs( i - median[j] );
//	for ( auto &i : fuel ) cout << i << ' ';
//}
//
//long long fuel_vol7( int average, int pos )
//{
//	int dist = abs( average - pos );
//	return (1 + dist)*dist / 2;
//}
//
//void adv71( istream &is )
//{
//	string s;
//	getline( is, s );
//	const TCHAR *ch = s.c_str();
//	vector<int> crab;
//	while ( *ch ){
//		int tmp = atoi( ch );
//		crab.push_back( tmp );
//		while ( *ch && *ch++ != ',' );
//	}
//	sort( begin( crab ), end( crab ) );
//	int avg{0};
//	for ( auto &i : crab )avg += i;
//	avg /= crab.size();
//	int average[11];
//	for ( int i = -5; i < 6; ++i ) average[i + 5] = avg+i;
//	long long fuel[_countof( average )] = {};
//	for ( auto &i : crab ) for ( int j = 0; j < _countof( average ); ++j ) fuel[j] += fuel_vol7( average[j], i );
//	for ( auto &i : fuel ) cout << i << ' ';
//}
//
//int probe8( vector<string> &probe )
//{
//	int res{0};
//	for(auto &i:probe ) 
//		switch ( i.length() ){
//		case 2:
//		case 3:
//		case 4:
//		case 7:
//		res++;
//		break;
//		}
//	return res;
//}
//
//
//void adv8( istream &is )
//{
//	int sum{0};
//	for ( ;!is.eof();){
//		vector<string> displey( 10 ), probe( 4 );
//		for ( int i = 0; i < 10;++i ){
//			is>>displey[i];
//		}
//		is >> probe[0];
//		for ( int i = 0; i < 4; ++i )is >> probe[i];
//		sum += probe8( probe );
//	}
//	cout << sum;
//}
//
//string diff( string &s1, string &s2 )
//{
//	string tmp;
//	set_difference( begin( s1 ), end( s1 ), begin( s2 ), end( s2 ), inserter( tmp, begin( tmp ) ) );
//	return tmp;
//}
//
//bool in( string &s1, string &s2 )
//{
//	return includes( begin( s1 ), end( s1 ), begin( s2 ), end( s2 ) );
//}
//
//int probe81( vector<string> &display, vector<string> &probe )
//{
//	vector<string> dig( 10 );
//	for ( auto &i : display ){
//		sort( begin( i ), end( i ) );
//		switch ( i.length() ){
//		case 2:
//		dig[1] = i;
//		break;
//		case 3:
//		dig[7] = i;
//		break;
//		case 4:
//		dig[4] = i;
//		break;
//		case 7:
//		dig[8] = i;
//		break;
//		}
//	}
//	map<TCHAR, string> seg;
//	seg['a'] = diff( dig[7], dig[1] );
//	string c = dig[1].substr( 0, 1 ), f = dig[1].substr( 1, 1 );
//	seg['c'] = seg['f'] = dig[1];
//	seg['b'] = seg['d'] = diff( dig[4], dig[1] );
//	string b = seg['b'].substr( 0, 1 ), d = seg['b'].substr( 1, 1 );
//	for ( auto &i : display ){
//		if ( i.length() == 6 ){
//			if ( !in( i, b ) || !in( i, d ) ){//0
//				dig[0] = i;
//				if ( !in( i, b ) ) {
//					seg['d'] = b;
//					seg['b'] = d;
//				}
//				else{
//					seg['d'] = d;
//					seg['b'] = b;
//				}
//			}
//			else if ( !in( i, c ) || !in( i, f ) ){//6
//				dig[6] = i;
//				if ( !in( i, c ) ) {
//					seg['c'] = c;
//					seg['f'] = f;
//				}
//				else{
//					seg['c'] = f;
//					seg['f'] = c;
//				}
//			}
//			else{ //9
//				dig[9] = i;
//				seg['e'] = diff( dig[8], i );
//			}
//		}
//	}
//	seg['e'] = diff( dig[0], dig[9] );
//	string g;
//	for ( auto &i : seg ) g += i.second;
//	sort( begin( g ), end( g ) );
//	seg['g'] = diff( string( "abcdefg" ), g );
//	dig[2] = seg['a'] + seg['c'] + seg['d'] + seg['e'] + seg['g'];
//	dig[3] = seg['a'] + seg['c'] + seg['d'] + seg['f'] + seg['g'];
//	dig[5] = seg['a'] + seg['b'] + seg['d'] + seg['f'] + seg['g'];
//	for ( auto i : {2,3,5} ) sort( begin( dig[i] ), end( dig[i] ) );
//	map<string, int> dg;
//	for ( int i = 0; i < 10; ++i )  dg[dig[i]] = i;
//	for ( auto &i : probe ) sort( begin( i ), end( i ) );
//	return dg[probe[0]]*1000 + dg[probe[1]]*100 + dg[probe[2]]*10 + dg[probe[3]];
//}
//
//void adv81( istream &is )
//{
//
//	long long sum{0};
//	for ( ; !is.eof();){
//		vector<string> displey( 10 ), probe( 4 );
//		for ( int i = 0; i < 10; ++i ){
//			is >> displey[i];
//		}
//		is >> probe[0];
//		for ( int i = 0; i < 4; ++i )is >> probe[i];
//		sum += probe81( displey, probe );
//	}
//	cout << sum;
//}
//
//void adv9( istream &is )
//{
//	vector<vector<char>> cave;
//	string ln;
//	while(getline(is,ln)){
//		vector<char> tmp;
//		for ( auto &i : ln ) tmp.push_back( i - '0' );
//		cave.push_back( tmp );
//	}
//	auto up = [&cave](int row, int col){
//		return cave[row][col] < cave[row][col - 1] && 
//			cave[row][col] < cave[row][col + 1] && 
//			cave[row][col] < cave[row + 1][col];
//	};
//	auto down = [&cave]( int row, int col ){
//		return cave[row][col] < cave[row][col - 1] && 
//			cave[row][col] < cave[row][col + 1] && 
//			cave[row][col] < cave[row - 1][col];
//	};
//	auto left = [&cave]( int row, int col ){
//		return cave[row][col] < cave[row][col + 1] && 
//			cave[row][col] < cave[row-1][col] && 
//			cave[row][col] < cave[row + 1][col];
//	};
//	auto right = [&cave]( int row, int col ){
//		return cave[row][col] < cave[row][col - 1] && 
//			cave[row][col] < cave[row - 1][col] && 
//			cave[row][col] < cave[row + 1][col];
//	};
//	auto inside = [&cave]( int row, int col ){
//		return cave[row][col] < cave[row][col - 1] && 
//			cave[row][col] < cave[row][col + 1]	&& 
//			cave[row][col] < cave[row - 1][col] && 
//			cave[row][col] < cave[row + 1][col];
//	};
//	int res{0}, eoc = cave[0].size() - 1, eor = cave.size() - 1;
//	//corners
//	if ( cave[0][0] < cave[0][1] && cave[0][0] < cave[1][0] ) 
//		res += cave[0][0] + 1;
//	if ( cave[0][eoc] < cave[0][eoc- 1] && cave[0][eoc] < cave[1][eoc] ) 
//		res += cave[0][eoc] + 1;
//	if ( cave[eor][0] < cave[eor - 1][0] && cave[eor][0] < cave[eor][1] ) 
//		res += cave[eor][0] + 1;
//	if ( cave[eor][eoc] < cave[eor - 1][eoc] && cave[eor][eoc] < cave[eor][eoc-1] ) 
//		res += cave[eor][eoc] + 1;
//	for ( int col = 1; col < eoc; ++col ) if ( up( 0, col ) ) 
//		res += cave[0][col] + 1;
//	for ( int col = 1; col < eoc; ++col ) if ( down( eor, col ) ) 
//		res += cave[eor][col] + 1;
//	for ( int row = 1; row < eor; ++row ) if ( left( row, 0 ) ) 
//		res += cave[row][0] + 1;
//	for ( int row = 1; row < eor; ++row ) if ( right( row, eoc ) ) 
//		res += cave[row][eoc] + 1;
//	for(int row = 1; row < eor; ++row )
//		for ( int col = 1; col < eoc; ++col ) 
//			if ( inside( row, col ) ) 
//				res += cave[row][col] + 1;
//
//	cout << res;
//
//}
//
//
//
//void adv91( istream &is )
//{
//	vector<vector<char>> cave;
//	using Postp = tuple<int, int>;
//	auto Pos = []( int r, int c ){ return make_tuple( r, c ); };
//	set<Postp> seeds;
//	set<map<Postp, bool>> basins;
//	string ln;
//	while ( getline( is, ln ) ){
//		vector<char> tmp;
//		for ( auto &i : ln ) tmp.push_back( i - '0' );
//		cave.push_back( tmp );
//	}
//	auto up = [&cave]( int row, int col ){
//		return cave[row][col] < cave[row][col - 1] &&
//			cave[row][col] < cave[row][col + 1] &&
//			cave[row][col] < cave[row + 1][col];
//	};
//	auto down = [&cave]( int row, int col ){
//		return cave[row][col] < cave[row][col - 1] &&
//			cave[row][col] < cave[row][col + 1] &&
//			cave[row][col] < cave[row - 1][col];
//	};
//	auto left = [&cave]( int row, int col ){
//		return cave[row][col] < cave[row][col + 1] &&
//			cave[row][col] < cave[row - 1][col] &&
//			cave[row][col] < cave[row + 1][col];
//	};
//	auto right = [&cave]( int row, int col ){
//		return cave[row][col] < cave[row][col - 1] &&
//			cave[row][col] < cave[row - 1][col] &&
//			cave[row][col] < cave[row + 1][col];
//	};
//	auto inside = [&cave]( int row, int col ){
//		return cave[row][col] < cave[row][col - 1] &&
//			cave[row][col] < cave[row][col + 1] &&
//			cave[row][col] < cave[row - 1][col] &&
//			cave[row][col] < cave[row + 1][col];
//	};
//	int res{0}, eoc = cave[0].size() - 1, eor = cave.size() - 1;
//	//corners
//	if ( cave[0][0] < cave[0][1] && cave[0][0] < cave[1][0] ) seeds.insert( Pos( 0, 0 ) );
//	if ( cave[0][eoc] < cave[0][eoc - 1] && cave[0][eoc] < cave[1][eoc] ) seeds.insert( Pos( 0, eoc ) );
//	if ( cave[eor][0] < cave[eor - 1][0] && cave[eor][0] < cave[eor][1] ) seeds.insert( Pos( eor, 0 ) );
//	if ( cave[eor][eoc] < cave[eor - 1][eoc] && cave[eor][eoc] < cave[eor][eoc - 1] ) seeds.insert( Pos( eor, eoc ) );
//	for ( int col = 1; col < eoc; ++col ) if ( up( 0, col ) ) seeds.insert( Pos( 0, col ) );
//	for ( int col = 1; col < eoc; ++col ) if ( down( eor, col ) ) seeds.insert( Pos( eor, col ) );
//	for ( int row = 1; row < eor; ++row ) if ( left( row, 0 ) ) seeds.insert( Pos( row, 0 ) );
//	for ( int row = 1; row < eor; ++row ) if ( right( row, eoc ) ) seeds.insert( Pos( row, eoc ) ); 
//	for ( int row = 1; row < eor; ++row )
//		for ( int col = 1; col < eoc; ++col )
//			if ( inside( row, col ) )
//				seeds.insert( Pos( row, col ) );
//	
//	auto area = [&Pos,&cave, &eor, &eoc]( map<Postp, bool> &bs, int r, int c ){
//		if ( r && cave[r - 1][c] != 9 && !bs.count( Pos( r - 1, c ) )) bs[Pos( r - 1, c )] = true;
//		if ( r != eor && cave[r + 1][c] != 9 && !bs.count( Pos( r + 1, c ) ) ) bs[Pos( r + 1, c )] = true;
//		if ( c &&  cave[r][c - 1] != 9 && !bs.count( Pos( r, c - 1 ) ) ) bs[Pos( r, c - 1 )] = true;
//		if ( c != eoc && cave[r][c + 1] != 9 && !bs.count( Pos( r, c + 1 ) ) ) bs[Pos( r, c + 1 )] = true;
//	};
//	for ( auto &i : seeds ){
//		map<Postp,bool> bs;
//		bs[i] = true;
//		size_t old_size = 1;
//		for( auto i = begin( bs ); i != end(bs);){
//			if ( i->second ){
//				i->second = false;
//				area( bs, get<0>(i->first), get<1>(i->first) );
//				if ( bs.size() != old_size ) {
//					i = begin( bs );
//					old_size = bs.size();
//					continue;
//				}
//			}
//			++i;
//		}
//		basins.insert( bs );
//	}
//	vector<int> v;
//	for ( auto i : basins ) v.push_back( -(int)i.size() );
//	sort( begin( v ), end( v ) );
//	cout << -v[0] * v[1] * v[2] << endl;
//}



void adv10( istream &is )
{
	int res{0};
	vector<long long> part2;
	string s;
	map<TCHAR, TCHAR> pairs = {{'(',')'},{'{','}'},{'[',']'},{ '<','>'}};
	map<TCHAR, int> price = {{')',3},{']',57},{'}',1197},{'>',25137}};
	map<TCHAR, int> price2 = {{'(',1},{'[',2},{'{',3},{'<',4}};
	while( getline( is, s ) ){
		stack<TCHAR> st;
		bool bad{false};
		for ( auto &i : s ){
			if ( pairs.count( i ) ) st.push( i );
			else if ( pairs[st.top()] != i ){
				res += price[i];
				bad = true;
				break;
			}
			else st.pop();
		}
		if ( bad ) continue;
		long long p2{0};
		while ( !st.empty() ){
			p2 = p2 * 5 + price2[st.top()];
			st.pop();
		}
		part2.push_back( p2 );
	}
	sort( begin( part2 ), end( part2 ) );

	cout << res << ' '<<part2[part2.size()/2]<< endl;
}

struct Cell{
	Cell( int r, int c ): r_( r ), c_( c ){}
	int r_;
	int c_;
};
bool operator<( const Cell &lth, const Cell &rth ) { return (lth.r_ < rth.r_ || (lth.r_ == rth.r_ && lth.c_ < rth.c_)); }

void adv11( istream &is )
{
	string ln;
	vector<vector<int>> desk;
	while ( getline( is, ln ) ){
		vector<int> tmp;
		for ( auto &ch : ln ) tmp.push_back( ch - '0' );
		desk.push_back( tmp );
	}
	int eor = desk.size(), eoc = desk[0].size();

	auto right_cell = [&]( int r, int c ){
		if ( r < 0 || r >= eor ) return false;
		if ( c < 0 || c >= eoc ) return false;
		return true;
	};
	int flashes{0};
	
	auto clear_flash = [&]( int r, int c ){
		if ( desk[r][c] > 9 ) {
			desk[r][c] = 0;
			++flashes;
		}
	};

	auto print_desk = [&]( string &&str ){
		cout << str << endl;
		for ( auto &r : desk ){
			for ( auto &c : r ) cout << setw( 1 ) << c;
			cout << endl;
		}
		cout << endl;
	};
	int turns{0};
	for(;;){
		deque<Cell> to_proc;
		set<Cell> used;
		auto add_one = [&]( int r, int c ){
			if ( right_cell( r, c ) && ++desk[r][c] > 9 && !used.count( Cell( r, c ) ) ){
				to_proc.push_back( Cell( r, c ) );
				used.insert( Cell( r, c ) );
			}
		};
		for ( int r = 0; r < eor; ++r )
			for ( int c = 0; c < eoc; ++c ) add_one( r, c );
		while ( to_proc.size() ){
			add_one( to_proc.front().r_ - 1, to_proc.front().c_ - 1 );
			add_one( to_proc.front().r_ - 1, to_proc.front().c_ );
			add_one( to_proc.front().r_ - 1, to_proc.front().c_ + 1 );

			add_one( to_proc.front().r_, to_proc.front().c_ - 1 );
			add_one( to_proc.front().r_, to_proc.front().c_ + 1 );

			add_one( to_proc.front().r_ + 1, to_proc.front().c_ - 1 );
			add_one( to_proc.front().r_ + 1, to_proc.front().c_ );
			add_one( to_proc.front().r_ + 1, to_proc.front().c_ + 1 );
			to_proc.pop_front();
		}
		for ( int r = 0; r < eor; ++r )
			for ( int c = 0; c < eoc; ++c ) clear_flash( r, c );
		print_desk( "After Step " + to_string(++turns) );
		int fl{0};
		for ( int r = 0; r < eor; ++r )
			for ( int c = 0; c < eoc; ++c ) fl += desk[r][c];
		cout << "Flashes = " << flashes << endl<<endl;
		if ( !fl ) break;
	}
}

int main()
{
	ifstream is ( "in11.txt" );
	adv11( is);


    return 0;
}

