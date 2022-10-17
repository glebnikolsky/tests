#include "StdAfx.h"
#include <algorithm>

using namespace std;
namespace fs = std::filesystem;
namespace po = boost::program_options;
using RegularReplace = tuple<regex, string, int>;
using Replace = tuple<string, string>;
vector< RegularReplace> rrv;
vector< Replace> rpv;
vector<string> master_files;
vector<string> slave_files;

void Align(vector<string>& master_files, vector<string>& slave_files)
{

}

void Normalize(vector<string>& master_files)
{

}

void SAReplace(vector<string>& master_files)
{

}

void ReplaceRegular(vector<string>& master_files)
{

}


char delimiter = '#';
bool proc_all{ false };

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ru-RU");
	po::options_description opt("Параметры командной строки");
	string master_dir;
	string slave_dir;
	bool normalize{ false };
	bool alignl{ false };
	opt.add_options()
		("help,h", "Обязательные параметры:\nmaster-dir slave-dir \nХотя бы один из normalize-names align-dir")
		("delimiter,d", po::value<char>(&delimiter)->default_value('#'), "Разделитель. По умолчанию '#'")
		("master-dir,m", po::value<string>(&master_dir), "Директория, по которой согласовывают или которую нормализуют")
		("slave-dir,s", po::value<string>(&slave_dir), "Директория, которую согласовывают или которую нормализуют")
		("normalize-names,n", "Нормализовать файлы в master-dir")
		("align-dir,a", "Согласовать slave-dir с master-dir");
		("replace,r", po::value< vector<string> >(), "Что найти, на что заменить");
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
	{
		vector<int> nums1{ 1,3 };
		vector<int> nums2{ 2 };
		//https://leetcode.com/problems/median-of-two-sorted-arrays/



		//int n{ (int)height.size() };
		//vector<int> pos(k);
		//generate(pos.begin(), pos.end(), []() {static int i{ 0 };  return i++; });
		//auto first = pos.begin();
		//auto last = pos.end();
		//int square{ 0 };
		//int tmp = min(height[pos[0]], height[pos[1]]) * abs(pos[1] - pos[0]);
		//square = max(tmp, square);
		//cout << '\t' << n << '\t' << k <<'\n';
		//while (*first != n - k ) {
		//	auto mt = last;
		//	while (*(--mt) == n - (last - mt) - 1);
		//	(*mt)++;
		//	while (++mt != last) *mt = *(mt - 1) + 1;
		//	for (auto& i : pos) cout << i << ' ';
		//	cout << '\n';
		//	tmp = min(height[pos[0]], height[pos[1]]) * abs(pos[1] - pos[0]);
		//	square = max(tmp, square);
		//}
		//cout << square;
	}

	if (vm.count("help") || !(!(vm.count("align-dir") && vm.count("master-dir") && vm.count("align-dir")) || !(vm.count("normalize-names") && vm.count("master-dir"))
		|| !(vm.count("replace") || vm.count("master-dir")) || !(vm.count("replace-regular") && vm.count("master-dir")))) {
		cout << opt;
		return 0;
	}
	try {
		for (auto f = fs::recursive_directory_iterator(fs::path(master_dir)); f != fs::recursive_directory_iterator(); ++f)
			master_files.emplace_back(f->path().string());
		if (vm.count("align-dir")) {
			for (auto f = fs::recursive_directory_iterator(fs::path(slave_dir)); f != fs::recursive_directory_iterator(); ++f)  slave_files.emplace_back(f->path().string());
			Align(master_files, slave_files);
		}
	}
	catch (fs::filesystem_error& e) {
		cerr << e.what();
		return 1;
	}
	if (vm.count("normailze-names")) Normalize(master_files);
	if (vm.count("replace")) {
		SAReplace(master_files);
	}
	if (vm.count("replace-regular")) {
		ReplaceRegular(master_files);
	}

	
	return 0;
}