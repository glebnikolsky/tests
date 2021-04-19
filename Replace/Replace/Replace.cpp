// Replace.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>


std::string path;
std::string ext;
std::string src;
std::string what;
std::string dest;

namespace fs = boost::filesystem;
namespace ba = boost::algorithm;

bool checkfile(const std::string &flnm)
{
	if (!ba::ends_with(flnm, ext)) return false;
	std::ifstream is(flnm);
	std::string line;
	while (std::getline(is, line))
	{
		ba::trim_left(line);
		if (ba::istarts_with(line, src) && ba::contains(line, "boost")) {
			std::cout << flnm << '\n';
			std::cout << '\t' << line << '\n';
			return true;
		}
	}
	return false;
}

void procline(std::string &line)
{
	//0123>567<9ab
	int b = line.find('>');
	int e = line.find('<', b);
	std::string str = line.substr(b + 1, e - b - 1);
	b = str.find(';');
	using tokenizer = boost::tokenizer<boost::char_separator<char> >;
	boost::char_separator<char> field_sep(_T(";"));
	tokenizer dirs(str, field_sep);
	for (auto i : dirs) {
		if (ba::icontains(i, what)) {
			if (dest.empty()) ba::replace_all(line, i + ";", dest);
			else ba::replace_all(line, i, dest);
			return;
		}
	}

}

void procfile(const std::string &flnm, const char *backup)
{
	//fs::rename(fs::path(flnm), fs::path(flnm + backup));
	//std::ifstream is(flnm + backup);
	//std::ofstream os(flnm);
	//std::string line;
	//while (std::getline(is, line)) {
	//	if (ba::icontains(line, src))
	//		procline(line);
	//	os << line << '\n';

	//}
	fs::rename(fs::path(flnm), fs::path(flnm + backup));
	std::ifstream is(flnm + backup);
	std::ofstream os(flnm);
	std::cout << flnm << '\n';
	std::string line;
	while (std::getline(is, line)) {
		if (ba::contains(line, "<IncludePath>") && ba::icontains(line, "boost")) {
			using tokenizer = boost::tokenizer<boost::char_separator<char> >;
			boost::char_separator<char> field_sep(_T(";<>"));
			tokenizer dirs(line, field_sep);
			std::string pattern;
			for (auto &i : dirs) {
				if (ba::icontains(i, "boost")) {
					pattern = i;
					break;
				}
			}
			ba::replace_all(line, pattern, "\\\\DEV-SRV\\lib\\boost\\include\\boost-1_74");
		}
		if (ba::contains(line, "<LibraryPath>") && ba::icontains(line, "boost")) {
			using tokenizer = boost::tokenizer<boost::char_separator<char> >;
			boost::char_separator<char> field_sep(_T(";<>"));
			tokenizer dirs(line, field_sep);
			std::string pattern;
			for (auto &i : dirs) {
				if (ba::icontains(i, "boost")) {
					pattern = i;
					break;
				}
						}
			ba::replace_all(line, pattern, "\\\\DEV-SRV\\lib\\boost\\lib");
		}
		if ((ba::contains(line, "<AdditionalDependencies>") || ba::contains(line, "<AdditionalOptions>") ) && ba::icontains(line, "boost")) {
			using tokenizer = boost::tokenizer<boost::char_separator<char> >;
			boost::char_separator<char> field_sep(_T(";<>"));
			tokenizer dirs(line, field_sep);
			std::string pattern;
			for (auto &i : dirs) {
				if (ba::icontains(i, "boost")) {
					pattern = i;
					break;
				}
			}
			int pos = pattern.find(".lib");
			if (pos != -1) {
				pattern = pattern.substr(pos - 4, 4);
				ba::replace_all(line, pattern, "1_74");
			}
		}
		os << line << '\n';
	}
}

void countallfiles()
{
	for (fs::recursive_directory_iterator i(path); i != fs::recursive_directory_iterator(); )
	{
		if (is_regular_file(i->status())) {
	//		if (checkfile(i->path().string()))
//			std::string s = i->path().extension().string();
			if ( i->path().extension().string() == ".vcxproj" )
				procfile	(i->path().string(), ".old");
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

int main(int argc, char **argv)
{
	if (argc < 6) return 1;
	path = argv[1];
	ext = argv[2];
	src = argv[3];
	what = argv[4];
	dest = argv[5];
	countallfiles();
    return 0;
}

