// AoC7.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <strstream>
#include <string>
#include <vector>

using namespace std;

long try_k(vector<long>& v, int k)
{
    long long res = v[0];
    for (int i = 1, pos =0; i < v.size(); ++i, ++pos) {
        if (k & (1 << pos)) res *= v[i];
        else res += v[i];
    }
    return res;
}

bool test_1(long long probe, vector<long>& v)
{
    int N = 1 << (v.size() - 1);
    for (int k = 0; k <N ; ++k) {
        if (probe == try_k(v, k)) return true;
    }
    return false;
}

void fill3(int n, string &s, int pos)
{
    div_t d = div(n, 3);
    s[--pos]='0'+d.rem;
    while(d.quot){
        d = div(d.quot, 3);
        s[--pos] = '0'+d.rem;
    }
}

int main()
{
    string s("0000");
    fill3(48, s, 4);
    cout << s << endl;

    ifstream ifs("AoC7.txt");
    string line;
    long long res{ 0 };
    while (getline(ifs, line)) {
        long long probe = stoll(line);
        vector<long> v;
        int pos = line.find(' ')+1;
        while (pos) {
            v.push_back(stol(line.substr(pos)));
            pos = line.find(' ',pos) + 1;
        }
        if (test_1(probe, v)) res += probe;
    }
    cout <<res << endl;

}

