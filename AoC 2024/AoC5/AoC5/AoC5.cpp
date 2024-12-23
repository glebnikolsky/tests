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

vector<int> try_change(vector<int> v, set<int> s)
{
    int cnd{ 0 };
    auto candidate = [&](int &cnd) {
        if (v.size()) {
            for (auto i : fst_lst)
                if (s.count(i.first) && i.second.size() - 1 >= s.size()) 
                {
                    cnd = i.first;
                    return true;
                }
        }
        };
    if (s.empty()) return v;
    if (candidate(cnd)) {
        v.push_back(cnd);
        s.erase(cnd);
        try_change(v, s);
    }
    return v;
}

int proc2(string& line)
{
    stringstream ss{ line };
    vector<int> v;
    copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(v));
    set<int> s(v.begin(),v.end());
    for (auto i : s) {
        set<int>ss(s);
        vector<int> vv;
        if (fst_lst.count(i)) {
            s.erase(i);
            if (includes(fst_lst[i].begin(), fst_lst[i].end(), ss.begin(), ss.end())) {

            }
        }&& fst_lst[i].size() >= s.size() - 1) {
            vv.push_back(i);
            s.erase(i);
            vv = try_change(vv, ss);
            if (!s.size()) return v[v.size() / 2];
        }
    }
    return 0;
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
        for (size_t pos = line.find(","); pos != string::npos; pos = line.find(",",pos+1)) line.replace(pos, 1, " ");
        int res = proc(line);
        res1 += res;
        if ( !res ) res2 += proc2(line);
    }
    cout << res1 << '\t'<<res2<<'\n';
}

