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

bool test_rep(vector<int>& v)
{
    int dir = v[1] - v[0];
    bool first_try{ false };
    for (int i = 1, j = 2; j < v.size(); ++i, ++j) {
        int newdir = v[j] - v[i];
        if (!dir || !newdir || signum(dir) != signum(newdir) || abs(dir) > 3 || abs(newdir) > 3) {
            if ( first_try) return false;
            if (++j == v.size()) return false;
            newdir = v[j] - v[i];
            if ((!newdir || signum(dir) != signum(newdir) || abs(newdir) > 3)) return false;
            first_try = true;
        }
        dir = newdir;
    }
    return true;
}

int main()
{
    ifstream input("2.txt");
    int res{ 0 };
    string rep;;
    while (getline(input, rep)) {
        vector<int> v;
        vector<int> d;
        istringstream in(rep); 
        copy(istream_iterator<int>(in), istream_iterator<int>(), back_inserter(v));
        for (auto i = v.begin(), j = i + 1; j != v.end(); ++i, ++j) d.push_back(*j - *i);
        copy(v.begin(), v.end(), ostream_iterator<int>(cout, "\t"));
        cout << "\n\t";
        copy(d.begin(), d.end(), ostream_iterator<int>(cout, "\t"));
        cout << '\n';

        //if (test_rep(v)) ++res;
     }
    cout << res << '\n';
}

