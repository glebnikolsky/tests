// strcount.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <boost/algorithm/string/case_conv.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>

using namespace std;

int main()
{

	ifstream ifs( "" );
	if ( !ifs ) {
		cout << "Could not open the response file\n";
		return 1;
	}
	stringstream ss;
	ss << ifs.rdbuf();
	string sstr = ss.str();
	char *b = sstr.c_str();
	char *e = ++b;

    return 0;
}

