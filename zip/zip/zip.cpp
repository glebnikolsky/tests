// zip.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/zlib.hpp>

int main(int argc, char **argv)
{
	using namespace std;
	namespace io = boost::iostreams;
	namespace fs = boost::filesystem;
	string src;
	string zipped;
	string unzipped;

	switch (argc) {
	default:
	case 1:
		cerr << "zip inputfile [zipped file [unzipped file]]\n";
		return 1;
	case 4:
		unzipped = argv[3];
	case 3:
		zipped = argv[2];
	case 2:
		src = argv[1];
		break;
	}
	if ( zipped.empty()) zipped = src + ".zip";
	if ( unzipped.empty() ) unzipped = src + ".unzipped";

	ifstream file(src, ios_base::in|ios_base::binary );
	io::filtering_streambuf<io::input> in; 
	in.push(io::zlib_compressor());
	in.push(file);
	if (fs::exists(zipped)) fs::remove(zipped);
	ofstream zip(zipped, ios_base::out | ios_base::binary);
	io::copy(in, zip); 
	zip.close();
	
	ifstream zfile(zipped, ios_base::in|ios_base::binary);
	io::filtering_streambuf<io::input> out;
	out.push(io::zlib_decompressor());
	if (fs::exists(unzipped)) fs::remove(unzipped);
	ofstream clone(unzipped, ios_base::out | ios_base::binary);
	out.push(zfile);
	io::copy(out, clone);

	return 0;
}

