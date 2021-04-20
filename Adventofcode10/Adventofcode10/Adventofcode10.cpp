// Adventofcode10.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>  
#include <vector>

using namespace std;


int main()
{
	vector<int> v;
	setlocale(LC_ALL, "ru-RU");
	ifstream ifs("in3.txt");
	ofstream ofs("out3.csv");
	copy(istream_iterator<int>(ifs), istream_iterator<int>(), inserter(v, end(v)));
	sort(begin(v), end(v));
	v.push_back(v[v.size() - 1] + 3);
	copy(begin(v), end(v), ostream_iterator<int>(ofs, ";"));
	ofs << '\n';
	vector<int> d;
	for (int i = 0; i < v.size()-1; ++i) d.push_back( v[i+1] - v[i]);
	copy(begin(d), end(d), ostream_iterator<int>(ofs, ";"));
	ofs << '\n';


    return 0;
}

