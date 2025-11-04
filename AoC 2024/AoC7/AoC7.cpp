// AoC7.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iostream>
#include <fstream>
#include <strstream>
#include <string>
#include <vector>

using namespace std;

void GetNext2(vector<int>& d2)
{
    for (size_t pos = 0; pos < d2.size() && ++d2[pos] == 2 ; ++pos)
        d2[pos] = 0;
}


int64_t try_k(vector<int64_t>& v, vector<int>& d2)
{
    int64_t res = v[1];
    for (size_t pos{ 2 }, i{ 0 }; pos < v.size(); ++i,++pos) {
        if (d2[i]) res *= v[pos];
        else res += v[pos];
    }
    return res;
}

bool test_1(vector<int64_t>& v)
{
    int N = 1 << (v.size() - 2);
    vector<int> d2(v.size() - 2, 0);
    for (int k = 0; k <N ; ++k) {
        if (v[0] == try_k(v, d2)) return true;
        GetNext2(d2);
    }
    return false;
}

void GetNext3(vector<int>& d3)
{
    for (size_t pos = 0; pos < d3.size() && ++d3[pos] == 3; ++pos) d3[pos] = 0;
}

int64_t Concat(int64_t l, int64_t r)
{
    if (!r) return l * 10;
    int64_t ord{  r};
    for (; ord; ord /= 10) l *= 10;
    return l + r;
}

int64_t try_3(vector<int64_t>& v, vector<int>& d3)
{
    int64_t res = v[1];
    for (int pos{ 2 }, i{ 0}; res <= v[0] && pos < v.size(); ++pos,++i)
        switch (d3[i])
        {
        case 0: res += v[pos]; break;
        case 1: res *= v[pos]; break;
        case 2: res = Concat(res, v[pos]); break;
        }
    return res;
}


bool test_2(vector<int64_t>& v)
{
    size_t N = pow(3,(v.size() - 2));
    vector<int> d3(v.size() - 2, 0);
    for (int k = 0; k < N; ++k) {
        if (v[0] == try_3(v, d3)) return true;
        GetNext3(d3);
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
    //vector<int> d2(7, 0);
    //for (int i = 1; i < 100; ++i) {
    //    GetNext2(d2);

    //    cout << i << '\t' << d2[6] << d2[5] << d2[4] << d2[3] << d2[2] << d2[1] << d2[0] << endl;
    //}
    //cout << Concat(1, 2) << '\t' << Concat(10, 0) << '\t' << Concat(1, 20001) << '\t' << endl;
    ifstream ifs("AoC7.txt");
    string line;
    int64_t res1{ 0 }, res2{ 0 };
    vector<vector<int64_t>> inputs;
    while (getline(ifs, line)) {
        int64_t val = stoll(line);
        vector<int64_t> v(1,val);
        size_t pos = line.find(' ');
        while ( pos != string::npos) {
            v.push_back(stoll(line.substr(pos+1)));
            pos = line.find(' ',pos+1);
        }
        inputs.push_back(v);
     }
    for(auto &v:inputs) if (test_1(v)) res1 += v[0];
    cout <<res1 <<endl;
    for (auto& v : inputs) if (test_2(v)) res2 += v[0];
    cout << res2 << endl;

}

