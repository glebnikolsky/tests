#include "StdAfx.h"
#include <algorithm>

using namespace std;
namespace fs = std::filesystem;
namespace po = boost::program_options;
using RegularReplace = tuple<regex, string, int>;
using Replace = tuple<string, string>;
vector< RegularReplace> rrv;
vector< Replace> rpv;

char delimiter;
bool proc_all{ false };

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ru-RU");
	po::options_description opt("��������� ��������� ������");
	string master_dir;
	string slave_dir;
	bool normalize{ false };
	bool alignl{ false };
	opt.add_options()
		("help,h", "������������ ���������:\nmaster-dir slave-dir \n���� �� ���� �� normalize-names align-dir")
		("delimiter,d", po::value<char>(&delimiter)->default_value('#'), "�����������. �� ��������� '#'")
		("master-dir,m", po::value<string>(&master_dir), "����������, �� ������� ������������� ��� ������� �����������")
		("slave-dir,s", po::value<string>(&slave_dir), "����������, ������� ������������� ��� ������� �����������")
		("normalize-names,n", "������������� ����� � master-dir")
		("align-dir,a", "����������� slave-dir � master-dir");
		("serach-replace,r", po::value< vector<string> >(), "��� �����, �� ��� ��������");
		("serach-replace-regular,g", po::value< vector<string> >(), "��� �����(����������), �� ��� ��������, ����� ������");
		po::variables_map vm;
	try {
		store(parse_command_line(argc, argv, opt), vm);
	}
	catch (exception& e) {
		cerr << e.what();
		return 1;
	}
	{
		vector<int> height{ 1,8,6,2,5,4,8,3,7 };
		int k{2};
		int n{ (int)height.size() };
		vector<int> pos(k);
		generate(pos.begin(), pos.end(), []() {static int i{ 0 };  return i++; });
		auto first = pos.begin();
		auto last = pos.end();
		int square{ 0 };
		int tmp = min(height[pos[0]], height[pos[1]]) * abs(pos[1] - pos[0]);
		square = max(tmp, square);
		cout << '\t' << n << '\t' << k <<'\n';
		while (*first != n - k ) {
			auto mt = last;
			while (*(--mt) == n - (last - mt) - 1);
			(*mt)++;
			while (++mt != last) *mt = *(mt - 1) + 1;
			for (auto& i : pos) cout << i << ' ';
			cout << '\n';
			tmp = min(height[pos[0]], height[pos[1]]) * abs(pos[1] - pos[0]);
			square = max(tmp, square);
		}
		cout << square;
	}

	if (vm.count("help") || !(vm.count("align-dir") && vm.count("master-dir") && vm.count("align-dir")) || !(vm.count("normalize-names") && vm.count("align-dir"))) {
		cout << opt;
		return 0;
	}

	return 0;
}