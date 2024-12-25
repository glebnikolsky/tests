// AoC5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
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

void try_change(vector<int> &res, set<int> &rest)
{
    if (!rest.size()) return ;
    cout << "\t";
    copy(res.begin(), res.end(), ostream_iterator<int>(cout, ","));
    cout << "\b:{";
    copy(rest.begin(), rest.end(), ostream_iterator<int>(cout, ","));
    cout << "\b}\n";
    for (auto i = rest.begin(); i != rest.end(); ) {
        int probe{ *i };
        if (fst_lst.count(probe)) {
            rest.erase(i);
            if (includes(fst_lst[probe].begin(), fst_lst[probe].end(), rest.begin(), rest.end())) {
                res.push_back(probe);
                if (!rest.size()) return ;
                try_change(res, rest);
            }
            else {
                rest.insert(probe);
            }
        }
        i = rest.find(probe);
        if ( i != rest.end() )++i;
    }
    return ;
}

int proc2(string& line)
{
    stringstream ss{ line };
    set<int> rest;
    copy(istream_iterator<int>(ss), istream_iterator<int>(), inserter(rest, rest.end()));
    for (auto i = rest.begin(); i != rest.end(); ) {
        vector<int> res;
        int probe{ *i };
        if (fst_lst.count(probe)) {
            rest.erase(i);
            if (includes(fst_lst[probe].begin(), fst_lst[probe].end(), rest.begin(), rest.end())) {
                res.push_back(probe);
                try_change(res, rest);
                if (!rest.size() ) return res[res.size() / 2];
            }
            rest.insert(probe);
        }
        i = rest.find(probe);
        ++i;
    }
    return 0;
}

int main()
{
    ifstream input("5.txt");
    string line;
    int res1{ 0 }, res2{ 0 };
    while (getline(input, line)) {
        if (line.find('|') != -1) {
            add_pair(line);
            continue;
        }
        else if (line.empty()) continue;
        for (auto &i : fst_lst) {
            cout << i.first << ":{";
            copy(i.second.begin(), i.second.end(), ostream_iterator<int>(cout, ","));
            cout << "\b}\n";
        }
        for (size_t pos = line.find(","); pos != string::npos; pos = line.find(",",pos+1)) line.replace(pos, 1, " ");
        int res = proc(line);
        res1 += res;
        if ( !res ) res2 += proc2(line);
    }
    cout << res1 << '\t'<<res2<<'\n';
}

