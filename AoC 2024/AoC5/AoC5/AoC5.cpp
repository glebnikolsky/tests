// AoC5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <sstream>
#include <tuple>

using namespace std;

set < tuple<int, int>> fst_lst, lst_fst;

void add_pair(string& line)
{
    int pos = line.find('|');
    int fst = stoi(line);
    int lst = stoi(line.substr(pos + 1));
    fst_lst.insert(make_tuple(fst, lst));
    lst_fst.insert(make_tuple(lst, fst));
}

int proc(string &line)
{
    return 0;
}

int main()
{
    ifstream input("5_1.txt");
    string line;
    int res{ 0 };
    while (getline(input, line)) {
        if (line.find('|') != -1) add_pair(line);
        if (line.empty()) continue;
        else res += proc(line);
    }
            cout << res;

}

