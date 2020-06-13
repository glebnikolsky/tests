// ByDir.cpp : Defines the entry point for the console application.
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

std::string dir_to_rename, pattern, replace;
enum Mode{mode_none, mode_regular_remove, mode_regular_add, mode_replace} mode = mode_none;

void Usage() {
	std::cout <<
		"Usage: ByDir [r|s|a] dir param1 param2\n"
		"\tr - replace param1 - old value, param2 - new value\n"
		"\ts - regular expresson param1 look like (group1)(group2)(group3) ->  (group1)param2(group3), param2 can be empty\n"
	    "\ta - regular expresson param1 look like (?:Name)(.*) or regexpr ->  param1 + line , param2 can't be empty\n";

	exit(1);
}

void ProcArgs(int ac, char* av[])
{
	if (ac < 4) Usage();
	dir_to_rename = av[2];
	switch (av[1][0]) {
	case 'a':
		mode = mode_regular_add;
		pattern = av[3];
		if (ac != 5) Usage();
		replace = av[4];
		break;
	case 's':
		mode = mode_regular_remove;
		pattern = av[3];
		if (ac == 5) replace = av[4];
		break;
	case 'r':
		mode = mode_replace;
		if (ac != 5) Usage();
		pattern = av[3];
		replace = av[4];
		break;
	default:
		Usage();
	}
}


int main(int ac, char* av[])
{

	ProcArgs(ac, av);

	for (fs::recursive_directory_iterator fl(dir_to_rename); fl != fs::recursive_directory_iterator(); ) {
		if (is_regular_file(fl->status())) {
			std::string name = fl->path().string(), oname;
			int pos = name.rfind('\\');
			oname = name = name.substr(pos+1);
			boost::regex e(pattern);
			boost::smatch what;
			switch (mode) {
			case mode_regular_remove:
				try
				{
					if (boost::regex_match(name, what, e, boost::match_perl))
					{
						name = what[1] + replace + what[3];
					}
				}
				catch (const std::exception& e)
				{
					std::cerr << e.what() << std::endl;
					return 1;
				}
				break;
			case mode_regular_add:
				try
				{
					if (boost::regex_match(name, what, e, boost::match_perl))
					{
						name = replace + name;
					}
				}
				catch (const std::exception& e)
				{
					std::cerr << e.what() << std::endl;
					return 1;
				}
				break;
			case mode_replace:
				boost::replace_all(name, pattern, replace);
				break;
			}
			if (oname != name) 
			{
				name = "\\" + name;
				if (!exists(path(dir_to_rename + name))) {
					std::cout << "rename \"" << fl->path().string() << "\" to \"" << dir_to_rename + name << "\"" << std::endl;
					fs::rename(fl->path(), path(dir_to_rename + name));
				}
				else {
					std::cout << "remove file " << fl->path().string() << std::endl;
					fs::remove(fl->path());
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


	return 0;
}

