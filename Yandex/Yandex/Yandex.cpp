// Yandex.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <filesystem>

namespace fs = std::experimental::filesystem;

using namespace std;

int main()
{
	//long long n;
	//cin >> n;
	//long long mx{-1}, mn{-1};
	//cin >> mx;
	//mn = mx;
	//for ( long long i = 1; i < n; ++i ) {
	//	long long tmp;
	//	cin >> tmp;
	//	if ( tmp < mx ){
	//		cout << -1;
	//		return 0;
	//	}
	//	mx = max( tmp, mx );
	//	mn = min( tmp, mn );
	//}
	//cout<< mx - mn;

	fs::path a( "C:\\Program Files( x86 )\\Common Files\\ABBYY TestReader 5.5 Shared\\FTC_GIA19 - 20\\FineForm60.dll" );
	fs::path b;
	int n = 0;
	for( auto i : a ){
		if ( n++ == 3 ) break;
		b /= i;
	}
	cout << b << '\n';
	b /= _T( "Federal Test Center\\DDOffice2019-2020FL" );
	cout << b << '\n';
	fs::path ñ;
	for( auto i = a.begin(); i != a.end(); ++i ){
		cout << i->string() << '\n';
	}
	return 0;
}

