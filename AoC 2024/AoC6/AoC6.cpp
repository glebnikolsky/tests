// AoC6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;


enum Dir { dir_up, dir_right, dir_down, dir_left };

struct Pos {
    Pos(int r, int c, int mr=0, int mc=0) : r_(r), c_(c), max_r_(mr), max_c_(mc) {}
    int r_;
    int c_;
    int max_r_;
    int max_c_;
    
    void SetMax(int mr, int mc) {
        max_r_ = mr;
        max_c_ = mc;
    }

    bool Move(Dir &d, vector<vector<char>>& v) {
        if (v[r_][c_] != '#') {
            v[r_][c_] = 'X';
            switch (d) {
            case dir_up: --r_; break;
            case dir_right: ++c_; break;
            case dir_down: ++r_; break;
            case dir_left: --c_; break;
            }
            if (r_ < 0 || r_ == max_r_) return false;
            if (c_ < 0 || c_ == max_c_) return false;
        }
        else {
            switch (d) {
                case dir_up: ++r_; break;
                case dir_right: --c_; break;
                case dir_down: --r_; break;
                case dir_left: ++c_; break;
            }
            d = (Dir)((d + 1) % 4);
        }
        return true;
    }
    
};

Pos GetInitPoint(vector<vector<char>>& v)
{
    for (int r = 0; r < v.size(); ++r)
        for (int c = 0; c < v[r].size(); ++c)
            if (v[r][c] == '^') return Pos(r, c);
    return Pos(0, 0);
}

void Walk(vector<vector<char>>& v)
{
    Pos p = GetInitPoint(v);
    p.SetMax(v.size(), v[0].size());
    Dir d = dir_up;
    while (p.Move(d, v)) {
    }
}

int proc_1(vector<vector<char>>& v) 
{
    Walk(v);
    int res{ 0 };
    for (auto s : v)
        for (auto& ch : s)
            if (ch == 'X')++res;
    return res;
}


int proc_2(vector<vector<char>>& v)
{
    vector<vector<char>>& copy_v(v);
    proc_1(v);
    
}

int main()
{
    ifstream input("6_1.txt");
    string line;
    vector<vector<char>> v;
    while( getline(input,line)) v.push_back(vector<char>(line.begin(), line.end()));
    cout << proc_1(v);
}

