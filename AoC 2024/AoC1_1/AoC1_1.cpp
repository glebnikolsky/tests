// aoc1_1.cpp : this file contains the 'main' function. program execution begins and ends there.
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

int main()
{
    ifstream input("1_1.txt");
    vector<long> l, r;
    while (!input.eof()) {
        long val;
        input >> val;
        l.push_back(val);
        input >> val;
        r.push_back(val);
    }
    sort(l.begin(), l.end());
    sort(r.begin(), r.end());
    long long res1{ 0 };
    for (auto i = l.begin(), j = r.begin(); i != l.end(); ++i, ++j)
        res1 += abs(*j - *i);
    cout << res1 << '\n';
// part 2 
    long long res2{ 0 };
    map<long, long> m;
    for (auto& i : r) ++m[i];
    for (auto& i : l) if (m.count(i)) res2 += i * m[i];
    cout << res2 << '\n';

}

