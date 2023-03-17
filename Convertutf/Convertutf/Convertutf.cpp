// Convertutf.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>

using namespace std;

char hexd[]{_T( "0123456789abcdef" )};

int main()
{
	ifstream ifs( "C:\\DATA\\Load\\77010140-04-22222.xml" );
	ofstream ofs( "d:\\77010140-04-22222.xml" );
	char ch;
	bool attr{false};
	while ( ifs.get(ch)){
		if ( ch == '"' ) {
			attr = !attr;
			ofs << ch;
			continue;
		}
		if ( attr && static_cast<unsigned char>(ch) >= 0x80 ){
			ofs << "\\x" << hexd[(ch >> 4) & 0xf] << hexd[ch & 0xf];
			ifs.get( ch );
			ofs << "\\x" << hexd[(ch >> 4) & 0xf] << hexd[ch & 0xf];
			continue;
		}
		else ofs << ch;
	
	}

    return 0;
}

