// AlignDir.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <set>
#include <map>
#include <string>
#include <algorithm>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#define UNREFERENCED_PARAMETER(P) (P)

namespace fs = boost::filesystem;
namespace ba = boost::algorithm;

using fs::path;

bool remove_dir = false;
bool add_dir	= false;
bool proc_names = false;
int  books_to_copy = 1000;

fs::path left_dir, right_dir;
std::string base;
std::map<_TCHAR, std::string> encode;
_TCHAR rus[] = _T("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЪЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя");
long dirs = 0;



bool Usage()
{
	std::cout << 
		"Usage: *|+|-|+- \"left dir\" \"right dir\" [books2copy]\n"
"\t* - normalize names\n"
"\t- - remove all files in the right folder and which are not in the left folder\n"
"\t+ - copy all new files to the right folder from the left folder\n"
"\tbooks2copy - no of books to copy\n"
;
	return false;
}

bool CheckPath(fs::path &p, char *pathname) {
	if (!exists(p)) {
		std::cout << "Dir " << p << " not exists\n";
		return Usage();
	}
	if (!is_directory(p)) {
		std::cout << p << " not a directory\n";
		return Usage();
	}
	return true;
}


bool GetArgs(int ac, char* av[])
{
	if (ac < 3) return Usage();
	char *ch = av[1];
	while (*ch) {
		if (*ch == '-') remove_dir = true;
		else if (*ch == '+') add_dir = true;
		else if (*ch == '*') proc_names = true;
		else return Usage();
		++ch;
	}
	if (proc_names && (add_dir || remove_dir)) return Usage();
	left_dir = fs::path(av[2]);
	if (!CheckPath(left_dir, av[2])) return false;
	if (remove_dir || add_dir) {
		if ( ac < 4) return Usage();
		right_dir = fs::path(av[3]);
		if (ac == 5) books_to_copy = atol(av[4]);
		std::string l(av[2]), r(av[3]);
		boost::replace_all(l, "/", "\\");
		boost::replace_all(r, "/", "\\");
		boost::to_upper(l);
		boost::to_upper(r);
		std::string::reverse_iterator li = l.rbegin(), ri = r.rbegin();
		if (*li == '\\') ++li;
		if (*ri == '\\') ++ri;
		while (li != l.rend() && ri != r.rend() && *li == *ri) {
			base = *li + base;
			++li;
			++ri;
		}
		--li;
		if (*li == '\\') base = base.substr(1);
		ba::to_lower(base);
		if (!CheckPath(right_dir, av[3])) return false;
	}
	encode['А'] = "A"; encode['а'] = "a";
	encode['Б'] = "B"; encode['б'] = "b";
	encode['В'] = "V"; encode['в'] = "v";
	encode['Г'] = "G"; encode['г'] = "g";
	encode['Д'] = "D"; encode['д'] = "d";
	encode['Е'] = "E"; encode['е'] = "e";
	encode['Ё'] = "Jo"; encode['ё'] = "jo";
	encode['Ж'] = "Zh"; encode['ж'] = "zh";
	encode['З'] = "Z"; encode['з'] = "z";
	encode['И'] = "I"; encode['и'] = "i";
	encode['Й'] = "Yy"; encode['й'] = "yy";
	encode['К'] = "K"; encode['к'] = "k";
	encode['Л'] = "L"; encode['л'] = "l";
	encode['М'] = "M"; encode['м'] = "m";
	encode['Н'] = "N"; encode['н'] = "n";
	encode['О'] = "O"; encode['о'] = "o";
	encode['П'] = "P"; encode['п'] = "p";
	encode['Р'] = "R"; encode['р'] = "r";
	encode['С'] = "S"; encode['с'] = "s";
	encode['Т'] = "T"; encode['т'] = "t";
	encode['У'] = "U"; encode['у'] = "u";
	encode['Ф'] = "F"; encode['ф'] = "f";
	encode['Х'] = "X"; encode['х'] = "x";
	encode['Ц'] = "C"; encode['ц'] = "c";
	encode['Ч'] = "Ch"; encode['ч'] = "ch";
	encode['Ш'] = "Sh"; encode['ш'] = "sh";
	encode['Щ'] = "Sch"; encode['щ'] = "sch";
	encode['Ъ'] = "'"; encode['ъ'] = "'";
	encode['Ы'] = "I"; encode['ы'] = "i";
	encode['Ь'] = "'"; encode['ь'] = "'";
	encode['Э'] = "E"; encode['э'] = "e";
	encode['Ю'] = "U"; encode['ю'] = "u";
	encode['Я'] = "Ya"; encode['я'] = "ya";


	return true;
}

void ProcName(std::string &fl)
{
	boost::regex e5("(?i)(.*)( - royallib\\.ru)(\\.epub)");
	boost::regex e4("(?i)(.*)( - royallib\\.com)(\\.epub)");
	boost::regex e3("(?i)(.*)( \\(\\d+\\))(\\.epub)");
	boost::regex e2("(?i)(.*)(\\.fb2)(\\.epub)");
	boost::regex e1("(?i)(.*)(\\.\\d+)(\\.epub)");
	boost::regex e0("(?i)(.*)(\\.\\d+\\.fb2)(\\.epub)");
	boost::smatch what;
	while(boost::regex_match(fl, what, e0, boost::match_perl)
		|| boost::regex_match(fl, what, e1, boost::match_perl)
		|| boost::regex_match(fl, what, e2, boost::match_perl)
		|| boost::regex_match(fl, what, e3, boost::match_perl)
		|| boost::regex_match(fl, what, e4, boost::match_perl)
		|| boost::regex_match(fl, what, e5, boost::match_perl)) {
		fl = what[1] + what[3];
	}
	fs::path nfl(fl);
	if (nfl.extension() == ".epub") {
		std::string name = nfl.filename().string();
		name = name.substr(0, name.length() - 5);
		boost::replace_all(name, "_", " ");
		boost::replace_all(name, "-", " ");
		boost::replace_all(name, ".", " ");
		boost::replace_all(name, "  ", " ");
		boost::trim(name);
		std::string newname;
		for (auto &ch : name) {
			if (strchr(rus, (int)ch))
				newname += encode[ch];
			else
				newname += ch;
		}
		while (newname[0] == newname[1])
			newname = newname.substr(1);
		newname[0] = toupper(newname[0]);
		int pos = newname.find(' ');
		if ( pos != -1 ) newname[pos+1] = toupper(newname[pos+1]);
		name = nfl.filename().string(); 
		name = name.substr(0, name.length() - 5);
		if (name != newname ) {
			boost::replace_last(fl, name, newname );
//			fl += ".epub";
		}
	}
}

