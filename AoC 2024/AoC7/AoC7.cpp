// AoC7.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iostream>
#include <fstream>
#include <strstream>
#include <string>
#include <vector>

using namespace std;


int64_t try_k(vector<int64_t>& v, int k)
{
    int64_t res = v[0];
    for (size_t i = 1, pos = v.size()-2; i < v.size(); ++i, --pos) {
        if (k & (1 << pos)) res *= v[i];
        else res += v[i];
    }
    return res;
}

bool test_1(int64_t probe, vector<int64_t>& v)
{
    int N = 1 << (v.size() - 1);
    for (int k = 0; k <N ; ++k) {
        if (probe == try_k(v, k)) return true;
    }
    return false;
}

void GetNext3(vector<int>& d3)
{
    int carry{ 0 };
    if (++d3[0] == 3) {
        d3[0] = 0;
        carry = 1;
    }
    for (size_t pos = 1; carry && pos < d3.size(); ++pos) {
        ++d3[pos]; 
        if (d3[pos] <= 2) return;
        else d3[pos] = 0;
    }
}

int64_t Concat(int64_t& l, int64_t& r)
{
    return atoll((to_string(l) + to_string(r)).c_str());
}

int64_t try_3(int64_t probe, vector<int64_t>& v, vector<int>& d3)
{
    int64_t res = v[0];
    for(int pos = 1; res< probe && pos < v.size(); ++pos)
        switch (d3[pos - 1])
        {
        case 0: res += v[pos]; break;
        case 1: res *= v[pos]; break;
        case 2: res = Concat(res, v[pos]); break;
        }
    GetNext3(d3);
    return res;
}


bool test_2(int64_t probe, vector<int64_t>& v)
{
    int N = (v.size() - 1)*3;
    vector<int> d3(v.size() - 1, 0);
    for (int k = 0; k < N; ++k) {
        if (probe == try_3(probe, v, d3)) return true;

    }
    return false;
}

int main()
{
    //vector<int> d3(5, 0);
    //for (int i = 1; i < 100; ++i) {
    //    GetNext3(d3);

    //    cout << i << '\t' << d3[4] << d3[3] << d3[2] << d3[1] << d3[0] << endl;
    //}

    ifstream ifs("AoC7.txt");
    string line;
    int64_t res1{ 0 }, res2{ 0 };
    while (getline(ifs, line)) {
        int64_t probe = stoll(line);
        vector<int64_t> v;
        int pos = line.find(' ')+1;
        while (pos) {
            v.push_back(stoll(line.substr(pos)));
            pos = line.find(' ',pos) + 1;
        }
        if (test_1(probe, v)) res1 += probe;
        if (test_2(probe, v)) res2 += probe;

    }
    cout <<res1 <<endl;
    cout << res2 << endl;

}

