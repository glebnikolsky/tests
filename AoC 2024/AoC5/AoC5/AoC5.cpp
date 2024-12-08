// AoC5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <tuple>
#include <vector>

using namespace std;

map<int, int> fst_lst, lst_fst;
set<int> 

void add_pair(string& line)
{
    int pos = line.find('|');
    int fst = stoi(line);
    int lst = stoi(line.substr(pos + 1));
    fst_lst[fst] = lst;
    lst_fst[lst] = fst;
}

int proc(string &line)
{
    istringstream is(line);
    vector<int> v;
    while (!is.eof()) {
        int val;
        is >> val;
        v.push_back(val);
    }
    int res{ 0 };

    return res;
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

