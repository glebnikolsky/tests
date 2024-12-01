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
#ifdef PART1
    vector<long> l, r;
    while (!input.eof()) {
        long lval,rval;
        input >> lval >> rval;
        l.push_back(lval);
        r.push_back(rval);
    }
    sort(l.begin(), l.end());
    sort(r.begin(), r.end());
    long long res1{ 0 };
    for (auto i = l.begin(), j = r.begin(); i != l.end(); ++i, ++j) res1 += abs(*j - *i);
    cout << res1 << '\n';
#else // part 2 
    long long res2{ 0 };
    map<long, long> lm,rm;
    while (!input.eof()) {
        long lval, rval;
        input >> lval >> rval;
        ++lm[lval];
        ++rm[rval];
    }
    for (auto& i : lm) 
        if (rm.count(i.first)) 
            res2 += i.first * rm[i.first]*i.second;
    cout << res2 << '\n';
#endif
}

