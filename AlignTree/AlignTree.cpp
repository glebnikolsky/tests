#include "StdAfx.h"
#include <algorithm>
#include <regex>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>

using namespace std;
namespace fs = filesystem;
namespace po = boost::program_options;

using RegularReplaceV = tuple<regex, string, int>;
using ReplaceV = tuple<string, string>;

vector< RegularReplaceV> replace_reg_v;
vector< ReplaceV> replace_v;
vector<fs::path> master_files;
vector<fs::path> slave_files;
vector<regex> rexp;

bool show{ false };
bool confirm{ false };
char delimiter = '#';

void Show(const fs::path &oldname, const fs::path &newname)
{
	if (show)
		cout << "Rename " << oldname.parent_path()<<'\n'<< oldname.filename() << "\nto " << newname.filename() << "\n\n";
}

bool Confirm() 
{
	cout << "Confirm Yes, No, All?";
	char ch;
	cin >> ch;
	if (ch == 'a' || ch == 'A') {
		confirm = false;
		return true;
	}
	return ch == 'y' || ch == 'Y';
}

vector<string> Tokenize(string& str, vector<string>& res)
{
	typedef boost::tokenizer<boost::char_separator<char> > 	tokenizer;
	char del[2]{ '\0' };
	del[0] = delimiter;
	boost::char_separator<char> sep(del,"", boost::keep_empty_tokens);
	tokenizer tokens(str, sep);
	for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter) {
		res.push_back(*tok_iter);
	}
	return res;
}

void ParseReplace(const vector<string>& input)
{
	for (auto& i : input) {
		vector<string> parsed;
		Tokenize(*const_cast<string*>(&i), parsed);
		if (parsed.size() != 2) {
			cerr << "replace требует 2 токена, например 'МаркТвен#Марк Твен'\n";
			exit(1);
		}
		replace_v.emplace_back(parsed[0], parsed[1]);
	}
}

void ParseReplaceReg(const vector<string>& input)
{
	for (auto& i : input) {
		vector<string> parsed;
		Tokenize(*const_cast<string*>(&i), parsed);
		if (parsed.size() != 3) {
			cerr << "replace-regular требует 3 токена\n";
			exit(1);
		}
		replace_reg_v.emplace_back(parsed[0], parsed[1], atoi(parsed[2].c_str()));
	}
}


bool RemoveRep(string& s)
{
	auto first = s.begin();
	if (isdigit(*first)) return false;
	auto second = first;
	while (*++second == *first && second != s.end());
	if (distance(first, second) != 1) {
		s = string(second - 1, s.end());
		s[0] = toupper(s[0]);
		return true;
	}
	return false;
}

bool Normalize(const fs::path &oldname, fs::path& newname)
{
	string name = oldname.stem().string();
	string ext = oldname.extension().string();
	string dir = oldname.parent_path().string();
	RemoveRep(name);
	boost::replace_all(name, "_", " ");
	boost::replace_all(name, "-", " ");
	boost::replace_all(name, "  ", " ");
	for( auto &i: rexp) {
		smatch what;
		if ( regex_match(name, what, i)) {
			name = what[1].str() + what[3].str();
		}
	}
	boost::trim_if(name, boost::is_any_of(" \t.-"));
	newname = fs::path(dir);
	newname /= name;
	newname += ext;
	return newname != oldname;
}

void Align(vector<fs::path>& master_files, vector<fs::path>& slave_files)
{
}

bool Replace(const fs::path &oldname, fs::path &newname)
{
	string name = oldname.stem().string();
	string ext = oldname.extension().string();
	string dir = oldname.parent_path().string();
	for (auto [srch, rep] : replace_v) {
		boost::replace_all(name, srch, rep);
	}
	newname = fs::path(dir);
	newname /= name;
	newname += ext;
	return newname != oldname;

}

bool ReplaceRegular(const fs::path& oldname, fs::path& newname)
{
	string name = oldname.stem().string();
	string ext = oldname.extension().string();
	string dir = oldname.parent_path().string();
	for (auto [re, rep, no] : replace_reg_v) {
		smatch what;
		if (regex_match(name, what, re)) {
			string nname;
			for (size_t i = 1; i < what.size(); ++i) {
				nname += (i == no) ? rep : what[i];
			}
			name = nname;
		}
	}
	newname = fs::path(dir);
	newname /= name;
	newname += ext;
	return newname != oldname;
}


