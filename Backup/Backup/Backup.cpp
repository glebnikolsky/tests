// Backup.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <time.h>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <Windows.h>


#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>

namespace fs = boost::filesystem;
namespace ba = boost::algorithm;

fs::path inpath;
fs::path outpath;
std::string extensions{ "txt;cpp;c;cc;cxx;def;odl;idl;bat;asm;asmx;h;hh;hpp;hxx;inl;inc;rc;ico;cur;bmp;dlg;rc2;gif;jpg;jpeg;jpe;tiff;tif;png;mfcribbon-ms;xml;vcxproj;filters;user;sln;reg" };

std::set<std::string> ext;

void make_path()
{
	time_t tt;
	struct tm tm;
	time(&tt);
	localtime_s(&tm, &tt);
	auto i = --inpath.end();
	std::ostringstream s;
	s << boost::format("%s.%4d %02d %02d %02d %02d %02d") %i->string() % (1900 + tm.tm_year) % (tm.tm_mon + 1) % tm.tm_mday %tm.tm_hour % tm.tm_min %tm.tm_sec;
	outpath /= s.str();
}

void make_extensions()
{
	boost::char_separator<char> field_sep(";", "");
	boost::tokenizer<boost::char_separator<char> > field_tokens(extensions, field_sep);
	ba::to_lower(extensions);
	for (auto i = field_tokens.begin(); i != field_tokens.end(); ++i) ext.insert("." + *i);
}

void proc_files()
{
	fs::create_directory(outpath);
	for (fs::recursive_directory_iterator i(inpath); i != fs::recursive_directory_iterator();)
	{
		if (is_regular_file(i->status())) {
			std::string e = i->path().extension().string();
			ba::to_lower(e);
			if (ext.count(e)) {
				fs::path p = fs::relative(i->path().parent_path(), inpath);
				if (!fs::exists(outpath / p))
				{
					fs::create_directories(outpath / p);
					std::cout << (outpath / p) << '\n';
				}
				fs::copy_file(i->path(), outpath / fs::relative(i->path(), inpath));
			}
		}
		try {
			++i;
		}
		catch (const std::exception &e) {
			(void)e;
			++i;
		}
	}
}


void dummy() {
	char *ptr = new char[1000];
}

int main(int argc, char **argv)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	setlocale(LC_ALL, "ru-RU");

	if (argc < 3) {
		dummy();
	char *ptr = new char[100];
		std::cerr << "Backup from to\n";
		return 1;
	}
	inpath = fs::canonical(fs::path(argv[1]));
	if (!exists(fs::path(argv[2]))) create_directories(fs::path(argv[2]));
	outpath = fs::canonical(fs::path(argv[2]));
	make_path();
	make_extensions();
	proc_files();
    return 0;
}

