// AoC2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int signum(int d)
{
    if (d < 0) return -1;
    else if (d > 0) return 1;
    else return 0;
}
#ifdef PART1
bool test_rep(vector<int>& v)
{
    int dir = v[1] - v[0];
    for (int i = 1, j = 2; j < v.size(); ++i, ++j) {
        int newdir = v[j] - v[i];
        if (!dir || !newdir || signum(dir) != signum(newdir) || abs(dir) > 3 || abs(newdir) > 3) return false;
        dir = newdir;
    }
    return true;
}
#endif

bool check_diff(vector<int>& d )
{
    int p{ 0 }, n{ 0 };
    for (auto& i : d) {
        if (abs(i) > 3) return false;
        switch (signum(i)) {
        case -1: ++n; break;
        case 0:  return false;
        case 1: ++p; break;
        }
    }
    if (p == d.size() || n == d.size()) {
        return true;
    }
    return false;
}

void make_d(vector<int>&v, vector<int>& d, int pos)
{
    d.clear();
    vector<int>v1{ v };
    if (pos != -1 )v1.erase(v1.begin() + pos);
    for (auto i = v1.begin(), j = i + 1; j != v1.end(); ++i, ++j) d.push_back(*j - *i);
}


bool test_rep(vector<int>& v)
{
    int pos{ -1 };
    vector<int> d;
    make_d(v, d, pos);
    if (check_diff(d)) return true;
    for (pos = 0; pos < v.size(); pos++) {
        make_d(v, d, pos);
        if (check_diff(d)) return true;
    }
    return false;
}

int main()
{
    ifstream input("2.txt");
    int res{ 0 };
    string rep;;
    while (getline(input, rep)) {
        vector<int> v;
        istringstream in(rep); 
        copy(istream_iterator<int>(in), istream_iterator<int>(), back_inserter(v));
        //copy(v.begin(), v.end(), ostream_iterator<int>(cout, "\t"));
        //cout << "\n\t";
        //copy(d.begin(), d.end(), ostream_iterator<int>(cout, "\t"));
        //cout << '\n';
        if (test_rep(v)) ++res;
      }
    cout << res << '\n';
}