int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ru-RU");
	po::options_description opt("Параметры командной строки");
	string master_dir;
	string slave_dir;
	opt.add_options()
		("help,h", "Обязательные параметры:\nmaster-dir slave-dir \nХотя бы один из normalize-names align-dir")
		("delimiter,d", po::value<char>(&delimiter)->default_value('#'), "Разделитель. По умолчанию '#'")
		("master-dir,m", po::value<string>(&master_dir), "Директория, по которой согласовывают или которую нормализуют")
		("slave-dir,s", po::value<string>(&slave_dir), "Директория, которую согласовывают или которую нормализуют")
		("show,w", "Директория, которую согласовывают или которую нормализуют")
		("confirm,c", "Директория, которую согласовывают или которую нормализуют")
		("normalize,n", "Нормализовать файлы в master-dir")
		("align-dir,a", "Согласовать slave-dir с master-dir")
		("replace,r", po::value< vector<string> >(), "Что найти, на что заменить")
		("replace-regular,g", po::value< vector<string> >(), "Что найти(регулярное), на что заменить, номер группы");
	po::variables_map vm;
	try {
		po::store(po::parse_command_line(argc, argv, opt), vm);
		po::notify(vm);
	}
	catch (exception& e) {
		cerr << e.what();
		return 1;
	}
	show = vm.count("show");
	confirm = vm.count("confirm");
	if (vm.count("help") || (!(vm.count("align-dir") && vm.count("master-dir") && vm.count("slave-dir")) 
							&& !(vm.count("normalize") && vm.count("master-dir"))
							&& !(vm.count("replace") && vm.count("master-dir")) 
							&& !(vm.count("replace-regular") && vm.count("master-dir")))) {
		cout << opt;
		return 0;
	}
	try {
		rexp.emplace_back("(.*)(royallib)($)",regex::icase);
		rexp.emplace_back("(.*)(\\.com)($)", regex::icase);
		rexp.emplace_back("(.*)(\\.fb2)($)", regex::icase);
		rexp.emplace_back("(.*)(\\.ru)($)", regex::icase);
		rexp.emplace_back("(.*)( com)($)", regex::icase);
		rexp.emplace_back("(.*)( fb2)($)", regex::icase);
		rexp.emplace_back("(.*)( ru)($)", regex::icase);
		rexp.emplace_back("(.*)(\\.\\d+)($)", regex::icase);
		rexp.emplace_back("(.*)([\\- \\.]+)($)", regex::icase);
	}
	catch (regex_error& e) {
		cerr << e.what();
		return 0;
	}

	bool normalize = vm.count("normalize") != 0;
	bool replace = vm.count("replace") != 0;
	bool replace_reg = vm.count("replace-regular") != 0;

	if (replace) {
		ParseReplace(vm["replace"].as< vector<string> >());
	}
	if (replace_reg) {
		ParseReplaceReg(vm["replace-regular"].as< vector<string> >());
	}
	try {
		for (auto f = fs::recursive_directory_iterator(fs::path(master_dir));
			f != fs::recursive_directory_iterator(); ++f) {
			fs::path oldname{ f->path() };
			if (!fs::is_regular_file(f->path())) 
				continue;
			bool rename{ false };
			fs::path newname{ oldname };
			if (normalize) 
				rename |= Normalize(oldname, newname);
			oldname = newname;
			if (replace) 
				rename |= Replace(oldname, newname);
			oldname = newname;
			if (replace_reg)
				rename |= ReplaceRegular(oldname, newname);
			if (rename) {
				Show(f->path(), newname);
				if (confirm && !Confirm()) continue;
				fs::rename(f->path(), newname);
			}
			master_files.emplace_back(newname);
		}
		if (vm.count("slave-dir")) {
			for (auto f = fs::recursive_directory_iterator(fs::path(slave_dir)); 
					f != fs::recursive_directory_iterator(); ++f)  
				slave_files.emplace_back(f->path());
			Align(master_files, slave_files);
		}
	}
	catch (fs::filesystem_error& e) {
		cerr << e.what();
		return 1;
	}
	int tmp = vm.count("normalize");

	
	
	return 0;
}