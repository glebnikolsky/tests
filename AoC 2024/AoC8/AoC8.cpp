// AoC8.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

struct Pos {
    Pos(int r, int c) :r_(r), c_(c) {}

    int r_;
    int c_;
    inline Pos operator +(const Pos rgt) {
        return Pos(r_ + rgt.r_, c_ + rgt.c_);
    }
    inline Pos operator -(const Pos rgt) {
        return Pos(r_ - rgt.r_, c_ - rgt.c_);
    }
    inline Pos operator *(int mul) {
        return Pos(r_*mul, c_*mul);
    }
    bool in_field(size_t rows, size_t cols) {
        if (r_ < 0 || r_ >= rows) return false;
        if (c_ < 0 || c_ >= cols) return false;
        return true;
    }
 };

void prepare_antennas1(vector<string>& field, map<char, vector<Pos>> &antennas)
{
    for (int r = 0; r < field.size(); ++r)
        for (int c = 0; c < field[r].length(); ++c) 
            if (field[r][c] != '.') antennas[field[r][c]].push_back(Pos(r, c));
}

int solve1(map<char, vector<Pos>>& antennas, vector<string>& field)
{
    size_t height = field.size();
    size_t width = field[0].length();
    long count{ 0 };
    for (auto& a : antennas) {
        auto k = a.first;
        auto ant = a.second;
        for (int i = 0; i < ant.size() - 1; ++i)
            for (int j = i + 1; j < ant.size(); ++j) {
                Pos up = ant[i] * 2 - ant[j];
                Pos dn = ant[j] * 2 - ant[i];
                if (up.in_field(height, width) && field[up.r_][up.c_] != k && field[up.r_][up.c_] != '#') {
                    ++count;
                    if (field[up.r_][up.c_] != '#') field[up.r_][up.c_] = '#';
                }
                if (dn.in_field(height, width) && field[dn.r_][dn.c_] != k && field[dn.r_][dn.c_] != '#') {
                    ++count;
                    if (field[dn.r_][dn.c_] != '#') field[dn.r_][dn.c_] = '#';
                }
            }
    }
    return count;
}

int solve2(map<char, vector<Pos>>& antennas, vector<string>& field)
{
    size_t height = field.size();
    size_t width = field[0].length();
    long count{ 0 };
    for (auto& a : antennas) {
        auto ant = a.second;
        if (ant.size() == 1) continue;
        auto k = a.first;
        count += ant.size();
        for (int i = 0; i < ant.size() - 1; ++i)
            for (int j = i + 1; j < ant.size(); ++j) {
                Pos dup = ant[i] - ant[j];
                Pos ddn = ant[j] - ant[i];
                for (Pos up = ant[i] + dup; up.in_field(height, width); up = up + dup) {
                    if (field[up.r_][up.c_] == '.') {
                        field[up.r_][up.c_] = '#';
                        ++count;
                    }
                }
                for (Pos dn = ant[j] + ddn; dn.in_field(height, width) ; dn = dn + ddn) {
                    if (field[dn.r_][dn.c_] == '.') {
                        field[dn.r_][dn.c_] = '#';
                        ++count;
                    }
                }
            }
    }
    return count;

}

int main()
{
    ifstream ifs("AoC8.txt");
    string line;
    vector<string> field;
    while (getline(ifs, line)) field.push_back(line);
    long res{ 0 };
    map<char, vector<Pos>> antennas;
    prepare_antennas1(field, antennas);
    cout << solve1(antennas, field) << endl;
    cout<<solve2(antennas, field) << endl;
    for (auto& i : field) cout << i << endl;

}


