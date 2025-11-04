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
    inline int distance(Pos rgt) { return abs(r_ - rgt.r_) + abs(c_ - rgt.c_); }
    inline Pos angle(Pos rgt) { return Pos(r_ - rgt.r_, c_ - rgt.c_); }
    inline const Pos operator +(const Pos rgt) {
        return Pos(r_ + rgt.r_, c_ + rgt.c_);
    }
    inline const Pos operator -(const Pos rgt) {
        return Pos(r_  rgt.r_, c_ + rgt.c_);
    }
    };

void find_antinode1(Pos pt, vector<Pos>& ant) 
{
    vector<int> distances;
    for (int i = 0; i < ant.size(); ++i) distances.push_back(pt.distance(ant[i]));
    sort(distances.begin(), distances.end());
    for (int i = 0; i < distances.size() - 1; ++i)
        for (int j = i+1; j < distances.size(); ++j) {
            if (2 * distances[i] == distances[j]) 
                return true;
        }
    return false;
}

void prepare_antennas1(vector<string>& field, map<char, vector<Pos>> &antennas)
{
    for (int r = 0; r < field.size(); ++r)
        for (int c = 0; c < field[r].length(); ++c) 
            if (field[r][c] != '.') antennas[field[r][c]].push_back(Pos(r, c));
}

int solve1(vector<string>& field)
{
    long count{ 0 };
    map<char, vector<Pos>> antennas;
    prepare_antennas1(field, antennas);
    for (auto& antenna : antennas) find_antinode1(antenna.second));
    for (auto& i : field)
        for (auto& j : i) if (j == '#') ++count;
    return count;
}



int main()
{
    ifstream ifs("AoC8_1.txt");
    string line;
    vector<string> field;
    while (getline(ifs, line)) field.push_back(line);
    long res{ 0 };
    res = solve1(field);
}


