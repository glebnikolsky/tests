// AoC10.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <stack>
#include <string>
#include <vector>

using namespace std;
struct Pos {
    Pos(int r=-1, int c=-1) :r_(r), c_(c) {}
    int r_;
    int c_;
    bool valid() { return r_ >= 0 && < rows_ && c_ >= 0 && c < cols_; }
    Pos up() { Pos p(r_ - 1, c_); return p; }
    Pos down() { Pos p(r_ +1, c_); return p; }
    Pos left() { Pos p(r_, c_ - 1); return p; }
    Pos right() { Pos p(r_, c_ + 1); return p; }

    static int rows_;
    static int cols_;
};

bool move(stack<Pos>& pos, vector<string>& topography, Pos &current_pos, char current_height)
{
    Pos p;
    size_t l = pos.size();
    p = current_pos.up();
    if (p.valid() && topography[p.r_][p.c_] == current_height + 1) pos.push(p);
    p = current_pos.down();
    if (p.valid() && topography[p.r_][p.c_] == current_height + 1) pos.push(p);
    p = current_pos.left();
    if (p.valid() && topography[p.r_][p.c_] == current_height + 1) pos.push(p);
    p = current_pos.right();
    if (p.valid() && topography[p.r_][p.c_] == current_height + 1) pos.push(p);
    return l != pos.size();
}


uint64_t trails1(vector<string>& topography, int r, int c)
{
    uint64_t trails{ 0 };
    stack<Pos> pos;
    char current_height = topography[r][c];
    Pos current_pos(r,c);
    do {
        if (!move(pos, topography, current_pos, current_height)) break;
        current_pos = pos.top();
        pos.pop();
        ++current_height;
        if (current_height == 9) {
            ++trails;
            current_pos = pos.top();
            pos.pop();
        }
    }while(!pos.empty())
    return trails;
}

uint64_t solve1(vector<string>& topography)
{
    uint64_t res{ 0 };
    Pos::rows_ = topography.size();
    Pos::cols_ = topography[0].length();
    for (int r{ 0 }; r < topography.size(); ++r)
        for (int c{ 0 }; c < topography[r].length(); ++c)
            if (topography[r][c] == '0') res += trails1(topography, r, c);
    return res;
}


int main()
{
    ifstream ifs("AoC10.txt");
    string line;
    vector<string> topography;
    while (getline(ifs, line)) topography.push_back(line);
    cout << solve1(topography) << endl;
}