void LoadDir(fs::path &p, std::set<std::string> &dir, std::string &load_base)
{
	bool no_base = true;
	int base_len = 0;
	for (fs::recursive_directory_iterator fl(p); fl != fs::recursive_directory_iterator(); ) {
		std::string filename = fl->path().string();
		ba::replace_all(filename, "\"", "");
		ba::to_lower(filename);
		if (no_base) {
			base_len = filename.find(base);
			base_len += base.length() + 1;
			load_base = filename.substr(0, base_len);
			no_base = false;
		}
		dir.insert(filename.substr(base_len));
		try {
			++fl;
		}
		catch (const std::exception & ex) {// Просто пропустим системные и т.д. файлы, на которые у нас нет прав
			(void)ex;
			++fl;
		}
	}
}

void AddDir(std::map<std::string, int> &empty_dirs, std::string filename, bool is_file)
{
	int pos;
	while ((pos = filename.rfind('\\')) != -1) {
		if (!is_file) empty_dirs[filename]++;
		is_file = false;
		filename = filename.substr(0, pos);
	}

}


int ProcNames(fs::path left_dir)
{
	std::map<std::string, int> empty_dirs;
	std::cout << "Normalize...\n";
	for (fs::recursive_directory_iterator fl(left_dir); fl != fs::recursive_directory_iterator(); ) {
		std::string filename = fl->path().string();
		boost::replace_all(filename, "/", "\\");
		boost::to_lower(filename);
		int pos = filename.rfind('\\');
		AddDir(empty_dirs, filename, is_regular_file(fl->status()));
		if (is_regular_file(fl->status())) {
			empty_dirs[filename.substr(0, pos)]++;
			std::string oldfilename = filename;
			ProcName(filename);
			boost::to_lower(filename);
			if (oldfilename != filename) {
				if (fs::exists(filename)) {
					std::cout << "remove file " << oldfilename<< std::endl;
					fs::remove(oldfilename);
				}
				else {
					std::cout << "rename \"" << oldfilename << "\" to \""<< filename << "\"" <<std::endl;
					fs::rename(oldfilename, filename);
				}
			}
		}
		try {
			++fl;
		}
		catch (const std::exception & ex) {// Просто пропустим системные и т.д. файлы, на которые у нас нет прав
			(void)ex;
			++fl;
		}
	}
	for (auto i : empty_dirs) {
		if (i.second == 1) {
			std::cout << "remove empty dir " << i.first << std::endl;
			fs::remove(i.first);
		}
	}
	return 0;
}


int main(int ac, char* av[])
{
	if (!GetArgs(ac, av)) return 1;

	if (proc_names) return ProcNames(left_dir);
	
	std::set<std::string> left, right;
	std::string left_base,  right_base;
	std::set<std::string> intersection;

	
	std::cout << "Load left...\n";
	LoadDir(left_dir, left, left_base);
	std::cout << "Directories "<<dirs<<std::endl;
	dirs = 0;
	if (!right_dir.empty()) {
		std::cout << "Load right...\n";
		LoadDir(right_dir, right, right_base);
		std::cout << "Directories " << dirs << std::endl;
		std::set_intersection(left.begin(), left.end(), right.begin(), right.end(), std::inserter(intersection, intersection.begin()));
	}
	if (remove_dir) {
		std::set<std::string> diff;
		std::set_difference(right.begin(), right.end(), intersection.begin(), intersection.end(), std::inserter(diff, diff.begin()));
		if (diff.size()) std::cout << "Remove...\n";
		for (std::set<std::string>::reverse_iterator i = diff.rbegin(); i != diff.rend(); i++) {
			try {
				fs::remove(right_base + *i);
				std::cout << *i << std::endl;
			}
			catch (const std::exception & ex) {
				UNREFERENCED_PARAMETER(ex);
			}
		}
	}
	if (add_dir) {
		std::set<std::string> diff;
		std::set_difference(left.begin(), left.end(), intersection.begin(), intersection.end(), std::inserter(diff, diff.begin()));
		if (diff.size()) std::cout << "Copy...\n";
		for (auto i :diff) {
			try {
				if (fs::is_directory(left_base + i)) fs::create_directory(right_base + i);
			}
			catch (const std::exception & ex) {
				UNREFERENCED_PARAMETER(ex);
			}
		}
		for (auto i : diff) {
			try {
				if (fs::is_directory(right_base + i)) continue;
				std::cout << left_base + i << "--->" << right_base + i << std::endl;
				fs::copy(left_base + i, right_base + i);
				if ( ! --books_to_copy ) return 0;
			}
			catch (const std::exception & ex) {
				UNREFERENCED_PARAMETER(ex);
			}
		}
	}

	return 0;
}

