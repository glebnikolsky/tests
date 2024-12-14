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

map<int, set<int>> fst_lst;

void add_pair(string& line)
{
    size_t pos = line.find('|');
    int fst = stoi(line);
    int lst = stoi(line.substr(pos + 1));
    fst_lst[fst].insert(lst);
}


bool vec_in_set(vector<int>& v, int pos, set<int> &s)
{
    for (int i = pos; i < v.size(); ++i) if (!s.count(v[i])) return false;
    return true;
}

int proc(string &line)
{
    vector<int> v;
    stringstream ss{ line };
    copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(v));
    for(int i = 0; i < v.size() - 1; ++i) if ( !vec_in_set(v,i+1, fst_lst[v[i]])) return 0;
    return v[v.size()/2];
}


vector<int> try_change(vector<int> v, set<int> s)
{

}

int proc2(string& line)
{
    stringstream ss{ line };
    vector<int> v;
    copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(v));
    set<int> s(v.begin(),v.end());
    v.clear();
    v = try_change(v, s);
    if ( !s.size() ) return v[v.size() / 2];
    else return 
}

int main()
{
    ifstream input("5_1.txt");
    string line;
    int res1{ 0 }, res2{ 0 };
    while (getline(input, line)) {
        if (line.find('|') != -1) {
            add_pair(line);
            continue;
        }
        else if (line.empty()) continue;
        for (size_t pos{ 0 }; pos != string::npos; pos = line.find(",")) line.replace(pos, 1, " ");
        int res = proc(line);
        res1 += res;
        if ( !res ) res2 += proc2(line);
    }
    cout << res1 << '\t'<<res2<<'\n';
}

