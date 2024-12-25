// AoC6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;


enum Dir(up, right, down, left);

struct Pos {
    Pos(int r, int c) : r_(r), c_(c) {}
    int r_;
    int c_;
};

Pos GetInitPoint(vector<vector<char>>& v)
{
    for (int r = 0; r < v.size(); ++r)
        for (int c = 0; c < v[r].size(); ++c)
            if (v[r][c] == '^') return Pos(r, c);
    return Pos(0, 0);
}

void Walk(vector<vector<char>>& v, Dir d)
{
    Pos p = GetInitPoint(v);

}

int proc_1(vector<vector<char>>& v) {
    
    return 0;
}

int main()
{
    ifstream input("6_1.txt");
    string line;
    vector<vector<char>> v;
    while( getline(input,line)) v.push_back(vector<char>(line.begin(), line.end()));
    cout << proc_1(v);
}

